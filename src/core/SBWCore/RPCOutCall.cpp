/**
 * @file RPCOutCall.cpp
 * @brief implementation of RPCOutCall class - handles communications involved with calls to other modules
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

#include <SBW/portableOS.h>

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * must be called when the connection to the broker is lost
 */
RPCOutCall::RPCOutCall() : event("out call") {}

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
