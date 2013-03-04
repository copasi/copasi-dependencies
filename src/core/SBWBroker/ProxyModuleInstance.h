#ifndef SBW_BROKER_PROXY_MODULE_INSTANCE
#define SBW_BROKER_PROXY_MODULE_INSTANCE

#include "InstanceBase.h"
#include "BrokerInstance.h"

#include <string>
#include <iostream>

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 *
		 * ProxyModuleInstance acts as a proxy for remote ModuleInstances. 
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class ProxyModuleInstance : public InstanceBase
		{
		public:
			/**
			 * constructs a local Module out of the parameters given
			 * \param id the local module identifier
			 * \param remoteId the remote module identifier
			 * \param name the name of the module
			 * \param *brokerInst the broker instance that controls the module			 
			 */
			ProxyModuleInstance(int id, int remoteId, std::string name, BrokerInstance *brokerInst);
			/**
			 *
			 * \return the remote module identifier
			 */
			virtual int getRemoteId();
			/**
			* Method used by other instances to transmit data to this instance.
			* This is called from the message-processing loop of another instance
			* to deliver a message intended for this module.
			* <p>
			* This variant of the transmit method handles transmitting data to the
			* module for the case where the data is in the form of a byte array.
			* The contents of <code>data</code> must be a message in the
			* appropriate format.  
			* <p>
			* @param data a byte array containing the data to be written
			* @param length the number of bytes in the message
			**/
			virtual void transmit(unsigned char *data, int length);
			/**
			* Connect the streams to the socket and shake hands with the module
			* instance.  The handshaking consists of reading the module's name
			* (which it sends as the first message immediately after the connection
			* is established) and then sending to the module its numeric module
			* identifier.
			* <p>
			* @return <code>true</code> if the streams were successfully connected
			* to the socket and handshaking was successful; <code>false</code>
			* otherwise
			**/
			virtual bool connect(const char* key);
			/**
			 * disconnect communication streams
			 */
			virtual void disconnect();
		private:
			int myRemoteId;						///< the remote identifier
			BrokerInstance *myBrokerInstance;	///< the broker that controls this module
			SBWOSMutex transmitMutex ;			///< transmission mutex to ensure thread safety
		};
	}
}

#endif

