#ifndef SBW_INSTANCE
#define SBW_INSTANCE

#include "Host.h"

#include <string>
#include <iostream>

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{

		/**
		 * \ingroup C++Broker
		 *
		 * An <code>Instance</code> is the interface that all module and broker
		 * objects implement.  This provides a common interface that can be used to
		 * treat local and remote module and broker instances in a generic manner.
		 * All instance classes ultimately must implement this interface.
		 * <p>
		 * The interface subclass RemoteInstance extends Instance
		 * with additional methods appropriate to objects that represent instances
		 * running on remote brokers.  Instance classes implement one or the other
		 * of these two interfaces.
		 * @see RemoteInstance
		 * @see InstanceBase
		 * @see InstanceThread
		 * @see ModuleInstance
		 * @see BrokerInstance
		 * @see ProxyModuleInstance
		 * @see ProxyLocalBrokerInstance
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class Instance
		{
		public:
			virtual ~Instance() {}
			/**
			* Returns the identifier for this instance.
			* <p>
			* @return the unique integer identifier for this instance.
			**/
			virtual int getId() = 0;

			/**
			* Returns the identifier of the broker responsible for this instance.
			* <p>
			* @return the unique integer identifier for the broker
			**/
			virtual int getBrokerId() = 0;

			/**
			* Returns the host structure for the computer where this instance
			* is running.
			* <p>
			* @return the host object
			**/
			virtual Host *getHost() = 0;

			/**
			* Returns the unique name of this module or broker instance, without
			* a host name prefix. 
			* <p>
			* @return the name of the module or broker instance
			**/
			virtual std::string getName() = 0;

			/**
			* Returns the full name of this module or broker instance, in the
			* form "hostname:modulename".
			* <p>
			* @return the name of this module or broker instance, with a host
			* prefix
			**/
			virtual std::string getFullName() = 0;

			/**
			* Returns the internal name of this module or broker instance, in the
			* form "hostaddress:modulename".
			* <p>
			* @return the internal name of this module or broker instance, with a host
			* address prefix
			**/
			virtual std::string getInternalName() = 0;

			/**
			* Returns a complete identification string for this instance.  The
			* string has the form <code>"type id (address:name)"</code>, where
			* <code>type</code> is the type of this instance (either
			* <code>"module"</code> or <code>"broker"</code>), <code>id</code> is
			* the integer identifier, and <code>address:name</code> is the host
			* and module or Broker name string.  This string is useful for
			* printing diagnostic messages.
			* <p>
			* @return an informative identification string for this instance
			* suitable for use in debugging traces and so on.
			**/
			virtual std::string getNameForMessage() = 0;

			/**
			* Method called to establish a connection to a broker.  It should
			* return <code>true</code> or <code>false</code>, depending on
			* whether the connection was successfully established.
			* <p>
			* @return <code>true</code> or <code>false</code>, depending on the
			* success of connecting to the broker.
			**/
			virtual bool connect(const char* key) = 0;

			/**
			* Called to disconnect this instance from a broker.  This should
			* result in this instance shutting down.
			**/
			virtual void disconnect() = 0;

			/**
			* Method used by other instances to transmit data to this instance.
			* This is called from the message-processing loop of another instance
			* to deliver a message intended for this module.
			* <p>
			* This variant of the transmit method handles transmitting data to the
			* module for the case where the data is in the form of a byte array.
			* The contents of <code>data</code> must be a message in the
			* appropriate format.  Unlike the other variant of this method (see
			* transmit(InputStream otherIn, int length)), this version
			* assumes that the message in the parameter <code>data</code> is the
			* entire message to be sent to our module, i.e., including the length
			* and destination fields.
			* <p>
			* Note that the <code>length</code> argument is the length of the
			* whole message to be transmitted, but the byte array in
			* <code>data</code> may not be of that length.  Thus, the underlying
			* code should not do <code>data.length</code> to judge the length of
			* the message; it should rely on the parameter <code>length</code>.
			* <p>
			* @param data a byte array containing the data to be written
			* @param length the number of bytes in the message
			*/
			virtual void transmit(unsigned char *data, int length) = 0;
		};
	}
}

#endif
