#include "SBW/config.h"
#include "SBW/SBWOSSocket.h"
#include "SBW/DataBlockReader.h"
#include "SBW/DataBlockWriter.h"
#include "SBW/SBWApplicationException.h"
#include "SBW/SBWCommunicationException.h"

#include "ModuleInstance.h"
#include "ProxyLocalBrokerInstance.h"
#include "SBW.h"
#include "SBWBrokerRPC.h"

#include <sstream>
#ifdef WIN32
#include <io.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif

using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;
using namespace std;

ModuleInstance::ModuleInstance(SOCKET socket, Host *host, InstanceThread::Hook *hook)
: InstanceThread( socket, "module", hook), transmitMutex("moduleinstance") {
	myBrokerId = -1 ; // BROKER ID
	myHost = host;	
	// this.myName, myFullName & myInternalName are set in connect()

	startupWaitTime = Properties::toInt(Properties::getProperty("sbw.module.startup.wait", "600"));
}
InstanceThread::Factory *ModuleInstance::getFactory(InstanceThread::Hook *hook,
													Host *host)
{
	return new Factory(host,hook);
}
bool ModuleInstance::connect(const char* key)
{	
	// Although the module id is not set in the constructor, by the time
	// this is called, the id will have been set by the onCreation hook.

	// We create the reply first so we can call release() in the finally
	// clause, but the reply is not sent until after we read the name.

	try
	{
		std::string moduleKey = Sys::readString(mySocket);			
		
		if (SBWOSSocket::isLocalAddress(myHost->getClearName().c_str()))
		{
			if (! (strncmp(key,moduleKey.c_str(), strlen(key)) == 0 )  )
			{
				TRACE("Security check failed against session key");
				closesocket(mySocket);

				setException(new SBWCommunicationException(
					"Security check failed for Broker connection from '"
					+ myHost->getName() + "'",
					"The SBW Broker received an attempted connection"\
					" from a module, but the"\
					" security key transmitted by the module"\
					" was incorrect.  There are three possible"\
					" reasons for this: (1) the connection did not"\
					" come from an SBW module; (2) the connection came"\
					" from a module, but the module is not owned"\
					" by you, the current user; or (3) there is a"\
					" mismatch in the session keys because of an"\
					" external change to the session key in the file"));
				return false;
			}
		}

		TRACE("Session key check succeeded");
		//SBWOSMutexLock ml(transmitMutex);
		myName = Sys::readString(mySocket);
		if (StringUtil::empty(myName))
			myName = "anonymous-" + Properties::toString(myId);
		myFullName = myHost->getName() + ":" + myName;
		myInternalName = myHost->getAddress() + ":" + myName;

		// send id back ...

		Integer nMessageLength = 0;
		char * reply = (char *)DataBlockWriter::createConnectReply(myId, nMessageLength);
		if (send(mySocket, reply, nMessageLength,0) < 0)
			throw new SBWCommunicationException("send() failed");
		delete[] reply;

	}
	catch (SBWCommunicationException *e)
	{
		// This will happen if the remote has closed the connection.
		TRACE("Connection failure; can't read from remote Broker: " << e->getMessage());
		setException(e);
		delete e;
	}
	return true;
}
void ModuleInstance::deliverMessage(int destId, Instance *destInstance, unsigned char* message, int length)
{
	//SBWOSMutexLock ml(transmitMutex);
	if (destInstance == NULL)
	{
		TRACE(getNameForMessage() << " attempted to" << " contact nonexistent module " << destId);
		reportTransmitFailed(length, destId, message, NULL);
	}
	else            // Module exists, so transmit to it.
	{
		TRACE("Msg from " << getNameForMessage() << " -> " << destId);
		try
		{				 
			//SBWOSMutexLock ml(transmitMutex);
			ProxyLocalBrokerInstance *oInstance = dynamic_cast<ProxyLocalBrokerInstance *>(destInstance);
			if (oInstance != NULL)
			{
				destInstance->transmit(message, length);
			}
			else 
			{
				DataBlockWriter *oWriter = new DataBlockWriter;
				oWriter->getImpl()->packWithoutType(length);
				oWriter->getImpl()->packWithoutType(destInstance->getId());
				oWriter->insertData(message,length-8);
				Integer totalLength;
				unsigned char * finalMessage = const_cast<unsigned char*>(oWriter->getData(&totalLength));
				destInstance->transmit(finalMessage, length);
				delete oWriter;
				finalMessage = NULL;

			}
		}
		catch (...)
		{
			TRACE(getNameForMessage() << " unable to" << " send msg to " << destId);
			reportTransmitFailed(length, destId, message, NULL);
		}
	}

}
void ModuleInstance::transmit(unsigned char *data, int length)
{
	//SBWOSMutexLock ml(transmitMutex);
	if (send(mySocket, (char *) data, length,0) < 0)
		throw new SBWCommunicationException("send() failed");	

}
void ModuleInstance::reportTransmitFailed(int length, int destModuleId, unsigned char* message, SBWException *ex)
{
	//SBWOSMutexLock ml(transmitMutex);
	DataBlockReader oReader(message, length);
	unsigned char type;
	int messageId;
	oReader.getImpl()->getWithoutType(type);
	oReader.getImpl()->getWithoutType(messageId);
	if (type == CALL_CODE || type == SEND_CODE)
	{
		int nLength;
		unsigned char *newMessage;
		std::string sTemp;
		if (ex == NULL) {
			std::string a =  sTemp + "Module instance id " + Properties::toString(destModuleId) + " does not exist";
			std::string b = sTemp + "Transmission to unknown module instance id " + Properties::toString(destModuleId) + " detected in Broker instance" + " receiver thread";
			newMessage = DataBlockWriter::createException(
				myId, messageId, (new SBWCommunicationException(""))->getCode(),a.c_str(),
				b.c_str(),nLength);
		}
		else
		{	
			std::string a = sTemp + "Unable to transmit to module instance id " + Properties::toString(destModuleId);
			std::string b = sTemp + "Unable to transmit to module instance id "	+ Properties::toString(destModuleId) + ", reason: " + ex->getMessage();
			newMessage = DataBlockWriter::createException(
				myId, messageId, (new SBWCommunicationException(""))->getCode(), a.c_str(), b.c_str(),nLength);
		}
		try
		{
			transmit(newMessage, nLength);
			free (newMessage);
		}
		catch (...)
		{
			TRACE("Can't transmit exception back to " << getNameForMessage());
		}
	}
	else
	{
		// Don't need to send anything to our module, so just log a msg.

		std::string msg = "Trying to transmit reply/exception from " + getNameForMessage() + " to unknown instance" + Properties::toString(destModuleId);
		TRACE (msg);
	}

}
void ModuleInstance::Hook::onCreation(InstanceThread *instance)
{
	// Note that instance id's get incremented even though the
	// module startup may ultimately fail.  This seems safer,
	// to avoid possible duplicate id's, but should be
	// reexamined at some point.

	instance->myId = InstanceTable::getTable()->getNextId();
}
/**
* Callback invoked as the first step by the InstanceThread::run
* method.
* <p>
* @param instance the instance
**/
void ModuleInstance::Hook::onStartup(InstanceThread *instance) 
{
	SBWBrokerRPC *rpc = dynamic_cast<SBWBrokerRPC *>(Broker::SBW::getInternalAPI());
	if (rpc == NULL || rpc->ShuttingDown()) 
		return; // shutdown in progress;
	// Add the instance and notify others that it's started.

	InstanceTable::getTable()->addInstance((Instance *) instance);
	rpc->notifyOthersOfStartup((Instance*) instance, false);

	// Notify any watchers that this module has started up.
}
/**
* Callback invoked after the InstanceThread message loop
* has stopped being operational.
* <p>
* @param instance the instance
**/
void ModuleInstance::Hook::onShutdown(InstanceThread *instance) 
{
	try
	{
	SBWBrokerRPC *rpc = dynamic_cast<SBWBrokerRPC *>(Broker::SBW::getInternalAPI());
	if (rpc == NULL || rpc->ShuttingDown()) 
		return; // shutdown in progress;
	rpc->notifyOthersOfDisconnect((Instance *) instance, false);
	InstanceTable::getTable()->removeInstance((Instance *) instance );
	}
	catch(...)
	{
	}
}

