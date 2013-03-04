/**
 * @file RPCOutCall.cpp
 * @brief implementation of RPCOutCall class - handles communications involved with calls to other modules
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: RPCOutCall.cpp,v 1.2 2006/07/08 23:49:46 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/RPCOutCall.cpp,v $
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

// RPCOutCall.cpp: implementation of the RPCOutCall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RPCOutCall.h"
#include "SBWCommunicationException.h"
#include "SBWApplicationException.h"
#include "SBWRawException.h"
#include "SBWCommunicationException.h"
#include "SBWModuleStartException.h"
#include "SBWIncompatibleMethodSignatureException.h"
#include "SBWTypeMismatchException.h"
#include "SBWModuleIdSyntaxException.h"
#include "SBWIncorrectCategorySyntaxException.h"
#include "SBWServiceNotFoundException.h"
#include "SBWMethodTypeNotBlockTypeException.h"
#include "SBWMethodAmbiguousException.h"
#include "SBWUnsupportedObjectTypeException.h"
#include "SBWMethodNotFoundException.h"
#include "SBWSignatureSyntaxException.h"
#include "SBWModuleDefinitionException.h"
#include "SBWModuleNotFoundException.h"
#include "SBWBrokerStartException.h"
#include "ImplDataBlockReader.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * must be called when the connection to the broker is lost
 */
void RPCOutCall::onBrokerShutdown()
{
	TRACE("Lost connection to broker");

	onEvent(BrokerDisconnectEvent);
}

/**
 * must be called when another module instance disconnects from the broker.
 * @param otherModuleId numeric identifier of other module instance
 */
void RPCOutCall::onOtherModuleInstanceShutdown(Integer otherModuleId)
{
	TRACE("Module " << moduleId << " notified that module " << otherModuleId
		  << " is shutting down");

	if (moduleId == otherModuleId)
		onEvent(ModuleDisconnectEvent);
}

/**
 * process the results (reply) for the outgoing call when they are received
 * @param r data block containing a partially read call reply message.
 *          The cursor is pointing at the first object after the message id in the reply message.
 * @param isException returns whether the reply message is an exception message (otherwise a normal reply message is assumed.
 */
void RPCOutCall::processReply(DataBlockReader r, bool isException)
{
	try
	{
		reader = r ;
		onEvent(isException ? NormalExceptionEvent : NormalReplyEvent);
	}
	catch (...)
	{
		throw new SBWCommunicationException("implementation module disconnected"); // TODO:  FIXME
	}
}

/**
 * block until a reply for the outgoing call is received or exception event occurs; then process the reply or exceptional event
 * Exceptional events include broker and implementation disconnection as well as standard exceptions.
 * @param mId numeric identifier for the module being called
 * @return a data block containing the result from the call
 */
DataBlockReader RPCOutCall::waitForReply(Integer mId)
{
	moduleId = mId ;

	event.wait();

	TRACE("Processing event type " << eventType);

	switch (eventType)
	{
		case BrokerDisconnectEvent :
			throw new SBWCommunicationException("broker disconnected");

		case ModuleDisconnectEvent :
			throw new SBWCommunicationException("implementation module disconnected");

		case NormalExceptionEvent :
			{
				unsigned char type;
				
				reader.getWithoutType(type);

				std::string message, detailedMessage ;

				reader >> message >> detailedMessage ;

				switch (type)
				{
					default:
					case ApplicationExceptionCode :
                        throw new SBWApplicationException(message, detailedMessage);

                    case RawExceptionCode :
                        throw new SBWRawException(message, detailedMessage);

                    case CommunicationExceptionCode :
                        throw new SBWCommunicationException(message, detailedMessage);

                    case ModuleStartExceptionCode :
                        throw new SBWModuleStartException(message, detailedMessage);

                    case IncompatibleMethodSignatureExceptionCode :
                        throw new SBWIncompatibleMethodSignatureException(message, detailedMessage);

                    case TypeMismatchExceptionCode :
                        throw new SBWTypeMismatchException(message, detailedMessage);

                    case ModuleIdSyntaxExceptionCode :
                        throw new SBWModuleIdSyntaxException(message, detailedMessage);

                    case IncorrectCategorySyntaxExceptionCode :
                        throw new SBWIncorrectCategorySyntaxException(message, detailedMessage);

                    case ServiceNotFoundExceptionCode :
                        throw new SBWServiceNotFoundException(message, detailedMessage);

                    case MethodTypeNotBlockTypeExceptionCode :
                        throw new SBWMethodTypeNotBlockTypeException(message, detailedMessage);

                    case MethodAmbiguousExceptionCode :
                        throw new SBWMethodAmbiguousException(message, detailedMessage);

                    case UnsupportedObjectTypeExceptionCode :
                        throw new SBWUnsupportedObjectTypeException(message, detailedMessage);

                    case MethodNotFoundExceptionCode :
                        throw new SBWMethodNotFoundException(message, detailedMessage);

					case SignatureSyntaxExceptionCode :
						throw new SBWSignatureSyntaxException(message, detailedMessage);

                    case ModuleDefinitionExceptionCode :
                        throw new SBWModuleDefinitionException(message, detailedMessage);

                    case ModuleNotFoundExceptionCode :
                        throw new SBWModuleNotFoundException(message, detailedMessage);

                    case BrokerStartExceptionCode :
                        throw new SBWBrokerStartException(message, detailedMessage);

                    //default :
                        //throw new SBWRawException(
						//	"Encountered unknown Exception code", "in RPCOutCall::waitForReply in C/C++ library");
                }
			}
		case NormalReplyEvent : 			
			return reader ;

		default :
			throw new SBWCommunicationException("strange event in RPCOutCall");
	}
}

/**
 * process call termination event
 * @param type of event that will terminate this call
 */
void RPCOutCall::onEvent(EventType e)
{
	eventType = e ;
	event.notify();
}
