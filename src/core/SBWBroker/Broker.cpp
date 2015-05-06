#include "BrokerDefines.h"
#include "BrokerInterface.h"
#include "ProxyLocalBrokerInstance.h"
#include "ProxyModuleInstance.h"
#include "Broker.h"

#include "SBWBrokerRPC.h"
#include "BrokerInstance.h"
#include "StringUtil.h"

#include <SBW/SBW.h>
#include <SBW/SBWOSSocket.h>
#include <SBW/SBWApplicationException.h>
#include <SBW/SBWModuleStartException.h>
#include <SBW/SBWLowLevel.h>
#include <SBW/RuntimeProperties.h>

#include <set>

#ifdef WIN32
#pragma warning (disable:4389)
#endif

using namespace SystemsBiologyWorkbench;
using namespace SystemsBiologyWorkbench::Broker;
using namespace std;

CBroker::CBroker() : brokerMutex("cBrokerMutex")
{
	rpc = dynamic_cast<SBWBrokerRPC *>(SBW::getInternalAPI()); 
	instanceTable = InstanceTable::getTable();
	moduleRegistry = ModuleRegistry::getRegistry();

	addBrokerRegistryEntry();
	addBrokerInstance();
}

void CBroker::addBrokerInstance()
{
	instanceTable->addInstance(new ProxyLocalBrokerInstance());
}

void CBroker::addBrokerRegistryEntry()
{
	DataBlockWriter *oWriter = new DataBlockWriter();
	(*oWriter) << "BROKER" << "SBW Broker" <<  UNIQUE <<  StringUtil::EMPTY << "The SBW Broker manages all modules.";
	ModuleDescriptor *md = new ModuleDescriptor(oWriter->getReader());
	delete oWriter;
	oWriter = new DataBlockWriter();
	(*oWriter) << "BROKER" << "BROKER" << "BROKER" << "/" << "The BROKER service implements the services of the SBW Broker.";
	md->addService(new ServiceDescriptor( oWriter->getReader(), *md) );
	moduleRegistry->addModuleDescriptor(BROKER_NAME, md, false);
	delete oWriter;
}

int CBroker::getModuleInstance(string name)
{
	TRACE("Received request for instance of '" << name << "'");

	// short cut
	if (name == "BROKER") return BROKER_MODULE;

	string moduleName = getModuleNamePart(name);
	Host *host = getHostPart(name);
	if (host == NULL || rpc->getHost()->equals(host))
		return getModuleInstanceLocal(moduleName);
	else
	{
		// first check our local modules ...
		std::vector<Instance *> instances = instanceTable->getInstances();
		Instance *oTemp = NULL;
		std::vector<Instance *>::iterator it;
		//for (unsigned int i =0; i < instances.size(); i++)
		for (it = instances.begin(); it!= instances.end(); it++)
		{
			//oTemp = instances[i];
			oTemp = *it;
			if (oTemp->getInternalName() == name)
				return oTemp->getId();			
		}
		// next
		return getModuleInstanceRemote(moduleName, host);
	}
}

int CBroker::getModuleInstanceLocal(string moduleName)
{
	Instance *instance = instanceTable->getInstance(rpc->getHost(), moduleName);
	ModuleDescriptor *md = moduleRegistry->getModuleDescriptor(moduleName);

	if (instance != NULL)
	{
		// If we find an instance (meaning there's already a copy of
		// the module running), and its management type is UNIQUE, then
		// return the existing instance, else try to start up a new one.

		if (md == NULL)
		{
			// There's an instance, but the module isn't registered.
			// Unfortunately, without registration info, we can't
			// know how to start up an instance of the module.  So punt.

			TRACE("Module '" << moduleName << "' is not registered;"  <<" returning existing instance.");
			return instance->getId();
		}
		else if (md->getManagementType() == UniqueModule)
		{
			TRACE("Reusing instance of unique module '" << moduleName <<  "' on '" << rpc->getHost()->getName() << "'");
			return instance->getId();
		}
		else
			return startNewLocalModuleInstance(md);
	}
	else  // There's no instance yet, so start one if we know how.
	{
		TRACE("No existing instance of '" << moduleName << "' found");

		if (md != NULL)
			return startNewLocalModuleInstance(md);
		else
		{
			stringstream a,b;
			a <<  "Unknown/unregistered module '" << moduleName << "'";
			b << "An attempt was made to obtain an instance of a module"
				<< " named '" << moduleName << "' from the broker on '"
				<< rpc->getHost()->getName() << "', but no such module is known";
			throw new SBWModuleNotFoundException(a.str(), b.str());
		}
	}
}

int CBroker::getModuleInstanceRemote(string moduleName, Host *host)
{
	// Part A: If the module is unique & there is an instance running,
	// we return that w/o contacting the remote broker.  If the module
	// is unregistered (==> descriptor null) & *not* unique, then we try
	// to return a running instance if there is one, or else we fail.
	// If the module is registered and not unique, we go to part B below.

	DataBlockWriter md = getModuleDescriptorRemote(moduleName, host, true);
	string sTemp; Integer nManagement;
	md.getReader() >> sTemp >> sTemp >> nManagement;

	TRACE("'" << host->getName() << ":" << moduleName << "' " << (nManagement == UNIQUE ? " is unique" : "is self managed" ));

	// Look for a running instance of this module on the broker.
	TRACE("Checking if an instance of '" << host->getName() << ":"
		<< moduleName << " is already running if not it will be started");

	BrokerInterface oBroker(getBrokerId(host));
	oBroker.getModuleInstance(moduleName);

	for (int i = 0; i < STARTUP_WAIT; i++) 
	{
		try {SBWThread::sleep(200); } catch (...) { }
		Instance* oInstance = instanceTable->getInstance(host, moduleName);
		if (oInstance == NULL)
		{
			continue;
		}
		else
		{
			return oInstance->getId();
		}
	}

	throw new SBWModuleNotFoundException(
		string("Unknown/unregistered module '") + host->getName()
		+ ":" + moduleName + "'",
		"An attempt was made to obtain an instance of a module"
		+ string(" named '") + moduleName + "' from the broker on '"
		+ host->getName() + "', but no such module is known");
}

DataBlockWriter CBroker::getModuleDescriptor(string name, bool includeRunning)
{
	string moduleName = getModuleNamePart(name);
	Host *host = getHostPart(name);

	if (host == NULL || rpc->getHost()->equals(host))
		return getModuleDescriptorLocal(moduleName, includeRunning);
	else
	{
		std::vector<Instance *> instances = instanceTable->getInstances();
		Instance *oTemp = NULL;
		//for (unsigned int i =0; i < instances.size(); i++)
		std::vector<Instance *>::iterator it;
		for (it = instances.begin(); it!= instances.end(); it++)
		{
			//oTemp = instances[i];
			oTemp = *it;
			if (oTemp->getInternalName() == name)
				return getModuleDescriptorLocal(name, includeRunning);
		}
		return getModuleDescriptorRemote(moduleName, host, includeRunning);
	}
}

DataBlockWriter CBroker::getModuleDescriptorLocal(string moduleName, bool includeRunning)
{
	ModuleDescriptor *md = moduleRegistry->getModuleDescriptor(moduleName);
	if (md != NULL)
	{
		TRACE("Found '" << moduleName << "' in the local registry");
		return listForModuleDescriptor(md);
	}
	else
	{
		// It's not in the registry.  Should we check running instances?

		TRACE("'" << moduleName << "' is not registered");
		if (includeRunning)
		{
			// We didn't find a module descriptor, but the module may
			// be running unregistered.  Go through the instances.
			// Note that we want to return the most recently started
			// module by this name, so we start from the end of the list.

			vector< Instance *> instances = instanceTable->getInstances(BROKER_MODULE);
			for (int i = (int)instances.size() - 1; i >= 0; i--)
			{
				Instance *inst = instances[i];
				if (inst != NULL)
				{
					if (inst->getName() == moduleName || inst->getInternalName() == moduleName)
					{
						TRACE("Found running instance of '" <<  moduleName << "'");
						return getModuleDescriptorLocal(inst->getId(), inst);
					}
				}
			}
		}
	}

	// If we get here, we didn't find it.
	TRACE("There is no module with name '" << moduleName << "'");
	throw new SBWModuleNotFoundException( "Unknown/unregistered module '" + moduleName + "'", 
		"An attempt was made to obtain an instance of a module named '" + moduleName + "', but no such module is known to the SBW registry.");
}

DataBlockWriter CBroker::getModuleDescriptorRemote(string moduleName, Host *host,
												   bool includeRunning)
{
	TRACE("Asking broker on " << host->getName()	<< " about '" << moduleName << "'");

	BrokerInterface *broker = getBrokerInterface(host);
	DataBlockWriter result = broker->getModuleDescriptor(moduleName, includeRunning);
	return mungeModuleDescriptor(result, moduleName, host);
}

DataBlockWriter CBroker::getModuleDescriptor(int moduleId)
{
	if (moduleId == BROKER_MODULE)
		return listForModuleDescriptor("BROKER", "SBW Broker", UNIQUE , StringUtil::EMPTY , "The SBW Broker manages all modules.");


	checkModuleId(moduleId);
	Instance *inst = getInstanceForModuleId(moduleId);

	if (inst->getBrokerId() == BROKER_MODULE)
		return getModuleDescriptorLocal(moduleId, inst);
	else
		return getModuleDescriptorRemote(moduleId, inst);
}

DataBlockWriter CBroker::getModuleDescriptorLocal(int moduleId, Instance *moduleInst)
{	
	string moduleName = moduleInst->getName();

	if (StringUtil::empty(moduleName))
	{
		// We may have been given a module id for a module connected
		// anonymously.  In that case, there will be no names or command
		// line associated with it.  Treat it as a unique module.

		TRACE("Module " << moduleId << " is anonymous");
		return listForModuleDescriptor(StringUtil::EMPTY,StringUtil::EMPTY,UniqueModule,StringUtil::EMPTY,StringUtil::EMPTY);
	}
	else
	{
		// We have an instance and the module is not anonymous.  See if
		// it's in the registry.  If it's not in the registry, it's a
		// module that connected with a name but did not register.  In
		// that case, return what info we can.

		ModuleDescriptor *md = moduleRegistry->getModuleDescriptor(moduleName);
		if (md != NULL)
		{
			TRACE("Found module " << moduleId << " ('" << moduleName << "'), in the registry");
			return listForModuleDescriptor(md);
		}
		else
		{
			TRACE("Module " << moduleId << " is not in the registry");
			return listForModuleDescriptor(moduleName, StringUtil::EMPTY, UniqueModule , StringUtil::EMPTY, StringUtil::EMPTY);
		}
	}
}

void CBroker::checkBrokerId(int brokerId)
{
	BrokerInstance *bInst = dynamic_cast<BrokerInstance *>( instanceTable->getInstance(brokerId));
	if (bInst == NULL)
	{
		vector <Instance *> instances = instanceTable ->getInstances(brokerId);
		vector <Instance *>::iterator oIterator;
		for (oIterator = instances.begin(); oIterator != instances.end(); oIterator++)
		{
			Instance* oInstance = *oIterator;
			ProxyModuleInstance *oModule = dynamic_cast<ProxyModuleInstance *>(oInstance);
			try
			{
				if (oModule != NULL)
				{
					rpc->notifyOthersOfDisconnect(oModule,false);
					instanceTable->removeInstance(oModule);
				}
			}
			catch(...)
			{
				//
			}
		}
		throw new SBWApplicationException(("error broker: " + Properties::toString(brokerId) + " is no longer reachable").c_str());
	}
}

DataBlockWriter CBroker::getModuleDescriptorRemote(int /*moduleId*/, Instance *moduleInst)
{
	// This must be a remote instance, so cast it to access getRemoteId().

	ProxyModuleInstance *oModule = dynamic_cast<ProxyModuleInstance *>(moduleInst);
	BrokerInstance *oBroker = dynamic_cast<BrokerInstance *>(moduleInst);
	if (oModule != NULL)
	{
		BrokerInterface *broker = getBrokerInterface(oModule->getBrokerId());

		// Note that moduleId refers to a module on the local broker --
		// must translate it to what it is on the *remote* broker.

		DataBlockWriter oWriter = broker->getModuleDescriptor(oModule->getRemoteId());

		return mungeModuleDescriptor(
			oWriter, 
			oModule->getName(), 
			oModule->getHost());
	}
	if (oBroker != NULL)
	{
		BrokerInterface *broker = getBrokerInterface(oBroker->getBrokerId());

		// Note that moduleId refers to a module on the local broker --
		// must translate it to what it is on the *remote* broker.

		DataBlockWriter oWriter = broker->getModuleDescriptor(oBroker->getRemoteId());

		return mungeModuleDescriptor(
			oWriter, 
			oBroker->getName(), 
			oBroker->getHost());
	}
	throw new SBWApplicationException("instance was no remote instance");
}

vector< DataBlockWriter > CBroker::getModuleDescriptors(bool localOnly, bool includeRunning)
{
	vector< DataBlockWriter > records;
	vector<int> brokerIds;
	if (localOnly)                  // Only want local broker's modules.
		brokerIds.push_back(BROKER_MODULE );
	else                            // Get all broker's modules.
		brokerIds = getBrokerIds();

	// Ask each broker for a list of descriptors for registered modules,
	// and if includeRunning is true, also check our table of running
	// instances to see if any unregistered instances are known to be
	// running on that broker.

	vector<int>::iterator it;
	for (it=brokerIds.begin(); it!=brokerIds.end();it++)
	//for (unsigned int i = 0; i < brokerIds.size(); i++)
	{
		//int brokerId = brokerIds[i];
		int brokerId = *it;
		vector<ModuleDescriptor *> mds;
		vector<ModuleDescriptor *>::iterator mdIt;
		try
		{
			mds = getDescriptors(brokerId);

			for (mdIt = mds.begin(); mdIt != mds.end(); mdIt++)
				records.push_back(listForModuleDescriptor(*mdIt));
			//for (unsigned int j = 0; j < mds.size(); j++)
			//	records.push_back(listForModuleDescriptor(mds[j]));

			if (includeRunning)
			{
				// We have to combine the info in the registry with
				// the known running instances, since some modules may
				// not have registered.  Modules that haven't
				// registered won't have a registry entry, but will
				// have an instance.

				vector <Instance *> instances = instanceTable->getInstances(brokerId);
				vector<Instance *>::iterator instIt;
				//for (unsigned int j = 0; j < instances.size(); j++)
				for (instIt = instances.begin(); instIt != instances.end(); instIt ++)
				{
					//string name = instances[j]->getName();
					string name = (*instIt)->getName();
					if (getDescriptor(brokerId, name) == NULL)
						records.push_back(listForModuleDescriptor(name, StringUtil::EMPTY,
						UniqueModule , StringUtil::EMPTY, StringUtil::EMPTY));
				}
			}
		}
		catch (...)
		{
			TRACE("Lost contact with SBW Broker " << brokerId);
			continue;
		}
	}

	return records;
}

DataBlockWriter CBroker::getServiceDescriptor(int moduleId, int serviceId)
{
	vector< string > serviceNames = SBW::getServiceNamesFromModule(moduleId);
	if (serviceId < (int)serviceNames.size() && serviceId >= 0 ) 
	{
		return getServiceDescriptor(moduleId, serviceNames[serviceId]);
	}
	else
	{
		throw new SBWServiceNotFoundException("Service id " + Properties::toString(serviceId) + " is out of range" + 
			"Service id " + Properties::toString(serviceId) + " is not within the range of"
			+ " service identifiers recognized by module " + Properties::toString(moduleId));
	}
}

DataBlockWriter CBroker::getServiceDescriptor(int moduleId, string servName)
{

	if (moduleId == BROKER_MODULE && servName == "BROKER")
	{
		return listForServiceDescriptor("BROKER", "BROKER", "BROKER", "/", "The BROKER service implements the services of the SBW Broker.");
	}

	checkModuleId(moduleId);
	Instance *inst = getInstanceForModuleId(moduleId);

	if (inst->getBrokerId() == BROKER_MODULE)
		return getServiceDescriptorLocal(moduleId, servName, inst);
	else
		return getServiceDescriptorRemote(moduleId, servName, inst);
}

DataBlockWriter CBroker::getServiceDescriptorLocal(int moduleId, string servName,
												   Instance *moduleInst)
{
	servName = normalizeServiceName(servName);
	string moduleName = moduleInst->getName();
	ModuleDescriptor *md = moduleRegistry->getModuleDescriptor(moduleName);

	// Easiest case: module is registered & has a service descriptor.
	if (md != NULL)
	{
		vector<ServiceDescriptor *> servicesHash = md->getServices();
		vector<ServiceDescriptor *>::iterator serviceIt;
		for (serviceIt = servicesHash.begin(); serviceIt != servicesHash.end(); serviceIt ++)
		{
			ServiceDescriptor *sd = *serviceIt;
				if (sd != NULL && sd->getName() == servName)
					return listForServiceDescriptor(moduleName, sd);
		}
		//if (servicesHash.size() > 0 )
		//{
		//	for (unsigned int i = 0; i < servicesHash.size(); i++)
		//	{
		//		ServiceDescriptor *sd = servicesHash[i];
		//		if (sd != NULL && sd->getName() == servName)
		//			return listForServiceDescriptor(moduleName, sd);
		//	}
		//}
	}
	// Harder case: there's a registry entry for the module, but no
	// associated ServiceDescriptor for the service.  We may have been
	// given a module id for a module connected anonymously.  In that
	// case, there will be no service record available for it, and the
	// best we can do is return a service descriptor with only the name
	// of the service in it.  However, to be safe, we first check with
	// the module itself to make sure that a service by that name
	// really does exist.

	vector<string> serviceNames = SBW::getServiceNamesFromModule(moduleId);
	vector<string>::iterator nameIt; 
	for (unsigned int i = 0; i < serviceNames.size(); i++)
		if (serviceNames[i] == servName)
			return listForServiceDescriptor(moduleName, servName,
			StringUtil::EMPTY, StringUtil::EMPTY, StringUtil::EMPTY);

	// If we get to this point here, we didn't find it.

	throw new SBWServiceNotFoundException(
		"Cannot find a service named '" + servName + "' on " + moduleName
		+ "A service descriptor for a service named '" + servName
		+ "' has been requested from " + moduleName
		+ " but the module does not define a service by that name.");
}

DataBlockWriter CBroker::getServiceDescriptorRemote(int /*moduleId*/, string servName,
													Instance *moduleInst)
{
	ProxyModuleInstance *oProxy = dynamic_cast<ProxyModuleInstance *>(moduleInst);
	if (oProxy != NULL)
	{
		BrokerInterface *broker = getBrokerInterface(moduleInst->getBrokerId());

		// Note that moduleId refers to a module on the local broker --
		// must translate it to what it is on the *remote* broker.

		DataBlockWriter list = broker->getServiceDescriptor(oProxy->getRemoteId(), servName);
		return mungeServiceDescriptor(list, moduleInst->getFullName(),moduleInst->getHost());
	}
	BrokerInstance *oBroker = dynamic_cast<BrokerInstance *> (moduleInst);
	if (oBroker != NULL)
	{
		BrokerInterface *broker = getBrokerInterface(moduleInst->getBrokerId());

		// Note that moduleId refers to a module on the local broker --
		// must translate it to what it is on the *remote* broker.

		DataBlockWriter list = broker->getServiceDescriptor(oBroker->getRemoteId(), servName);
		return mungeServiceDescriptor(list, moduleInst->getFullName(),moduleInst->getHost());
	}

	return DataBlockWriter();
}

vector<DataBlockWriter> CBroker::getServiceDescriptors(int moduleId)
{
	checkModuleId(moduleId);
	Instance *inst = getInstanceForModuleId(moduleId);

	if (inst->getBrokerId() == BROKER_MODULE)
		return getServiceDescriptorsLocal(inst->getName(),inst);
	else
	{
		try
		{
			return getServiceDescriptorsRemote(inst->getName(), inst->getHost());
		}
		catch (SBWException *e)
		{
			// We should never get this -- we have a module id, therefore
			// the corresponding remote broker should be running.

			TRACE("Problem contacting broker " << inst->getBrokerId() << e->getMessage());
			delete e;
			throw new SBWCommunicationException(
				"Problem communicating with the Broker responsible for module "
				+ Properties::toString(moduleId),
				"Although the module identifier " + Properties::toString(moduleId) + " is known to"
				+ " SBW, the SBW Broker responsible for that module appears to"
				+ " be unreachable or otherwise unavailable.");			
		}
	}
}

vector<DataBlockWriter> CBroker::getServiceDescriptors(string name)
{
	string moduleName = getModuleNamePart(name);
	Host *host = getHostPart(name);

	if (host == NULL|| rpc->getHost()->equals(host))
		return getServiceDescriptorsLocal(moduleName);
	else
	{
		std::vector<Instance *> instances = instanceTable->getInstances();
		Instance *oTemp = NULL;
		for (unsigned int i =0; i < instances.size(); i++)
		{
			oTemp = instances[i];
			if (oTemp->getInternalName() == name)
				return getServiceDescriptorsLocal(name,oTemp);
		}
		return getServiceDescriptorsRemote(moduleName, host);
	}
}

vector<DataBlockWriter> CBroker::getServiceDescriptorsLocal(string moduleName, Instance *oInstance)
{
	vector<DataBlockWriter> oResult;
	moduleName = normalizeModuleName(moduleName);

	// If the module is not registered, it's an exception.  If it's
	// registered, return the services found (even if that means
	// returning a null list).

	ModuleDescriptor *md = moduleRegistry->getModuleDescriptor(moduleName);
	if (md == NULL) 
	{


		// ok ... it seems like the module has not been registered ... that might be bad ... lets try plan B
		try
		{


			// maybe we've done this befor so lets check that: 
			md = moduleRegistry->getTempModuleDescriptor(moduleName);

			if (md != NULL)
			{
				vector<ServiceDescriptor *> services = md->getServices();
				if (services.size()  == 0)
					return oResult;

				for (unsigned int j = 0; j <  services.size(); j++ )
				{                
					oResult.push_back(listForServiceDescriptor(moduleName, services[j]));
				}
				return oResult;
			}

			// we already know that we have a local instance ... so lets look at that instance
			if (oInstance == NULL)
			{
				// the message will be specified below 
				throw new SBWModuleNotFoundException(StringUtil::EMPTY,StringUtil::EMPTY);
			}
			// we are still here so the instance is valid ... 
			// so lets create a new module descriptor 
			DataBlockWriter oWriter;
			oWriter << oInstance->getName() << StringUtil::EMPTY << 0 << StringUtil::EMPTY << StringUtil::EMPTY;
			ModuleDescriptor *oDescriptor = new ModuleDescriptor(oWriter .getReader());//SBWLowLevel::getModuleDescriptor(oTemp->getName(), true);			
			// 1) get the number of methods
			SBWLowLevel::setRPC(rpc);
			int nNumber = SBWLowLevel::moduleGetNumberOfServices(oInstance->getId());
			for (int i = 0; i < nNumber; i++)
			{
				ServiceDescriptor oService = SBWLowLevel::serviceGetDescriptorHandle(oInstance->getId(), i);
				ServiceDescriptor *oTempSD = new ServiceDescriptor(&oService);
				oDescriptor->addService(oTempSD);
			}
			// ok ... saving these 'constructed' ModuleDescriptors in the Registry will result
			// in problems since they could be persisted the next time the user registeres a new 
			// element so lets add a secondary storage
			moduleRegistry->addTempModuleDescriptor(oInstance->getName(), oDescriptor);
			vector<ServiceDescriptor *> services = oDescriptor->getServices();
			if (services.size()  == 0)
				return oResult;

			for (unsigned int j = 0; j <  services.size(); j++ )
			{                
				oResult.push_back(listForServiceDescriptor(moduleName, services[j]));
			}
			return oResult;
		}
		catch(...)
		{
			// we are already desperate ... so we might as well try the following ... 
			// plan c
			try
			{
				if (oInstance == NULL) throw new SBWCommunicationException("");
				vector<string> serviceNames = SBW::getServiceNamesFromModule(oInstance->getId());
				for (unsigned int i = 0; i < serviceNames.size(); i++)
					oResult.push_back(listForServiceDescriptor(moduleName, serviceNames[i], StringUtil::EMPTY, StringUtil::EMPTY, StringUtil::EMPTY));
				return oResult;
			}
			catch(...)
			{
				// ok ... the module had three chances ... that's more then enough
				stringstream a,b;
				a << "Unknown/unregistered module '" << moduleName << "'";
				b << "An attempt was made to obtain a list of services for a"
					<< " module named '" << moduleName << "', but no such module"
					<< " is known to the SBW registry.";
				throw new SBWModuleNotFoundException( a.str(),b.str());
			}
		}
	}
	else
	{
		vector<ServiceDescriptor *> services = md->getServices();
		if (services.size()  == 0)
			return oResult;

		for (unsigned int j = 0; j <  services.size(); j++ )
		{                
			oResult.push_back(listForServiceDescriptor(moduleName, services[j]));
		}
		return oResult;
	}
}

vector<DataBlockWriter> CBroker::getServiceDescriptorsRemote(string moduleName, Host *host)
{
	BrokerInterface *broker = getBrokerInterface(host);
	vector<DataBlockWriter> lists = broker->getServiceDescriptors(moduleName);

	for (unsigned int i = 0; i < lists.size(); i++)
		lists[i] = mungeServiceDescriptor(lists[i], moduleName, host);

	return lists;
}
vector<int> CBroker::getExistingModuleInstanceIds()
{
	vector<int> oTemp;
	vector<Instance *> instances = instanceTable->getInstances();
	for (unsigned int i = 0; i < instances.size(); i++)
		oTemp.push_back( instances[i]->getId());

	return oTemp;
}
vector<string> CBroker::getServiceCategories(string parentCategory)
{
	parentCategory = SBW::normalizeCategoryName(parentCategory);

	if (parentCategory[0] == '/')
		parentCategory = parentCategory.substr(1);

	// This is not an especially efficient implementation, but I don't
	// think it needs to be. This method isn't likely to be used often,
	// so there doesn't seem to be a point to making this super-fast.

	// Using hashset here is just a simple way of getting unique keys.
	// The "* 5" is just an estimate of the space that might be needed.
	// The number is not founded in any empirical data whatsoever :-).

	vector<ModuleDescriptor *> mds = getDescriptors();	
	set< string > categories;

	for (unsigned int i = 0; i < mds.size(); i++)
	{
		vector<DataBlockWriter> sds = getServiceDescriptors(mds[i]->getName());
		for (unsigned int j = 0; j < sds.size(); j++)
		{
			DataBlockWriter oWriter = sds[j];
			string sTemp;
			oWriter.getReader() >> sTemp >> sTemp >> sTemp >> sTemp;
			categories.insert( sTemp );
		}
	}

	// Now we have a list of unique category names.
	// Do a substring match to find subcategories of the given parent.

	set< string > subcategories;
	set <string> ::iterator oIterator;
	for ( oIterator = categories.begin(); oIterator != categories.end(); oIterator++ )
	{
		string thisCat = SBW::normalizeCategoryName( *oIterator );

		if (thisCat[0] == '/')
			thisCat = thisCat.substr(1);

		if (parentCategory.length() == 0)
		{
			if (thisCat.length() != 0)
				subcategories.insert( thisCat );
		}
		else if (thisCat.find(parentCategory) == 0 &&
			thisCat.length() > parentCategory.length() &&
			thisCat[parentCategory.length()] == '/')
		{
			subcategories.insert (thisCat.substr(parentCategory.length() + 1));
		}
	}

	if (subcategories.size() < 1)
		return vector<string>();
	else
	{
		vector<string> oResult;
		set< string >::iterator oSubIterator;
		for (oSubIterator = subcategories.begin(); oSubIterator != subcategories.end(); oSubIterator++)
			oResult.push_back( *oSubIterator );
		return oResult;
	}
}
vector < DataBlockWriter > CBroker::findLocalServices(string categoryIn, bool recursive)
{
	TRACE("findLocalServices called with: " << categoryIn << " and " << ((recursive ? "true" : "false")));
	string category = SBW::normalizeCategoryName(categoryIn);

	// This is not an especially efficient implementation, but I don't
	// think it needs to be.  The number of modules and categories is
	// likely to always remain small (e.g., less than 100) and this
	// method isn't likely to be used often, so there doesn't seem to
	// be a point to making this super-fast.

	vector< ModuleDescriptor *> mds = getDescriptors(-1);
	vector < DataBlockWriter > descriptors;

	for (unsigned int i = 0; i < mds.size(); i++)
	{
		vector<DataBlockWriter> sds = getServiceDescriptors(mds[i]->getName());
		for (unsigned int j = 0; j < sds.size(); j++)
		{
			ServiceDescriptor *oTemp = new ServiceDescriptor (sds[j].getReader(),*mds[i]);
			string thisCategory = oTemp->getCategory ();
			if ((!recursive && thisCategory.find(category) == 0)
				|| (recursive && matchCategory(thisCategory, category, 0)))
				descriptors.push_back ( listForServiceDescriptor(mds[i]->getName(),oTemp));
		}
	}
	return descriptors;

}
vector < DataBlockWriter > CBroker::findServices(string categoryIn, bool recursive)
{
	TRACE("findLocalServices called with: " << categoryIn << " and " << ((recursive ? "true" : "false")));
	
	string category = SBW::normalizeCategoryName(categoryIn);

	// This is not an especially efficient implementation, but I don't
	// think it needs to be.  The number of modules and categories is
	// likely to always remain small (e.g., less than 100) and this
	// method isn't likely to be used often, so there doesn't seem to
	// be a point to making this super-fast.

	vector< ModuleDescriptor *> mds = getDescriptors();
	vector < DataBlockWriter > descriptors;

	for (unsigned int i = 0; i < mds.size(); i++)
	{
		vector<DataBlockWriter> sds = getServiceDescriptors(mds[i]->getName());
		for (unsigned int j = 0; j < sds.size(); j++)
		{
			ServiceDescriptor *oTemp = new ServiceDescriptor (sds[j].getReader(),*mds[i]);
			string thisCategory = oTemp->getCategory ();
			int nIndex = thisCategory.find(category);
			if (recursive)
			{
				if (matchCategory(thisCategory, category, 0))
					descriptors.push_back ( listForServiceDescriptor(mds[i]->getName(),oTemp));					
				else 
					if (nIndex == 0 || nIndex == 1)
						descriptors.push_back ( listForServiceDescriptor(mds[i]->getName(),oTemp));									
			}
			else
			{
				if (nIndex == 0 || nIndex == 1)
					descriptors.push_back ( listForServiceDescriptor(mds[i]->getName(),oTemp));									
			}
//			if ((!recursive && thisCategory.find(category) == 0)
//				|| (recursive && ))
//				descriptors.push_back ( listForServiceDescriptor(mds[i]->getName(),oTemp));
		}
	}
	return descriptors;
}
void CBroker::registerModule(string moduleName, string moduleDisplayName,
							 int moduleType, string commandLine, string help)
{
	// This has a lot of similarity to what ModuleImpl.initModuleImpl
	// does.  We have to do the same tests here too b/c this method can
	// be called from the low-level API directly.

	if (StringUtil::empty(moduleName))
		throw new SBWModuleDefinitionException(
		"Null or empty module name",
		"'Null' and empty strings are invalid module names.");

	if (getHostPart(moduleName) != NULL)
		throw new SBWModuleDefinitionException(
		"Module registration cannot use host:modulename format",
		"The program attempted to register a module whose name"\
		" has the form 'host:modulename'.  This form is illegal"\
		" during module registration because modules are always"\
		" registered with their local broker only.");

	if (StringUtil::empty(moduleDisplayName))
		throw new SBWModuleDefinitionException(
		"Null or empty module display name",
		"Module display names cannot be null or empty strings.");

	if (moduleType != UNIQUE
		&& moduleType != SELF_MANAGED){
			string msg = "Invalid module type specified";
			string detailed = "Module type must be one of ModuleImpl.SELF_MANAGED"\
				"ModuleImpl.SBW_MANAGED or ModuleImpl.UNIQUE";
			throw new SBWModuleDefinitionException(msg, detailed);
		}

		if (StringUtil::empty(commandLine)) {
			string msg = "Empty command line given for module";
			string detailed = "The command line for starting the module must be"\
				" non-empty; otherwise, the SBW Broker will not"\
				" be able to start up the module.";

			throw new SBWModuleDefinitionException(msg,detailed);
		}

		TRACE("Registering module '" << moduleName
			<< "', display name '" << moduleDisplayName
			<< "', type '" << moduleType
			<< "', cmdLine = " << commandLine
			<< "', help = '" << help << "'")

			moduleRegistry->addModuleDescriptor(moduleName, moduleDisplayName,
			moduleType, commandLine, help);
		rpc->notifyOthersOfRegistrationChange(BROKER_MODULE, true);
}
void CBroker::changeModuleName(int moduleId, string newModuleName)
{
	checkModuleId(moduleId);

	// Before going much further, check if newModuleName is an
	// acceptable module name.  The high-level APIs also check this,
	// but we have to do it again here because this method may be
	// called from the low-level API.

	if (getHostPart(newModuleName) != NULL)
	{
		throw new SBWModuleDefinitionException(
			"New module name cannot have host:modulename format",
			"The program attempted to change the name of module "
			+ Properties::toString(moduleId)
			+ " to a module whose name has the form 'host:modulename'."
			+ "  This form is illegal in a call to changeModuleName.");
	}

	// The new name looks ok, so get on with it.

	Instance *inst = getInstanceForModuleId(moduleId);

	if (inst->getBrokerId() == BROKER_MODULE)
		changeModuleNameLocal(moduleId, inst, newModuleName);
	else
		changeModuleNameRemote(moduleId, inst, newModuleName);
}
void CBroker::changeModuleNameLocal(int moduleId, Instance *moduleInst,
									string newModuleName)
{
	string moduleName = moduleInst->getName();

	TRACE("Changing name for local module " << Properties::toString(moduleId) << " ('" << moduleName << "') to '" << newModuleName << "'");

	// Get the current module descriptor for this module.  Because we
	// started with a moduleId, we know that if getModuleDescriptor
	// returns null, the module is currently unregistered.

	ModuleDescriptor *md = moduleRegistry->getModuleDescriptor(moduleName);
	if (md != NULL)
	{
		TRACE("Found '" << moduleName << "' in the local registry");
		moduleRegistry->addModuleDescriptor(newModuleName,
			md->getDisplayName(),
			md->getManagementType(),
			md->getCommandLine(),
			md->getHelp());
	}
	else
	{
		TRACE("Found " << moduleId << " is currently unregistered;"
			<< " adding new entry with default fields");
		moduleRegistry->addModuleDescriptor(newModuleName, StringUtil::EMPTY,
			SELF_MANAGED, StringUtil::EMPTY, StringUtil::EMPTY);
	}
	rpc->notifyOthersOfRegistrationChange(BROKER_MODULE, true);
}
void CBroker::changeModuleNameRemote(int /*moduleId*/, Instance *moduleInst,
									 string newModuleName)
{
	// This must be a remote instance, so cast it to access getRemoteId().
	int nRemoteId = 0;
	ProxyModuleInstance *oProxy = dynamic_cast<ProxyModuleInstance *> (moduleInst);
	BrokerInstance *oBroker = dynamic_cast<BrokerInstance *> (moduleInst);
	if (oProxy != NULL) nRemoteId = oProxy->getRemoteId();
	else if (oBroker != NULL) nRemoteId = oBroker->getRemoteId();
	else return;

	BrokerInterface *broker = getBrokerInterface(moduleInst->getBrokerId());

	// Note that moduleId refers to a module on the local broker --
	// must translate it to what it is on the *remote* broker.

	broker->changeModuleName(nRemoteId, newModuleName);
}
void CBroker::registerService(string moduleName, string serviceName,
							  string serviceDisplayName, string category,
							  string help)
{	
	// This has a lot of similarity to what ModuleImpl.addService
	// does.  We have to do the same tests here too b/c this method can
	// be called from the low-level API directly.

	if (StringUtil::empty(moduleName))
		throw new SBWModuleDefinitionException(
		"Null or empty module name",
		"'Null' and empty strings are invalid module names.");

	if (getHostPart(moduleName) != NULL)
		throw new SBWModuleDefinitionException(
		"Service registration cannot use host:modulename format",
		"The program attempted to register a service with a"\
		" module whose name has the form 'host:modulename'."\
		" This form is illegal during module registration"\
		" because modules are always registered with their"\
		" local broker only.");

	if (StringUtil::empty(serviceName))
		throw new SBWModuleDefinitionException(
		"Null or empty service name",
		"'Null' and empty strings are invalid service names.");

	if (StringUtil::empty(serviceDisplayName)) 
	{
		//throw new SBWModuleDefinitionException(
		//    "Null or empty service display name",
		//    "Service display names cannot be null or empty strings.");
		TRACE("Service display names cannot be null or empty strings.");
		serviceDisplayName = serviceName;
	}

	moduleName = normalizeModuleName(moduleName);
	serviceName = serviceName;
	category = SBW::normalizeCategoryName(category);

	ModuleDescriptor *md = moduleRegistry->getModuleDescriptor(moduleName);

	if (md == NULL)
	{
		throw new SBWModuleNotFoundException( string() +
			"No such module '" + moduleName + "'",
			"A module must be defined before services can be defined on it");
	}

	TRACE("Registering service on module '" << moduleName
		<< "' named '" << serviceName
		<< "', display name = '" << serviceDisplayName
		<< "', category = '" << category
		<< "', help = '" << help << "'");

	// People should be able to overwrite the definition of a service
	// on a module.  We print a notice just in case they didn't mean to.

	vector< ServiceDescriptor *> services = md->getServices();
	int nPosition = -1;
	for (unsigned int i = 0; i < services.size(); i ++) {
		if (services[i]->getName() == serviceName)
			nPosition = (int)i;
	}
	if (nPosition != -1) 
	{
		TRACE("Overwriting service '" << serviceName << "' on module '" << moduleName << "'");
	}

	DataBlockWriter oWriter;
	oWriter << md->getName() << serviceName << serviceDisplayName << category << help;
	ServiceDescriptor *service = new ServiceDescriptor(oWriter.getReader(), *md);
	md->addService(service);

	// Overwrite the previous registry entry for this module, forcing
	// the registry to be checkpointed.

	moduleRegistry->addModuleDescriptor(moduleName, md);
	rpc->notifyOthersOfRegistrationChange(BROKER_MODULE, true);
}
void CBroker::unregisterModule(string moduleName)
{
	if (getHostPart(moduleName) != NULL)
		throw new SBWModuleDefinitionException(
		"Module registration cannot use host:modulename format",
		"The program attempted to unregister a module whose name"\
		" has the form 'host:modulename'.  This form is illegal"\
		" during module registration because modules are always"\
		" registered with their local broker only.");

	moduleName = normalizeModuleName(moduleName);
	TRACE("Removing registration for module '" << moduleName << "'");

	// This doesn't throw an exception if the module name is empty.
	// Should it?

	if (! StringUtil::empty(moduleName))
	{
		moduleRegistry->removeModuleDescriptor(moduleName);
		rpc->notifyOthersOfRegistrationChange(BROKER_MODULE, true);
	}
}
int CBroker::linkBroker(string hostName)
{
	Host* oNewHost = new Host(hostName);
	if (oNewHost->getPort() == -1)
		oNewHost->setPort(RuntimeProperties::getSBWBrokerPort());
	return getBrokerId(oNewHost);
}
void CBroker::shutdownBroker()
{
	//! TODO fix that ...
	rpc->pleaseStop();
}
string CBroker::getVersion()
{
	string sResult(VERSION);
	sResult += " (";
#ifdef _DEBUG
	sResult += "C++ Debug";
#else
	sResult += "C++ Release";
#endif
#ifdef WIN32
	sResult += ",WIN32";
#elif DARWIN
	sResult += ",OS X";
#elif SOLARIS
	sResult += ",Solaris";
#elif LINUX
	sResult += ",Linux";
#endif
	sResult += " running on port: " + StringUtil::toString(RuntimeProperties::getSBWBrokerPort()) + ")";
	return sResult;
}

#ifndef WIN32
#include <dirent.h>
#endif

std::string findCellDesigner(std::string& commandLine)
{
#ifdef WIN32
  size_t pos = commandLine.find("jre"); 
  if (pos == std::string::npos)
    return commandLine;
	
  std::string path = commandLine.substr (0, pos);
	
   WIN32_FIND_DATA FindFileData;
   HANDLE hFind;
   std::string test = path + "CellDesigner*.exe";
   hFind = FindFirstFile(test.c_str(), &FindFileData);
   if (hFind != INVALID_HANDLE_VALUE) 
   {
      std::string result = "cmd /c start \"\" /b /d \"" +  path + "\" " + FindFileData.cFileName;
      FindClose(hFind);
	  return result;
   }
   return commandLine;
#elif DARWIN
  std::string path;
  size_t pos = commandLine.find("-classpath \"");
  if (pos != std::string::npos)
  { 
    path = commandLine.substr(pos + 12);
    size_t end = path.find("exec");
    path = path.substr(0, end);    
  }
  else 
   return commandLine;   
    
   DIR *dirp = opendir(path.c_str());
   struct dirent *dp = NULL;
   while ((dp = readdir(dirp)) != NULL)
   {
     string current (dp->d_name);
     if (current.find("CellDesigner") == 0 && current.find(".app") != std::string::npos)
     {
       (void)closedir(dirp);
       return "open -n " + path + current;
     }
   }
   (void)closedir(dirp);

#else

  std::string path;
  size_t pos = commandLine.find("-classpath \"");
  if (pos != std::string::npos)
  { 
    path = commandLine.substr(pos + 12);
    size_t end = path.find("exec");
    path = path.substr(0, end);    
  }
  else 
   return commandLine;   
    
   DIR *dirp = opendir(path.c_str());
   struct dirent *dp = NULL;
   while ((dp = readdir(dirp)) != NULL)
   {
     string current (dp->d_name);
     if (current.find("CellDesigner") == 0 && current.find(".sh") != std::string::npos)
     {
       (void)closedir(dirp);
       return path + current;
     }
   }
   (void)closedir(dirp);
#endif	
   return commandLine;
}

int CBroker::startNewLocalModuleInstance(ModuleDescriptor *md)
{
	string moduleName = md->getName();
	string commandLine = md->getCommandLine();
	
	// add workaround for celldesigner
	if (moduleName == "celldesigner")
	{
		commandLine = findCellDesigner(commandLine);
	}

	
	TRACE("Attempting to start an instance of '" << moduleName << "'");
	rpc->addStartupWatcher(moduleName);
	try
	{
		SBWOS::startProcess((char*)(commandLine.c_str()));
		for (int i = 0; i < STARTUP_WAIT; i++) {
			try {SBWThread::sleep(200); } catch (...) { }
			int nModuleId = rpc->getStartupStat(moduleName);
			if (nModuleId != -1) 
			{
				Instance* oInstance = instanceTable->getInstance(nModuleId);
				if (oInstance == NULL)
				{
					nModuleId = -1;
					continue;
				}

				if (oInstance->getName() == moduleName)
				{
					return nModuleId;
				}
			}
		}
	}
	catch(SBWException *e)
	{
		TRACE ("exception during startup: " << e->getMessage() << " - " << e->getDetailedMessage() << " \ntried to start: '"  << commandLine << "'");
		delete e;
	}
	TRACE("Module '" << moduleName << "' couldn't be started in time.");	
	throw new SBWModuleStartException( moduleName + " couldn't be started" , moduleName + " couldn't be started" );
}
DataBlockWriter CBroker::listForModuleDescriptor(ModuleDescriptor *md)
{	
	DataBlockWriter oResult;
	oResult << md->getName() << md->getDisplayName() << md->getManagementType() << md->getCommandLine() << md->getHelp();
	return oResult;
}
DataBlockWriter CBroker::listForModuleDescriptor(string moduleName, string displayName,
												 int mgmtType, string cmdLine, string help)
{
	DataBlockWriter oResult;
	oResult << moduleName << displayName << mgmtType << cmdLine << help;
	return oResult;
}
DataBlockWriter CBroker::listForServiceDescriptor(string moduleName, string serviceName,
												  string serviceDisplayName, string category,
												  string helpString)
{
	DataBlockWriter oResult;
	oResult << moduleName << serviceName << serviceDisplayName << category << helpString;
	return oResult;
}
DataBlockWriter CBroker::listForServiceDescriptor(string moduleName, ServiceDescriptor *sd)
{
	DataBlockWriter oResult;
	oResult << moduleName << sd->getName() << sd->getDisplayName() << sd->getCategory() << sd->getHelp();
	return oResult;
}
bool CBroker::matchCategory(string category, string toMatch, unsigned int start)
{
	if (category.length() <= start || toMatch.length() > category.length())
	{
		TRACE("category: " << category << " toMatch: " << toMatch << " does not match " );
		return false;
	}
	else if (category.substr(start).find(toMatch, start) == start)
	{
		TRACE("category: " << category << " toMatch: " << toMatch << " does match " );
		return true;
	}
	else if (category.substr(start).find('/', start) != category.npos)
	{
		int nFound = (int)category.substr(start).find('/', start);
		if (nFound < 0)
			return false;
		int newPos = nFound + 1;
		return matchCategory(category.substr(start), toMatch,	newPos);
	}
	else
	{
		TRACE("category: " << category << " toMatch: " << toMatch << " does not match " );
		return false;
	}
}
string CBroker::normalizeModuleName(string moduleName)
{
	if (StringUtil::empty(moduleName))
		throw new SBWModuleNotFoundException( "Null or empty module name", "'Null' and empty strings are invalid module names.");
	return moduleName;

}
string CBroker::normalizeServiceName(string serviceName)
{
	return serviceName;
}
void CBroker::checkModuleId(int moduleId)
{
	if (moduleId != BROKER_MODULE
		&& ! instanceTable->exists(moduleId))
	{
		TRACE("Given unknown module id " << moduleId);
		stringstream a,b;
		a << "Unknown module identifier " << moduleId;
		b << "An attempt was made to obtain an instance of a module"
			<< " with identifier " << moduleId  << ", but no such module"
			<< " is known to the SBW Broker.";
		throw new SBWModuleNotFoundException(a.str(), b.str());
	}
}
Host *CBroker::getHostPart(string name)
{
	int pos = (int)name.find(':');
	if (pos != (int)name.npos)
	{
		Host* host = new Host(name.substr(0, pos));
		if (host->getAddress() == "127.0.0.1") 
		{
			delete host; return NULL;
		}
		return host;
	}
	else
		return NULL;
}

string CBroker::getModuleNamePart(string name)
{
	int pos = (int)name.find(':');
	if (pos != (int)name.npos)
		return normalizeModuleName(name.substr(pos + 1));
	else
		return normalizeModuleName(name);
}
Instance *CBroker::getInstanceForModuleId(int moduleId)
{
	Instance *inst = instanceTable->getInstance(moduleId);
	if (inst != NULL)
		return inst;

	stringstream a,b;
	a <<"Unknown module " << moduleId ;
	b << "An attempt was made to obtain an instance of a module"
		<< " with identifier " << moduleId << " from the broker on '"
		<< rpc->getHost()->getName() 
		<< ", but no such module is known to the Broker.";
	throw new SBWModuleNotFoundException( a.str(),b.str());
}
ModuleDescriptor *CBroker::getDescriptor(int brokerId, string moduleName)
{
	if (brokerId == BROKER_MODULE)
		return moduleRegistry->getModuleDescriptor(moduleName);
	else
	{
		BrokerInstance *brokerInst = (BrokerInstance *) instanceTable->getInstance(brokerId);
		BrokerInterface *broker = getBrokerInterface(brokerId);

		DataBlockWriter list = broker->getModuleDescriptor(moduleName, false);

		Host *host = brokerInst->getHost();
		return new ModuleDescriptor(mungeModuleDescriptor(list, moduleName, host).getReader());
	}
}

vector<ModuleDescriptor *> CBroker::getDescriptors()
{
	vector<ModuleDescriptor *> oTemp;
	vector<int> brokerIds = getBrokerIds();
	for (unsigned int i = 0; i < brokerIds.size(); i++)
	{
		vector<ModuleDescriptor *> descriptors = getDescriptors(brokerIds[i]);
		for (unsigned int j = 0; j < descriptors.size(); j++)
			oTemp.push_back(descriptors[j]);
	}
	return oTemp;
}

vector<ModuleDescriptor *> CBroker::getDescriptors(int brokerId)
{
	if (brokerId == BROKER_MODULE)
		return moduleRegistry->getModuleDescriptors();
	else
	{
		BrokerInstance *brokerInst = dynamic_cast<BrokerInstance *> (instanceTable->getInstance(brokerId));
		BrokerInterface *broker = getBrokerInterface(brokerId);
		Host *host = brokerInst->getHost();

		vector< DataBlockWriter> records = broker->getModuleDescriptors(true, false);

		vector<ModuleDescriptor *>  mds;
		for (unsigned int i = 0; i < records.size(); i++)
		{
			int nLength = 0;
			DataBlockWriter list = records[i];
			DataBlockWriter *oWriter = new DataBlockWriter;
			unsigned char * copy = list.getDataCopy(nLength);
			oWriter->insertData(copy,nLength); 
			string moduleName;
			oWriter->getReader() >> moduleName;
			delete oWriter;
			mds.push_back(new ModuleDescriptor(mungeModuleDescriptor(list, moduleName, host).getReader()));
		}
		return mds;
	}	
}
int CBroker::getBrokerId(Host *host)
{
	if ((rpc->getHost()->equals(host) || host->getAddress() == "127.0.0.1") 
		&& host->getPort() == rpc->getHost()->getPort())
		return BROKER_MODULE;
	else
	{
		// See if we already have a connection to a Broker on the host.
		if (host->getPort() == -1)
			host->setPort(rpc->getHost()->getPort());
		vector<Instance *> instances = instanceTable->getInstances();
		for (int i = (int)instances.size() - 1; i >= 0; i--)
		{
			BrokerInstance *inst = dynamic_cast<BrokerInstance *>(instances[i]);
			if (inst != NULL && inst->getHost()->equals(host))
				return inst->getId();
		}

		// If we get here, we haven't connected to the host yet.				
		int brokerId = rpc->connectBroker(host);
		// now lets wait for the connection to be established ...
		for (int i = 0; i < 600; i++)
		{
			BrokerInstance *bInst = dynamic_cast<BrokerInstance *>( instanceTable->getInstance(brokerId));
			if (bInst == NULL)
				try {SBWThread::sleep(100);} catch (...) {}
			else
				break;
		}
		return brokerId;

	}
}

vector<int> CBroker::getBrokerIds()
{
	vector<int> oTemp;
	vector<Instance *> instances = instanceTable->getInstances();
	oTemp.push_back( BROKER_MODULE );
	for (unsigned int i = 0; i < instances.size(); i++)
	{
		Instance *inst = instances[i];
		BrokerInstance *oBroker = dynamic_cast<BrokerInstance *> (inst);

		if (oBroker != NULL)
			oTemp.push_back(inst->getId());
	}
	return oTemp;
}
BrokerInterface *CBroker::getBrokerInterface(Host *host)
{
	return getBrokerInterface(getBrokerId(host));
}

BrokerInterface *CBroker::getBrokerInterface(int bId)
{    
	checkBrokerId(bId);
	BrokerInstance *bInst = dynamic_cast<BrokerInstance *>( instanceTable->getInstance(bId));
	if (bInst != NULL)
		return bInst->getBrokerInterface();
	else
		throw new SBWApplicationException("instance: " + Properties::toString(bId) + " could not be found");
}
DataBlockWriter CBroker::mungeModuleDescriptor(DataBlockWriter list, string modName, Host *host)
{
	DataBlockWriter oResult;
	DataBlockReader oReader = list.getReader();
	string sTemp;
	int nLength = 0;

	oResult << host->getName() + ":" + modName;
	oReader >> sTemp;
	oReader >> sTemp;
	oResult << sTemp + " on " + host->getName();
	unsigned char * copy = oReader.getRemainingDataForC(nLength);
	oResult.insertData(copy,nLength);

	return oResult;
}
DataBlockWriter CBroker::getListOfModules()
{
	DataBlockWriter oResult;
	vector <Instance *> instances = instanceTable->getInstances();
	vector <Instance *> ::iterator oIterator;
	for (oIterator = instances.begin(); oIterator != instances.end(); oIterator ++)
	{
		Instance *currentInstance = (*oIterator);
		DataBlockWriter oTemp;
		string moduleName;
		// TODO ... different brokers on the same host???
		if (currentInstance->getHost()->equals(rpc->getHost()))
		{
			moduleName = currentInstance->getName();
		}
		else
		{
			moduleName = currentInstance->getHost()->getName() + ":" + currentInstance->getName();
		}
		oTemp << currentInstance->getId() << moduleName;
		oResult << oTemp;
	}
	return oResult;
}
DataBlockWriter CBroker::getMethodIds(int moduleId,int serviceId)
{
	try
	{
		checkModuleId(moduleId);
		DataBlockWriter oResult;
		DataBlockWriter args ;
		args << serviceId;
		vector<string> methods;
		rpc->call(moduleId, SBWSystemService, SBWGetMethodsMethod, args) >> methods;
		for (unsigned int i = 0; i < methods.size(); i++)
		{
			DataBlockWriter oTemp;
			oTemp << (signed)i << methods[i];
			oResult << oTemp;
		}

		return oResult;
	} 
	catch (SBWException *e)
	{
		throw e;
	}
	catch (...)
	{
		throw new SBWApplicationException ("error retrieving method ids");
	}
}
DataBlockWriter CBroker::getMethodIds(string moduleName, string serviceName)
{
	try
	{
		std::vector<std::string> services ;
		Integer serviceId = 0 ;
		Instance *oTemp = instanceTable->getInstance((getHostPart(moduleName) == NULL ? rpc->getHost() : getHostPart(moduleName)),  getModuleNamePart(moduleName));
		if (oTemp == NULL)
			throw new SBWApplicationException("error retrieving method ids for " + moduleName + "and " + serviceName);
		rpc->call(oTemp->getId(), SBWSystemService, SBWGetServicesMethod, DataBlockWriter()) >> services ;		
		while (serviceId !=  (int)services.size()
			&& services[serviceId].compare(serviceName))
			serviceId++ ;

		if (serviceId ==  (int)services.size())
			throw new SBWServiceNotFoundException("service not found");

		return getMethodIds(oTemp->getId(),serviceId);
	}
	catch(...)
	{
		throw new SBWApplicationException("error retrieving method ids for " + moduleName + "and " + serviceName);
	}
}
bool CBroker::isRemoteInstance(int nModuleId)
{	
	Instance * oTemp = instanceTable->getInstance(nModuleId);
	if (oTemp == NULL) return false;
	if (SBWOSSocket::isLocalAddress(rpc->getHost()->getAddress().c_str()) &&
		SBWOSSocket::isLocalAddress(oTemp->getHost()->getAddress().c_str()))
		return false;
	return !(oTemp->getHost()->equals(rpc->getHost()));	
}

DataBlockWriter CBroker::mungeServiceDescriptor(DataBlockWriter list, string modName, Host *host)
{
	SBWOSMutexLock ml(brokerMutex);
	DataBlockWriter oResult;
	DataBlockReader oReader = list.getReader();
	string sTemp;
	int nLength = 0;
	oReader >> sTemp;
	oResult << host->getName() + "/" + StringUtil::toString(host->getPort()) + ":" + modName;
	oReader >> sTemp;
	oResult << sTemp;
	oReader >> sTemp;
	oResult << sTemp + " on " + host->getName();
	unsigned char * copy = oReader.getRemainingDataForC(nLength);
	oResult.insertData(copy,nLength-1);
	return oResult;
}

SBWBrokerRPC *CBroker::rpc = NULL;
InstanceTable *CBroker::instanceTable = NULL;
ModuleRegistry *CBroker::moduleRegistry = NULL;
string	CBroker::BROKER_NAME = "BROKER";
