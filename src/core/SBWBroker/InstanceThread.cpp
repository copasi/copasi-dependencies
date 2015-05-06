#include "SBW/SBWOSSocket.h"
#include "SBW/DataBlockReader.h"
#include "SBW/SBWCommunicationException.h"
#include "SBW/SBWApplicationException.h"
#include "SBWBrokerRPC.h"

#include "InstanceThread.h"
#include "BrokerDefines.h"
#include "BrokerInstance.h"
#include "ModuleInstance.h"
#include "SBW.h"

#include "Sys.h"
#ifdef WIN32
#include <io.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;
using namespace std;

void InstanceThread::Hook::onCreation(InstanceThread *instance)
{
	BrokerInstance::Hook * oBroker = dynamic_cast<BrokerInstance::Hook*>(this);
	ModuleInstance::Hook * oModule = dynamic_cast<ModuleInstance::Hook *>(this);

	if (oBroker != NULL) 
	{
		oBroker->onCreation(instance);
	} 
	else if( oModule != NULL) 
	{
		oModule->onCreation(instance);
	}
	else
	{
		throw new SBWApplicationException("onCreation failed");
	}
}
void InstanceThread::Hook::onStartup(InstanceThread *instance)
{
	BrokerInstance::Hook * oBroker = dynamic_cast<BrokerInstance::Hook*>(this);
	ModuleInstance::Hook * oModule = dynamic_cast<ModuleInstance::Hook *>(this);

	if (oBroker != NULL) 
	{
		oBroker->onStartup(instance);
	} 
	else if( oModule != NULL) 
	{
		oModule->onStartup(instance);
	}
	else
	{
		throw new SBWApplicationException("onStartup failed");
	}
}
void InstanceThread::Hook::onShutdown(InstanceThread *instance)
{
	BrokerInstance::Hook * oBroker = dynamic_cast<BrokerInstance::Hook*>(this);
	ModuleInstance::Hook * oModule = dynamic_cast<ModuleInstance::Hook *>(this);

	if (oBroker != NULL) 
	{
		oBroker->onShutdown(instance);
	} 
	else if( oModule != NULL) 
	{
		oModule->onShutdown(instance);
	}
	else
	{
		throw new SBWApplicationException("onShutdown failed");
	}
}
InstanceThread *InstanceThread::Factory::newInstance(SOCKET socket, Host *host)
{
	BrokerInstance::Factory * oBroker = dynamic_cast<BrokerInstance::Factory *>(this);
	ModuleInstance::Factory * oModule = dynamic_cast<ModuleInstance::Factory *>(this);

	if (oBroker != NULL) 
	{
		return oBroker->newInstance(socket, host);
	} 
	else if( oModule != NULL) 
	{
		return oModule->newInstance(socket, host);
	}
	else
	{
		throw new SBWApplicationException("not sure what instance to create");
	}
}
std::string InstanceThread::Factory::getType()
{
	BrokerInstance::Factory * oBroker = dynamic_cast<BrokerInstance::Factory *>(this);
	ModuleInstance::Factory * oModule = dynamic_cast<ModuleInstance::Factory *>(this);

	if (oBroker != NULL) 
	{
		return oBroker->getType();
	} 
	else if( oModule != NULL) 
	{
		return oModule->getType();
	}
	else
	{
		throw new SBWApplicationException("encountered unknown factory type.");
	}
}
InstanceThread::InstanceThread(SOCKET socket, std::string type, Hook *hook) : SBWThread(type)
{
	mySocket = socket;
	myType = type;
	myInstanceHooks.push_back(hook);	
	instanceTable = InstanceTable::getTable();

	runCreationHooks();
}
bool InstanceThread::start()
{
	SBWThread::start();

	// When the run() method reaches the point where it's ready,
	// it sets operational to true.

	int waitTime = startupWaitTime;
	while (! started && ! operational && waitTime-- > 0)
	{
		try
		{
			SBWThread::sleep(100);
		}
		catch (...)
		{
			return true;
		}
	}

	if (! started && ! operational)
	{
		try
		{
			std::string msg = "Failed to start " + getNameForMessage();
			std::string detailed = "The SBW Broker successfully contacted "
				+ getNameForMessage() + " but then was unable to"
				+ " start the message-handling thread.  It is"
				+ " likely that" + getNameForMessage() + " was"
				+ " interrupted shortly after the connection was"
				+ " established, or perhaps a network failure"
				+ " occurred.";

			setException(new SBWModuleStartException(msg, detailed));
			TRACE(msg);

			return false;
		}
		catch (...)
		{
			// Can't actually happen.
			return false;
		}
	}
	else
	{
		return started;
	}
}
void InstanceThread::run()
{
	TRACE("Starting instance thread for " << getNameForMessage());

	runStartupHooks();

	runMessageLoop();               // Won't exit until instance is done.

	disconnectInstance();
	runShutdownHooks();
	shutdownInstanceThread();

	TRACE("Terminated instance thread for " << getNameForMessage());
}
void InstanceThread::runMessageLoop()
{
	unsigned char *message = NULL;
	started = true;
	operational = true;

	while (operational)
	{
		TRACE("Waiting for next msg from " << getNameForMessage());
		try
		{
			// We block on attempting to read from the socket until new
			// data is available.  Once data is available, the first
			// item in the msg from our module is the message length.
			// the size of the message received

			Integer length = Sys::readInteger(mySocket);
			if (length == DISCONNECT_MESSAGE)
			{
				TRACE(getNameForMessage() << " disconnecting");
				operational = false;
				break;
			}
			int destId = Sys::readInteger(mySocket);

			Instance *destInstance = instanceTable->getInstance(destId);

			// This is the key hand-off step in message handling.  This
			// invokes the delivery method for the destination
			// instance.  Note this may be on a different kind of
			// instance object than the object running this message
			// loop.
			message = (unsigned char *) malloc ( length * sizeof(unsigned char));
			if (message == NULL)
				throw new SBWCommunicationException("couldn't allocate enough memory for message");
			memset(message, 0, length * sizeof(unsigned char));
			try
			{
				Sys::receive(mySocket, message, length-8);
				deliverMessage(destId, destInstance, message, length);
			}
			catch (SBWCommunicationException *e)
			{
				free (message);
				message = NULL;
				throw e;
			}
			free (message);
			message = NULL;
		}
		catch (SBWCommunicationException *e)
		{
			// We've lost the connection to the module.

			TRACE("Lost connection to " << getNameForMessage() << e->getMessage());
			operational = false;
			delete e;
		}
		catch (SBWException *e)
		{
			// We've lost the connection to the module.

			TRACE("Lost connection to " << getNameForMessage() << e->getMessage());
			operational = false;
			delete e;
		}
		catch (...)
		{
			TRACE("Exception in instance handler for " << getNameForMessage() <<  "while reading msg stream ");
			operational = false;
		}
		if (message != NULL) free (message);
	}
}
void InstanceThread::disconnect()
{
	if (! operational && ! started)
	{
		TRACE("disconnect() called, but thread is not operational");
		return;
	}

	// oups ... got no streams in the first place ...
	TRACE("Closing streams for " << getNameForMessage());

	operational = false;
}
void InstanceThread::disconnectInstance()
{
	disconnect();

	TRACE("Closing socket for " << getNameForMessage());
	if (closesocket(mySocket) == SOCKET_ERROR)
		TRACE("error closing socket");
}
void InstanceThread::shutdown()
{
	TRACE("Issuing shutdown system call to " << getNameForMessage());

	DataBlockWriter args;
	try
	{
		SBWBrokerRPC *rpc = dynamic_cast<SBWBrokerRPC *>(Broker::SBW::getInternalAPI());
		rpc->send(myId, SYSTEM_SERVICE,
			SHUTDOWN_METHOD, args);
	}
	catch (SBWException *e)
	{
		std::string msg = "Unable to send shutdown to module ";
		TRACE (msg << myId << " " <<  e->getMessage());
		delete e;
	}
}
void InstanceThread::shutdownInstanceThread()
{
	TRACE("Shutting down thread ");
	// If we're being called at the end of run(), no need to do anything.
	operational = false;
}

void InstanceThread::runCreationHooks()
{
	TRACE("Running creation hooks for new instance");
	for (unsigned int i = 0; i <  myInstanceHooks.size(); i++ ) 
	{
		InstanceThread::Hook *oHook = myInstanceHooks[i];
		if (oHook != NULL)
		{
			oHook->onCreation(this);
		}
	}
}
void InstanceThread::runStartupHooks()
{
	TRACE("Running startup hooks for " << getNameForMessage());
	for (unsigned int i = 0; i <  myInstanceHooks.size(); i++ ) 
	{
		InstanceThread::Hook *oHook = myInstanceHooks[i];
		if (oHook != NULL)
		{
			oHook->onStartup(this);
		}
	}
}
void InstanceThread::runShutdownHooks()
{
	TRACE("Running shutdown hooks for " << getNameForMessage());
	for (unsigned int i = 0; i <  myInstanceHooks.size(); i++ ) 
	{
		InstanceThread::Hook *oHook = myInstanceHooks[i];
		if (oHook != NULL)
		{
			oHook->onShutdown(this);
		}
	}
}

