#ifndef SBW_BROKER_INSTANCE_BASE
#define SBW_BROKER_INSTANCE_BASE

#include "SBW/SBWOSSocket.h"

#include "Instance.h"
#include "Host.h"

#include "SBW/portableOS.h"

#include <string>
#include <sstream>

using namespace std;

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 *
		 * Base class for instance objects.  This class provides members and
		 * methods that are common to all the instance objects used by the Broker.
		 * Instance classes such as ModuleInstance extend this class.
		 * <p>
		 * This class purposefully does not implement the Instance interface, 
		 * because the different subclasses need to provide different implementations
		 * of some of the methods of the Instance interface.
		 * <p>
		 * @see Instance
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
		class InstanceBase : public Instance
		{
		public:
			/** The local identifier for this instance.  **/		
			int myId;
		protected:
			/** The identifier of the broker responsible for this instance. **/
			int myBrokerId;
			/** The host on which this instance is running. **/
			Host *myHost;
			/** The full name of this instance sans host prefix. **/
			std::string myName;
			/** The full name, including host prefix, of this instance. **/
			std::string myFullName;
			/** The full name, including host prefix, of this instance. **/
			std::string myInternalName;
			/** The type of this instance, either "module" or "broker". **/
			std::string myType;
			/**
			* Protected constructor for this class.  There never needs to be
			* an actual instance of InstanceBase per se, since all
			* the work is done in subclasses.
			**/
			InstanceBase() {}
		public: 
			/**
			* Returns the numeric identifier for this instance as it was
			* assigned by the <i>local</i> broker.  An instance may have different
			* identifiers on other brokers.
			* <p>
			* @return the integer id of this instance.
			**/
			virtual int getId() { return myId; }
			/**
			* Returns the numeric identifier of the broker responsible for this
			* instance.
			* <p>
			* @return the integer id of the broker
			**/
			virtual int getBrokerId() { return myBrokerId; }
			/**
			* Returns the Host object for the computer on which this
			* instance is running.
			* <p>
			* @return the name of the host.
			**/
			virtual Host *getHost() { return myHost; }
			/**
			* Return the name (sans host) for this instance, i.e., the unique
			* name of the module or Broker.
			* <p>
			* @return the unique name of the module or broker, with a host prefix
			* @see getFullName
			**/
			virtual std::string getName() { return myName; }
			/**
			* Returns a full name for this instance, in the form of
			* <code>"host:modulename"</code>, where the <code>modulename</code>
			* portion is equal to what is returned by getName()
			* and the <code>host</code> portion is equal to what is returned by
			* getHost().
			* <p>
			* @return the full name of this instance in <code>host:name</code> form
			* @see getName
			* @see getHost
			**/
			virtual std::string getFullName() { return myFullName; }
			/**
			* Returns the internal name for this instance, in the form of
			* <code>"hostaddress:modulename"</code>, where the <code>modulename</code>
			* portion is equal to what is returned by getName()
			* and the <code>hostaddress</code> portion is equal to what is returned by
			* getHostAddress().
			* <p>
			* @return the full name of this instance in <code>host:name</code> form
			* @see getName
			* @see getHost
			**/
			virtual std::string getInternalName() { return myInternalName; }
			/**
			* Returns a string that includes the type, identifier and full name
			* of this instance.  This string is suitable for use in printing
			* diagnostic messages.
			* <p>
			* @return a string completely describing this instance.
			**/
			virtual std::string getNameForMessage() 
			{ 
				std::stringstream sResult;
				sResult << myType << " " << myId << " ('" << myFullName << "')";
				return sResult.str(); 
			}
		};
	}
}

#endif
