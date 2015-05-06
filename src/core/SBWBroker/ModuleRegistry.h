#ifndef SBW_BROKER_MODULE_REGISTRY
#define SBW_BROKER_MODULE_REGISTRY

#include "ModuleRegistryFile.h"
#include "SBW/ModuleDescriptor.h"

#include <string>
#include <vector>
#include <map>

#if    _MSC_VER > 1000
#pragma once
#pragma warning (disable: 4996)
#endif

using namespace std;

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 *
		 * Basically this is a hash table of module descriptors, keyed by module names
		 * and backed up by an XML file.
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class ModuleRegistry
		{
		private:
			/**
			* Private constructor that suppresses the default public constructor.
			* This, and the following method (#getRegistry()) for returning
			* an instance of this class, implement the "Singleton" design pattern
			* (named in the Design Patterns book by Gamma et al.).  See <a
			* href="http://developer.java.sun.com/developer/qow/archive/111/">
			* http://developer.java.sun.com/developer/qow/archive/111/</a> for
			* more information.
			**/
			ModuleRegistry();
		public:
			/**
			* Returns the (single) instance of this class.
			**/
			static ModuleRegistry *getRegistry();

			/**
			 * adds a module descriptor into the secondary cache. That means information on that
			 * module were requested but it is not registered yet. So provide as much information about
			 * it as we can. 
			 * \param moduleName the module name to be registered
			 * \param *md the actual module descriptor
			 */
			void addTempModuleDescriptor(string moduleName, ModuleDescriptor *md);
			/**
			 * adds a ModuleDescriptor under the ModuleName. After addinf a checkpoint 
			 * is made. Meaning the registry will be saved. 
			 * \param moduleName the modulename
			 * \param *md the moduledescriptor
			 */
			void addModuleDescriptor(string moduleName, ModuleDescriptor *md);
			/**
			 * adds a ModuleDescriptor under the ModuleName. if doCheckpoint is <b>true</b> 
			 * the registry will be saved after adding. 
			 * \param moduleName the module name
			 * \param *md the descriptor
			 * \param doCheckpoint flag indicating whether to sace the registry after adding or not
			 */
			void addModuleDescriptor(string moduleName, ModuleDescriptor *md,
				bool doCheckpoint);
			/**
			 * creates a moduledescriptor out of the values, adds it to the hash table and sets a checkpoint.
			 * \param moduleName  module name
			 * \param moduleDisplayName display name of the module
			 * \param moduleType the type of the Module (UNIQUE or SELF_MANAGED)
			 * \param commandLine command line to be used to start the module
			 * \param help help string
			 */
			void addModuleDescriptor(string moduleName, string moduleDisplayName,
				int moduleType, string commandLine, string help);
			/**
			 * remove a module descriptor from the registry
			 * \param moduleName module name in short form (without Host)
			 */
			void removeModuleDescriptor(string moduleName);
			/**
			 *
			 * \param moduleName module name in short form
			 * \return the requested module descriptor
			 */
			ModuleDescriptor *getModuleDescriptor(string moduleName);

			/**
			 * this function will return a temporary module decriptor out of the secondary 
			 * registry. The secondary cache holds modules that have not been registered yet
			 * (this means running modules) that have been queried by other modules. 
			 * \param moduleName the module name
			 * \return the requested module descriptor or NULL
			 */
			ModuleDescriptor *getTempModuleDescriptor(string moduleName);
			/**
			 *
			 * \return all ModuleDescriptors
			 */
			vector<ModuleDescriptor *> getModuleDescriptors();
			/**
			 * Should the Broker be started and no Registry be present, this function will be 
			 * called to re-register the Modules included in the Windows Installer Package. Thus
			 * this function works only under Win32
			 */
			static void registerModules();
		private:
			/**
			* The readResolve method serves to prevent the release of multiple
			* instances upon deserialization.  I don't know if we will ever need
			* to worry about serialization, but I may as well add this because it's
			* trivial to do and documented in 
			* <a href="http://developer.java.sun.com/developer/qow/archive/111/">
			* http://developer.java.sun.com/developer/qow/archive/111/</a> for
			**/
			ModuleRegistry *readResolve();
 
			typedef pair< string , ModuleDescriptor *> StringMD; ///< type definition for the hash table
			map< string , ModuleDescriptor *> table;		 ///< the hash table
			map< string , ModuleDescriptor *> m_oTempTable; ///< the secondary hash table(not persisted)
			static ModuleRegistryFile *registryFile;			 ///< pointer to the registry file
			static ModuleRegistry *instance;					 ///< the one and only registry instance
		};
	}
}

#endif

