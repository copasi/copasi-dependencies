/**
 * @file SBWOSSocket.h
 * @brief platform independant socket implementation
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWOSSocket.h,v 1.3 2009/09/18 22:56:59 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWOSSocket.h,v $
 */

/*
** Copyright 2001 California Institute of Technology and
** Japan Science and Technology Corporation.
** 
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 2.1 of the License, or
** any later version.
** 
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
** MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software and
** documentation provided hereunder is on an "as is" basis, and the
** California Institute of Technology and Japan Science and Technology
** Corporation have no obligations to provide maintenance, support,
** updates, enhancements or modifications.  In no event shall the
** California Institute of Technology or the Japan Science and Technology
** Corporation be liable to any party for direct, indirect, special,
** incidental or consequential damages, including lost profits, arising
** out of the use of this software and its documentation, even if the
** California Institute of Technology and/or Japan Science and Technology
** Corporation have been advised of the possibility of such damage.  See
** the GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with this library; if not, write to the Free Software Foundation,
** Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
**
** The original code contained here was initially developed by:
**
**     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri
**     The Systems Biology Workbench Development Group
**     ERATO Kitano Systems Biology Project
**     Control and Dynamical Systems, MC 107-81
**     California Institute of Technology
**     Pasadena, CA, 91125, USA
**
**     http://www.cds.caltech.edu/erato
**     mailto:sysbio-team@caltech.edu
**
** Contributor(s):
**
*/

/// MSVC generated include loop block
#if !defined(AFX_SBWOSSOCKET_H__7662C8C2_47D9_4DD4_9213_0AE12D6A3A05__INCLUDED_)
#define AFX_SBWOSSOCKET_H__7662C8C2_47D9_4DD4_9213_0AE12D6A3A05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32
#include "winsock2.h"
#define _WINSOCKAPI_
#define _WINSOCK2API_
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#if defined(DARWIN) || defined(SOLARIS) || defined(_DARWIN)
#include <netinet/in.h>	
#endif

/* Compatibility with the Windows definitions. */
#ifndef SOCKET
#define SOCKET int
#define INVSOC (-1)
#define INVALID_SOCKET INVSOC
#define SOCKET_ERROR (-1)
#define SD_SEND SHUT_WR
#define closesocket close
#endif	// SOCKET

#endif

#include <string>
#include <vector>

#include "portableOS.h"

namespace SystemsBiologyWorkbench
{

/// represents SBW library's use of the operating systems socket library
class SBWOSSocketSystem
{
public:
	SBW_API SBWOSSocketSystem();
	SBW_API ~SBWOSSocketSystem();
};

/// platform independant socket implementation
class SBWOSSocket  
{
public:
	SBW_API SBWOSSocket(Integer port, const char *hostname = ""); 
	SBW_API virtual ~SBWOSSocket();

	SBW_API void receive(unsigned char *data, Integer length);
	SBW_API void transmit(unsigned char *data, Integer length);

	SBW_API static bool isLocalAddress(const char *hostname);
	SBW_API static bool portInUse(int port);

	/**
	 * return the address of a given host
	 * \param host the host to look up
	 * \return the address of a given host
	 */
	SBW_API static std::string getHostAddress(std::string host);
	/**
	 *
	 * \return the name of the local host
	 */
	SBW_API static std::string getHostName();
	
	
	/**
	*
	* \return the local host address
	*/
	SBW_API static std::string getHostAddress();


private:
	SBW_API void throwError();
	/** 
	 *
	 * \return all local addresses 
	 */
	static std::vector< std::string > getLocalAddresses();
	
	/// operating system socket object
	SOCKET sock ;
	static SBWOSSocketSystem system;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWOSSOCKET_H__7662C8C2_47D9_4DD4_9213_0AE12D6A3A05__INCLUDED_)
