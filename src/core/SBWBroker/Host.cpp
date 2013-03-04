#include "Host.h"
#include "Sys.h"
#include "StringUtil.h"
#include "SBW/SBWOSSocket.h"
#include "SBW/RuntimeProperties.h"
#include <sstream>
#include <iostream>

using namespace std;
using namespace SystemsBiologyWorkbench::Broker;


#ifdef WIN32
#pragma warning (disable:4389)
#endif

int Host::getPort()
{
	return m_nPort;
}
std::string Host::getClearName()
{
	size_t nPos = givenHostName.find('/');
	if (nPos != givenHostName.npos)
	{
		return givenHostName.substr(0,nPos);
	}
	else
	{
		return givenHostName;
	}
}
Host::Host(string name)
{
	m_nPort = -1;
	size_t nPos = name.find('/');
	if (nPos != name.npos)
	{
		stringstream oTemp(name.substr(nPos+1));
		oTemp >> m_nPort;		
		givenHostName = name.substr(0,nPos);
		if (m_nPort < 1 ) m_nPort = -1;		
	}
	else
	{
		givenHostName = name;
	}
	fullHostName = Sys::fullyQualifiedHostName(givenHostName);
	hostAddressString = Sys::getHostAddress(fullHostName);
	givenHostName = name;

	if (SBWOSSocket::isLocalAddress(hostAddressString.c_str()))
	{
		hostAddressString = "127.0.0.1";
		fullHostName = "localhost";
		givenHostName = name;
	}
}
Host::Host(std::string name, std::string address, int nPort) 
{
	givenHostName = name;
	fullHostName = Sys::fullyQualifiedHostName(givenHostName);
	hostAddressString = address;
	m_nPort = nPort;
	if (SBWOSSocket::isLocalAddress(hostAddressString.c_str()))
	{
		hostAddressString = "127.0.0.1";
		fullHostName = "localhost";
		givenHostName = name;
	}
}
Host::Host(string name, string address)
{
	givenHostName = name;
	fullHostName = Sys::fullyQualifiedHostName(givenHostName);
	hostAddressString = address;
	if (SBWOSSocket::isLocalAddress(hostAddressString.c_str()))
	{
		hostAddressString = "127.0.0.1";
		fullHostName = "localhost";
		givenHostName = name;
	}
}

void  Host::setPort(int nPort)
{ 
	if (m_nPort != nPort)
	{
		std::string sTemp = "/" + StringUtil::toString(nPort);
		givenHostName = givenHostName.substr(0,hostAddressString.find('/')) + sTemp ;
		m_nPort = nPort; 
	}
}
string Host::getName()
{
	return givenHostName;
}
string Host::getFullName()
{
	return fullHostName;
}
string Host::getAddress()
{
	return hostAddressString;
}
bool Host::equals(Host *other)
{
	string sTemp1 = hostAddressString.substr(0,hostAddressString.find('/'));
	string sTemp2 = other->getAddress().substr(0,other->getAddress().find('/'));

//	cout << "this one: " << sTemp1 << " port: " << getPort() << " module port: " << RuntimeProperties::getSBWModulePort() << endl;
//	cout << "other one: " << sTemp2 << " port: " << other->getPort() << endl;
	
	// now with the ports *sometimes* in the string and sometimes not lets check ...
	if (((sTemp1 == sTemp2) || 
		(  (SBWOSSocket::isLocalAddress(sTemp1.c_str()) == true) 
		&& (SBWOSSocket::isLocalAddress(sTemp2.c_str()) == true) ) )
		&& ((getPort() == other->getPort()) || 	(getPort() == -1 || other->getPort() == -1)
			|| (getPort() == -1 && (other->getPort() == RuntimeProperties::getSBWModulePort()))))
		return true;
	
	return false;
}
