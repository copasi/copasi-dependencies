#include "SBW/SBWOSSocket.h"
#include "SBW/DataBlockWriter.h"
#include "SBW/ServiceImpl.h"
#include "SBW/ModuleImpl.h"
#include "SBW/MethodImpl.h"
#include "SBW/Handler.h"
#include "SBW/Module.h"
#include "SBW/MethodTable.h"
#include "SBW/SBWException.h"
#include "SBW/SBWApplicationException.h"
#include "SBW/SBWMethodNotFoundException.h"

#include "ObjectOrientedReceiver.h"
#include "BrokerDefines.h"
#include "Sys.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <exception>

using namespace std;
using namespace SystemsBiologyWorkbench;
using namespace SystemsBiologyWorkbench::Broker;


ObjectOrientedReceiver::ObjectOrientedReceiver(std::string moduleName)
{
	this->moduleName = moduleName;
	this->moduleId = -1;
}
void ObjectOrientedReceiver::addService(std::string name, std::string displayName,
										std::string category, CBroker  *impl, std::string help)
{
	ServiceImpl* oImpl = new ServiceImpl(name, displayName, category, help);
	std::vector<std::string> oMethods = getBrokerMethods();
	for (unsigned int i = 0; i < oMethods.size(); i ++ )
		oImpl->addMethod(NULL,oMethods[i],false,m_oHelp[i]);
	services.push_back(oImpl);
	numServices = (int)services.size();
	m_oBroker = impl;
}
DataBlockWriter ObjectOrientedReceiver::receive(Integer fromModuleId, Integer serviceId,
												Integer methodId, DataBlockReader args)
{
	if (serviceId == SYSTEM_SERVICE)
	{
		DataBlockWriter writer;
		try
		{
			// See SBWLowLevel.java for the list of system methods
			// that must be handled.
			switch (methodId)
			{
			case GET_SERVICES_METHOD:
				{
					writer << getServices();
					return writer;
				}
			case GET_METHODS_METHOD:
				{
					int queryServiceId;
					args >> queryServiceId;
					ServiceImpl *srvImpl = getService(queryServiceId);
					writer << srvImpl ->getSignatureStrings();
					return writer;
				}

			case GET_METHOD_HELP_METHOD: 
				{
					int targetServiceId,  targetMethodId; 
					args >> targetServiceId >> targetMethodId;
					ServiceImpl *srvImpl = getService(targetServiceId);
					writer << srvImpl->getMethod(targetMethodId)->getHelp();
					return writer;
				}

			case SHUTDOWN_METHOD:
			case ON_OTHER_MODULE_INSTANCE_SHUTDOWN_METHOD:
			case ON_OTHER_MODULE_INSTANCE_STARTUP_METHOD:
			case ON_REGISTRATION_CHANGE_METHOD:
				writer.add(0);
				return writer;

			default:
				{
					std::stringstream sTemp;
					sTemp << "Method "  <<  methodId << " does not exist on service " <<  serviceId;
					throw new SBWMethodNotFoundException(sTemp.str() );
				}
			}
		}
		catch (...)
		{
			throw new SBWApplicationException("receive failed ...");
		}
	}
	else
	{
		// Since ConfigVars is 'final', the following should get optimized
		// out completely when not compiled in debug mode.
		ServiceImpl *srvImpl = getService(serviceId);
#ifdef _DEBUG

		if (srvImpl == NULL)
		{
			TRACE("Module " << fromModuleId << " invoked" <<  " nonexistent service " <<  serviceId);
		}
		else
		{
			std::string name = srvImpl->getName();
			if (name != "BROKER") 
			{
				std::vector<std::string> sigs = srvImpl->getSignatureStrings();
				if (methodId < 0 || methodId >= (int)sigs.size())
				{
					TRACE("Module " << fromModuleId  << " passed in" <<  " out-of-range method id " << methodId);
				}
				else
				{
					TRACE("Module " <<  fromModuleId << " invoking method " <<  methodId <<  " ('"\
						<< sigs[methodId] << "') on service " << serviceId << " ('" << name << "') of module "\
						<< moduleId << " (name '" << moduleName << "')");
				}
			}
		}
#endif
		if (srvImpl != NULL)
		{
			try
			{	
				if ( srvImpl->getName() == "BROKER")
				{
					TRACE("Module " <<  fromModuleId << " invoking method " <<  methodId <<  " ('"\
						<< getBrokerMethods()[methodId] << "') on service " << serviceId << " ('" << srvImpl->getName() << "') of module "\
						<< moduleId << " (name '" << moduleName << "')");

					DataBlockWriter oWriter;
					switch (methodId)
					{
					case GetVersion:
						oWriter << m_oBroker->getVersion();
						break;
					case GetModuleInstance: 
						{
							std::string sName;
							args >> sName;					
							oWriter << m_oBroker->getModuleInstance(sName);
							break;
						}
					case GetModuleDescriptors:
						{						
							bool bTemp1,bTemp2;
							args >> bTemp1 >> bTemp2;
							oWriter << m_oBroker->getModuleDescriptors(bTemp1,bTemp2);
							break;
						}
					case GetModuleDescriptorString:
						{
							std::string sTemp; 
							bool bTemp;
							args >> sTemp >> bTemp;
							oWriter << m_oBroker->getModuleDescriptor(sTemp,bTemp);
							break;
						}
					case GetModuleDescriptorInt:
						{
							int nTemp;
							args >> nTemp;						
							oWriter << m_oBroker->getModuleDescriptor(nTemp);
							break;
						}
					case FindServices:
						{
							std::string sTemp; 
							bool bTemp;
							args >> sTemp >> bTemp;
							oWriter << m_oBroker->findServices(sTemp,bTemp);
							break;
						}
					case GetServiceCategories:
						{
							std::string sName;
							args >> sName;					
							oWriter << m_oBroker->getServiceCategories(sName);
							break;
						}
					case LinkBroker:
						{
							std::string sName;
							args >> sName;					
							if (!Sys::isSecure() || !m_oBroker->isRemoteInstance(fromModuleId))
								oWriter << m_oBroker->linkBroker(sName);
							else throw new SBWApplicationException("insecure execution denied", "linking the remote broker to another computer is not allowed.");
							break;
						}
					case GetExistingModuleInstanceIds:
						{
							oWriter << m_oBroker->getExistingModuleInstanceIds();
							break;
						}
					case GetServiceDescriptorString:
						{
							int nTemp;
							std::string sTemp;
							args >> nTemp >> sTemp;					
							oWriter << m_oBroker->getServiceDescriptor(nTemp, sTemp);
							break;
						}
					case GetServiceDescriptorInt:
						{
							int nTemp1, nTemp2;
							args >> nTemp1 >> nTemp2;					
							oWriter << m_oBroker->getServiceDescriptor(nTemp1, nTemp2);
							break;
						}
					case GetServiceDescriptorsString:
						{
							std::string sTemp;
							args >> sTemp;					
							oWriter << m_oBroker->getServiceDescriptors(sTemp);
							break;
						}
					case GetServiceDescriptorsInt:
						{
							int nTemp;
							args >> nTemp;					
							oWriter << m_oBroker->getServiceDescriptors(nTemp);
							break;
						}
					case RegisterModule:
						{
							std::string s1,s2,s3,s4;
							int n1;
							args >> s1 >> s2 >> n1 >> s3 >> s4;					
							if (!Sys::isSecure() || !m_oBroker->isRemoteInstance(fromModuleId))
								m_oBroker->registerModule(s1,s2,n1,s3,s4);
							else throw new SBWApplicationException("insecure execution denied", "changing the remote registry is not allowed");
							break;
						}
					case ChangeModuleName:
						{
							int nTemp;
							std::string sTemp;
							args >> nTemp >> sTemp;					
							if (!Sys::isSecure() || !m_oBroker->isRemoteInstance(fromModuleId))
								m_oBroker->changeModuleName(nTemp, sTemp);
							else throw new SBWApplicationException("insecure execution denied", "changing the remote registry is not allowed");
							break;
						}
					case RegisterService:
						{
							std::string s1,s2,s3,s4,s5;
							args >> s1 >> s2 >> s3 >> s4 >> s5;					
							if (!Sys::isSecure() || !m_oBroker->isRemoteInstance(fromModuleId))
								m_oBroker->registerService(s1,s2,s3,s4,s5);
							else throw new SBWApplicationException("insecure execution denied", "changing the remote registry is not allowed");
							break;
						}
					case UnregisterModule:
						{
							std::string s1;
							args >> s1;					
							if (!Sys::isSecure() || !m_oBroker->isRemoteInstance(fromModuleId))
								m_oBroker->unregisterModule(s1);
							else throw new SBWApplicationException("insecure execution denied", "changing the remote registry is not allowed");
							break;
						}
					case ShutdownBroker:
						if (!Sys::isSecure() || !m_oBroker->isRemoteInstance(fromModuleId))
							m_oBroker->shutdownBroker();
						else 
							throw new SBWApplicationException("insecure execution denied", "shutdown of remote broker is not supported.");
						break;
					case GetListOfModules:
						{
							oWriter << m_oBroker->getListOfModules();
							break;
						}
					case GetMethodIdsInt:
						{
							int n1,n2;
							args >> n1 >> n2;
							oWriter << m_oBroker->getMethodIds(n1,n2);
							break;
						}
					case GetMethodIdsString:
						{
							string s1,s2;
							args >> s1 >> s2;
							oWriter << m_oBroker->getMethodIds(s1,s2);
							break;
						}
					case ReadFromFile:
						{

							if (!Sys::isSecure() || !m_oBroker->isRemoteInstance(fromModuleId))
							{
								string sFileName;

								string sReturn, buffer;							
								args >> sFileName;
								ifstream oFile(sFileName.c_str());
								if (!oFile.is_open())
									throw new SBWApplicationException("error reading file", "an error occured while trying to read: '" + sFileName + "'");
								try
								{
									while (!oFile.eof()) 
									{
										getline(oFile, buffer);
										sReturn+=buffer;
									}
									oWriter << sReturn;
									oFile.close();
								} 
								catch (...)
								{
									throw new SBWApplicationException("error reading file", "an error occured while trying to read: '" + sFileName + "'");
								}

							}
							else throw new SBWApplicationException("insecure execution denied", "reading a file on a remote machine is not allowed");
							break;
						}
					case FindLocalServices:
						{
							std::string sTemp; 
							bool bTemp;
							args >> sTemp >> bTemp;
							oWriter << m_oBroker->findLocalServices(sTemp,bTemp);
							break;
						}

					default:
						TRACE("the method is not yet supported: " << methodId);
						break;
					}
					return  oWriter;
				} 
				else
				{
					MethodImpl *oMethod = srvImpl->getMethod(methodId);
					if (oMethod != NULL)
						return oMethod->invoke(Module(fromModuleId), args);
				}
			}
			catch (SBWException *e) 
			{	
				throw e;
			}
			catch (std::exception e) 
			{
				throw new SBWApplicationException(e.what());
			}			
			catch (...) 
			{
				throw new SBWApplicationException("error calling method.");
			}
		}
		return DataBlockWriter();
	}
}
void ObjectOrientedReceiver::setModuleId(int moduleId)
{
	this -> moduleId = moduleId;
}
std::vector<std::string> ObjectOrientedReceiver::getServices()
{
	std::vector<std::string> sResult;

	for (unsigned int i = 0; i < services.size(); i++)
	{
		ServiceImpl *service = services[i];

		if (service == NULL)
			sResult.push_back("");
		else
			sResult.push_back(service->getName());
	}

	return sResult;
}
ServiceImpl *ObjectOrientedReceiver::getService(int serviceId)
{
	if (serviceId < 0 || serviceId >= numServices) {
		std::stringstream sTemp;
		sTemp << "Unknown service id " << serviceId;
		throw new SBWApplicationException(sTemp.str());
	}

	ServiceImpl *service = services[serviceId];

	if (service == NULL)
	{
		std::stringstream sTemp;
		sTemp << "Service id " << serviceId << " no longer supported"
			<< "The service identifier " << serviceId << " is not known to this"
			<<	 " module.  Either it is invalid or the module no longer"
			<<  "offers that service.";
		throw new SBWApplicationException(sTemp.str());
	}
	else
		return service;
}
std::vector<std::string> ObjectOrientedReceiver::getBrokerMethods() 
{
	if (m_oMethods.size() == 0)
	{
		m_oMethods.push_back("string getVersion()");
		m_oHelp.push_back("returns the version of the broker");
		m_oMethods.push_back("int getModuleInstance(string)");
		m_oHelp.push_back("returns the module identifier of the given name, or starts up a new module");
		m_oMethods.push_back("{}[] getModuleDescriptors(boolean, boolean)");
		m_oHelp.push_back("returns an array of module descriptors. The flags are localOnly (search only for local modules) and includeRunning (search also running modules)");
		m_oMethods.push_back("{} getModuleDescriptor(string, boolean)");
		m_oHelp.push_back("returns the module descriptor for the module with the given name, if the flag is true running modules will also be included in the search, otherwise only registered modules");
		m_oMethods.push_back("{} getModuleDescriptor(int)");
		m_oHelp.push_back("returns the module descriptor for the module with the given id");
		m_oMethods.push_back("{}[] findServices(string, boolean)");
		m_oHelp.push_back("returns an array of all services registered with a given category, the flag indicates whether services of subcategories should also be returned");
		m_oMethods.push_back("string[] getServiceCategories(string)");
		m_oHelp.push_back("returns an array of subcategories of the given string. e.g.: '/' represents the root category and returns all subcategories.");
		m_oMethods.push_back("int linkBroker(string)");
		m_oHelp.push_back("links this broker to a remote broker ... the parameter is the hostname or hostaddress of the remote computer. A connection will be made to the same port as the local broker. A different port can be used by using <hostname or ip>/<port>");
		m_oMethods.push_back("int[] getExistingModuleInstanceIds()");
		m_oHelp.push_back("returns an array of currently running module id's");
		m_oMethods.push_back("{} getServiceDescriptor(int, string)");
		m_oHelp.push_back("returns the service descriptor with the given module id and service name");
		m_oMethods.push_back("{} getServiceDescriptor(int, int)");
		m_oHelp.push_back("returns the service descriptor with the given module id and service id");
		m_oMethods.push_back("{}[] getServiceDescriptors(string)");
		m_oHelp.push_back("returns the service descriptors of the module with the given name");
		m_oMethods.push_back("{}[] getServiceDescriptors(int)");
		m_oHelp.push_back("returns the service descriptors of the module with the given identifier");
		m_oMethods.push_back("void registerModule(string, string, int, string, string)");
		m_oHelp.push_back("registers a module with: moduleName, moduleDisplayName, moduleType, commandLine, help");
		m_oMethods.push_back("void changeModuleName(int, string)");
		m_oHelp.push_back("changes the module-name of the module with the given id to the new given name");
		m_oMethods.push_back("void registerService(string, string, string, string, string)");
		m_oHelp.push_back("registers the service with: moduleName, serviceName, serviceDisplayName, category,help");
		m_oMethods.push_back("void unregisterModule(string)");
		m_oHelp.push_back("unregisters the module with the given name");
		m_oMethods.push_back("void shutdownBroker()");
		m_oHelp.push_back("shuts down the broker");
		m_oMethods.push_back("{} getListOfModules()");
		m_oHelp.push_back("returns the ids and names of all currently running modules");
		m_oMethods.push_back("{} getMethodIds(int,int)");
		m_oHelp.push_back("returns all method id's and names for the given module id and service id");
		m_oMethods.push_back("{} getMethodIds(string,string)");
		m_oHelp.push_back("returns all method id's and names for the given module name and service name");
		m_oMethods.push_back("string readFromFile(string)");
		m_oHelp.push_back("reads a file from the file system and returns a string containing the results. (file must be with absolute path)");
		m_oMethods.push_back("{}[] findLocalServices(string, boolean)");
		m_oHelp.push_back("returns an array of all local services registered with a given category, the flag indicates whether services of subcategories should also be returned");
	}
	return m_oMethods;
}
std::vector<std::string> ObjectOrientedReceiver::m_oMethods;
std::vector<std::string> ObjectOrientedReceiver::m_oHelp;
