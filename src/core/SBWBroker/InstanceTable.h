#ifndef SBW_INSTANCE_TABLE
#define SBW_INSTANCE_TABLE

#include "Instance.h"
#include "IntegerHashtable.h"
#include "Host.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		* \ingroup C++Broker
		*
		* This is used to maintain a table of all instances known to this
		* broker.  The instance classes are currently ModuleInstance,
		* BrokerInstance, ProxyModuleInstance and
		* ProxyLocalBrokerInstance.
		* <p>
		* Instances are keyed by integer identifiers and names.  The instance
		* table maintained by this class is keyed by both the integer identifers
		* and full (i.e., "host:module") module names.  The mappings are
		* implemented using two different kinds of hash tables.
		* <p>
		* This class also implements the generator of unique module identifiers.
		* The method #getNextId() returns the next integer identifier
		* and increments an internal counter.  There is no reset method.  The
		* numerical identifiers keep increasing monotonically until the integers
		* wrap, and then start over again.
		* <p>
		* This class enforces a singleton design pattern.  Only one instance
		* of InstanceTable exists at run-time.  This object is created
		* in a static initializer and can be retrieved using #getTable().
		* <p>
		* @see SBWBrokerRPC
		*
		* \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class InstanceTable
		{
		private:

			/** The actual instance table, a singleton. **/
			static InstanceTable *table;
			/** The hash table mapping identifers to instances. **/
			IntegerHashtable<Instance *> idHash;
			/** The hash table mapping module names to instances. **/
			typedef pair< string , Instance *> StringInstance;
			map< string , Instance *> nameHash;			
			/** The internal counter of identifers used by #getNextId(). **/
			int nextId;
			/**
			* Private constructor that suppresses the default public constructor.
			* This, and the following method for returning an instance of this
			* class (#getTable()), implement the "Singleton" design
			* pattern (named in the Design Patterns book by Gamma et al.).  See
			* <a href="http://developer.java.sun.com/developer/qow/archive/111/">
			* http://developer.java.sun.com/developer/qow/archive/111/</a> for
			* more information.
			**/
			InstanceTable() : instanceMutex("instanceTableMutex")	{nextId = 0;}
			SBWOSMutex instanceMutex ;		///< mutex object ... 
		public:
			/**
			* \return the (single) instance of this class.
			**/
			static InstanceTable *getTable();

      static void deleteTable();

			/**
			* Add an instance to the table of instances.
			* <p>
			* @param inst the instance to be added
			**/
			void addInstance(Instance *inst);
			/**
			* Remove an instance from the table of instances.
			* <p>
			* @param inst the instance to be removed
			**/
			void removeInstance(Instance *inst);
			/**
			* Remove an instance from the table of instances.  This variant
			* of the remove method takes a module identifier instead of
			* an Instance object.
			* <p>
			* @param moduleId the identifer of the module to be removed
			**/
			void removeInstance(int moduleId);
			/**
			* Retrieve the instance whose identifier is given.
			* <p>
			* @param moduleId the identifier of the instance to be retrieved
			**/
			Instance *getInstance(int moduleId);
			/**
			* Retrieve the instance given a host and a module name.
			* <p>
			* @param host a host object representing the host computer for the
			* instance
			* @param moduleName the name of the module
			* @return an object implementing {@link Instance} 
			**/
			Instance *getInstance(Host *host, string moduleName);
			/**
			* Returns all instances that are running on the given broker.
			* This is implemented by retrieving all instances whose
			* Instance::getBrokerId() method returns a value matching
			* the <code>brokerId</code> parameter.  The array of returned
			* instances also includes the broker whose identifier is
			* <code>brokerId</code>.
			* <p>
			* @param brokerId the identifier of the broker whose instances
			* are to be returned
			* @returns an array of Instance objects for the instances
			* running on the specified broker
			**/
			vector<Instance *> getInstances(int brokerId);
			/**
			* Retrieve all instances in the table.
			* <p>
			* @returns an array of all Instance objects in the instance table
			**/
			vector<Instance *> getInstances();
			/**
			* Returns <code>true</code> if a module with the given identifier
			* is found in the table of instances, <code>false</code> otherwise.
			* <p>
			* @param moduleId the identifier of the module to be checked
			* @returns <code>true</code> or <code>false</code> depending on whether
			* the module is found in the table
			**/
			bool exists(int moduleId);
			/**
			* Returns the next unique identifier to be used for a new instance
			* <p>
			* @returns the integer to be used as the next module identifier
			**/
			int getNextId();
		private:
			/**
			* The readResolve method serves to prevent the release of multiple
			* instances upon deserialization.  I don't know if we will ever need
			* to worry about serialization, but I may as well add this because it's
			* trivial to do and documented in
			* <a href="http://developer.java.sun.com/developer/qow/archive/111/">
			* http://developer.java.sun.com/developer/qow/archive/111/</a> for
			**/
			InstanceTable *readResolve();
		};
	}
}
#endif
