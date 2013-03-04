/**
 * @file SBWRPC.cpp
 * @brief common lowlevel implementation of SBW message handling common to both broker and module
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWRPC.cpp,v 1.7 2008/07/13 19:12:53 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SBWRPC.cpp,v $
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

// SBWRPC.cpp: implementation of the SBWRPC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SBWRPC.h"

#ifdef WIN32
#include <crtdbg.h>
#endif


#include "portableOS.h"
#include "DoNothingReceiver.h"
#include "DataBlockWriter.h"
#include "DataBlockReaderClass.h"
#include "RPCOutCall.h"
#include "RPCInCall.h"
#include "SBWRPCListener.h"
#include "SBWApplicationException.h"

using namespace SystemsBiologyWorkbench ;

/// the name of the main broker service on the broker
const char *SBWRPC::brokerServiceName = "BROKER";

/// the value of the leading integer of a disconnect message
const Integer SBWRPC::disconnectMessage = -1;

/**
 * creates an RPC mechanism which initially doesn't process incomming calls
 */
SBWRPC::SBWRPC(SBWRPCListener *l) : listener(l), receiver(new DoNothingReceiver()), inCallsMutex("inCalls"), outCallsMutex("outCalls")
{
}

/**
 * deallocates the call processor
 */
SBWRPC::~SBWRPC()
{
	delete receiver ;
}

/**
 * adds an event listener 
 * @param x event listener
 */
void SBWRPC::setListener(SBWRPCListener *x)
{
	listener = x;
}

/**
 * calls an SBW Method.
 * @param moduleTo the numeric module instance identifier of the destination module instance.
 * @param service the numeric service identifier of the destination service.
 * @param method the numeric method identifier to be called.
 * @param args the argument data for the call.
 * @return data block contain the reult of the call.
 */
DataBlockReader SBWRPC::call(Integer moduleTo, Integer service, Integer method, DataBlockWriter args)
{
	Integer messageId = - 1;
	RPCOutCall *call = NULL;

	TRACE("Creating new RPCOutCall to module " << moduleTo);
	try
	{
		call = new RPCOutCall();
		
		{
			SBWOSMutexLock ml(outCallsMutex);
			Integer mId = 0 ;

			while ((Integer) outCalls.size() != mId && outCalls[mId] != NULL)
				mId++;

			if ( (Integer)outCalls.size() == mId)
				outCalls.resize(mId + 1, call);
			else
				outCalls[mId] = call ;

			messageId = mId ;
		}

		Integer length ;
		unsigned char *message = args.createCall(moduleTo, getModuleId(), messageId, service, method, length);
		transmit(moduleTo, message, length);
		DataBlockReader reader = call->waitForReply(moduleTo);
		SBWOSMutexLock ml(outCallsMutex);

		delete call ;
		outCalls[messageId] = NULL ;

		return reader ;
	}
	catch (SBWException *e)
	{
		std::string sMessage = e->getMessage();
		if (messageId != -1)
		{
			SBWOSMutexLock ml(outCallsMutex);

			delete call ;
			outCalls[messageId] = NULL ;
		}

		throw e;
	}
	catch(...)
	{
		if (messageId != -1)
		{
			SBWOSMutexLock ml(outCallsMutex);

			delete call ;
			outCalls[messageId] = NULL ;
		}
		throw new SBWApplicationException("internal error", "The call could not be completed.");
	}
}

/**
 * Process incoming message.
 * Assumes that length and destination part of message have already been consumed.
 * @param reader incoming message data.
 */
void SBWRPC::receive(DataBlockReader reader)
{
	unsigned char type;
	
	reader.getWithoutType(type);

	switch (type)
	{
		case SBWSendCode :
			receiveCall(reader, false);
			break ;

		case SBWCallCode :
			receiveCall(reader, true);
			break ;

		case SBWReplyCode :
			receiveReply(reader, false);
			break ;

		case SBWExceptionCode :
			receiveReply(reader, true);
			break ;

		default :
			{
				reader.dump();
				SBWException::log(
				"Corrupted Message",
				"Error while decoding message, expecting message type in range 0-3, got value outside that range");
			}
	}
}

/**
 * attach incoming call processor.
 * @param r incoming call processor.
 */
void SBWRPC::registerReceiver(Receiver *r)
{
	if (receiver->canDelete())
		delete receiver ;

	receiver = r ;
}

/**
 * send non-blocking call message.
 * @param moduleTo the numeric module instance identifier of the destination module instance.
 * @param service the numeric service identifier of the destination service.
 * @param method the numeric method identifier to be called.
 * @param args the argument data for the call.
 */
void SBWRPC::send(Integer moduleTo, Integer service, Integer method, DataBlockWriter args)
{
	TRACE("Sending to module " << moduleTo);

	Integer length ;
	unsigned char *message = args.createSend(moduleTo, getModuleId(), service, method, length);

	transmit(moduleTo, message, length);
}

/**
 * transmit a message, blocking until data is sent.
 * @param moduleTo numeric module instance identifier of destination module instance. 
 * @param message message data.
 * @param length number of bytes to send.
 */
void SBWRPC::transmit(Integer moduleTo, unsigned char *message, int length)
{
	if (this == NULL) return;
	if (moduleTo == getModuleId())
	{
		DataBlockReader reader(message, length, false);
		Integer i ;

		reader.getWithoutType(i); // length
		reader.getWithoutType(i); // destination
		receive(reader); // send to self
	}
	else
	{
		transmitExternalOnly(moduleTo, message, length);
	}
}

/**
 * process event where another module instance has disconnected from the broker.
 * @param otherModuleId numeric module instance identifier of module instance which has recently disconnected from the broker.
 */
void SBWRPC::onOtherModuleInstanceShutdown(DataBlockReader *reader, int /*brokerId*/)//Integer otherModuleId)
{
	
	int otherModuleId;
	(*reader) >> otherModuleId;
	SBWOSMutexLock ml(outCallsMutex);
	unsigned int outCallId = 0 ;

	while (outCalls.size() != outCallId)
	{
		RPCOutCall *outCall = outCalls[outCallId] ;

		if (outCall != NULL)
			outCall->onOtherModuleInstanceShutdown(otherModuleId);

		outCallId++ ;
	}


	listener->onModuleShutdown(otherModuleId);
}

/**
 * process event where another module instance has connected to the broker.
 * @param otherModuleId numeric module instance identifier of module instance which has recently connected to the broker.
 */
void SBWRPC::onOtherModuleInstanceStartup(DataBlockReader *reader, int /*brokerId*/)//Integer otherModuleId)
{
		int otherModuleId;
		(*reader) >> otherModuleId;
		listener->onModuleStart(otherModuleId);
}

/**
 * process event where the broker's module registery has changed
 */
void SBWRPC::onRegistrationChange(DataBlockReader * /*reader*/, int /*brokerId*/)//)
{
	listener->onRegistrationChange();
}

/**
 * recover resources and notify listeners after this module disconnects from the broker.
 * Shutsdown incoming call processing threads and throws exceptions in outstanding outgoing calls.
 */
void SBWRPC::cleanup()
{
	listener->onShutdown();

	{
		SBWOSMutexLock ml(inCallsMutex);
		unsigned int thread = 0 ;

		while (inCalls.size() != thread)
		{
			inCalls[thread]->stop(); // signal we want thread to stop
			// was inCalls[thread]->join(); // wait for thread to stop
			inCalls[thread]->waitTillComplete(); // wait for thread to stop
			delete inCalls[thread] ; // safely destroy thread object
			inCalls[thread] = NULL ;
			thread++ ;
		}
	}

	{
		SBWOSMutexLock ml(outCallsMutex);
		unsigned int outCallId = 0 ;

		while (outCalls.size() != outCallId)
		{
			RPCOutCall *outCall = outCalls[outCallId];

			if (outCall != NULL)
				outCall->onBrokerShutdown();
			
			outCallId++ ;
		}
	}
}

/**
 * process incoming call
 * @param reader contains a partially read call or send message:
 *               this method assumes that the cursor is at the first item after the message type field of the message.
 * @param transmitReply indicates that the incoming message is a call message (a reply must transmitted)
 */
void SBWRPC::receiveCall(DataBlockReader reader, bool transmitReply)
{
	TRACE("Receiving call/send message, transmitReply = " << transmitReply);

	RPCInCall *call ;
	SBWOSMutexLock ml(inCallsMutex);
	unsigned int thread = 0 ;

	while (inCalls.size() != thread && inCalls[thread] != NULL && inCalls[thread]->isActive())
		thread++ ;

	if (inCalls.size() == thread || inCalls[thread] == NULL)
	{
		char buffer[20];
		std::string name("in call ");

		sprintf(buffer, "%d", thread);

		call = new RPCInCall(this, receiver, name);
		call->createThread();

		if (inCalls.size() == thread)
			inCalls.resize(thread + 1, call);
		else
			inCalls[thread] = call ;
	}
	else
		call = inCalls[thread];

	call->execute(reader, transmitReply);
}

/**
 * process reply message.
 * @param reader contains a partially read reply or exception message:
 *               this method assumes that the cursor is at the message identifier item field in the message.
 * @param isException indicates that this message is an exception message otherwise its a call result message.
 */
void SBWRPC::receiveReply(DataBlockReader reader, bool isException)
{
	TRACE("Receiving reply message, isException = " << isException);

	int messageId;
		
	reader.getWithoutType(messageId);
	outCalls[messageId]->processReply(reader, isException);
}
