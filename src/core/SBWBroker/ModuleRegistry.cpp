#include <stdlib.h>
#include "ModuleRegistry.h"
#include "StringUtil.h"

#include "SBW/SBWC.h"
#include "SBW/ModuleDescriptor.h"
#include "SBW/DataBlockWriter.h"
#include "SBW/SBWConfig.h"
#include "SBW/portableOS.h"

#ifdef WIN32
#include <direct.h>
#include <fstream>
#endif

using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;

void ModuleRegistry::registerModules()
{
#ifdef WIN32
	std::string sCreate = "\"" + Config::getSBWHome();
	sCreate += "\\bin\\registerModules.bat\"";
	_chdir(sCreate.c_str());
	sCreate += " " + Config::getSBWHome() + "\\bin\\";
	try
	{
		TRACE("registering from " << sCreate);
		SBWOS::startProcess((char *)sCreate.c_str());
		//SBWThread::sleep(10000);
	}
	catch (...)
	{
		TRACE("automatic registering failed");
	}
#endif
}
ModuleRegistry::ModuleRegistry()
{
	// Restore the in-core registry contents.  If there's a problem
	// reading the registry file, log some diagnostics but don't choke.

	registryFile = new ModuleRegistryFile(this);
	if (registryFile == NULL || ! registryFile->exists())
	{
		TRACE("Unable to open registry file; trying to register");
#ifdef WIN32
		// here the outline of how the registration mechanism will work ... 
		// first the program checks wether a registration is in Progress ...
		// if so it will work normally to allow registration
		//
		// otherwise it searches for a file containing filenames
		// and arguments to register modules ...
		//
		std::string sLock = getenv("TEMP") + string("\\.lock");

		FILE *oLock = fopen(sLock.c_str(), "rb");
		if (oLock != NULL)
		{
			// registration in progress
			fclose(oLock);
		}
		else
		{
			oLock = fopen(sLock.c_str(), "w");
			if (oLock == NULL)
			{
				// can't register for whatever reason ... 
				// stop trying ...
				return;
			}
			fprintf(oLock, "registering ... ");
			fflush(oLock);
			fclose(oLock);
			registerModules();
			exit(0);
		}
#endif
		return;
	}
	else
	{
		TRACE("Using registry file '" << registryFile->getPath() << "'");
		registryFile->readRegistry();
	}
}
ModuleRegistry *ModuleRegistry::getRegistry()
{
	if (instance == NULL) 
		instance = new ModuleRegistry();
	return instance;
}
void ModuleRegistry::addTempModuleDescriptor(string moduleName, ModuleDescriptor *md)
{
	m_oTempTable.insert(StringMD(moduleName.c_str(), md));
}
void ModuleRegistry::addModuleDescriptor(string moduleName, ModuleDescriptor *md)
{
	removeModuleDescriptor(moduleName);
	table.insert(StringMD(moduleName.c_str(), md));	
	registryFile->checkpoint();
}
void ModuleRegistry::addModuleDescriptor(string moduleName, ModuleDescriptor *md,
										 bool doCheckpoint)
{
	removeModuleDescriptor(moduleName);
	table.insert(StringMD(moduleName.c_str(), md));
	if (doCheckpoint)
		registryFile->checkpoint();
}
void ModuleRegistry::addModuleDescriptor(string moduleName, string moduleDisplayName,
										 int moduleType, string commandLine, string help)
{
	DataBlockWriter oWriter;
	oWriter << moduleName << moduleDisplayName  << moduleType << commandLine << help;
	addModuleDescriptor(moduleName, new ModuleDescriptor(oWriter.getReader()));
}
void ModuleRegistry::removeModuleDescriptor(string moduleName)
{
	map< string , ModuleDescriptor *> ::iterator oIterator;
	oIterator = table.find(moduleName);
	if (oIterator != table.end())
	{
		table.erase(oIterator);
		registryFile->checkpoint();
	}
}

ModuleDescriptor *ModuleRegistry::getTempModuleDescriptor(string moduleName)
{
	if (StringUtil::empty(moduleName))
		return NULL;
	else
	{
		map< string , ModuleDescriptor *> ::iterator oIterator;
		oIterator = m_oTempTable.find(moduleName);
		if (oIterator == m_oTempTable.end())
		{
			return NULL;
		}
		else 
		{
			return oIterator->second;
		}
	}

}
ModuleDescriptor *ModuleRegistry::getModuleDescriptor(string moduleName)
{
	if (StringUtil::empty(moduleName))
		return NULL;
	else
	{
		map< string , ModuleDescriptor *> ::iterator oIterator;
		oIterator = table.find(moduleName);
		if (oIterator == table.end())
		{
			return NULL;
		}
		else 
		{
			return oIterator->second;
		}
	}
}
vector<ModuleDescriptor *> ModuleRegistry::getModuleDescriptors()
{
	vector<ModuleDescriptor *> oTemp;
	map< string , ModuleDescriptor *> ::iterator oIterator;
	for (oIterator = table.begin(); oIterator != table.end(); oIterator++)
		oTemp.push_back(oIterator->second);	
	return oTemp;
}
ModuleRegistry *ModuleRegistry::readResolve()
{
	return instance;
}
ModuleRegistryFile *ModuleRegistry::registryFile = NULL;
ModuleRegistry *ModuleRegistry::instance = NULL;
