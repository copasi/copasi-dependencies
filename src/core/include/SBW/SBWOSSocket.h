/**
 * @file SBWOSSocket.h
 * @brief platform independent socket implementation
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

/// MSVC generated include loop block
#if !defined(AFX_SBWOSSOCKET_H__7662C8C2_47D9_4DD4_9213_0AE12D6A3A05__INCLUDED_)
#define AFX_SBWOSSOCKET_H__7662C8C2_47D9_4DD4_9213_0AE12D6A3A05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32
#include <winsock2.h>
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

#include <SBW/sbwdefs.h>
#include <SBW/sbwplusbasictypes.h>

#ifndef SBW_STRICT_INCLUDES
#include <SBW/portableOS.h>
#endif

namespace SystemsBiologyWorkbench
{

/// represents SBW library's use of the operating systems socket library
class SBW_API SBWOSSocketSystem
{
public:
	SBWOSSocketSystem();
	~SBWOSSocketSystem();
};

/// platform independent socket implementation
class SBW_API SBWOSSocket  
{
public:
	SBWOSSocket(Integer port, const char *hostname = ""); 
	virtual ~SBWOSSocket();

	void receive(unsigned char *data, Integer length);
	void transmit(unsigned char *data, Integer length);

	static bool isLocalAddress(const char *hostname);
	static bool portInUse(int port);

	/**
	 * return the address of a given host
	 * \param host the host to look up
	 * \return the address of a given host
	 */
	static std::string getHostAddress(std::string host);
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


private:
	void throwError();
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
