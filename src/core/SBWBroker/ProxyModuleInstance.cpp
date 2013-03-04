#include "ProxyModuleInstance.h"

#include "SBW/SBWApplicationException.h"

using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;

ProxyModuleInstance::ProxyModuleInstance(int id, int remoteId, std::string name, BrokerInstance *brokerInst): transmitMutex("proxymodule")
{
	myId = id;
	myRemoteId = remoteId;
	myName = name;
	myType = "module";
	myHost = brokerInst->getHost();
	myFullName = myHost->getName() + ":" + myName;
	myInternalName = myHost->getAddress() + ":" + myName;
	myBrokerInstance = brokerInst;
	myBrokerId = myBrokerInstance->getId();

	brokerInst->setLocalIdForRemote(remoteId, id);
}
int ProxyModuleInstance::getRemoteId()
{
	return myRemoteId;
}
void ProxyModuleInstance::transmit(unsigned char *data, int length)
{
	SBWOSMutexLock ml(transmitMutex);
	TRACE("Transmitting to " << getNameForMessage());
	DataBlockWriter writer;
	// The data array is a complete message, but b/c we're
	// rerouting it, we have to replace the destination id.

	writer.getImpl()->packWithoutType(length);
	writer.getImpl()->packWithoutType(myRemoteId);

	int nLength;
	unsigned char *array = const_cast<unsigned char*>(writer.getData(&nLength));

	myBrokerInstance->transmitDirect((char*)array, 0, nLength, false);
	myBrokerInstance->transmitDirect((char*)data, 8, length - 8, false);
}
bool ProxyModuleInstance::connect(const char* /*key*/)
{
	return myBrokerInstance != NULL;
}
void ProxyModuleInstance::disconnect()
{
	// ???
}
