/**
 * @file RPCInCall.h
 * @brief  provides a seperate thread environment for each incoming call
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: RPCInCall.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/RPCInCall.h,v $
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
#if !defined(AFX_RPCINCALL_H__C67C68CC_E2BF_4E9E_99EC_8D0BE181A365__INCLUDED_)
#define AFX_RPCINCALL_H__C67C68CC_E2BF_4E9E_99EC_8D0BE181A365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "portableOS.h"
#include "DataBlockReaderClass.h"
#include "DataBlockWriter.h"
#include "ImplDataBlockReader.h"

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
class RPCInCall  
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
