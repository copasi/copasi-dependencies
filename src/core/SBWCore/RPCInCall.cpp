/**
 * @file RPCInCall.cpp
 * @brief implementation of RPCInCall class - processes incoming calls from other modules
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: RPCInCall.cpp,v 1.5 2007/10/23 03:45:43 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/RPCInCall.cpp,v $
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

// RPCInCall.cpp: implementation of the RPCInCall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RPCInCall.h"
#include "DataBlockWriterClass.h"
#include "SBWRawException.h"
#include "SBWApplicationException.h"
#include "SBWRPC.h"

#include "Receiver.h"

namespace SystemsBiologyWorkbench
{

/// thread in which incoming calls are processed
class RPCInCallThread : public SBWThread
{
public:
	/**
	 * constructs thread for processing incoming calls
	 * @param x object holding the incoming call data
	 */
	RPCInCallThread(RPCInCall *x) : SBWThread("in call"), inCall(x) {}

	/// process incoming calls - thread body
	void run() { inCall->run(); }

private:
	/// object holding the incoming call data
	RPCInCall *inCall;
};

}

using namespace SystemsBiologyWorkbench ;

/**
 * creates a thread that processes incoming calls
 * @param x object holding incoming call data
 */
void defaultIncomingCallThreadCreator(RPCInCall *x)
{
	x->start();
}

/// pointer to function that is registered to create threads that process incoming calls 
IncomingCallThreadCreator RPCInCall::threadCreator = defaultIncomingCallThreadCreator ;

/**
 * create an RPCInCall to receive calls.
 * @param s SBWRPC object invoking calls using this object
 * @param r Receiver object that provides implementations for call and send messages
 * @param name name of this object
 */
RPCInCall::RPCInCall(SBWRPC *s, Receiver *r, std::string name)
    : thread(NULL), event(name), completeEvent("complete event"),
      rpc(s), receiver(r), active(false), operational(true), callPending(false),
      completed(false)
{
}

/**
 * release OS resources used by this object
 */
RPCInCall::~RPCInCall()
{
	try
	{
		if (thread)
			delete thread ;
	}
	catch(...)
	{
	}
}

/// create thread to process incoming calls
void RPCInCall::start()
{
	thread = new RPCInCallThread(this);
	thread->start();
}

/// block until thread processing incoming calls terminates
void RPCInCall::waitTillComplete()
{
	if (thread)
		thread->join();
	else if (!completed)
		completeEvent.wait();
}

/**
 * process incoming calls.
 * This function is run inside a seperate thread.
 * [Continues processing calls until operational becomes false.
 * Sets active to false when finished processing a message.]
 */
void RPCInCall::run()
{
	TRACE("Running new message-handling thread");

	while (operational)
	{
		try
		{
			TRACE("Waiting for a new message");

			event.wait();

			if (!operational)
			{
				completed = true ;
				completeEvent.notify();
				return ;
			}

			callPending = false ;

			TRACE("Parsing new message");			
			Integer messageId = 0, fromModuleId = 0;
			DataBlockWriter result;

			try
			{


				reader.getWithoutType(messageId);
				reader.getWithoutType(fromModuleId);


				Integer serviceId, methodId;

				reader.getWithoutType(serviceId);
				reader.getWithoutType(methodId);

				TRACE("Received msg id = " << messageId
					  << ", src module = " << fromModuleId
					  << ", service = " << serviceId
					  << ", method = " << methodId);

				if (serviceId == SBWSystemService)
				{
					switch (methodId)
					{
						case SBWOnOtherModuleInstanceShutdownMethod :
							{
								//int moduleId ;

								//reader.get(moduleId);
								rpc->onOtherModuleInstanceShutdown(&reader, fromModuleId);//moduleId);
							}
							break ;
						case SBWOnOtherModuleInstanceStartupMethod :
							{
								//int moduleId ;

								//reader.get(moduleId);
								rpc->onOtherModuleInstanceStartup(&reader, fromModuleId);//moduleId);
							}
							break ;
						case SBWOnRegistrationChangeMethod :
							rpc->onRegistrationChange(&reader, fromModuleId);//);
							break;

						default:
							{
							if (receiver == NULL) throw new SBWDisconnectException("Receiver disconnected");
							result = receiver->receive(fromModuleId, serviceId, methodId, reader);
							break;
							}
					}
				}
				else
				{					
					if (receiver == NULL) throw new SBWDisconnectException("Receiver disconnected");
					result = receiver->receive(fromModuleId, serviceId, methodId, reader);
				}

				if (transmitReply)
				{
					TRACE("Transmitting reply to module " << fromModuleId);

					Integer length ;
					unsigned char *message = result.createReply(fromModuleId, messageId, length);
					rpc->transmit(fromModuleId, message, length);
					
				}
			}
			catch (SBWException &e)
			{
				if (transmitReply)
				{
					e.log();

					Integer length ;
					unsigned char *message = DataBlockWriter::createException(
						fromModuleId, 
						messageId,
						e.getCode(),
						e.getMessage().c_str(),
						e.getDetailedMessage().c_str(),
						length);
					
					TRACE("Transmitting the following exception"
						  << message);
					
					rpc->transmit(fromModuleId, message, length);
				}
				else
					throw new SBWApplicationException(e.getMessage(), e.getDetailedMessage());
 			}			
			catch (SBWException *e)
			{
				if (transmitReply)
				{
					e->log();

					Integer length ;
					unsigned char *message = DataBlockWriter::createException(
						fromModuleId, 
						messageId,
						e->getCode(),
						e->getMessage().c_str(),
						e->getDetailedMessage().c_str(),
						length);
					
					TRACE("Transmitting the following exception"
						  << message);

					delete e ;
					rpc->transmit(fromModuleId, message, length);
				}
				else
					throw ;
 			}		
			catch (...) 
			{
				if (transmitReply)
				{
					Integer length ;
					unsigned char *message = DataBlockWriter::createException(
						fromModuleId, 
						messageId,
						(unsigned char) -1,
						"unexpected error",
						"unexpected error",
						length);
					
					TRACE("Transmitting the following exception"
						  << message);

					rpc->transmit(fromModuleId, message, length);
				}
			}
		}
		catch (SBWException *e)
		{
			e->log();
			delete e ;
		}		

		active = false ;
	}

	completed = true ;
	completeEvent.notify();
}

/** 
 * returns whether this object is processing a call ie unavailable to handle a new call
 * @return whether this object is processing a call ie unavailable to handle a new call
 */
bool RPCInCall::isActive()
{
	return active; 
}

/**
 * process a new incoming call.
 * @param r contains a partially read call or send message:
 *          this method assumes that the cursor is at the first item after the message type field of the message.
 * @param tr indicates that the incoming message is a call message (a reply must transmitted)
 */
void RPCInCall::execute(DataBlockReader r, bool tr)
{
	if (isActive())
	{
		SBWException::log("ERROR ATTEMPTING TO EXECUTE INCOMING CALL IN ACTIVE CALL THREAD");
		return;
	}

	active = true ;
	transmitReply = tr ;
	reader = r;

	TRACE("Execute call, transmitReply = " << tr);

	callPending = true ;
	event.notify();
}

/// tell this object to stop processing incoming calls - run() will then terminate
void RPCInCall::stop()
{
	TRACE("Stopping");

	operational = false ;
	event.notify();
}

/// create a thread to process incoming calls using registered thread creation function
void RPCInCall::createThread()
{
	threadCreator(this);
}

// static
/**
 * register a given function that creates threads to process incoming calls
 * @param f function that creates threads to process incoming calls
 */
void RPCInCall::registerThreadCreator(IncomingCallThreadCreator f)
{
	threadCreator = f ;
}

// static
/**
 * process incoming calls using given data on incoming calls
 * @param inCall data on incoming calls
 */
void RPCInCall::processIncomingCalls(RPCInCall *inCall)
{
	inCall->run();
}
