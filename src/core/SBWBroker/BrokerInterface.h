#ifndef SBW_BROKER_INTERFACE
#define SBW_BROKER_INTERFACE

#include <string>
#include <vector>

#include "SBW.h"

#include "SBW/SBWRPC.h"
#include "SBW/DataBlockWriter.h"

using namespace std;
using namespace SystemsBiologyWorkbench;

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 * 
		 * The BrokerInterface provides the Interface to remote brokers. It allows to execute each of the 
		 * commands implemented.
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class BrokerInterface
		{
		public:
			/**
			 * constructs a BrokerInterface for the given id
			 * \param nId the id of the remote broker			 
			 */
			BrokerInterface(int nId);
			/**
			* Returns a string giving the version number of the SBW Broker.
			* <p>
			* @retun the version number as a string
			**/
			string getVersion();
			/**
			* Returns an array of lists describing all the services registered
			* with SBW in the given <code>category</code>.  If the boolean
			* parameter <recursive> is true, it searches subcategories as well.
			* <p>
			* The format of each list returned in the array is:
			* <pre>
			*    string moduleIdentificationName,
			*    string serviceIdentificationName,
			*    string serviceDisplayName,
			*    string serviceCategory
			*    string serviceHelpString</pre>
			*
			* @param category the category of services to find
			* @param recursive whether to descend recursively down the
			* hierarchy of categories, matching the given category
			* @return an array of lists, each of which represents the contents
			* of a ServiceDescriptor class object
			**/
			vector< DataBlockWriter > findServices(string category, bool recursive);
			/**
			* Returns an array of lists describing all local services registered
			* with SBW in the given <code>category</code>.  If the boolean
			* parameter <recursive> is true, it searches subcategories as well.
			* <p>
			* The format of each list returned in the array is:
			* <pre>
			*    string moduleIdentificationName,
			*    string serviceIdentificationName,
			*    string serviceDisplayName,
			*    string serviceCategory
			*    string serviceHelpString</pre>
			*
			* @param category the category of services to find
			* @param recursive whether to descend recursively down the
			* hierarchy of categories, matching the given category
			* @return an array of lists, each of which represents the contents
			* of a ServiceDescriptor class object
			**/						
			std::vector < DataBlockWriter > findLocalServices(std::string category, bool recursive);
			/**
			* Returns a module descriptor (in list form) corresponding to the
			* named module.  The argument <code>includeRunning</code> determines
			* whether running modules are checked in the event that the named
			* module is not registered.  If <code>includeRunning</code> is
			* <code>true</code>, then running modules are considered; otherwise
			* they are not.
			* <p>
			* The module name in <code>name</code> can be prefixed with a
			* host name, as in <code>"host:moduleName"</code>.  In that case,
			* the module is taken to refer to a module registered or running
			* on the given host.  If a remote broker is not already running
			* on the host, the request for startup is cancelled.
			* <p>
			* The format of the list returned is:
			* <pre>
			*     moduleIdentificationName
			*     moduleDisplayName
			*     moduleManagementType
			*     moduleCommandLine
			*     moduleHelpString</pre>
			* @param name the name of the module sought
			* @param includeRunning whether running modules are included
			* during search
			* @return a list whose fields correspond, in order, to the
			* contents of a ModuleDescriptor class object
			**/
			DataBlockWriter getModuleDescriptor(string moduleName, bool includeRunning);
			/**
			* Returns a module descriptor (in list form) corresponding to the
			* module whose identifier is given.  The argument must be an integer
			* identifier for a running module.  The module may be local or remote.
			* <p>
			* The format of the list returned is:
			* <pre>
			*     moduleIdentificationName
			*     moduleDisplayName
			*     moduleManagementType
			*     moduleCommandLine
			*     moduleHelpString</pre>
			* @param moduleId the unique identifier of the module sought
			* @return a list whose fields correspond, in order, to the
			* contents of a ModuleDescriptor class object
			**/
			DataBlockWriter getModuleDescriptor(int moduleId);
			/**
			* Returns module descriptors for all known modules, modified by the
			* two given arguments.  If the boolean flag <code>localOnly</code> is
			* <code>true</code>, only modules known to the local broker will be
			* considered; otherwise, if the flag is <code>false</code>, all
			* modules, including those on remote brokers, will be considered.  If
			* the boolean flag <code>includeRunning</code> is <code>false</code>,
			* only registered modules will be included (since those are the only
			* kinds of modules for which the Broker will have registration
			* information).  If the flag is <code>true</code>, this method
			* additionally includes running module instances.  The difference in
			* behavior is relevant when there are unregistered modules connected
			* to SBW: when <code>includeRunning</code> is <code>false</code>,
			* unregistered modules are not included in the array of module
			* descriptors returned.
			* <p>
			* In all cases, one of the entries in the list returned will be
			* the local broker itself.  If the parameter <code>localOnly</code>
			* is <code>false</code>, there will be an additional entry for each
			* SBW Broker connected to the local Broker
			* <p>
			* This returns an array of lists, where each list contains:
			* <pre>
			*     moduleIdentificationName
			*     moduleDisplayName
			*     moduleManagementType
			*     moduleCommandLine
			*     moduleHelpString</pre>
			*
			* @param localOnly whether to include only modules known to the local
			* broker or all modules anywhere
			* @param includeRunning whether to include running modules or only
			* modules that have been registered with SBW
			* @return an array of lists, where the fields of each list correspond,
			* in order, to the contents of a ModuleDescriptor class object
			**/
			vector< DataBlockWriter  > getModuleDescriptors(bool localOnly, bool includeRunning);
			/**
			* Returns an integer module id for an instance of the module named
			* <code>name</code>.  If the module instance is
			* UNIQUE, then an existing instance identifier is returned; otherwise,
			* a new instance is launched.  A new instance is always launched if no
			* existing instance exists.
			* <p>
			* The module name in <code>name</code> can be prefixed with a
			* host name, as in <code>"host:moduleName"</code>.  In that case,
			* the module is taken to refer to a module registered or running
			* on the given host.  If a remote broker is not already running
			* on the host, the request for startup is cancelled.
			* <p>
			* @param name the name of the module to instantiate
			* @return an integer module identifier
			**/
			int getModuleInstance(string moduleName);
			/**
			* Returns an array of strings representing the categories that are
			* subcategories of the given category.  The category hierarchy is
			* set by using the slash '/' character as a separator in category
			* strings.
			* <p>
			* @param parentCategory the root of the subcategories being sought
			* @return an array of strings, the subcategories of the given
			* category
			**/
			vector< string > getServiceCategories(string parentCategory);
			/**
			* Establishes a connection between the current SBW Broker and a Broker
			* running on the named host.  This will if no Broker is running at the
			* remote host this operation will fail.
			* <p>
			* @param hostName the name or IP address (as a string) of the target host
			* @return the numeric identifier for the remote Broker instance
			**/
			int linkBroker(string hostNameOrAddress);
			/**
			* Return an array of identifiers corresponding to all known running
			* modules and brokers.
			* <p>
			* @return an array of integers
			**/			
			vector<int> getExistingModuleInstanceIds();
			/**
			* This changes the name of a running module.  It does this by changing
			* the registry entry for the module, and sending a notification to all
			* other running modules and brokers that a registration change has
			* taken place.
			* <p>
			* @param moduleId the identifer of the module
			* @param newModuleName the new unique name for the module
			**/
			void changeModuleName(int moduleId, string newModuleName);
			/**
			* Returns service descriptors for a given service on a given module.
			* This handles the case where both the module and service are specified
			* in terms of numeric identifiers.
			* This returns a single list with the following contents:
			* <pre>
			*  0  string moduleName
			*  1  string serviceName
			*  2  string serviceDisplayName
			*  3  string category
			*  4  string helpString</pre>
			*
			* @param moduleId the identifer of the module
			* @param serviceId the identifier of the service on the module
			* @return a single list representing the contents of a
			*  ServiceDescriptor class object
			**/
			DataBlockWriter  getServiceDescriptor(int moduleId, int serviceId);
			/**
			* Returns service descriptors for a given service on a given module.
			* This handles the case where the module is specified in terms of its
			* numeric identifier and the service is specified by its name.
			* This returns a single list with the following contents:
			* <pre>
			*  0  string moduleName
			*  1  string serviceName
			*  2  string serviceDisplayName
			*  3  string category
			*  4  string helpString</pre>
			*
			* @param moduleId the identifer of the module
			* @param servName the name of the service
			* @return a single list representing the contents of a
			* ServiceDescriptor class object
			**/
			DataBlockWriter getServiceDescriptor(int moduleId, string serviceName);
			/**
			* Returns service descriptors for all services on a given module.
			* This is for the case where the module is specified in terms of its
			* numeric identifier.
			* This returns an array of lists, where each list contains:
			* <pre>
			*  0  string moduleName
			*  1  string serviceName
			*  2  string serviceDisplayName
			*  3  string category
			*  4  string helpString</pre>
			*
			* @param moduleId the identifer of the module
			* @return an array of lists, where the fields of each list correspond,
			* in order, to the contents of a ServiceDescriptor class object
			**/
			vector<DataBlockWriter > getServiceDescriptors(int moduleId);
			/**
			* Returns service descriptors for all services on a given module.
			* This is for the case where the module is specified in terms of its
			* name.  The module name in <code>name</code> can be prefixed with a
			* host name, as in <code>"host:moduleName"</code>.  In that case, the
			* module is taken to refer to a module registered or running on the
			* given host.  If a remote broker is not already running on the host,
			* one is started up and the request is handed to it.
			* <p>
			* This returns an array of lists, where each list contains:
			* <pre>
			*  0  string moduleName
			*  1  string serviceName
			*  2  string serviceDisplayName
			*  3  string category
			*  4  string helpString</pre>
			*
			* @param name the name of the module
			* @return an array of lists, where the fields of each list correspond,
			* in order, to the contents of a ServiceDescriptor class object
			**/
			vector<DataBlockWriter > getServiceDescriptors(string moduleName);
			/**
			* This registers a service on a module.  It adds an entry
			* to the module registry and sends a notification to all other
			* running modules and brokers that a registration change has taken
			* place.
			* <p>
			* The given <code>moduleName</code> cannot have a host prefix.  Module
			* registration can only be performed with respect to the local broker;
			* it is not possible to register a module with a remote broker via
			* the local broker.
			* <p>
			* @param moduleName the unique name of the module
			* @param serviceName the name of the service to register
			* @param serviceDisplayName the display name of the service
			* @param category the category of the service
			* @param help a help string for the service
			**/
			void registerModule(string moduleName, string moduleDisplayName,
				int moduleType, string commandLine,
				string helpString);
			/**
			* This registers a service on a module.  It adds an entry
			* to the module registry and sends a notification to all other
			* running modules and brokers that a registration change has taken
			* place.
			* <p>
			* The given <code>moduleName</code> cannot have a host prefix.  Module
			* registration can only be performed with respect to the local broker;
			* it is not possible to register a module with a remote broker via
			* the local broker.
			* <p>
			* @param moduleName the unique name of the module
			* @param serviceName the name of the service to register
			* @param serviceDisplayName the display name of the service
			* @param category the category of the service
			* @param help a help string for the service
			**/
			void registerService(string moduleName, string serviceName,
				string serviceDisplayName, string category,
				string helpString);
			/**
			* Shuts down the broker in a clean fashion.  All module connections
			* will be closed and the broker will exit.
			* <p>
			* There is no version of this method that takes a remote host name
			* or IP address, because it's not needed.  To shut down a remote
			* Broker, a client simply needs to get the instance id of the remote
			* Broker, and then send it a shutdownBroker message.
			**/
			void shutdownBroker();
			/**
			* Unregister the named module from the local broker.
			* <p>
			* The given <code>moduleName</code> cannot have a host prefix.  Module
			* registration can only be performed with respect to the local broker;
			* it is not possible to register a module with a remote broker via
			* the local broker.
			* <p>
			* @param moduleName the unique name of the module
			**/
			void unregisterModule(string moduleName);
		private:
			int m_nId;					///< the broker identifier this interface belongs to
			SBWRPC *m_oRPC;				///< the SBWRPC object used to communicate
		};
	}
}

#endif
