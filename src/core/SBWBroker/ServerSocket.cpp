#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include "ServerSocket.h"
#include "SBW/SBWOSSocket.h"
#include "SBW/SBWException.h"
#include "SBW/SBWRawException.h"
#include "SBW/SBWCommunicationException.h"
#include "SBW/portableOS.h"

#if defined(WIN32)
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
#include "SBW/SBWOSSocket.h"

#ifndef INADDR_NONE
#define INADDR_NONE ((unsigned long) -1)
#endif


using namespace SystemsBiologyWorkbench;
using namespace SystemsBiologyWorkbench::Broker;

ServerSocket::ServerSocket (int nPort, std::string sHost /*= ""*/)
{
	m_nPort = nPort;

	// create socket for connections
	if ((m_nServerSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		throwError();
	//int opt = 1;
	//setsockopt(m_nServerSocket,IPPROTO_TCP,SO_REUSEADDR,(const char*)&opt, sizeof(int));
	// consturct adress structure
	memset(&oServerAddr, 0, sizeof(oServerAddr));
	oServerAddr.sin_family = AF_INET;
	oServerAddr.sin_port = htons((unsigned short)m_nPort);
	//SBWThread::sleep(1000);
#if defined(WIN32)
	//oServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	oServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (oServerAddr.sin_addr.S_un.S_addr == INADDR_NONE)
		throwError();
#else
	//oServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	oServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (oServerAddr.sin_addr.s_addr == INADDR_NONE)
		throwError();
#endif


	// bind to local address
	if (bind(m_nServerSocket, (struct sockaddr *) &oServerAddr, sizeof(oServerAddr)) < 0)
	{
		throwError();
	}
	// listen for connections
	if (listen(m_nServerSocket, SOMAXCONN) < 0 )
		throwError();

	m_nClientSocket = 1;
	m_nClientLength = 0;
}

void ServerSocket::throwError()
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

SOCKET ServerSocket::acceptClient()
{
	m_nClientLength = sizeof(this->oClientAddr);
	memset(&oClientAddr, 0, sizeof(oClientAddr));
#ifndef WIN32
	m_nClientSocket = accept( (SOCKET)m_nServerSocket, (struct sockaddr *) &oClientAddr, (socklen_t *) &m_nClientLength);
#else
	m_nClientSocket = accept( (SOCKET)m_nServerSocket, (struct sockaddr *) &oClientAddr, &m_nClientLength);
#endif
	if (m_nClientSocket < 0)
		throwError();

	return m_nClientSocket;
}

std::string ServerSocket::getCurrentClientAddress()
{
	std::string sAddress( inet_ntoa(oClientAddr.sin_addr ));
	return sAddress;
}
