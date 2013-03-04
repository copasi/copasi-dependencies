#ifndef SBW_SYS
#define SBW_SYS

#include "SBW/SBWOSSocket.h"

#if defined(DARWIN) || defined(SOLARIS)
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <string>
#include <iostream>
#include <vector>

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 *
		 * Utility Classes for network communication & more. 
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class Sys
		{
		public:

			/**
			 *
			 * \return the name of the local host
			 */
			static std::string getHostName();
			/**
			 *
			 * \return the local host address
			 */
			static std::string getHostAddress();
			/**
			 * return the address of a given host
			 * \param host the host to look up
			 * \return the address of a given host
			 */
			static std::string getHostAddress(std::string host);

			/**
			 * returns a fully qualified host name for a given host
			 * \param host the host
			 * \return the fully qualified name
			 */
			static std::string fullyQualifiedHostName(std::string host);
			
			/**
			 * this function checks, whether a lookup of the given name is possible 
			 * \param sHost the host to look up
			 * \return <b>true</b> if a lookup is possible, <b>false</b> otherwise 
			 */
			static bool lookupPossible(std::string sHost);

			/**
			 * finds a free port in the given range
			 * \param portRange the range
			 * \param remoteAccessible *not used*
			 * \return the next free port
			 */
			static int findFreePort(std::vector<int> portRange, bool remoteAccessible);
			/**
			 * checks whether a specific port is in use or not
			 * \param port the port to check
			 * \param remoteAllowed *not used*
			 * \return <b>true</b> if port is in use, <b>false</b> otherwise
			 */
			static bool portInUse(int port, bool remoteAllowed);	
			static bool portInUse(int port);
			/**
			 * receive a message of given length from the socket and store it 
			 * \param sock the socket to read from
			 * \param *data the storage for the data read 
			 * \param length the length of data to be read
			 */
			static void receive(SOCKET sock, unsigned char *data, Integer length);
			/**
			 * reads an integer (4 Byte) from a socket
			 * \param sock the socket to read from
			 * \return the integer
			 */
			static int  readInteger(SOCKET sock);
			/**
			 * reads a sbw string from a socket. Assumes that length integer will be sent first
			 * \param sock the socket to read from
			 * \return the string read
			 */
			static std::string readString (SOCKET sock);
			/**
			 * checks whether all Broker methods are remote accessible. This cann be allowed by compiling
			 * the project with the option INSECURE
			 * \return true if INSECURE was set false otherwise.
			 */
			static bool isSecure();

			/**
			 * this function creates a directory recursively
			 * \param sDir teh directory to create
			 */
			static void createDirectory(std::string sDir);

		private :
			/**
			 * checks whether the given string represents an Address
			 * \param str the given string
			 * \return <b>true</b> if string represents an address. 
			 */
			static bool isAddress(std::string str);		
			Sys(){ }// No op.
		};
	}
}
#endif
