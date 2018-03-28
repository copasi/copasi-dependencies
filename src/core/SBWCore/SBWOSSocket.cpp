/**
 * @file SBWOSSocket.cpp
 * @brief platform independant interface to sockets
 * 
 * This file is part of SBW.  Please visit http://sbw.sf.org for more
 * information about SBW, and the latest version of libSBW.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the New BSD license.
 *
 * Copyright (c) 2010-2014, Frank T. Bergmann and 
 *                          University of Washington
 * Copyright (c) 2008-2010, University of Washington and 
 *                          Keck Graduate Institute.
 * Copyright (c) 2005-2008, Keck Graduate Institute.
 * Copyright (c) 2001-2004, California Institute of Technology and
 *               Japan Science and Technology Corporation.
 * 
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are 
 * met: 
 * 
 * 1. Redistributions of source code must retain the above 
 *    copyright notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 
 * 3. Neither the name of the copyright holder nor the names of its 
 *    contributors may be used to endorse or promote products derived from 
 *    this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * The original code contained here was initially developed by:
 *
 *     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri
 *     The Systems Biology Workbench Development Group
 *     ERATO Kitano Systems Biology Project
 *     Control and Dynamical Systems, MC 107-81
 *     California Institute of Technology
 *     Pasadena, CA, 91125, USA
 *
 *
 * Contributor(s):
 *
 */


// SBWOSSocket.cpp: implementation of the SBWOSSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "config.h"
#include "SBWRawException.h"
#include <string.h>

#if defined(WIN32)
#pragma comment(lib,"ws2_32.LIB")
#include <winsock2.h>
#else
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#include <unistd.h>
#endif
#include "SBWOSSocket.h"

#include <SBW/portableOS.h>



#ifndef INADDR_NONE
#define INADDR_NONE ((unsigned long) -1)
#endif

#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#endif 

//#ifdef WIN32
//#include <string.h>
//#include <iphlpapi.h>
//#include <iptypes.h>
//#include <ws2tcpip.h>
//#endif

#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
/**
* WIN32 specific - initializes the WIN32 WSA system
*/
SBWOSSocketSystem::SBWOSSocketSystem()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 3, 0 );

	err = WSAStartup( wVersionRequested, &wsaData );
	//assert ( err == 0 );
}

/**
* WIN32 specific - shutdown WSA system
*/
SBWOSSocketSystem::~SBWOSSocketSystem()
{
	WSACleanup();
}

/// WIN32 specific - manages the WSA system
SBWOSSocketSystem SBWOSSocket::system;
#endif // WIN32

/**
* Create a socket and connect to the given port on this host.
* @param port server port on hostname to connect to.
* @param hostname machine to connect to, "" implies local.
*/
SBWOSSocket::SBWOSSocket(Integer port, const char *hostname)
{
	if (hostname == NULL || strcmp(hostname , "") == 0)
	{
		TRACE("Opening socket to localhost");
	}
	else
	{
		TRACE("Opening socket to host named '" << hostname << "'");
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		TRACE("Could not create socket");
		throwError();
	}

	sockaddr_in address ;

	address.sin_family = AF_INET;// short
	address.sin_port = htons((unsigned short)port) ; // u_short

#if defined(WIN32)
	unsigned long hostAddress ;
#else
	in_addr_t hostAddress ;
#endif

	if ( isLocalAddress(hostname))
	{
		// if we are running on a local address bind to 127.0.0.1 this solves
		// a communication problem that arose after hibernate/standby of a machine
		//
		hostAddress = inet_addr("127.0.0.1");
		//TRACE("Host address = " << SBWOSSocket::getHostAddress());
		//hostAddress = inet_addr(SBWOSSocket::getHostAddress().c_str());
	}
	else
	{
		TRACE("Trying to obtain address for host '" << hostname << "'");
		std::string given(hostname);
		std::string::size_type first = given.find_first_not_of("1234567890.");

		if (first == std::string::npos)
		{
			hostAddress = inet_addr(hostname) ;
		}
		else
		{
			struct hostent *hptr = gethostbyname(hostname);

			if (hptr == NULL)
			{
				TRACE("Failed to get host entry from gethostbyname");
				throwError();
			}

			memcpy(&hostAddress, hptr->h_addr_list[0], sizeof(hostAddress));
		}
	}

#if defined(WIN32)
	address.sin_addr.S_un.S_addr = hostAddress; 
	if (address.sin_addr.S_un.S_addr == INADDR_NONE)
		throw new SBWSocketException("address error");
#else
	address.sin_addr.s_addr = hostAddress; 
	if (address.sin_addr.s_addr == INADDR_NONE)
		throw new SBWSocketException("address error");
#endif

	TRACE("Connecting socket");
	if (connect(sock, (sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
		throwError();
}

/**
* close/disconnect socket
*/
SBWOSSocket::~SBWOSSocket()
{
	TRACE("Destroying socket object");
#ifdef WIN32
	WSAEVENT event = WSACreateEvent();

  if (event == WSA_INVALID_EVENT)
    return;

  if (WSAEventSelect(sock, event, FD_CLOSE) == SOCKET_ERROR)
    return;
#endif

  if (shutdown(sock, SD_SEND) == SOCKET_ERROR)
    return;

#ifdef WIN32
	if (WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE)
		== WSA_WAIT_FAILED)
	{
    return;
	}
#endif

  if (closesocket(sock) == SOCKET_ERROR)
    return;
}

/**
* receive data from socket.
* Blocks until all request bytes have been received or disconnection occurs.
* @param data buffer to receive data
* @param length size of buffer and number of bytes requested.
*/
void SBWOSSocket::receive(unsigned char *data, Integer length)
{
	Integer cursor = 0 ;

	while (cursor != length)
	{
		Integer result = recv(sock, (char *)data + cursor, length - cursor, 0);

		if (result == 0)
			throw new SBWDisconnectException("graceful disconnect while reading");

		if (result == SOCKET_ERROR)
			throwError();

		cursor += result ;
	}
}

/**
* transmit data to socket.
* Blocks until all bytes have been transmitted or disconnection occurs.
* @param data data to be transmitted.
* @param length number of bytes to be transmitted.
*/
void SBWOSSocket::transmit(unsigned char *data, Integer length)
{
	Integer cursor = 0;

	while (cursor != length)
	{
		Integer result = send(sock, (char *)data + cursor, length - cursor, 0);

		if (result == SOCKET_ERROR)
			throwError();

		cursor += result ;
	}
}

/**
* throws an SBWException * corresponding to the current error condition of this socket.
* @throw an SBWException * corresponding to the current error condition of this socket.
*/
void SBWOSSocket::throwError()
{
#if defined(WIN32)
	int code = WSAGetLastError();

	switch (code)
	{
	case WSANOTINITIALISED :
		throw new SBWSocketException("A successful WSAStartup must occur before using this function.");

	case WSAENETDOWN :
		throw new SBWDisconnectException("The network subsystem or the associated service provider has failed.");

	case WSAEAFNOSUPPORT :
		throw new SBWSocketException("The specified address family is not supported.");

	case WSAEINPROGRESS :
		throw new SBWSocketException("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");

	case WSAEMFILE :
		throw new SBWSocketException("No more socket descriptors are available.");

	case WSAENOBUFS :
		throw new SBWSocketException("No buffer space is available. The socket cannot be created.");

	case WSAEPROTONOSUPPORT :
		throw new SBWSocketException("The specified protocol is not supported.");

	case WSAEPROTOTYPE :
		throw new SBWSocketException("The specified protocol is the wrong type for this socket.");

	case WSAESOCKTNOSUPPORT :
		throw new SBWSocketException("The specified socket type is not supported in this address family.");

	case WSAEADDRINUSE :
		throw new SBWSocketException("The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs when executing bind, but could be delayed until this function if the bind was to a partially wild-card address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function."); 

	case WSAEINTR :
		throw new SBWSocketException("The (blocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall."); 

	case WSAEALREADY :
		throw new SBWSocketException("A nonblocking connect call is in progress on the specified socket.");

	case WSAEADDRNOTAVAIL :
		throw new SBWSocketException("The remote address is not a valid address (such as ADDR_ANY).");

	case WSAECONNREFUSED :
		throw new SBWConnectException("The attempt to connect was forcefully rejected.");

	case WSAEFAULT :
		throw new SBWSocketException("The name or the namelen parameter is not a valid part of the user address space, the namelen parameter is too small, or the name parameter contains incorrect address format for the associated address family."); 

	case WSAEINVAL :
		throw new SBWSocketException("The parameter s is a listening socket, or the destination address specified is not consistent with that of the constrained group the socket belongs to."); 

	case WSAEISCONN :
		throw new SBWSocketException("The socket is already connected (connection-oriented sockets only).");

	case WSAENETUNREACH :
		throw new SBWDisconnectException("The network cannot be reached from this host at this time.");

	case WSAENOTSOCK :
		throw new SBWSocketException("The descriptor is not a socket.");

	case WSAETIMEDOUT :
		throw new SBWConnectException("Attempt to connect timed out without establishing a connection.");

	case WSAEWOULDBLOCK :
		throw new SBWSocketException("The socket is marked as nonblocking and the connection cannot be completed immediately."); 

	case WSAEACCES :
		throw new SBWSocketException("Attempt to connect datagram socket to broadcast address failed because setsockopt option SO_BROADCAST is not enabled."); 

	case WSAENOTCONN :
		throw new SBWSocketException("The socket is not connected.");

	case WSAENETRESET :
		throw new SBWDisconnectException("The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress.");

	case WSAEOPNOTSUPP :
		throw new SBWSocketException("MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, out-of-band data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.");

	case WSAESHUTDOWN :
		throw new SBWDisconnectException("The socket has been shut down; it is not possible to recv on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH."); 

	case WSAEMSGSIZE :
		throw new SBWSocketException("The message was too large to fit into the specified buffer and was truncated.");

	case WSAECONNABORTED :
		throw new SBWDisconnectException("The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.");

	case WSAECONNRESET :
		throw new SBWDisconnectException("The virtual circuit was reset by the remote side executing a \"hard\" or \"abortive\" close. The application should close the socket as it is no longer usable. On a UDP datagram socket this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message."); 

	case WSAEHOSTUNREACH :
		throw new SBWDisconnectException("The remote host cannot be reached from this host at this time."); 

	case WSAHOST_NOT_FOUND :
		throw new SBWConnectException("Authoritative answer host not found");

	case WSATRY_AGAIN :
		throw new SBWConnectException("Nonauthoritative host not found, or server failure");

	case WSANO_RECOVERY :
		throw new SBWConnectException("A nonrecoverable error occurred");

	case WSANO_DATA :
		throw new SBWConnectException("Valid name, no data record of requested type");


	default:
		throw new SBWSocketException("Unknown code");

	}

#elif defined(HAVE_ERRNO_H)

	switch (errno)
	{
	case EPROTONOSUPPORT:
	case EAFNOSUPPORT:
	case EPERM:
	case ENFILE:
	case EMFILE:
	case ENOBUFS:
	case ENOMEM:
	case EINVAL:
	case EACCES:
	case EFAULT:
	case ENOTCONN:
	case ENOTSOCK:
	case EBADF:
	case EADDRINUSE:
		throw new SBWSocketException(strerror(errno));

	case ECONNREFUSED:
	case EISCONN:
	case ETIMEDOUT:
	case EINPROGRESS:
	case ENETUNREACH:
	case EALREADY:
	case EAGAIN:
		throw new SBWConnectException(strerror(errno));

	default:
		throw new SBWSocketException("Unknown code");

	}

#endif
}
std::string SBWOSSocket::getHostName()
{
	std::string thisHost;
	char shortname[256];
	int nResult = gethostname(shortname, 255);
	if (  nResult  >= 0)
	{
		thisHost = shortname;		
	} 
	return thisHost;
}
std::string SBWOSSocket::getHostAddress(std::string sAddress)
{
	std::string host;
	int nPos = host.find('/');
	if (nPos != (int)host.npos)
	{
		host = sAddress.substr(0,nPos);
	}
	else
	{
		host = sAddress;
	}

	hostent* oHost = gethostbyname(host.c_str());
	if (oHost == NULL)
	{
		TRACE("couldn't get host address");
		return host;
	}
	unsigned long addr = *(( unsigned long *) oHost->h_addr_list[0]);

	in_addr oTemp;
#ifdef WIN32
	oTemp.S_un.S_addr = addr;
#else
	oTemp.s_addr = (in_addr_t) addr;
#endif
	std::string sResult ( inet_ntoa(oTemp ));	
	return sResult;
}
std::string SBWOSSocket::getHostAddress()
{	
	return getHostAddress(getHostName());
}
std::vector< std::string > SBWOSSocket::getLocalAddresses()
{
	vector<string> result; 
#ifdef WIN32

	char szHostName[255];
	gethostname(szHostName, 255);

	struct hostent *host_entry;
	host_entry=gethostbyname(szHostName);

	char * szLocalIP;
	struct in_addr addr;

	int i = 0;
	 i = 0;
        if (host_entry->h_addrtype == AF_INET)
        {
            while (host_entry->h_addr_list[i] != 0) {
				{
					addr.s_addr = *(u_long *) host_entry->h_addr_list[i++];
					szLocalIP = inet_ntoa (addr);
					//printf("address %s\n", szLocalIP);
					result.push_back(string(szLocalIP));
				}
			}
		}
	
	
	
	//printf("address %s\n", szLocalIP);


	//PIP_ADAPTER_ADDRESSES pAddresses;
	//pAddresses = (IP_ADAPTER_ADDRESSES*) malloc(sizeof(IP_ADAPTER_ADDRESSES));
	//memset(pAddresses, 0, sizeof(IP_ADAPTER_ADDRESSES));
	//ULONG outBufLen = 0;
	//DWORD dwRetVal = 0;

	//// Make an initial call to GetAdaptersAddresses to get the 
	//// size needed into the outBufLen variable
	//if (GetAdaptersAddresses(AF_INET,  GAA_FLAG_INCLUDE_PREFIX,  NULL,  pAddresses,  &outBufLen) == ERROR_BUFFER_OVERFLOW) 
	//{
	//	free(pAddresses);
	//	pAddresses = (IP_ADAPTER_ADDRESSES*) malloc(outBufLen);
	//	memset(pAddresses, 0, outBufLen);
	//}
	//PIP_ADAPTER_PREFIX pIpAdapterPrefix;
	//int i;

	//// Make a second call to GetAdapters Addresses to get the
	//// actual data we want
	//if ((dwRetVal = GetAdaptersAddresses(AF_INET, 
	//	GAA_FLAG_INCLUDE_PREFIX, 
	//	NULL, 
	//	pAddresses, 
	//	&outBufLen)) == NO_ERROR) {
	//		// If successful, output some information from the data we received
	//		while (pAddresses) {
	//			//pAddresses->
	//			printf("\tFriendly name: %S\n", pAddresses->FriendlyName);
	//			printf("\tDescription: %S\n", pAddresses->Description);

	//			for ( i = 0, pIpAdapterPrefix= pAddresses->FirstPrefix;
	//				pIpAdapterPrefix;
	//				i++, pIpAdapterPrefix = pIpAdapterPrefix->Next) {
	//					char szAddress[NI_MAXHOST];

	//					if (getnameinfo(pIpAdapterPrefix->Address.lpSockaddr,
	//						pIpAdapterPrefix->Address.iSockaddrLength,
	//						szAddress, sizeof(szAddress), NULL, 0,
	//						NI_NUMERICHOST)) {
	//							fprintf(stderr,
	//								"can't convert network format to presentation format");
	//							exit(1);
	//						}

	//						printf("  Prefix#%d\n", i);
	//						switch((pIpAdapterPrefix->Address.lpSockaddr)->sa_family) {
	//	case AF_INET:
	//		printf("    Family=IPv4, Addr=%s\n", szAddress);
	//		break;
	//	case AF_INET6:
	//		printf("    Family=IPv6, Addr=%s/64\n", szAddress);
	//		break;
	//	default:
	//		printf("    Family=Unknown, Addr=%s\n", szAddress);
	//						}
	//				}


	//				pAddresses = pAddresses->Next;



	//		}
	//	}

#else
	struct ifaddrs * ifAddrStruct=NULL;
	void * tmpAddrPtr=NULL;
	char addressBuffer[20];
	getifaddrs(&ifAddrStruct);
	while (ifAddrStruct!=NULL) 
	{
		if (ifAddrStruct->ifa_addr->sa_family==AF_INET )
		{ // check it is IP4 and not lo0
			// is a valid IP4 Address
			tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
			result.push_back( string(inet_ntop(AF_INET, tmpAddrPtr, addressBuffer,20)));
		}
		ifAddrStruct=ifAddrStruct->ifa_next;
	}
#endif
	return result;
}
bool SBWOSSocket::isLocalAddress(const char *hostname)
{
	if (hostname == NULL) return true;
	//cout << "isLocalAddress(" << hostname << "), address: " << getHostAddress() << " name: " << getHostName() << endl;
	static vector<string> localAddresses = getLocalAddresses();
	vector<string>::iterator it; 
	for (it = localAddresses.begin(); it != localAddresses.end(); it++)
		if (strcmp((*it).c_str(), hostname)) return true;

	return strlen(hostname) == 0 || strcmp(hostname, "localhost") == 0
		|| strcmp(hostname, "127.0.0.1" ) == 0
		|| strcmp(hostname, getHostAddress().c_str())  == 0
		|| strcmp(hostname, getHostName().c_str())  == 0;
}

bool SBWOSSocket::portInUse(int port)
{

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	bool inUse = false;

	if (sock > 0)
	{
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));		
		addr.sin_family = AF_INET;
		addr.sin_port = htons((unsigned short)port);

#if defined(WIN32)
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (addr.sin_addr.S_un.S_addr == INADDR_NONE)
			throw new SBWSocketException("address error");
#else
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		if (addr.sin_addr.s_addr == INADDR_NONE)
			throw new SBWSocketException("address error");
#endif

		// A failure to bind to the socket indicates it's in use (or unusable),
		// but a successful bind is not enough to say that it's NOT in use.

		int bindCode = bind(sock, (struct sockaddr *) &addr, sizeof(addr));

#if defined(WIN32)
		if (bindCode != 0 && WSAGetLastError() == WSAEADDRINUSE)
#elif defined(HAVE_ERRNO_H)
		if (bindCode != 0 && errno == EADDRINUSE)
#endif
		{
			TRACE("Port " << port << " appears to be in use");
			inUse = true;
		}
		else
		{
			// Binding was successful.  Now try listening on the port.
			// If we can listen, then it's not in use.

			int listenCode = listen(sock, 1);
#if defined(WIN32)
			if (listenCode != 0 && WSAGetLastError() == WSAEADDRINUSE)
#elif defined(HAVE_ERRNO_H)
			if (listenCode != 0 && errno == EADDRINUSE)
#endif
			{
				TRACE("Port " << port << " appears to be in use");
				inUse = true;
			}
		}
		closesocket(sock);
		return inUse;
	}

	return inUse;
}
