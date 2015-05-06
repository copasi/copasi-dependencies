#include "BrokerDefines.h"
#include "SBW/sbwenums.h"
#include "BrokerInterface.h"
#include "SBW/DataBlockReader.h"
#include "SBW/SBWApplicationException.h"

using namespace std;
using namespace SystemsBiologyWorkbench;
using namespace SystemsBiologyWorkbench::Broker;

BrokerInterface::BrokerInterface(int nId)
{
	m_nId = nId;
	m_oRPC = Broker::SBW::getInternalAPI();
}
string BrokerInterface::getVersion()
{
	DataBlockWriter oWriter;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,GetVersion,oWriter);
	string  sResult;
	oReader >> sResult;				
	return sResult;
}
vector< DataBlockWriter > BrokerInterface::findLocalServices(string category, bool recursive)
{
	DataBlockWriter oWriter; oWriter << category << recursive;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,FindLocalServices,oWriter);
	vector< DataBlockReader > oTemp;
	oReader >> oTemp;	
	vector< DataBlockWriter  > oResult;
	vector< DataBlockReader >::iterator oIterator;
	for (oIterator = oTemp.begin(); oIterator != oTemp.end(); oIterator++)
	{
		int dataLength = 0;
		unsigned char *data = (*oIterator).getRemainingDataForC(dataLength);
		DataBlockWriter oTempWriter;	
		oTempWriter.insertData(data,dataLength-1);
		oResult.push_back(oTempWriter);
	}
	return oResult;
}
vector< DataBlockWriter > BrokerInterface::findServices(string category, bool recursive)
{
	DataBlockWriter oWriter; oWriter << category << recursive;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,FindServices,oWriter);
	vector< DataBlockReader > oTemp;
	oReader >> oTemp;	
	vector< DataBlockWriter  > oResult;
	vector< DataBlockReader >::iterator oIterator;
	for (oIterator = oTemp.begin(); oIterator != oTemp.end(); oIterator++)
	{
		int dataLength = 0;
		unsigned char *data = (*oIterator).getRemainingDataForC(dataLength);
		DataBlockWriter oTempWriter;	
		oTempWriter.insertData(data,dataLength-1);
		oResult.push_back(oTempWriter);
	}
	return oResult;
}
DataBlockWriter BrokerInterface::getModuleDescriptor(string moduleName, bool includeRunning)
{
	DataBlockWriter oWriter; oWriter << moduleName << includeRunning;
	DataBlockReader oReader;
	m_oRPC->call(m_nId,0,GetModuleDescriptorString,oWriter) >> oReader;
	int dataLength = 0;
	unsigned char *data = oReader.getRemainingDataForC(dataLength);
	DataBlockWriter oResult;	
	oResult.insertData(data,dataLength);
	return oResult;
}
DataBlockWriter BrokerInterface::getModuleDescriptor(int moduleId)
{
	DataBlockWriter oWriter; oWriter << moduleId;
	DataBlockReader oReader;
	m_oRPC->call(m_nId,0,GetModuleDescriptorInt,oWriter) >> oReader;
	int dataLength = 0;
	unsigned char *data = oReader.getRemainingDataForC(dataLength);
	DataBlockWriter oResult;	
	oResult.insertData(data,dataLength);
	return oResult;
}
vector< DataBlockWriter  > BrokerInterface::getModuleDescriptors(bool localOnly, bool includeRunning) 
{
	DataBlockWriter oWriter; oWriter << localOnly << includeRunning;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,GetModuleDescriptors,oWriter);
	vector< DataBlockReader > oTemp;
	oReader >> oTemp;	
	vector< DataBlockWriter  > oResult;
	vector< DataBlockReader >::iterator oIterator;
	for (oIterator = oTemp.begin(); oIterator != oTemp.end(); oIterator++)
	{
		int dataLength = 0;
		unsigned char *data = (*oIterator).getRemainingDataForC(dataLength);
		DataBlockWriter oTempWriter;	
		oTempWriter.insertData(data,dataLength);
		oResult.push_back(oTempWriter);
	}
	return oResult;
}
int BrokerInterface::getModuleInstance(string moduleName)
{
	DataBlockWriter oWriter; oWriter << moduleName;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,GetModuleInstance,oWriter);
	int nResult;
	oReader >> nResult;				
	return nResult;
}
vector< string > BrokerInterface::getServiceCategories(string parentCategory)
{
	DataBlockWriter oWriter; oWriter << parentCategory;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,GetServiceCategories,oWriter);
	vector < string > oResult;
	oReader >> oResult;				
	return oResult;
}
int BrokerInterface::linkBroker(string hostNameOrAddress)
{
#ifdef INSECURE
	DataBlockWriter oWriter; oWriter << hostNameOrAddress;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,LinkBroker,oWriter);
	int nResult;
	oReader >> nResult;				
	return nResult;
#else
	throw new SBWApplicationException("insecure execution denied", "linking the remote broker to another computer is not allowed.");
#endif
}
vector<int> BrokerInterface::getExistingModuleInstanceIds()
{
	DataBlockWriter oWriter;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,GetExistingModuleInstanceIds,oWriter);
	vector<int> oResult;
	oReader >> oResult;				
	return oResult;
}
void BrokerInterface::changeModuleName(int moduleId, string newModuleName)
{
#ifdef INSECURE
	DataBlockWriter oWriter; oWriter << moduleId << newModuleName;
	m_oRPC->call(m_nId,0,ChangeModuleName,oWriter);
#else
	SBW_UNREFERENCED_PARAMETER(moduleId);
	throw new SBWApplicationException("insecure execution denied", "changing the remote registry is not allowed");
#endif
}
DataBlockWriter  BrokerInterface::getServiceDescriptor(int moduleId, int serviceId)
{
	DataBlockWriter oWriter; oWriter << moduleId << serviceId;
	DataBlockReader oReader;
	m_oRPC->call(m_nId,0,GetServiceDescriptorInt,oWriter) >> oReader;
	int dataLength = 0;
	unsigned char *data = oReader.getRemainingDataForC(dataLength);
	DataBlockWriter oResult;	
	oResult.insertData(data,dataLength);
	return oResult;
}
DataBlockWriter BrokerInterface::getServiceDescriptor(int moduleId, string serviceName)
{
	DataBlockWriter oWriter; oWriter << moduleId << serviceName;
	DataBlockReader oReader; 
	m_oRPC->call(m_nId,0,GetServiceDescriptorString,oWriter) >> oReader;
	int dataLength = 0;
	unsigned char *data = oReader.getRemainingDataForC(dataLength);
	DataBlockWriter oResult;	
	oResult.insertData(data,dataLength);
	return oResult;
}
vector<DataBlockWriter > BrokerInterface::getServiceDescriptors(int moduleId)
{
	DataBlockWriter oWriter; oWriter << moduleId;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,GetServiceDescriptorsInt,oWriter);
	vector< DataBlockReader > oTemp;
	oReader >> oTemp;	
	vector< DataBlockWriter  > oResult;
	vector< DataBlockReader >::iterator oIterator;
	for (oIterator = oTemp.begin(); oIterator != oTemp.end(); oIterator++)
	{
		int dataLength = 0;
		unsigned char *data = (*oIterator).getRemainingDataForC(dataLength);
		DataBlockWriter oTempWriter;	
		oTempWriter.insertData(data,dataLength-1);
		oResult.push_back(oTempWriter);
	}
	return oResult;
}
vector<DataBlockWriter > BrokerInterface::getServiceDescriptors(string moduleName)
{
	DataBlockWriter oWriter; oWriter << moduleName;
	DataBlockReader oReader = m_oRPC->call(m_nId,0,GetServiceDescriptorsString,oWriter);
	vector< DataBlockReader > oTemp;
	oReader >> oTemp;	
	vector< DataBlockWriter  > oResult;
	vector< DataBlockReader >::iterator oIterator;
	for (oIterator = oTemp.begin(); oIterator != oTemp.end(); oIterator++)
	{
		int dataLength = 0;
		unsigned char *data = (*oIterator).getRemainingDataForC(dataLength);
		DataBlockWriter oTempWriter;	
		oTempWriter.insertData(data,dataLength-1);
		oResult.push_back(oTempWriter);
	}
	return oResult;
}
void BrokerInterface::registerModule(string moduleName, string moduleDisplayName,
									 int moduleType, string commandLine,
									 string helpString)
{
#ifdef INSECURE
	DataBlockWriter oWriter; 
	oWriter << moduleName << moduleDisplayName << moduleType 
		<< commandLine << helpString;
	m_oRPC->call(m_nId,0,RegisterModule,oWriter);
#else
	SBW_UNREFERENCED_PARAMETER(moduleType);
	throw new SBWApplicationException("insecure execution denied", "changing the remote registry is not allowed");
#endif
}
void BrokerInterface::registerService(string moduleName, string serviceName,
									  string serviceDisplayName, string category,
									  string helpString)
{
#ifdef INSECURE
	DataBlockWriter oWriter; oWriter << moduleName << serviceName 
		<< serviceDisplayName << category << helpString;
	m_oRPC->call(m_nId,0,RegisterService,oWriter);
#else
	throw new SBWApplicationException("insecure execution denied", "changing the remote registry is not allowed");
#endif
}
void BrokerInterface::shutdownBroker()
{
#ifdef INSECURE
	DataBlockWriter oWriter;
	m_oRPC->call(m_nId,0,ShutdownBroker,oWriter);
#else
	throw new SBWApplicationException("insecure execution denied", "shutdown of remote broker is not supported.");
#endif
}
void BrokerInterface::unregisterModule(string moduleName)
{
#ifdef INSECURE
	DataBlockWriter oWriter; oWriter << moduleName;
	m_oRPC->call(m_nId,0,UnregisterModule,oWriter);
#else
	throw new SBWApplicationException("insecure execution denied", "changing the remote registry is not allowed");
#endif
}
