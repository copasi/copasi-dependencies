#ifndef SBW_BROKER_CONNECTION_THREAD
#define SBW_BROKER_CONNECTION_THREAD

#include "SBW/SBWOSSocket.h"
#include "SBW/portableOS.h"

#include "HostAccessTable.h"
#include "InstanceThread.h"
#include "ServerSocket.h"

#include <string> 


namespace SystemsBiologyWorkbench
{
	namespace Broker
	{	

		/**
		 * \ingroup C++Broker
		 *
		 *
		 * Class for handling incoming module connections.  This is
		 * instantiated and invoked from SBWBrokerRPC.
		 * <p>
		 * Module connections use a separate socket and thread from broker
		 * connections.  To isolate details of what is actually done with
		 * instances, the class constructor takes a number of arguments, including
		 * the socket on which to listen, and a factory object of type 
		 * InstanceThread::Factory for creating instances of the different type.
		 * As a result, the #run() method is very simple; it simply tests
		 * if an incoming connection is allowed based on the host access table and
		 * if so, it creates a new instance using 
		 * InstanceThread::Factory::newInstance.
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class ConnectionThread  : public SBWThread 
		{
		private :
			/** The server socket for the connections. **/
			ServerSocket *serverSocket;

			/** The port number on which the socket is listening. **/
			int port;

			/** The string we store in the properties file for the port number. **/
			std::string portProperty;

			/** An identifier for the type of connections: "broker" or "module".  **/
			std::string type;

			/** The session key to use for connections. **/
      std::string sessionKey;

			/** The host access table for connections of this type.  **/
			HostAccessTable *hostAccessTable;

			/** The factory object used to create new instances.  **/
			InstanceThread::Factory *instanceFactory;

			/** Flag indicating whether the connection thread is operational.**/
			bool operational;
		public:
			/**
			* Constructor; creates a new broker thread for this type of connection
			* (either "module" or "broker") listening on a particular port on the
			* local host.
			* <p>
			* @param instanceFactory the instance factory object used to create
			* a new instance when a connection is successful
			* @param socket the socket on which this connection thread is listening
			* @param sessionKey the session authentication key to use
			* @param hostAccessTable the host access table object used to check
			* whether a given host can make a connection
			* @throws SBWCommunicationException if there is a problem configuring
			* the server socket
			**/
			ConnectionThread(InstanceThread::Factory *instanceFactory,
        ServerSocket *socket, const std::string &sessionKey,
				HostAccessTable *hostAccessTable);
			/**
			* The <code>run()</code> method for this thread.  This is what is
			* executed when the thread is started.  It implements a socket
			* listener loop that continually calls <code>accept()</code> on the
			* server socket, waiting for new connections and calling the
			* starter object when new connections are finally made.  The loop
			* terminates when either an error occurs or a stopThread()
			* or disconnect() is called.
			**/
			void run();
			/**
			* Returns true if the broker connection thread is operational.
			* <p>
			* @return <code>true</code> if the connection thread is operational,
			* <code>false</code> otherwise
			**/
			bool isOperational();
			/**
			* Method to start the thread.  This is called externally when the
			* main Broker communications code is ready to start listening for
			* broker connections.  It's really just a wrapper around the
			* standard Thread#start method, but it deals with the
			* exceptions more nicely for the caller.
			**/
			void startThread();
			/**
			* Disconnect the broker connection socket.  This also sets the
			* #operational flag to <code>false</code> and removes the
			* port number from the runtime properties file.  If an exception
			* occurs, this logs it but doesn't throw to the caller, because if
			* this is being called then the Broker must already be in the process
			* of shutting down.
			**/
			void disconnect();
			/**
			* Shuts down the broker connection socket and stops the thread.
			* This gets called when the Broker is shutting down.
			**/
			void shutdown();
		};
	}
}

#endif
