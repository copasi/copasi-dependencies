#include "ProxyLocalBrokerInstance.h"
#include "SBW.h"
#include "SBW/RuntimeProperties.h"
#include "SBW/SBWRPC.h"
#include "SBW/DataBlockReader.h"
#include "SBW/SBWApplicationException.h"

using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;

ProxyLocalBrokerInstance::ProxyLocalBrokerInstance() : transmitMutex("proxylocalbroker")
{
	myId = BROKER_MODULE;
	myBrokerId = myId;
	myHost = SBWBrokerRPC::getHost();
	myName = "BROKER";
	myType = "broker";
	myFullName = myHost->getName () + ":BROKER";
	myInternalName = myHost->getAddress() + ":BROKER";
	myHost->setPort(RuntimeProperties::getSBWBrokerPort());
	rpc = Broker::SBW::getInternalAPI();
}
bool ProxyLocalBrokerInstance::connect(const char* /*key*/)
{
	return true;
}
void ProxyLocalBrokerInstance::transmit(unsigned char *data, int length)
{
	SBWOSMutexLock ml(transmitMutex);
	TRACE("Transmitting to " << getNameForMessage());
	int nType = 0;	
	DataBlockReader reader (data,length,true);
	DataBlockReader backup (data,length,true);
	reader.getWithoutType(nType); // consume length
	if (nType == length)
	{
		int nDestination = 0;
		reader.getWithoutType(nDestination); // consume destination
		rpc->receive( reader );
	}
	else 
	{
		rpc->receive( backup );
	}
}
void ProxyLocalBrokerInstance::disconnect()
{
	// no op
}
SBWRPC *ProxyLocalBrokerInstance::rpc = NULL;
