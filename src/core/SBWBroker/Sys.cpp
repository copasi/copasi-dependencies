#include "Sys.h"
#include "BrokerDefines.h"
#include "StringUtil.h"

#include "SBW/config.h"
#include "SBW/SBWOSSocket.h"
#include "SBW/DataBlockReader.h"
#include "SBW/DataBlockWriter.h"
#include "SBW/SBWApplicationException.h"
#include "SBW/SBWCommunicationException.h"
#include "SBW/SBWRawException.h"
#include "SBW/portableOS.h"

#ifdef WIN32
#include <direct.h>
#pragma warning(disable: 4389)
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif


#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <netdb.h>
#include <unistd.h>
#endif

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

#ifndef INADDR_NONE
#define INADDR_NONE ((unsigned long) -1)
#endif


using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;
using namespace std;

std::string Sys::readString (SOCKET sock)
{
	std::string sResult;
	int length = readInteger(sock);
	char *oBuffer = (char *) malloc ( length+1 * sizeof(char));
	if (oBuffer == NULL)
		throw new SBWCommunicationException("corrupted message", "string length was given with: " + StringUtil::toString(length) + " not enough memory for that though.");
	memset(oBuffer,0,length * sizeof(char));
	try
	{
		receive(sock,(unsigned char*)oBuffer,length);
	}
	catch (SBWCommunicationException *exception)
	{
		free (oBuffer);
		throw exception;
	}
	oBuffer[length]=0;
	sResult = oBuffer;
	free (oBuffer);
	oBuffer = NULL;
	return sResult;
}
int Sys::readInteger(SOCKET sock)
{
	int result = -1;
	unsigned char *buffer = new unsigned char[4];
	memset(buffer,0,4);
	try
	{
		receive(sock, buffer, 4);
	}
	catch (SBWCommunicationException *exception)
	{
		delete[] buffer;
		throw exception;
	}

	DataBlockReader oTemp (buffer, 4, false);
	oTemp.getImpl()->getWithoutType(result);
	buffer = NULL;
	return result;
}
void Sys::receive(SOCKET sock, unsigned char *data, Integer length)
{
	Integer cursor = 0 ;

	while (cursor != length)
	{
		Integer result = recv(sock, (char *)data + cursor, length - cursor, 0);

		if (result == 0)
			throw new SBWCommunicationException("graceful disconnect while reading");

		if (result == SOCKET_ERROR)
			throw new SBWCommunicationException("SOCKET_ERROR");

		cursor += result ;
	}
}

bool Sys::lookupPossible(std::string sHost)
{
	hostent* oHost = gethostbyname(sHost.c_str());
	if (oHost != NULL)
		return true;
	return false;
}

std::string Sys::fullyQualifiedHostName(std::string sName)
{
	string host;
	size_t nPos = host.find('/');
	if (nPos != host.npos)
	{
		host = sName.substr(0,nPos);
	}
	else
	{
		host = sName;
	}

	hostent* oHost = gethostbyname(host.c_str());
	if (oHost != NULL)
		return oHost->h_name;
	else
	{
		TRACE("couldn't resolve host");
		return host;
	}
}
std::string Sys::getHostName()
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
std::string Sys::getHostAddress(std::string sAddress)
{
	string host;
	size_t nPos = host.find('/');
	if (nPos != host.npos)
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
std::string Sys::getHostAddress()
{
	return getHostAddress("localhost");
}
bool Sys::isAddress(std::string /*str*/)
{
	throw new SBWApplicationException("not yet implemented");
}
bool Sys::portInUse(int port, bool /*remoteAllowed*/)
{
	return portInUse(port);
	//return SBWOSSocket::portInUse(port);
}
int Sys::findFreePort(std::vector<int> portRange, bool remoteAccessible)
{
	int low = portRange[0];
	int high = portRange[1];

	for (int port = low; port <= high; port++)
	{
		if (!  portInUse(port, remoteAccessible))
			return port;
	}
	return -1;                      // Didn't find one.
}
bool Sys::isSecure()
{
#ifdef INSECURE
	return false;
#else
	return true;
#endif
}
bool Sys::portInUse(int port)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    bool inUse = false;

    if (sock > 0)
    {
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons((unsigned short)port);

#if defined(WIN32)
        addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		if (addr.sin_addr.S_un.S_addr == INADDR_NONE)
			throw new SBWSocketException("address error");
#else
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
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

void Sys::createDirectory(std::string sDir)
{
	TRACE("Creating directory: " << sDir);
	string sCreateDir = sDir + SBWOS::DirectorySeparator();
	if (MKDIR(sCreateDir.c_str()) == -1)
	{
		size_t nPosLast = 0;
		size_t nPosCurrent = sCreateDir.find( SBWOS::DirectorySeparator(),nPosLast);
		std::string sTemp;
		while (nPosCurrent != sCreateDir.npos)
		{
			sTemp = sCreateDir.substr(0,nPosCurrent+1);
			MKDIR(sTemp.c_str());
			nPosLast = nPosCurrent+1;
			nPosCurrent = (int)sCreateDir.find(SBWOS::DirectorySeparator(),nPosLast);
		}
	}
}
