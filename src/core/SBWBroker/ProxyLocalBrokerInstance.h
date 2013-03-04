#ifndef SBW_BROKER_PROXY_LOCAL_BROKER_INSTANCE
#define SBW_BROKER_PROXY_LOCAL_BROKER_INSTANCE

#include "BrokerDefines.h"
#include "InstanceBase.h"
#include "Instance.h"
#include "SBWBrokerRPC.h"

#include "SBW/SBWRPC.h"

#include <string>
#include <iostream>

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 *
		 * ProxyLocalBrokerInstance represents a proxy for the remote broker instances. 
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class ProxyLocalBrokerInstance : public InstanceBase
		{
		private:
			static SBWRPC *rpc;			///< the RPC object used to communicate
			SBWOSMutex transmitMutex ;	///< mutex to ensure thread safety
		public:
			/**
			 *
			 * default constructor
			 */
			ProxyLocalBrokerInstance();
			/**
			* Connects this instance's streams to the socket and performs initial
			* handshaking with the remote broker.  The handshaking consists of
			* verifying the session key for the broker, and exchanging the id's
			* that each broker uses for the other one.
			**/			
			bool connect(const char *key);
			/**
			* This variant of the transmit method handles transmitting data to the
			* module for the case where the data is in the form of a byte array.
			* The contents of <code>data</code> must be a message in the
			* appropriate format. This version
			* assumes that the message in the parameter <code>data</code> is the
			* entire message to be sent to our module, i.e., including the length
			* and destination fields.
			* <p>
			* @param data a byte array containing the data to be written
			* @param length the number of bytes in the message
			*
			**/
			void transmit(unsigned char *data, int length);
			/**
			 * disconnects communication streams
			 */
			void disconnect();
		};
	}
}

#endif

