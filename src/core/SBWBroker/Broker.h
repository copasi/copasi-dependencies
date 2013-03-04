#ifndef SBW_BROKER
#define SBW_BROKER

#include "Host.h"
#include "SBWBrokerRPC.h"
#include "InstanceTable.h"
#include "ModuleRegistry.h"
#include "Instance.h"
#include "BrokerInterface.h"
#include "BrokerInstance.h"

#include "SBW/MethodTable.h"
#include "SBW/ModuleDescriptor.h"
#include "SBW/ServiceDescriptor.h"
#include "SBW/portableOS.h"

#include <string>
#include <vector>

using namespace SystemsBiologyWorkbench;
using namespace SystemsBiologyWorkbench::Broker;
using namespace std;

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 *
		 * The class CBroker is represents the core of the C++ port of the SBW Java Broker. Here all callable 
		 * methods will be processed. Most functionality is separated into a local and in a remote function. 
		 * The function to be called depends on the ModuleInstance. 
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
		 *     http://www.cds.caltech.edu/erato
		 *     mailto:sysbio-team@caltech.edu
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman@kgi.edu
		 *
		 *
		 */
		class CBroker
		{
		public:
			/**
			* Constructor.  This adds an entry for the local broker into the
			* module registry and adds an instance into the instance table.
			**/
			CBroker();
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
			int getModuleInstance(std::string name);
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
			*     moduleHelpstd::string</pre>
			* @param name the name of the module sought
			* @param includeRunning whether running modules are included
			* during search
			* @return a list whose fields correspond, in order, to the
			* contents of a ModuleDescriptor class object
			**/
			DataBlockWriter getModuleDescriptor(std::string name, bool includeRunning);
			/**
			* Return an array of identifiers corresponding to all known running
			* modules and brokers.
			* <p>
			* @return an array of integers
			**/			
			std::vector<int> getExistingModuleInstanceIds();
			/**
			* Returns an array of std::strings representing the categories that are
			* subcategories of the given category.  The category hierarchy is
			* set by using the slash '/' character as a separator in category
			* std::strings.
			* <p>
			* @param parentCategory the root of the subcategories being sought
			* @return an array of std::strings, the subcategories of the given
			* category
			**/
			std::vector<std::string> getServiceCategories(std::string parentCategory);
			/**
			* Returns an array of lists describing all the services registered
			* with SBW in the given <code>category</code>.  If the boolean
			* parameter <recursive> is true, it searches subcategories as well.
			* <p>
			* The format of each list returned in the array is:
			* <pre>
			*    std::string moduleIdentificationName,
			*    std::string serviceIdentificationName,
			*    std::string serviceDisplayName,
			*    std::string serviceCategory
			*    std::string serviceHelpstd::string</pre>
			*
			* @param category the category of services to find
			* @param recursive whether to descend recursively down the
			* hierarchy of categories, matching the given category
			* @return an array of lists, each of which represents the contents
			* of a ServiceDescriptor class object
			**/
			std::vector < DataBlockWriter > findServices(std::string category, bool recursive);
			/**
			* Returns an array of lists describing all local services registered
			* with SBW in the given <code>category</code>.  If the boolean
			* parameter <recursive> is true, it searches subcategories as well.
			* <p>
			* The format of each list returned in the array is:
			* <pre>
			*    std::string moduleIdentificationName,
			*    std::string serviceIdentificationName,
			*    std::string serviceDisplayName,
			*    std::string serviceCategory
			*    std::string serviceHelpstd::string</pre>
			*
			* @param category the category of services to find
			* @param recursive whether to descend recursively down the
			* hierarchy of categories, matching the given category
			* @return an array of lists, each of which represents the contents
			* of a ServiceDescriptor class object
			**/						
			std::vector < DataBlockWriter > findLocalServices(std::string category, bool recursive);
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
			* @param help a help std::string for the service
			**/
			void registerModule(std::string moduleName, std::string moduleDisplayName,
				int moduleType, std::string commandLine, std::string help);
			/**
			* This changes the name of a running module.  It does this by changing
			* the registry entry for the module, and sending a notification to all
			* other running modules and brokers that a registration change has
			* taken place.
			* <p>
			* @param moduleId the identifer of the module
			* @param newModuleName the new unique name for the module
			**/
			void changeModuleName(int moduleId, std::string newModuleName);
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
			* @param help a help std::string for the service
			**/
			void registerService(std::string moduleName, std::string serviceName,
				std::string serviceDisplayName, std::string category,
				std::string help);
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
			void unregisterModule(std::string moduleName);
			/**
			* Establishes a connection between the current SBW Broker and a Broker
			* running on the named host.  This will if no Broker is running at the
			* remote host this operation will fail.
			* <p>
			* @param hostName the name or IP address (as a std::string) of the target host
			* @return the numeric identifier for the remote Broker instance
			**/
			int linkBroker(std::string hostName);
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
			* Returns a std::string giving the version number of the SBW Broker.
			* <p>
			* @retun the version number as a std::string
			**/
			static std::string getVersion();
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
			*     moduleHelpstd::string</pre>
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
			*     moduleHelpstd::string</pre>
			*
			* @param localOnly whether to include only modules known to the local
			* broker or all modules anywhere
			* @param includeRunning whether to include running modules or only
			* modules that have been registered with SBW
			* @return an array of lists, where the fields of each list correspond,
			* in order, to the contents of a ModuleDescriptor class object
			**/
			std::vector< DataBlockWriter > getModuleDescriptors(bool localOnly, bool includeRunning);
			/**
			* Returns service descriptors for a given service on a given module.
			* This handles the case where both the module and service are specified
			* in terms of numeric identifiers.
			* This returns a single list with the following contents:
			* <pre>
			*  0  std::string moduleName
			*  1  std::string serviceName
			*  2  std::string serviceDisplayName
			*  3  std::string category
			*  4  std::string helpstd::string</pre>
			*
			* @param moduleId the identifer of the module
			* @param serviceId the identifier of the service on the module
			* @return a single list representing the contents of a
			*  ServiceDescriptor class object
			**/
			DataBlockWriter getServiceDescriptor(int moduleId, int serviceId);
			/**
			* Returns service descriptors for a given service on a given module.
			* This handles the case where the module is specified in terms of its
			* numeric identifier and the service is specified by its name.
			* This returns a single list with the following contents:
			* <pre>
			*  0  std::string moduleName
			*  1  std::string serviceName
			*  2  std::string serviceDisplayName
			*  3  std::string category
			*  4  std::string helpstd::string</pre>
			*
			* @param moduleId the identifer of the module
			* @param servName the name of the service
			* @return a single list representing the contents of a
			* ServiceDescriptor class object
			**/
			DataBlockWriter getServiceDescriptor(int moduleId, std::string servName);
			/**
			* Returns service descriptors for all services on a given module.
			* This is for the case where the module is specified in terms of its
			* numeric identifier.
			* This returns an array of lists, where each list contains:
			* <pre>
			*  0  std::string moduleName
			*  1  std::string serviceName
			*  2  std::string serviceDisplayName
			*  3  std::string category
			*  4  std::string helpstd::string</pre>
			*
			* @param moduleId the identifer of the module
			* @return an array of lists, where the fields of each list correspond,
			* in order, to the contents of a ServiceDescriptor class object
			**/
			std::vector<DataBlockWriter> getServiceDescriptors(int moduleId);
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
			*  0  std::string moduleName
			*  1  std::string serviceName
			*  2  std::string serviceDisplayName
			*  3  std::string category
			*  4  std::string helpstd::string</pre>
			*
			* @param name the name of the module
			* @return an array of lists, where the fields of each list correspond,
			* in order, to the contents of a ServiceDescriptor class object
			**/
			std::vector<DataBlockWriter> getServiceDescriptors(std::string name);
			/**
			 * Returns the list of running modules. Each entry consists of the 
			 * module identifier and the module name. 
			 * \return list of running modules
			 */
			DataBlockWriter getListOfModules();
			/**
			 * Returns a list of methods of the service <code>serviceId</code> on 
			 * module <code>moduleId</code>. Each entry consists of the method id and
			 * the method signature. 
			 *
			 * \param moduleId the module identifier of the module
			 * \param serviceId the service identifier to list the methods from
			 * \return list of methodIds and signatures
			 */
			DataBlockWriter getMethodIds(int moduleId,int serviceId);
			/**
			 * Returns a list of methods of the service <code>serviceName</code> on 
			 * module <code>moduleName</code>. Each entry consists of the method id and
			 * the method signature. 
			 *
			 * \param moduleName the name of the module
			 * \param serviceName the name of the service
			 * \return list of methodIds and signatures
			 */
			DataBlockWriter getMethodIds(std::string moduleName,std::string serviceName);
			/**
			 * This method checks whether a given module is a remote or a local module. 
			 * It will return <b>true</b> if it is an remote module and <b>false</b> otherwise.
			 * \param nModuleId the module identifier to check
			 * \return <b>true</b> if it is an remote module and <b>false</b> otherwise.
			 */
			bool isRemoteInstance(int nModuleId);
		private:
			/**
			* Helper for #getModuleInstance, for the case of a local
			* module.  The given <code>moduleName</code> must have any host
			* prefix stripped off.  The host is assumed to be the local host.
			* <p>
			* @param moduleName the name of the module to instantiate
			* @return an integer module identifier
			**/
			int getModuleInstanceLocal(std::string moduleName);
			/**
			* Helper for #getModuleInstance, for the case of a remote
			* module.  The given <code>moduleName</code> must have any host
			* prefix stripped off.  The host must be given in <code>host</code>.
			* <p>
			* @param moduleName the name of the module to instantiate
			* @param host the host on which the module should be started up
			* @return an integer module identifier
			**/    
			int getModuleInstanceRemote(std::string moduleName, Host *host);
			/**
			* Helper for getModuleDescriptor(std::string name, boolean incRunning),
			* This assumes that the named module is local to the current broker.
			* The given <code>moduleName</code> must have any host prefix stripped off.
			* The host is assumed to be the local host.
			* <p>
			* @param moduleName the name of the module sought
			* @param includeRunning whether running modules are included
			* during search
			* @return a list whose fields correspond, in order, to the
			* contents of a ModuleDescriptor class object
			**/
			DataBlockWriter getModuleDescriptorLocal(std::string moduleName,
				bool includeRunning);
			/**
			* Helper for getModuleDescriptor(std::string name, boolean incRunning),
			* for the case of a remote module.  The given <code>moduleName</code> must
			* have any host prefix stripped off.  The host must be given in
			* <code>host</code>.
			* <p>
			* @param moduleName the name of the module sought
			* @param host the name of the remote host
			* @param includeRunning whether running modules are included
			* during search
			* @return a list whose fields correspond, in order, to the
			* contents of a ModuleDescriptor class object
			**/
			DataBlockWriter getModuleDescriptorRemote(std::string moduleName, Host *host,
				bool includeRunning);
			/**
			* Helper method for getModuleDescriptor(int moduleId), for the
			* case of a local module.  The identifier is assumed to have been checked
			* for being a module on the local broker only.
			* <p>
			* @param moduleId the unique identifier of the module sought
			* @param moduleInst the Instance for the module whose id is
			* <code>moduleId</code>
			**/
			DataBlockWriter getModuleDescriptorLocal(int moduleId, Instance *moduleInst);
			/**
			* Helper method for getModuleDescriptor(int moduleId), for the
			* case of a remote module.  The identifier is assumed to have been
			* checked for being a module on a remote broker.
			* <p>
			* @param moduleId the unique identifier of the module sought
			* @param moduleInst the Instance for the module whose id is
			* <code>moduleId</code>
			* @return a list whose fields correspond, in order, to the
			* contents of a ModuleDescriptor class object
			**/
			DataBlockWriter getModuleDescriptorRemote(int moduleId, Instance *moduleInst);
			/**
			* Helper for #getServiceDescriptor(int moduleId, std::string servName).
			* This handles the case of a local module.
			* <p>
			* @param moduleId the identifer of the module
			* @param servName the name of the service
			* @return a single list representing the contents of a
			* ServiceDescriptor class object
			* @return an array of lists, where the fields of each list correspond,
			* in order, to the contents of a ServiceDescriptor class object
			**/
			DataBlockWriter getServiceDescriptorLocal(int moduleId, std::string servName,
				Instance *moduleInst);
			/**
			* Helper for #getServiceDescriptor(int moduleId, std::string servName).
			* This handles the case of a remote module.
			* <p>
			* @param moduleId the identifer of the module
			* @param servName the name of the service
			* @param moduleInst the Instance for the module whose id is
			* <code>moduleId</code>
			* @return a single list representing the contents of a
			* ServiceDescriptor class object
			**/
			DataBlockWriter getServiceDescriptorRemote(int moduleId, std::string servName,
				Instance *moduleInst);
			/**
			* Helper for #getServiceDescriptors(std::string name).  This
			* handles the case of a module on the local broker.  The given
			* <code>moduleName</code> must have any host prefix stripped off.  The
			* host is assumed to be the local host.
			* <p>
			* @param name the name of the module
			* @return an array of lists, where the fields of each list correspond,
			* in order, to the contents of a ServiceDescriptor class object
			**/
			std::vector<DataBlockWriter> getServiceDescriptorsLocal(std::string moduleName, Instance *oInstance = NULL);
			/**
			* Helper for #getServiceDescriptors(std::string name).  This
			* handles the case of a remote module.  The given
			* <code>moduleName</code> must have any host prefix stripped off.  The
			* host must be given in <code>host</code>.
			* <p>
			* @param moduleName the name of the module
			* @param host the host on which the module should be started up
			* @return an array of lists, where the fields of each list correspond,
			* in order, to the contents of a ServiceDescriptor class object
			**/
			std::vector<DataBlockWriter> getServiceDescriptorsRemote(std::string moduleName, Host *host);
			/**
			* Handles changing the name of a module, for the case where the module
			* is local to the current broker.
			* <p>
			* @see #changeModuleName
			* @param moduleId the identifer of the module
			* @param moduleInst the module instance
			* @param newModuleName the new unique name for the module
			**/
			void changeModuleNameLocal(int moduleId, Instance *moduleInst,
				std::string newModuleName);
			/**
			* Handles changing the name of a module, for the case where the module
			* is remote.
			* <p>
			* @see #changeModuleName
			* @param moduleId the identifer of the module
			* @param moduleInst the module instance
			* @param newModuleName the new unique name for the module
			**/
			void changeModuleNameRemote(int moduleId, Instance *moduleInst,
				std::string newModuleName);
			/**
			* Starts a module and returns the module id.  This is tricky because
			* we cannot tell the new module which module id it should receive --
			* we have to wait until the requested module starts up, connects to
			* the Broker, and is finally assigned an id by the Broker's normal
			* processing sequence.  This is complicated by several factors: (a)
			* other modules may connect to the Broker in-between the time we try
			* to start up our module and it connects; (b) multiple modules may
			* request new modules to be started up, so we may end up having to
			* wait for multiple incoming modules, and (c) we may end up having to
			* wait for multiple copies of the <b>same</b> module to start up.
			* <p>
			* The approach taken here is to set up a notification scheme that
			* is invoked by callbacks in SBWBrokerRPC.
			* <p>
			* @param descriptor the ModuleDescriptor of the module to be
			* started up
			* @return the integer id of the module that is started up, if startup
			* is successful
			**/
			int startNewLocalModuleInstance(ModuleDescriptor *md);
			/**
			* Returns a list corresponding to a given ModuleDescriptor
			* object.  The format of the list returned is:
			* <pre>
			*     moduleIdentificationName
			*     moduleDisplayName
			*     moduleManagementType
			*     moduleCommandLine
			*     moduleHelpstd::string</pre>
			*
			* @param md the ModuleDescriptor
			* @return a list containing the fields of the module descriptor
			**/
			DataBlockWriter listForModuleDescriptor(ModuleDescriptor *md);
			/**
			* Returns a list corresponding to a module descriptor, given the
			* individual elements that should go into the descriptor.  The format
			* of the list returned is:
			* <pre>
			*     moduleIdentificationName
			*     moduleDisplayName
			*     moduleManagementType
			*     moduleCommandLine
			*     moduleHelpstd::string</pre>
			*
			* @param moduleName the name of the module
			* @param displayName the display name of the module
			* @param mgmtType the management type of the module
			* @param cmdLine the command line for the module
			* @param help the help std::string for the module
			* @return a list containing the fields of the module descriptor
			**/
			DataBlockWriter listForModuleDescriptor(std::string moduleName, std::string displayName,
				int mgmtType, std::string cmdLine, std::string help);
			/**
			* Returns a list corresponding to a service descriptor, given the
			* individual elements that should go into the descriptor.  The format
			* of the list returned is:
			* <pre>
			*  0  std::string moduleName
			*  1  std::string serviceName
			*  2  std::string serviceDisplayName
			*  3  std::string category
			*  4  std::string helpstd::string</pre>
			*
			* @param moduleName the name of the module
			* @param serviceName the name of the service
			* @param serviceDisplayName the display name of the service
			* @param category the category of the service
			* @param helpstd::string the help std::string for the service
			* @return a list containing the fields of the service descriptor
			**/
			DataBlockWriter listForServiceDescriptor(std::string moduleName, std::string serviceName,
				std::string serviceDisplayName, std::string category,
				std::string helpString);
			/**
			* Returns a list corresponding to a service descriptor, given a
			* ServiceDescriptor object and a module name.  The format
			* of the list returned is:
			* <pre>
			*  0  std::string moduleName
			*  1  std::string serviceName
			*  2  std::string serviceDisplayName
			*  3  std::string category
			*  4  std::string helpstd::string</pre>
			*
			* @param moduleName the name of the module
			* @param sd the service descriptor object
			* @return a list containing the fields of the service descriptor
			**/
			DataBlockWriter listForServiceDescriptor(std::string moduleName, ServiceDescriptor *sd);
			/**
			 * Match a std::string against a service category.
			 *
			 * \param category the whole category std::string
			 * \param toMatch the std::string to match
			 * \param start the starting point
			 * \return <b>true</b> if <code>toMatch</code> matches <code>category</code> <b>false</b> otherwise
			 */
			bool matchCategory(std::string category, std::string toMatch, unsigned int start);
			/**
			* Normalize a given module name and return the normalized version.
			* This checks whether the name is non-empty, trims whitespace, etc.
			* <p>
			* @param moduleName the name to normalize
			* @return the normalized version of the name
			**/
			std::string normalizeModuleName(std::string moduleName);
			/**
			* Normalize a given service name and return the normalized version.
			* This checks whether the name is non-empty, trims whitespace, etc.
			* <p>
			* @param moduleName the name to normalize
			* @return the normalized version of the name
			**/
			std::string normalizeServiceName(std::string serviceName);
			/**
			* Check whether a given module identifier is valid and refers to
			* an existing instance.  This returns nothing but throws an error
			* if the id is invalid.  This is handy as a test to call in other
			* methods.
			* <p>
			* @param moduleId the identifier to check
			**/
			void checkModuleId(int moduleId);
			/**
			 * checks whether a given remote broker id is still valid. If it is not notify other modules of shutdown. 
			 * \param brokerId the broker identifier
			 */
			void checkBrokerId(int brokerId);
			/**
			* Take a module name that may optionally have a host prefix and return
			* just the host part.
			* <p>
			* @param name the full name of a module
			* @return a Host object containing the host
			**/
			Host *getHostPart(std::string name);
			/**
			* Take a module name that may optionally have a host prefix and return
			* just the host part.
			* <p>
			* @param name the full name of a module
			* @return a std::string containing the host prefix
			**/
			std::string getModuleNamePart(std::string name);
			/**
			* Returns an instance given a module id, throwing an exception
			* if there is no such module.
			* <p>
			* @param moduleId the identifier of the module instance to return
			* @return the instance object for the module
			**/
			Instance *getInstanceForModuleId(int moduleId);

			/**
			 * returns descriptor for module, querying remote broker if need
			 * \param brokerId the broker identifier where the module is searched
			 * \param moduleName modulename is short form
			 * \return returns descriptor for module, querying remote broker if need
			 */
			ModuleDescriptor *getDescriptor(int brokerId, std::string moduleName);

			/**
			 * return all descriptors including for all brokers
			 * \return List of ModuleDescriptors for all Brokers
			 */
			std::vector<ModuleDescriptor *> getDescriptors();
			/**
			 * return all ModuleDescriptors for the specified broker
			 * \param brokerId the broker identifier to get the descriptors from
			 * \return all ModuleDescriptors for the specified broker
			 */
			std::vector<ModuleDescriptor *> getDescriptors(int brokerId);
			/**
			* Return the identifier for the broker running on a given host,
			* connecting to the remote broker first if necessary.  If the
			* argument given is NULL, it is taken to signify the local host.
			* <p>
			* @param host the host or IP address of the remote machine
			**/
			int getBrokerId(Host *host);

			/**
			 * This method will check all known Instances for BrokerInstances and return their ids
			 * \return a list of all broker ids
			 */
			std::vector<int> getBrokerIds();
			/**
			 *
			 * \param *host the host of the broker to get an interface for
			 * \return an interface to a remote broker
			 */
			BrokerInterface *getBrokerInterface(Host *host);
			/**
			 *
			 * \param bId the broker identifier to get an interface for
			 * \return an interface to a remote broker
			 */
			BrokerInterface *getBrokerInterface(int bId);
			/**
			 * Help function to include the host in the description of modules
			 * \param list current list
			 * \param modName module name
			 * \param *host host where the module is running
			 * \return altered list
			 */
			DataBlockWriter mungeModuleDescriptor(DataBlockWriter list, std::string modName, Host *host);
			/**
			 * Help function to include the host in the description of services
			 * \param list list current list
			 * \param modName module name
			 * \param *host host where the module is running
			 * \return altered list
			 */
			DataBlockWriter mungeServiceDescriptor(DataBlockWriter list, std::string modName, Host *host);
			/**
			 * Add a special entry for the Broker.
			 */
			void addBrokerInstance();
			/**
			 *  Add a special entry for the Broker.
			 */
			void addBrokerRegistryEntry();
			static string	BROKER_NAME;
			static SBWBrokerRPC *rpc;					///< the RPC object for this Broker
			static InstanceTable *instanceTable;		///< the table where all instances will be stored
			static ModuleRegistry *moduleRegistry;		///< the current registry of available modules
			/// mutex 
			SBWOSMutex brokerMutex ;
		};
	}
}
#endif
