/**
 * @file SBWRPC.h
 * @brief common lowlevel implementation of message handling common to both broker and module
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWRPC.h,v 1.2 2006/09/07 19:51:02 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWRPC.h,v $
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

#include "DataBlockReaderClass.h"
#include "SBWRPCListener.h"
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
class SBWRPC  
{
public:
	SBWRPC(SBWRPCListener *l);
	virtual ~SBWRPC();

	SBW_API DataBlockReader call(Integer moduleTo, Integer service, Integer method, DataBlockWriter args);

	/**
	 * make connection.
	 * @param moduleIdentificationString module identification string of this module instance.
	 *                                   default is the empty string which indicates an anonymous connection 
	 */
	virtual void connect(const char *moduleIdentificationString = "", const char *hostname = "") = 0;

	/// disconnect.
	void disconnect()
	{
		signalDisconnect();
		waitForDisconnect();
	}

	/** 
	 * returns whether there is a connection on this link.
	 * @return whether there is a connection on this link.
	 */
	virtual bool isConnected() = 0;

	/// block until disconnection
	virtual void waitForDisconnect() = 0 ;

	/// send message requesting disconnection
	virtual void signalDisconnect() = 0 ;

	SBW_API void receive(DataBlockReader reader);
	SBW_API void registerReceiver(Receiver *);
	SBW_API void send(Integer moduleTo, Integer service, Integer method, DataBlockWriter args);
	SBW_API void transmit(Integer moduleTo, unsigned char *messageForRelease, int length);

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
