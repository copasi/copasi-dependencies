#include "BrokerInstance.h"
#include "SBWBrokerRPC.h"
#include "SBW.h"
#include "ProxyLocalBrokerInstance.h"
#include "ProxyModuleInstance.h"
#include "BrokerInterface.h"

#include "SBW/SBWApplicationException.h"
#include "SBW/SessionKey.h"
#include "SBW/SBWCommunicationException.h"
#include "SBW/RuntimeProperties.h"

#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#endif

using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;

InstanceThread *BrokerInstance::Factory::newInstance(SOCKET socket, Host *host, InstanceThread::Hook *hook)
{
	return new BrokerInstance(socket, host, hook);
}
InstanceThread *BrokerInstance::Factory::newInstance(SOCKET socket,Host *host)
{
	return new BrokerInstance(socket, host, _hook);
}
InstanceThread::Factory *BrokerInstance::getFactory(InstanceThread::Hook *hook, Host *host)
{
	return new BrokerInstance::Factory(host,hook);
}
bool BrokerInstance::connect(const char* /*key*/)
{
	//SBWOSMutexLock ml(transmitMutex);
	TRACE("Attempting to connect & shake hands with remote broker");
	return connectStreams() && exchangeIds();
}
bool BrokerInstance::connectStreams()
{
	//SBWOSMutexLock ml(transmitMutex);
	TRACE("Trying to connect message streams");
	try
	{
		Integer length;
		string sKey = SessionKey::getKey();
		const char *key = sKey.c_str();
		unsigned char *message
			= DataBlockWriter::createConnectMessage(key, "BROKER", length);
#ifndef WIN32
		if ( send(mySocket,(char*)message,length,0) < 0)
#else	
		if ( ::send(mySocket,(char*)message,length,0) < 0)
#endif
		{
			delete[] message;
			return false;
		}
		delete[] message;

		string sRemoteKey = Sys::readString(mySocket);
		string sRemoteName = Sys::readString(mySocket);
		if (sRemoteKey == sKey)
		{
			TRACE("remote keys match");
		}
		else 
		{
			TRACE("remote key does not match Broker key");
		}

		TRACE("Successfully connected message streams");
		return true;
	}
	catch (...)
	{
		TRACE("Could not connect message streams; disconnecting");
		return false;
	}
}

bool BrokerInstance::exchangeIds()
{
	//SBWOSMutexLock ml(transmitMutex);
	int nLength = 0;
	unsigned char * array = DataBlockWriter::createConnectReply(myId,nLength);
	int remotePort = -1;
	string remoteHostName;
	string remoteHostAddress;
	try
	{
		// Tell the other broker the id that is assigned to it locally.
		if (::send(mySocket,(char*) array, nLength,0) < 0)
			throw new SBWSocketException("error ... sending ...");

		// Second, read what id the remote broker has assigned to us.
		remoteIdForLocalBroker = Sys::readInteger(mySocket);

		// tell the other broker the port we run on 
		delete [] array;
		array = DataBlockWriter::createConnectReply(RuntimeProperties::getSBWBrokerPort(),nLength);
		if (::send(mySocket,(char*) array, nLength,0) < 0)
			throw new SBWSocketException("error ... sending ...");

		// and read the remote port
		remotePort = Sys::readInteger(mySocket);

		// Next, send our host name to the remote broker
		delete [] array;
		array = DataBlockWriter::createConnectReply( const_cast<char *>(Sys::getHostName().c_str()),nLength);
		if (::send(mySocket,(char*) array, nLength,0) < 0)
			throw new SBWSocketException("error ... sending ...");

		// ... and read the other end's host name.

		remoteHostName = Sys::readString(mySocket);

		// Finally, send our IP address to the remote broker
		delete [] array;
		array = DataBlockWriter::createConnectReply( const_cast<char *>(Sys::getHostAddress(Sys::getHostName()).c_str()), nLength);
		if (::send(mySocket,(char*) array, nLength,0) < 0)
			throw new SBWSocketException("error ... sending ...");

		// ... and read the other end's host address.

		remoteHostAddress = Sys::readString(mySocket);
	}
	catch (...)
	{
		TRACE("Connection failure; can't write to remote broker");
		setException(new SBWCommunicationException(
			"Unable to write to remote Broker.",
			"The SBW Broker encountered a connection failure"\
			" while attempting to write a message to the"\
			" Broker located on '" + myHost->getName() + "'"));
		return false;
	}

	TRACE("Shook hands with remote broker " << myId);

	// Reset the host to what the remote machine knows itself by.
	// This allows to get the right host when using SSH tunneling.

	if (! StringUtil::empty(remoteHostName))
	{
		TRACE("Resetting host name to " << remoteHostName);
		if (Sys::lookupPossible(remoteHostName))
		{
			myHost = new Host(remoteHostName, remoteHostAddress);
		}
		else
		{
			myHost = new Host(remoteHostAddress);
		}
		
		myHost->setPort(remotePort);
		
		myFullName = myHost->getName() + ":BROKER";
		myInternalName = myHost->getAddress() + ":BROKER";
	}
	return true;
}
void BrokerInstance::deliverMessage(int destId, Instance *destInstance, unsigned char* message, int length) 
{
	SBWOSMutexLock ml(transmitMutex);
	if (destInstance == NULL)
	{
		TRACE(getNameForMessage() << " attempted to" << " contact nonexistent module " << destId);
		reportTransmitFailed(length-8, destId, message, NULL);
	}
	else            // Module exists, so transmit to it.
	{
		TRACE("Msg from " << getNameForMessage() << " -> " << destId);
		try
		{
			DataBlockReader data(message, length-8, true);
			DataBlockWriter header;

			unsigned char type;
			data.getImpl()->getWithoutType(type);

			header.getImpl()->packWithoutType(length);
			header.getImpl()->packWithoutType(destId);
			header.getImpl()->packWithoutType(type);
			int offset = 1;

			// If this is a call or send, we must rewrite the source id.
			// This means we have to read further into the msg.
			// Reply and error messages don't have source id's.

			if (type == CALL_CODE || type == SEND_CODE)
			{
				int messageId; int sourceId;

				data.getImpl()->getWithoutType(messageId);
				data.getImpl()->getWithoutType(sourceId);

				int localSourceId = getLocalIdForRemote(sourceId);

				TRACE("Original source id was " << sourceId << "; rewriting to " << localSourceId);

				header.getImpl()->packWithoutType(messageId);
				header.getImpl()->packWithoutType(localSourceId);
				offset += 8;
			}
			int nArrayLength = 0;
			unsigned char *array = const_cast<unsigned char*>(header.getData(&nArrayLength));
			int nTotalLength = (nArrayLength + length - 8 + 1 - offset);
			unsigned char* total = (unsigned char*) malloc(sizeof(unsigned char) * nTotalLength);
			if (total == NULL)
				throw new SBWApplicationException("couldn't allocate enough memory");
			memset(total, 0, sizeof(unsigned char) * nTotalLength);
			memmove(total, array, nArrayLength * sizeof(unsigned char));
			memmove(total+nArrayLength, message + offset, (length - 8 - offset) * sizeof(unsigned char));
			destInstance->transmit(total, nTotalLength - 1);
		}
		catch (...)
		{
			TRACE(getNameForMessage() << " unable to" << " send msg to " << destId);
			reportTransmitFailed(length-8, destId, message, NULL);
		}
	}

}
void BrokerInstance::transmit(unsigned char *data, int length)
{
	//SBWOSMutexLock ml(transmitMutex);
	TRACE("Transmitting to " << getNameForMessage());
	// Do this in two steps.  First, write the length and module id
	// fields, using the DataBlockWriter to pack them up appropriately.

	try
	{
		DataBlockWriter writer;

		writer.getImpl()->packWithoutType(length);
		writer.getImpl()->packWithoutType(BROKER_MODULE);

		int nArrayLength = 0;
		unsigned char *array = const_cast<unsigned char *>(writer.getData(&nArrayLength));

		TRACE("Transmitting to " << getNameForMessage());
		::send(mySocket,(char*)array, nArrayLength, 0);
		::send(mySocket,(char*)(data+8), length-8, 0);		
	}
	catch (...)
	{
		//
	}

}
void BrokerInstance::transmitDirect(char *raw, int offset, int length, bool addLength /*= true*/)
{
	//SBWOSMutexLock ml(transmitMutex);
	TRACE("Transmitting " << length << " bytes to " << getNameForMessage());
	if (addLength) {
		DataBlockWriter oWriter;
		oWriter.getImpl()->packWithoutType(length);
		::send(mySocket, (char*)const_cast<unsigned char *>(oWriter.getData()),4,0);
	}
	::send(mySocket, raw + offset, length,0);

}
DataBlockReader BrokerInstance::readDirect()
{
	//SBWOSMutexLock ml(transmitMutex);
	int length = Sys::readInteger(mySocket);
	unsigned char* message = (unsigned char*) malloc(sizeof(unsigned char) * length + 1);
	if (message == NULL)
		throw new SBWCommunicationException("couldn't allocate enough memory for message");

	memset(message,0,sizeof(unsigned char) * length + 1);
	Sys::receive(mySocket,message,length);
	return DataBlockReader(message,length,false);

}
void BrokerInstance::reportTransmitFailed(int length, int destModuleId, unsigned char* message, SBWException *ex)
{
	//SBWOSMutexLock ml(transmitMutex);
	// By the time this method is called, the length and destination
	// fields have been consumed by runMessageLoop() elsewhere in this
	// class.  The rest of the message is still in the input stream.
	// We need to slurp the message into a temporary buffer, read out
	// the type byte, messageId and sourceId, then possibly construct
	// an exception message to be sent to the originating module.

	DataBlockReader data(message,length);

	// Find out what kind of message this was, so we can decide whether
	// we need to send back an exception to the originating module.

	unsigned char type; int messageId; int sourceId;
	data.getImpl()->getWithoutType(type);

	// If the originating module was trying to do a call or send, tell
	// the module it failed.  If the message was a reply or error,
	// don't bother, because if the destination is gone, the source is
	// not going to care about a failure to deliver a reply or error.

	data.getImpl()->getWithoutType(messageId);
	data.getImpl()->getWithoutType(sourceId);

	// sourceId refers to the remote broker.
	// Remap it to something on the local broker.

	int localSourceId = getLocalIdForRemote(sourceId);
	TRACE("Original destination id was " << sourceId
		<< "; rewriting to " << localSourceId);

	if (type == CALL_CODE || type == SEND_CODE)
	{
		int messageLength = 0;
		unsigned char *newMessage = NULL;
		if (ex == NULL)
		{
			newMessage = DataBlockWriter::createException(
				localSourceId, messageId, SBWCommunicationException("").getCode(),
				("Module instance id " + Properties::toString(destModuleId) + " does not exist").c_str(),
				("Transmission to unknown module instance id "
				+ Properties::toString(destModuleId) + " detected in Broker instance"\
				" receiver thread").c_str(), messageLength);
		}
		else
		{
			newMessage = DataBlockWriter::createException(
				localSourceId, messageId, SBWCommunicationException("").getCode(),
				("Unable to transmit to module instance id "
				+ Properties::toString(destModuleId)).c_str(),
				("Unable to transmit to module instance id "
				+ Properties::toString(destModuleId) + ", reason: " + ex->getMessage()).c_str(), messageLength);
		}

		try
		{
			transmitDirect((char*)newMessage, 0, messageLength);
		}
		catch (...)
		{
			TRACE("Can't transmit exception back to module " << sourceId);
		}
		delete [] newMessage;

	}
	else
	{
		stringstream msg; msg << "Trying to transmit reply/exception from "
			<< getNameForMessage() << " to unknown instance" << destModuleId;

		if (ex == NULL)
		{
			TRACE(msg.str());
		}
		else
		{
			TRACE(msg.str() << " exception: " << ex->getMessage());
		}
	}

}
BrokerInterface *BrokerInstance::getBrokerInterface()
{	
	if (brokerInterface != NULL)
		return brokerInterface;
	else
	{
		// create new broker interface
		brokerInterface = new BrokerInterface(myId);
		return brokerInterface;
	}
}
int BrokerInstance::getRemoteId()
{
	return myRemoteId;
}
int BrokerInstance::getRemoteIdForLocalBroker()
{
	return remoteIdForLocalBroker;
}
int BrokerInstance::getLocalIdForRemote(int remoteModuleId)
{
	int id = (int) remoteIdMap.get(remoteModuleId);
	return id;
}
void BrokerInstance::setLocalIdForRemote(int remoteModuleId, int localId)
{
	remoteIdMap.put(remoteModuleId, localId);
}
void BrokerInstance::removeLocalIdForRemote(int remoteModuleId)
{
	remoteIdMap.remove(remoteModuleId);
}
void BrokerInstance::Hook::onCreation(InstanceThread *instance)
{
	instance->myId = InstanceTable::getTable()->getNextId();
}
void BrokerInstance::Hook::onStartup(InstanceThread *instance) 
{
	InstanceTable::getTable()->addInstance((Instance *) instance);

	// Exchange lists of running instances.
	SBWBrokerRPC *rpc = dynamic_cast<SBWBrokerRPC *>(Broker::SBW::getInternalAPI());
	rpc->exchangeModuleLists((BrokerInstance *) instance);
	rpc->notifyOthersOfStartup((Instance *) instance, true);
}
void BrokerInstance::Hook::onShutdown(InstanceThread *instance) 
{
	// If the local broker was started by this broker, we shut
	// down the local broker too.
	// FIXME: is this the right policy?

	SBWBrokerRPC *rpc = dynamic_cast<SBWBrokerRPC *>(Broker::SBW::getInternalAPI());
	std::string remote = Properties::getProperty("sbw.broker.remote-from");
	bool shutdown = false;

	if (! StringUtil::empty(remote)
		&& instance->getHost()->equals(new Host(remote)))
		shutdown = true;

	// remove modules from this broker first
	vector < Instance* > remoteModules = InstanceTable::getTable()->getInstances(instance->getId());
	vector <Instance *>::iterator oIterator;
	for (oIterator = remoteModules.begin(); oIterator != remoteModules.end(); oIterator++)
	{
		Instance* oInstance = *oIterator;
		ProxyModuleInstance *oModule = dynamic_cast<ProxyModuleInstance *>(oInstance);
		try
		{
			if (oModule != NULL)
			{
				InstanceTable::getTable()->removeInstance(oModule);
				rpc->notifyOthersOfDisconnect(oModule,true);
			}
		}
		catch(...)
		{
			//
		}
	}

	// then remove this broker instance	
	InstanceTable::getTable()->removeInstance((Instance *) instance);

	if (shutdown)
	{
		TRACE ("The Broker that started this Broker has shut down -- this Broker will shut down too");
		rpc->pleaseStop();
	}
	rpc->notifyOthersOfDisconnect((Instance *) instance, true);
}

