/**
 * @file RPCInCall.h
 * @brief  provides a seperate thread environment for each incoming call
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
#if !defined(AFX_RPCINCALL_H__C67C68CC_E2BF_4E9E_99EC_8D0BE181A365__INCLUDED_)
#define AFX_RPCINCALL_H__C67C68CC_E2BF_4E9E_99EC_8D0BE181A365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/sbwdefs.h>

#ifndef SBW_STRICT_INCLUDES
#include <SBW/portableOS.h>
#endif

#include <SBW/SBWOSEvent.h>
#include <SBW/DataBlockReaderClass.h>
#include <SBW/DataBlockWriter.h>
#include <SBW/ImplDataBlockReader.h>

namespace SystemsBiologyWorkbench
{

class Receiver ;
class SBWRPC ;
class RPCInCall;

/**
 * function that creates threads for processing incoming calls
 * @param x incoming calls data
 */
typedef void (* IncomingCallThreadCreator) (RPCInCall *x);

class RPCInCallThread ;

/// incoming call processor.
class SBW_API RPCInCall
{
public:
	RPCInCall(SBWRPC *, Receiver *, std::string name);
	virtual ~RPCInCall();

	void execute(DataBlockReader reader, bool transmitReply);
	bool isActive();
	void stop();
	void run();
	void start();

	void createThread();
	void waitTillComplete();

	static void registerThreadCreator(SystemsBiologyWorkbench::IncomingCallThreadCreator);
	static void processIncomingCalls(RPCInCall *);

private:
	/// thread processing incoming calls - default if no thread creater is registered 
	RPCInCallThread *thread ;

	/// event that signals when the thread should process a new incoming call.
	SBWOSEvent event ;

	SBWOSEvent completeEvent ;

	/// the incoming call message.
	DataBlockReader reader ;

	/// whether a reply message should be returned to the caller.
	bool transmitReply ;

	/// the RPC processor.
	SBWRPC *rpc ;

	/// the object that implements the call.
	Receiver *receiver;

	/// whether this object is currently processing a call.
	bool active ;

	/// whether this object can process new calls when not active - false implies thread should stop
	bool operational ;

	/// whether an new incoming call is avaliable to be processed.
	bool callPending ;

	/// whether the thread has finished
	bool completed ;

	/// function that creates htreads for processing incoming calls
	static SystemsBiologyWorkbench::IncomingCallThreadCreator threadCreator ;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_RPCINCALL_H__C67C68CC_E2BF_4E9E_99EC_8D0BE181A365__INCLUDED_)
