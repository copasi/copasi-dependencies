#ifndef SBW_BROKER_SERVERSOCKET
#define SBW_BROKER_SERVERSOCKET

#include "SBW/SBWOSSocket.h"
#if !defined(WIN32)
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <string>

namespace SystemsBiologyWorkbench

{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 *
		 * ServerSocket is a help class that provides basic Network IO operations.
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class ServerSocket 
		{
		public:
			
			/**
			 * create a server socket on a given host and given port 
			 * \param nPort the port to open
			 * \param sHost the name of the host. If empty a local port will be opened			 
			 */
			ServerSocket (int nPort, std::string sHost = "");
			/**
			 *
			 * \return the port opened
			 */
			int getServerPort() { return m_nPort; }
			/**
			 * blocks until a new connection arises
			 * \return the SOCKET
			 */
			SOCKET acceptClient();
			/**
			 *
			 * \return the server socket
			 */
			SOCKET getSocket() { return m_nServerSocket; }
			/**
			 *
			 * \return the address of the current client
			 */
			std::string getCurrentClientAddress();

		private:
			void throwError();
			int m_nPort;					///< the port to be opened
			SOCKET m_nServerSocket;			///< the server socket
			SOCKET m_nClientSocket;			///< the client socket

			struct sockaddr_in oServerAddr;	///< the address of the server
			struct sockaddr_in oClientAddr;	///< the address of the client

			int m_nClientLength;			///< the length of the client address

		};
	}
}


#endif
