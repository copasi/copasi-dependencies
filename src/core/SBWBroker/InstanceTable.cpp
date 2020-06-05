#include "InstanceTable.h"
#include "StringUtil.h"
#include "Instance.h"

#include "SBW/config.h"
#include "SBW/sbwdefs.h"
#include "SBW/portableOS.h"
#include "SBW/SBWApplicationException.h"

#include <cstdlib>

using namespace SystemsBiologyWorkbench::Broker;

InstanceTable *InstanceTable::table = NULL;

InstanceTable *InstanceTable::getTable()
{
	if (table == NULL) 
  {
		table = new InstanceTable();
    std::atexit(&deleteTable);
  }
	return table;
}

void InstanceTable::deleteTable()
{
  delete table;
  table = NULL;
}

void InstanceTable::addInstance(Instance *inst)
{
	SBWOSMutexLock ml(instanceMutex);
	TRACE("Adding instance " <<  inst->getId() << " to the list of running modules");
	idHash.put(inst->getId(), inst);
	nameHash.insert( StringInstance(inst->getInternalName().c_str(), inst));
}

void InstanceTable::removeInstance(Instance *inst)
{
	if (inst == NULL) return;
	SBWOSMutexLock ml(instanceMutex);

	TRACE("Removing instance " << inst->getId() << " from list of running modules");

	idHash.remove(inst->getId());
	map< string , Instance *> ::iterator oIterator;
	string sName = inst->getInternalName();
	oIterator = nameHash.find(sName);
	if (oIterator != nameHash.end())
		nameHash.erase(oIterator);
}

void InstanceTable::removeInstance(int moduleId)
{
	SBWOSMutexLock ml(instanceMutex);
	removeInstance(getInstance(moduleId));
	idHash.remove(moduleId);
}

Instance *InstanceTable::getInstance(int moduleId)
{
	return (Instance*) idHash.get(moduleId);
}

Instance *InstanceTable::getInstance(Host *host, string moduleName)
{
	if (StringUtil::empty(moduleName) || host == NULL)
		return NULL;
	else {
		string sTemp = host->getAddress() + ":" + moduleName;
		map< string , Instance *> ::iterator oTemp;
		oTemp = nameHash.find(sTemp);
		if (oTemp == nameHash.end())
		{
			return NULL;
		}
		else return oTemp ->second;
	}
}

vector<Instance *> InstanceTable::getInstances(int brokerId)
{        
	vector<Instance *> oTemp = idHash.getAll();
	vector<Instance *> oResult;

	for (unsigned int i = 0; i < oTemp.size(); i++)
		if (oTemp[i]->getBrokerId() == brokerId)
			oResult.push_back(oTemp[i]);

	return oResult;
}

vector<Instance *> InstanceTable::getInstances()
{	
	return idHash.getAll();
}

bool InstanceTable::exists(int moduleId)
{
	return idHash.exists(moduleId);
}

int InstanceTable::getNextId()
{
	return nextId++;
}

InstanceTable *InstanceTable::readResolve()
{
	return table;
}
