/**
 * @file SBWRPC.h
 * @brief common lowlevel implementation of message handling common to both broker and module
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
#if !defined(AFX_SBWRPC_H__90F43945_EB1B_476C_B02D_6497210BFB27__INCLUDED_)
#define AFX_SBWRPC_H__90F43945_EB1B_476C_B02D_6497210BFB27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// don't understand why this is needed here but...
#ifdef WIN32
// disable silly messages about debug identifier sizes being to big, typically caused by the STL.
//#pragma warning(disable: 4786 4788)
#endif

#include <SBW/DataBlockReaderClass.h>
#include <SBW/SBWRPCListener.h>
#include <SBW/SBWOSMutex.h>

#include <set>

namespace SystemsBiologyWorkbench
{

class Receiver ;
class RPCInCall ;
class RPCOutCall ;


/**
 * services that have standard numeric identifers.
 * Encoding used here is used across the SBW system.
 */
enum SBWStandardServices
{
    SBWErrorService = -2 , /**< placeholder for error conditions */
    SBWSystemService = -1 /**< service on all module instances that provides information on the services and methods exported by the module.*/
};

/**
 * module instances that have standard numeric identifers.
 * Encoding used here is used across the SBW system.
 */
enum SBWStandardModules
{
    SBWErrorModule = -2, /**< placeholder for error conditions */
    SBWBrokerModule = -1 /**< the Broker */
};

/**
 * The standard methods on SBWSystemService.
 * Encoding used here is used across the SBW system.
 * @see SBWSystemService.
 */
enum SBWStandardMethods
{
    SBWErrorMethod = -1, /**< placeholder for error conditions */
    SBWGetServicesMethod = 0 , /**< returns the services on the module instance */
    SBWGetMethodsMethod = 1, /**< returns the signatures of the methods on a given service on the module instance */
    SBWOnOtherModuleInstanceShutdownMethod = 2, /**< called when another module is disconnected from the broker */
    SBWShutdownMethod = 3, /**< called when another module requests that this module instance disconnects */
    SBWGetMethodHelpMethod = 4, /**< returns help for a given method on a given service on the module instance */
    SBWOnOtherModuleInstanceStartupMethod = 5, /**< called when another module connects to the broker */
    SBWOnRegistrationChangeMethod = 6 /**< called when the broker's registration data changes */
};

/**
 * Implementation of the SBW remote procedure call protocol.
 * The actual connection setup, disconnect and data transmission is
 * abstracted via virtual functions.
 */
class SBW_API SBWRPC
{
public:
    SBWRPC(SBWRPCListener *l);
    virtual ~SBWRPC();

    DataBlockReader call(Integer moduleTo, Integer service, Integer method, DataBlockWriter args);

    /**
     * make connection.
     * @param moduleIdentificationString module identification string of this module instance.
     *                                   default is the empty string which indicates an anonymous connection
     */
    virtual void connect(const char *moduleIdentificationString = "", const char *hostname = "") = 0;

    /// disconnect.
    void disconnect();

    /**
     * returns whether there is a connection on this link.
     * @return whether there is a connection on this link.
     */
    virtual bool isConnected() = 0;

    /// block until disconnection
    virtual void waitForDisconnect() = 0 ;

    /// send message requesting disconnection
    virtual void signalDisconnect() = 0 ;

    void receive(DataBlockReader reader);
    void registerReceiver(Receiver *);
    void send(Integer moduleTo, Integer service, Integer method, DataBlockWriter args);
    void transmit(Integer moduleTo, unsigned char *messageForRelease, int length);

    static const char *brokerServiceName;
    static const Integer disconnectMessage ;

    /*SBW_API */virtual void onOtherModuleInstanceShutdown(DataBlockReader *reader, int brokerId);//Integer otherModuleId);
    /*SBW_API */virtual void onOtherModuleInstanceStartup(DataBlockReader *reader, int brokerId);//Integer otherModuleId);
    /*SBW_API */virtual void onRegistrationChange(DataBlockReader *reader, int brokerId);//);

    void setListener(SBWRPCListener *);

    /**
     * return the numeric module identifier of this module
     */
    virtual Integer getModuleId() = 0 ;

protected:
    static const char *brokerJar;

    void cleanup();

    /**
     * transmit message over connection.
     * @param moduleTo numeric module instance identifier of destination module instance.
     * @param message message to be sent in its final form.
     * @param length length of message in bytes.
     */
    virtual void transmitExternalOnly(Integer moduleTo, unsigned char *message, int length) = 0;

private:
    void receiveCall(DataBlockReader reader, bool transmitReply);
    void receiveReply(DataBlockReader reader, bool isException);

    /// reciever for RPC events
    SBWRPCListener *listener;

    /// incoming call threads
    std::vector<RPCInCall *> inCalls ;

    /// pending outgoing calls
    std::vector<RPCOutCall *> outCalls ;

    /// incoming call implementation processor
    Receiver *receiver ;

    /**
     * mutex for controlling access to incoming threads structure.
     * @see inCalls
     */
    SBWOSMutex inCallsMutex ;

    /**
     * mutex for controlling access to outgoing threads structure.
     * @see outCalls
     */
    SBWOSMutex outCallsMutex ;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWRPC_H__90F43945_EB1B_476C_B02D_6497210BFB27__INCLUDED_)
