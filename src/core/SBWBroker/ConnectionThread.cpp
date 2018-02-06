#include "SBW/SBWOSSocket.h"
#include "SBW/SBWCommunicationException.h"
#include "SBW/SBWApplicationException.h"

#include "ConnectionThread.h"
#include "BrokerInstance.h"
#include "ModuleInstance.h"
#include "Properties.h"


#ifdef WIN32
#include <io.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;

ConnectionThread::ConnectionThread(InstanceThread::Factory *_instanceFactory,
                   ServerSocket *_socket, const string& _sessionKey,
                   HostAccessTable *_hostAccessTable) : SBWThread( _instanceFactory->getType() +  "-connection-thread"),
								   serverSocket(_socket), 
                   sessionKey(_sessionKey),
								   hostAccessTable(_hostAccessTable),
								   instanceFactory(_instanceFactory)  

								   
{
	type = _instanceFactory->getType();
	portProperty = "sbw." + type + ".port";
	port = _socket->getServerPort();
	//this->instanceFactory = instanceFactory;
	//this->type = instanceFactory->getType();
	//this->serverSocket = socket;
	//this->sessionKey = sessionKey;
	//this->hostAccessTable = hostAccessTable;
	//this->portProperty = "sbw." + type + ".port";
	//this->port = socket->getServerPort();

#ifdef WIN32
	try
	{
		//Apparently when serverSocket.accept() is waiting (see the
		//run loop below), doing serverThread.interrupt() does not
		//interrupt this thread.  The only recourse seems to be to set
		//the socket timeout to a nonzero number and periodically get
		//out of our waiting state in our run() loop.
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if (setsockopt(serverSocket->getSocket(), SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval )) < 0)
			TRACE ("failed to set socket option")
			int nReceive=0; int nSockOpt=sizeof(nReceive);
		if (getsockopt(serverSocket->getSocket(), SOL_SOCKET, SO_RCVBUF, (char*)&nReceive, &nSockOpt) < 0)
			TRACE ("failed to get socket option")
			nReceive *=2;
		if (setsockopt(serverSocket->getSocket(), SOL_SOCKET, SO_RCVBUF, (char*)&nReceive, sizeof(nReceive)) < 0)
			TRACE ("failed to set socket option")
			if (getsockopt(serverSocket->getSocket(), SOL_SOCKET, SO_SNDBUF, (char*)&nReceive, &nSockOpt) < 0)
				TRACE ("failed to get socket option")
				nReceive *=2;
		if (setsockopt(serverSocket->getSocket(), SOL_SOCKET, SO_SNDBUF, (char*)&nReceive, sizeof(nReceive)) < 0)
			TRACE ("failed to set socket option")
	}
	catch (...)
	{
		throw new SBWCommunicationException( "Setting up server socket");
	}
#endif
	TRACE("Broker listening for " << type << "s on port " << port);
}
void ConnectionThread::run()
{
	TRACE(type << " server thread started.");

	// This keeps looping until operational = false, at which point it
	// runs a shutdown() at the end.  The server socket has a timeout
	// set on it, so that the serverSocket.accept() times out after a
	// time and the loop goes around.  Without this timeout, the thread
	// can't be interrupted while it's in the accept() call.

	operational = true;

	Properties::setProperty(portProperty, Properties::toString(port));
	Properties::save();

	while (operational)
	{
#ifdef WIN32
		SOCKET socket = NULL;
#else
		SOCKET socket = 0;
#endif

		// Wait until a socket connection is accepted.
		try
		{
			socket = serverSocket->acceptClient();			
#ifdef WIN32
			linger oLinger;
			oLinger.l_linger = 2;
			oLinger.l_onoff  = 1;
			int nNoDelay = 1;
			setsockopt(socket, SOL_SOCKET, SO_LINGER, (char *)&oLinger, sizeof(linger));
			setsockopt(socket, SOL_SOCKET, TCP_NODELAY, (char *)&nNoDelay, sizeof(int));
#endif
		}								
		catch (...)
		{
			// If we're NOT operational now, means we've been
			// interrupted because of a shutdown.  This is ok.
			if (operational)
				TRACE("Problem during socket accept");
			break;
		}
		// Got a socket connection; check if it's allowed & start it.

		TRACE("New socket connection accepted");
		try
		{
			if (hostAccessTable->checkAddress(serverSocket->getCurrentClientAddress()))
			{
				TRACE("New connection is from a " << type << " at " << serverSocket->getCurrentClientAddress());

				BrokerInstance::Factory *oBrokerFactory = dynamic_cast<BrokerInstance::Factory *>(instanceFactory);
				ModuleInstance::Factory *oModuleFactory = dynamic_cast<ModuleInstance::Factory *>(instanceFactory);
				InstanceThread *inst = NULL;

				if (oBrokerFactory != NULL) 
					inst = oBrokerFactory->newInstance(socket, new Host(serverSocket->getCurrentClientAddress()));
				else if (oModuleFactory != NULL)
					inst = oModuleFactory->newInstance(socket, new Host(serverSocket->getCurrentClientAddress()));
				else
					throw new SBWApplicationException("error creating instance - don't know what instance to create.");

        if (inst != NULL && (inst->connect(sessionKey.c_str()) && inst->start()))
				{
					TRACE("New instance " << inst->getId() << " started");
				}
				else
				{
					throw new SBWApplicationException("Error creating instance. Sessionkey check or instance startup failed.");
				}
			}
			else
			{
				TRACE("Refusing connection from " << serverSocket->getCurrentClientAddress());
				closesocket(socket);
			}
		}
		catch (SBWException *e)
		{
			TRACE("Connection received but startup failed" << e ->getMessage());
			delete e;
		}
		catch (...)
		{
			TRACE(type << " server socket exception for " << serverSocket->getCurrentClientAddress());
		}
	}

	// If we get here, we're no longer operational.

	TRACE(type << " server thread stopped.");
	disconnect();
}
bool ConnectionThread::isOperational()
{
	return operational;
}
void ConnectionThread::startThread()
{
	this->start();	
}
void ConnectionThread::disconnect()
{
	if (!operational)
		return;

	operational = false;

	try
	{
		TRACE("Closing " << type << " server socket.");
		closesocket(serverSocket->getSocket());
	}
	catch (...)
	{
		TRACE("Problem closing " << type << " server socket");
	}
	Properties::removeProperty(portProperty);
	Properties::save();
}
void ConnectionThread::shutdown()
{
	disconnect();	
}

