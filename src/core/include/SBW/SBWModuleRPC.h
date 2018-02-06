/**
 * @file SBWModuleRPC.h
 * @brief lowlevel socket management specific to a module instance (as opposed to the broker)
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
#if !defined(AFX_SBWMODULERPC_H__99E3B76D_3215_447D_9EC8_B49B7C3D1066__INCLUDED_)
#define AFX_SBWMODULERPC_H__99E3B76D_3215_447D_9EC8_B49B7C3D1066__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/sbwdefs.h>
#ifndef SBW_STRICT_INCLUDES
#include <SBW/portableOS.h>
#endif
#include <SBW/SBWThread.h>
#include <SBW/SBWRPC.h>
#include <SBW/SBWListener.h>

namespace SystemsBiologyWorkbench
{

	/// lowlevel socket management specific to a module instance (as opposed to the broker)
	class SBW_API SBWModuleRPC : public SBWRPC, private SBWThread  
	{
	public:
		SBWModuleRPC(SBWRPCListener *);
		~SBWModuleRPC();
		void connect(const char *moduleIdentificationString, const char *hostname = "");
		void disconnect();
		void signalDisconnect();
		bool isConnected();
		void waitForDisconnect();
		static bool fileExists(const char *path);
		bool connectToBroker(const char *moduleName, const char *hostname, int port);

	protected :
		Integer getModuleId();
		void transmitExternalOnly(Integer destinationModuleId, unsigned char *message, Integer length);

	private :
		void run();
		int startBroker();

		void connectMessageStreams(const char *moduleName);

		SBWOSSocket *socket;

		/// state of connection to broker.
		enum State
		{
			Disconnected, /**< not connected */
			Connecting, /**< in process of creating connection */
			Connected, 
			Disconnecting /**< in process of disconnecting */
		};

		/// current state of connection 
		State state;

		/// mutex that controls ensures that conncurrent transmits over the socket don't occur.
		SBWOSMutex transmitMutex ;

		/// the numeric module instance identifier for this module instance.
		Integer moduleId ;
	};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWMODULERPC_H__99E3B76D_3215_447D_9EC8_B49B7C3D1066__INCLUDED_)
