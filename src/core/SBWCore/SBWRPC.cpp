/**
 * @file SBWRPC.cpp
 * @brief common lowlevel implementation of SBW message handling common to both broker and module
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


// SBWRPC.cpp: implementation of the SBWRPC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SBWRPC.h"

#ifdef WIN32
#include <crtdbg.h>
#endif


#include <SBW/sbwdefs.h>
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
 * creates an RPC mechanism which initially doesn't process incoming calls
 */
SBWRPC::SBWRPC(SBWRPCListener *l) 
: listener(l)
, receiver(new DoNothingReceiver())
, inCallsMutex("inCalls")
, outCallsMutex("outCalls")
{
}

/**
 * deallocates the call processor
 */
SBWRPC::~SBWRPC()
{
    delete receiver ;
    receiver = NULL;
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
 * @return data block contain the result of the call.
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

void SBWRPC::disconnect()
{
    signalDisconnect();
    waitForDisconnect();
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
    if (receiver != NULL && receiver->canDelete())
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
 * process event where the broker's module registry has changed
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
    try
    {
      listener->onShutdown();
    }
    catch (...)
    {
    }

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
            ++thread ;
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

            ++outCallId ;
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
