/**
 * @file SBWC.cpp
 * @brief exception throw by service implementations
 * 
 * This file is part of SBW.  Please visit http://sbw.sf.org for more
 * information about SBW, and the latest version of libSBW.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the New BSD license.
 *
 * Copyright (c) 2010-2014, Frank T. Bergmann and 
 *                          University of Washington
 * Copyright (c) 2008-2010, University of Washington and 
 *                          Keck Graduate Institute.
 * Copyright (c) 2005-2008, Keck Graduate Institute.
 * Copyright (c) 2001-2004, California Institute of Technology and
 *               Japan Science and Technology Corporation.
 * 
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are 
 * met: 
 * 
 * 1. Redistributions of source code must retain the above 
 *    copyright notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 
 * 3. Neither the name of the copyright holder nor the names of its 
 *    contributors may be used to endorse or promote products derived from 
 *    this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
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
 *
 * Contributor(s):
 *
 */



#include "stdafx.h"
#include "SBWC.h"
//#ifndef WIN32 
#include <stdarg.h>
//#endif

#include "sbwbasictypes.h"
#include "SBWLowLevel.h"
#include "Signature.h"
#include "ModuleImpl.h"
#include "SBWApplicationException.h"
#include "HandlerForC.h"
#include "SignatureElement.h"
#include "SignatureType.h"
#include "RPCInCall.h"
#include <SBW/portableOS.h>

#include <string>
#include <iostream> 
using namespace SystemsBiologyWorkbench ;


#define SBW_GET_RESULTS			-23


/**
 * class that converts between the C datablock types and the C++ datablock types
 */
namespace SystemsBiologyWorkbench {
class DataBlockConverter
{
public :
	/**
	 * returns a DataBlockWriter given a SBWDataBlockWriter *
	 * @param x C data block pointer
	 * @return C++ data block handle object 
	 */
	static DataBlockWriter convert(SBWDataBlockWriter *x)
	{
		return (SystemsBiologyWorkbench::sbwDataBlockWriter *)x;
	}

	/**
	 * returns a DataBlockReader given a SBWDataBlockReader *
	 * @param x C data block pointer
	 * @return C++ data block handle object 
	 */
	static DataBlockReader convert(SBWDataBlockReader *x)
	{
		return (SystemsBiologyWorkbench::sbwDataBlockReader *)x;
	}
};
}
/**
 * transfers the contents of a ModuleDescriptor into a SBWModuleDescriptor.
 * @param descriptor set to the contents of handle.
 * @param handle source of data.
 */
void LoadSBWModuleDescriptor(SBWModuleDescriptor *descriptor, ModuleDescriptor handle)
{
	descriptor->name = SBWLowLevel::resultStringForC(handle.getName().c_str());
	descriptor->displayName = SBWLowLevel::resultStringForC(handle.getDisplayName().c_str());
	descriptor->managementType = (SBWModuleManagementType)handle.getManagementType();
	descriptor->commandLine = SBWLowLevel::resultStringForC(handle.getCommandLine().c_str());
	descriptor->help = SBWLowLevel::resultStringForC(handle.getHelp().c_str());
}

/**
 * transfers the contents of a ServiceDescriptor into a SBWServiceDescriptor.
 * @param descriptor set to the contents of handle.
 * @param handle source of data.
 */
void LoadSBWServiceDescriptor(SBWServiceDescriptor *descriptor, ServiceDescriptor handle)
{
	descriptor->serviceDisplayName = SBWLowLevel::resultStringForC(handle.getDisplayName().c_str());
	descriptor->serviceName = SBWLowLevel::resultStringForC(handle.getName().c_str());
	descriptor->serviceCategory = SBWLowLevel::resultStringForC(handle.getCategory().c_str());
	descriptor->help = SBWLowLevel::resultStringForC(handle.getHelp().c_str());
	LoadSBWModuleDescriptor(&(descriptor->module), handle->getModuleDescriptor());
}
/**
 * method handler for the C API
 */
class HandlerForStdCall : public Handler  
{
public:
	/**
	 * creates a method handler given a C function and application data.
	 * @param f C function that implements an SBW method.
	 * @param ud application data associated with the SBW method.
	 */
	HandlerForStdCall(SBWCHandler f, int nService, int nMethod) : function(f) { m_nService = nService; m_nMethod = nMethod;} 

	/**
	 * returns the C function which implements this SBW method.
	 */
	SBWCHandler getFunction() { return function ;}

	DataBlockWriter receive(Module from, DataBlockReader reader);

private:
	/// C function implementing this SBW Method
	SBWCHandler function ;
	/// the service to be called
	int m_nService;   
	/// the method to be called
	int m_nMethod;
};
/**
 * executes this SBW method.
 * calls the C function to implement this SBW method.
 * @param from the numeric identifier for the module that's calling this SBW method.
 * @param reader data block contain the argument data for this call.
 */
DataBlockWriter HandlerForStdCall::receive(Module from, DataBlockReader args)
{
	SBWException::clearStoredException();	
		
	// due to the use of namespaces the C and C++ types don't match give errors
	// hence the casting
	int nArgumentLength = 0;
	unsigned char *sTemp = args.getRemainingDataForC(nArgumentLength);
	unsigned char *sArgument = (unsigned char *)malloc(sizeof(unsigned char)*nArgumentLength);
	memcpy(sArgument, sTemp, nArgumentLength);
	
	int nResultLength = 0;
	unsigned char* sResult = NULL;
	function(
		(int)from.getModuleInstanceId(), sArgument, nArgumentLength , sResult, &nResultLength, m_nService, m_nMethod);
	
	sResult = (unsigned char*)malloc(sizeof(unsigned char) * nResultLength);
	function(
		SBW_GET_RESULTS, sArgument, nArgumentLength , sResult, &nResultLength, m_nService, m_nMethod);

	DataBlockWriter result;
	result.insertData(sResult, nResultLength);
	if (SBWException::getStoredException() != NULL)
			SBWException::throwStoredException();	

	try
	{
		free(sArgument);
		free(sResult);
		free(sTemp);

	}
	catch(...)
	{
	}
	return result ;
}

/**
 * method handler for the C API
 */
class HandlerForC : public Handler  
{
public:
	/**
	 * creates a method handler given a C function and application data.
	 * @param f C function that implements an SBW method.
	 * @param ud application data associated with the SBW method.
	 */
	HandlerForC(SBWHandler f, void *ud) : function(f), userData(ud) {} 

	/**
	 * returns the application data associated with this SBW method.
	 * @return returns the application data associated with this SBW method.
	 */
	void *getUserData() { return userData ;}

	/**
	 * returns the C function which implements this SBW method.
	 */
	SBWHandler getFunction() { return function ;}

	DataBlockWriter receive(Module from, DataBlockReader reader);

private:
	/// C function implementing this SBW Method
	SBWHandler function ;

	/// application data that's passed to the C function
	void *userData ;
};

/**
 * executes this SBW method.
 * calls the C function to implement this SBW method.
 * @param from the numeric identifier for the module that's calling this SBW method.
 * @param reader data block contain the argument data for this call.
 */
DataBlockWriter HandlerForC::receive(Module from, DataBlockReader args)
{
	SBWException::clearStoredException();
	
	DataBlockWriter result; 
		
	// due to the use of namespaces the C and C++ types don't match give errors
	// hence the casting
	function(
		from.getModuleInstanceId(), (SBWDataBlockReader *)args.getImpl(), (SBWDataBlockWriter *)result.getImpl(), userData);

	if (SBWException::getStoredException() != NULL)
			SBWException::throwStoredException();	

	return result ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C API functions

/**
 * the ModuleImpl object used by the C API 
 */
static ModuleImpl *moduleImpl = NULL ;

/// @deprecated use SBWFree instead
 void SBWFreeArray(void *x)
{
	free(x);
}

/// @deprecated use SBWFree instead
 void SBWFreeObject(void *x)
{
	free(x);
}

/**
 * deallocate object created by the C API.
 * @param x object created by C API that is to be deallocated.
 */
 void SBWFree(void *x)
{
	free(x);
}

/**
 * deallocate 2 dimensional array allocated by the C API.
 * @param xSize size of the first dimension of the array.
 * @param array array created by C API that is to be deallocated.
 */
 void SBWFree2DArray(SBWInteger xSize, void **array)
{
	while (xSize != 0)
	{
		xSize-- ;
		free(array[xSize]) ;
	}

	free(array) ;
}

/**
 * deallocate array of module descriptors.
 * @param size size of the first dimension of the array of module descriptors.
 * @param x array of module descriptors created by C API that is to be deallocated.
 */
 void SBWFreeModuleDescriptorArray(int size, SBWModuleDescriptor *x)
{
	while (size != 0)
	{
		size-- ;
		delete[] x[size].commandLine ;
		delete[] x[size].displayName ;
		delete[] x[size].name ;
	}

	delete[] x ;
}

/**
 * deallocate service descriptor structure.
 * @param descriptor service descriptor to be deallocated.
 */
 void SBWFreeServiceDescriptor(SBWServiceDescriptor *descriptor)
{
	delete[] descriptor->serviceName;
	delete[] descriptor->serviceDisplayName ;
	delete[] descriptor->serviceCategory ;
	delete[] descriptor->module.name ;
	delete[] descriptor->module.displayName ;
	delete[] descriptor->module.commandLine ;
	delete descriptor ;
	descriptor = NULL;
}

/**
 * deallocate array of service descriptors.
 * @param size the number of service descriptors in the array of service descriptors.
 * @param descriptor array of service descriptors created by C API that is to be deallocated.
 */
 void SBWFreeServiceDescriptorArray(int size, SBWServiceDescriptor *descriptor)
{
	while (size != 0)
	{
		size-- ;

		delete[] descriptor[size].serviceName;
		delete[] descriptor[size].serviceDisplayName ;
		delete[] descriptor[size].serviceCategory ;
		delete[] descriptor[size].module.name ;
		delete[] descriptor[size].module.displayName ;
		delete[] descriptor[size].module.commandLine ;
	}

	delete[] descriptor;
}

/**
 * deallocate given SBWDataBlockWriter.
 * @param x SBWDataBlockWriter to be deallocated. 
 */
 void SBWFreeSBWDataBlockWriter(SBWDataBlockWriter *x)
{
	((SystemsBiologyWorkbench::sbwDataBlockWriter *)x)->Release();
}

/**
 * dellocate given SBWDataBlockReader.
 * @param x SBWDataBlockReader to be deallocated.
 */
 void SBWFreeSBWDataBlockReader(SBWDataBlockReader *x)
{
	((SystemsBiologyWorkbench::sbwDataBlockReader *)x)->Release();
}

/**
 * dellocate given SBWSignature.
 * @param x SBWSignature to be deallocated.
 */
 void SBWFreeSBWSignature(SBWSignature *x)
{
	((SystemsBiologyWorkbench::sbwSignature *)x)->Release();
}

/**
 * dellocate given SBWSignatureType.
 * @param x SBWSignatureType to be deallocated.
 */
 void SBWFreeSBWSignatureType(SBWSignatureType *x)
{
	((SystemsBiologyWorkbench::sbwSignatureType *)x)->Release();
}

/**
 * deallocate array of signature arguments.
 * @param size the number of signature arguments in the array.
 * @param descriptor array of signature arguments created by C API that is to be deallocated.
 */
 void SBWFreeSBWSignatureElementArray(SBWInteger size, SBWSignatureElement **array)
{
	SystemsBiologyWorkbench::sbwSignatureElement **a = (SystemsBiologyWorkbench::sbwSignatureElement **)array;

	while (size != 0)
	{
		size-- ;
		a[size]->Release();
	}

	delete[] a ;
}

/**
 * returns the user message associated with the pending exception.
 * The result should be deallocated using SBWFree.
 * @return the user message associated with the pending exception.
 *         NULL if there is no pending exception.
 */
 char *SBWExceptionGetMessage()
{
	SBWException *e = SBWException::getStoredException();

	if (e == NULL)
		return NULL ;
	else
		return SBWLowLevel::resultStringForC(e->getMessage().c_str());
}

/**
 * returns the detailed message associated with the pending exception.
 * The result should be deallocated using SBWFree.
 * @return the detailed message associated with the pending exception.
 *         NULL if there is no pending exception.
 */
 char *SBWExceptionGetDetailedMessage()
{
	SBWException *e = SBWException::getStoredException();

	if (e == NULL)
		return NULL ;
	else
		return SBWLowLevel::resultStringForC(e->getDetailedMessage().c_str());
}

/**
 * returns the exception code associated the pending exception.
 * @return the exception code associated the pending exception.
 *         -1 if there is no pending exception.
 */
 int SBWExceptionGetCode()
{
	SBWException *e = SBWException::getStoredException();

	if (e == NULL)
		return -1 ;
	else
		return e->getCode();
}

/**
 * creates an new application exception and stores it as the pending exception.
 * @param userMessage user message associated with the exception.
 * @param detailedMessage detailed message associated with the exception.
 */
 void SBWSetException(const char *userMessage, const char *detailedMessage)
{
	SBWException *e = new SBWApplicationException(userMessage, detailedMessage);

	e->storeException();
}

/** 
 * clears ALL exceptions on ALL threads. Really a kludge to fix the MATLAB-SBW 
 * "sticky errors" problem. 
 */ 
 void SBWExceptionClear() 
{ 
    SBWException::clearAllExceptions(); 
} 
/**
 * connects this application anonymously with the broker.
 * @return
 * - true the connection was made
 * - false the connection was not made because an error occurred and there is a pending exception
 */
 SBWBoolean SBWConnect()
{
	try
	{
		SBWLowLevel::connect();
	}
	catch (SBWException *e)
	{
		e->storeException();

		return false ;
	}

	return true ;
}

/**
 * request that the broker connects to a remote broker.  This ensures
 * that the SBW environment appears to be a combination of the facilities
 * of all the linked brokers.  This is the prefered mechanism for accessing
 * remote resources.
 * @param hostname the hostname of machine on which you wish to connect.
 * @returns true if succesful false otherwise.
 */
 SBWBoolean SBWLink(const char *hostname)
{
	try
	{
		SBWLowLevel::link(hostname);
	}
	catch (SBWException *e)
	{
		e->storeException();

		return false ;
	}

	return true ;
}

/**
 * connects this application anonymously with the broker.
 * @param hostname hostname of the machine you wish to connect to.
 * @return
 * - true the connection was made
 * - false the connection was not made because an error occurred and there is a pending exception
 */
 SBWBoolean SBWConnectOnHost(const char *hostname)
{
	try
	{
		SBWLowLevel::connect("", hostname);
	}
	catch (SBWException *e)
	{
		e->storeException();

		return false ;
	}

	return true ;
}

/// block until this application is disconnected from the broker
 void SBWWaitForDisconnect()
{
	SBWLowLevel::waitForDisconnect();
}

/**
 * returns the services that are in a given service category.
 * Deallocate the result using SBWFreeSBWServiceDescriptorArray.
 * @param serviceCategory the category that contains all the returned services.
 * @param numberOfServices referenced integer is set to the size of the returned array.
 * @param recursive include services that are in subcategories of this service.
 * @param oReturn the services that are in the given service category.
 */
 void SBWFindServicesA(
	const char *serviceCategory, SBWInteger *numberOfServices, SBWBoolean recursive, SBWServiceDescriptor* oReturn)
{
	oReturn = SBWFindServices(serviceCategory, numberOfServices, recursive);
}

/**
 * returns the services that are in a given service category.
 * Deallocate the result using SBWFreeSBWServiceDescriptorArray.
 * @param serviceCategory the category that contains all the returned services.
 * @param numberOfServices referenced integer is set to the size of the returned array.
 * @param recursive include services that are in subcategories of this service.
 * @return the services that are in the given service category.
 */
 SBWServiceDescriptor *SBWFindServices(
	const char *serviceCategory, SBWInteger *numberOfServices, SBWBoolean recursive)
{
	std::vector<ServiceDescriptor> *descriptors;
	
	try
	{
		descriptors = SBWLowLevel::findServices(serviceCategory, recursive != 0);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	SBWInteger i = *numberOfServices = descriptors->size();
	SBWServiceDescriptor *result = new SBWServiceDescriptor[i];

	while (i != 0)
	{
		i-- ;
		LoadSBWServiceDescriptor(&(result[i]), (*descriptors)[i]);
	}

	delete descriptors ;
	descriptors = NULL;

	return result ;
}
/**
 * returns the services that are in a given service category.
 * Deallocate the result using SBWFreeSBWServiceDescriptorArray.
 * @param serviceCategory the category that contains all the returned services.
 * @param numberOfServices referenced integer is set to the size of the returned array.
 * @param recursive include services that are in subcategories of this service.
 * @return the services that are in the given service category.
 */
 SBWServiceDescriptor *SBWFindLocalServices(
	const char *serviceCategory, SBWInteger *numberOfServices, SBWBoolean recursive)
{
	std::vector<ServiceDescriptor> *descriptors;
	
	try
	{
		descriptors = SBWLowLevel::findLocalServices(serviceCategory, recursive != 0);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	SBWInteger i = *numberOfServices = descriptors->size();
	SBWServiceDescriptor *result = new SBWServiceDescriptor[i];

	while (i != 0)
	{
		i-- ;
		LoadSBWServiceDescriptor(&(result[i]), (*descriptors)[i]);
	}

	delete descriptors ;
	descriptors = NULL;

	return result ;
}

/**
 * add a callback function which will be called when this application disconnects from the broker.
 * @param ml a callback function which will be called when this application disconnects from the broker.
 */
 void SBWRegisterShutdownListener(SBWSimpleListener ml)
{
	SBWLowLevel::registerShutdownListener((SimpleListener) ml);
}
/**
 * add a callback function which will be called when the broker's registry changes.
 * @param sl a callback function which will be be called when the broker's registry changes.
 */
 void SBWRegisterRegistrationChangeListener(SBWSimpleListener sl)
{
	SBWLowLevel::registerRegistrationChangeListener((SimpleListener) sl);
}
/**
 * add a callback function which will be called whenever another module connects to the broker.
 * @param ml a callback function which will be called whenever another module connects to the broker.
 */
 void SBWRegisterModuleStartupListener(SBWModuleListener ml)
{
	SBWLowLevel::registerModuleStartupListener(ml);
}
/**
 * remove the callback function which was passed to SBWRegisterModuleShutdownListener
 * @param ml the callback function to be removed
 */
 void SBWRemoveModuleShutdownListener(SBWModuleListener ml)
{
	SBWLowLevel::removeModuleShutdownListener(ml);
}
/**
 * remove the callback function which was passed to SBWRegisterModuleStartupListener
 * @param ml the callback function to be removed
 */
 void SBWRemoveModuleStartupListener(SBWModuleListener ml)
{
	SBWLowLevel::removeModuleStartupListener(ml);
}
/**
 * remove the callback function which was passed to SBWRegisterShutdownListener
 * @param ml the callback function to be removed
 */
 void SBWRemoveShutdownListener(SBWSimpleListener ml)
{
	SBWLowLevel::removeShutdownListener((SimpleListener) ml);
}
/**
 * remove the callback function which was passed to SBWRegisterRegistrationChangeListener
 * @param ml the callback function to be removed
 */
 void SBWRemoveRegistrationChangeListener(SBWSimpleListener sl)
{
	SBWLowLevel::removeRegistrationChangeListener((SimpleListener) sl);
}
/**
 * add a callback function which will be called whenever another module disconnects from the broker.
 * @param ml a callback function which will be called whenever another module disconnects from the broker.
 */
 void SBWRegisterModuleShutdownListener(SBWModuleListener ml)
{
	SBWLowLevel::registerModuleShutdownListener(ml);
}
/**
 * add a callback function which will be called when this application disconnects from the broker.
 * @param ml a callback function which will be called when this application disconnects from the broker.
 */
 void SBWRegisterShutdownListenerA(SBWSimpleListenerStdCall ml)
{
	SBWLowLevel::registerShutdownListenerStdCall((SimpleListenerStdCall) ml);
}
/**
 * add a callback function which will be called whenever another module disconnects from the broker.
 * @param ml a callback function which will be called whenever another module disconnects from the broker.
 */
 void SBWRegisterModuleShutdownListenerA(SBWModuleListenerStdCall ml)
{
	SBWLowLevel::registerModuleShutdownListenerStdCall(ml);
}

/**
 * add a callback function which will be called whenever another module connects to the broker.
 * @param ml a callback function which will be called whenever another module connects to the broker.
 */
 void SBWRegisterModuleStartupListenerA(SBWModuleListenerStdCall ml)
{
	SBWLowLevel::registerModuleStartupListenerStdCall(ml);
}

/**
 * add a callback function which will be called when the broker's registry changes.
 * @param sl a callback function which will be be called when the broker's registry changes.
 */
 void SBWRegisterRegistrationChangeListenerA(SBWSimpleListenerStdCall sl)
{
	SBWLowLevel::registerRegistrationChangeListenerStdCall((SimpleListenerStdCall) sl);
}

/**
 * remove the callback function which was passed to SBWRegisterShutdownListener
 * @param ml the callback function to be removed
 */
 void SBWRemoveShutdownListenerA(SBWSimpleListenerStdCall ml)
{
	SBWLowLevel::removeShutdownListenerStdCall((SimpleListenerStdCall) ml);
}

/**
 * remove the callback function which was passed to SBWRegisterModuleShutdownListener
 * @param ml the callback function to be removed
 */
 void SBWRemoveModuleShutdownListenerA(SBWModuleListenerStdCall ml)
{
	SBWLowLevel::removeModuleShutdownListenerStdCall(ml);
}

/**
 * remove the callback function which was passed to SBWRegisterModuleStartupListener
 * @param ml the callback function to be removed
 */
 void SBWRemoveModuleStartupListenerA(SBWModuleListenerStdCall ml)
{
	SBWLowLevel::removeModuleStartupListenerStdCall(ml);
}

/**
 * remove the callback function which was passed to SBWRegisterRegistrationChangeListener
 * @param ml the callback function to be removed
 */
 void SBWRemoveRegistrationChangeListenerA(SBWSimpleListenerStdCall sl)
{
	SBWLowLevel::removeRegistrationChangeListenerStdCall((SimpleListenerStdCall) sl);
}

/**
 * returns the numeric identifier of this module.
 * @return the numeric identifier of this module.
 */
 SBWInteger SBWGetThisModule()
{
	try
	{
		return SBWLowLevel::getThisModule();
	}
	catch (SBWException *e)
	{
		e->storeException();
		return -1;
	}
}

// TODO shutdown module if the service isn't on the module
/**
 * locates a module instance and a service in that module that correspond to a given service descriptor.
 * May create a new module instance depending on the module management type.
 * @param service defines which module and service to locate
 * @param moduleIdentifier set to the numeric module identifier of the located module instance
 * @param serviceIdentifier set to the numeric service identifier of the located service
 * @return 
 * - true if the module instance and service are located successfully
 * - false unable to locate either the module or service. exception pending.
 */
 SBWBoolean SBWGetServiceInModuleInstance(
	SBWServiceDescriptor *service, SBWInteger *moduleIdentifier, SBWInteger *serviceIdentifier)
{
	*moduleIdentifier = SBWGetModuleInstance(service->module.name);

	if (*moduleIdentifier == -1)
		return false ;

	*serviceIdentifier = SBWModuleFindServiceByName(*moduleIdentifier, service->serviceName);

	if (*serviceIdentifier == -1)
		return false ;

	return true ;
}

/**
 * returns either the top level service categories or the subcategories within a given service category.
 * The result should be deallocated using SBWFree2DArray. 
 * @param serviceCategory the enclosing service category.  
 *                        If an empty string is passed then the top level categories are returned.
 * @param numberOfCategories referenced integer is set to the number of categories returned.
 * @return
 * - NULL which indicates an error has occurred and a exception is pending otherwise
 * - the result is an array of service categories
 */
 char **SBWGetServiceCategories(const char *serviceCategory, SBWInteger *numberOfCategories)
{
	std::vector<std::string> *categories ;
	
	try
	{
		categories = SBWLowLevel::getServiceCategories(serviceCategory);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	int i = *numberOfCategories = categories->size();
	char **result = (char **)calloc(i, sizeof(char *)) ;

	while (i != 0)
	{
		i-- ;
		result[i] = SBWLowLevel::resultStringForC((*categories)[i].c_str());
	}

	delete categories;

	return result ;
}

/**
 * returns the numeric identifiers of all the module instances connected to the broker.
 * @param numberOfModuleInstances referenced integer is set to the number of identifiers returned.
 * @return 
 * - NULL an error has occurred and exception is pending
 * - otherwise an array of numeric module instance identifiers
 */
 SBWInteger *SBWGetExistingModuleInstances(SBWInteger *numberOfModuleInstances)
{
	std::vector<Module> *modules ;

	try
	{
		modules = SBWLowLevel::getExistingModuleInstances();
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	return SBWLowLevel::mapModuleArrayForC(modules, numberOfModuleInstances);
}

/**
 * returns the numeric identifiers of all the instances of a given module.
 * The result should be recovered using SBWFree.
 * @param numberOfModuleInstances referenced integer is set to the number of identifiers returned.
 * @return 
 * - NULL an error has occurred and exception is pending
 * - otherwise an array of numeric module instance identifiers
 */
 SBWInteger *SBWGetExistingNamedModuleInstances(const char *moduleIdName, SBWInteger *numberOfModuleInstances)
{
	std::vector<Module> *modules ;

	try
	{
		modules = SBWLowLevel::getExistingModuleInstances(moduleIdName);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	return SBWLowLevel::mapModuleArrayForC(modules, numberOfModuleInstances);
}

/**
 * returns the module descriptors for all the modules known to the broker.
 * The result should be recovered using SBWFreeSBWModuleDescriptorArray.
 * @param includeRunning whether modules that are not registered but are running should be included in the returned array.
 * @param numberOfModules referenced integer is set to the number of returned descriptors.
 * @param oReturn
 * - NULL an error has occurred and exception is pending
 * - otherwise the module descriptors for all the modules known to the broker.
 */
 void SBWGetModuleDescriptorsA(SBWBoolean includeRunning, SBWBoolean localOnly, SBWInteger *numberOfDescriptors, SBWModuleDescriptor *oReturn)
{
	oReturn = SBWGetModuleDescriptors(includeRunning, localOnly, numberOfDescriptors);
}
/**
 * returns the module descriptors for all the modules known to the broker.
 * The result should be recovered using SBWFreeSBWModuleDescriptorArray.
 * @param includeRunning whether modules that are not registered but are running should be included in the returned array.
 * @param numberOfModules referenced integer is set to the number of returned descriptors.
 * @return
 * - NULL an error has occurred and exception is pending
 * - otherwise the module descriptors for all the modules known to the broker.
 */
 SBWModuleDescriptor *SBWGetModuleDescriptors(
	SBWBoolean includeRunning, SBWBoolean localOnly, SBWInteger *numberOfModules)
{
	std::vector<ModuleDescriptor> *descriptors ;

	try
	{
		descriptors = SBWLowLevel::getModuleDescriptors(includeRunning != 0, localOnly != 0);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	SBWInteger i = *numberOfModules = descriptors->size();
	SBWModuleDescriptor *result = new SBWModuleDescriptor[i];

	while (i != 0)
	{
		i-- ;
		LoadSBWModuleDescriptor(&(result[i]), (*descriptors)[i]);
	}

	delete descriptors ;

	return result ;
}
/**
 * returns the module descriptors for all the modules known to the broker.
 * The result should be recovered using SBWFreeSBWModuleDescriptorArray.
 * @param includeRunning whether modules that are not registered but are running should be included in the returned array.
 * @param numberOfModules referenced integer is set to the number of returned descriptors.
 * @param oReturn
 * - NULL an error has occurred and exception is pending
 * - otherwise the module descriptors for all the modules known to the broker.
 */
 void SBWGetModuleDescriptorA(const char *moduleIdName, SBWBoolean includeRunning, SBWModuleDescriptor *oReturn)
{
	oReturn = SBWGetModuleDescriptor(moduleIdName, includeRunning);
}
/**
 * returns the module descriptors for all the modules known to the broker.
 * The result should be recovered using SBWFreeSBWModuleDescriptorArray.
 * @param includeRunning whether modules that are not registered but are running should be included in the returned array.
 * @param numberOfModules referenced integer is set to the number of returned descriptors.
 * @return
 * - NULL an error has occurred and exception is pending
 * - otherwise the module descriptors for all the modules known to the broker.
 */
 SBWModuleDescriptor *SBWGetModuleDescriptor(const char *moduleIdName, SBWBoolean includeRunning)
{
	ModuleDescriptor descriptor ;

	try
	{
		descriptor = SBWLowLevel::getModuleDescriptor(moduleIdName, includeRunning != 0);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	SBWModuleDescriptor *result = new SBWModuleDescriptor;

	LoadSBWModuleDescriptor(result, descriptor);

	return result ;
}
/**
 * returns the service descriptors for all the registered services for a given module
 * The result should be recovered using SBWFreeSBWServiceDescriptorArray.
 * @param moduleIdName module identification name
 * @param numberOfDescriptors referenced integer is set to the number of returned descriptors.
 * @param oReturn
 * - NULL an error has occurred and exception is pending
 * - otherwise the service descriptors for all the registered services for the given module
 */
 void SBWGetServiceDescriptorsA(const char *moduleIdName, SBWInteger *numberOfDescriptors, SBWServiceDescriptor *oReturn)
{
	oReturn = SBWGetServiceDescriptors(moduleIdName, numberOfDescriptors);
}
/**
 * returns the service descriptors for all the registered services for a given module
 * The result should be recovered using SBWFreeSBWServiceDescriptorArray.
 * @param moduleIdName module identification name
 * @param numberOfDescriptors referenced integer is set to the number of returned descriptors.
 * @return
 * - NULL an error has occurred and exception is pending
 * - otherwise the service descriptors for all the registered services for the given module
 */
 SBWServiceDescriptor *SBWGetServiceDescriptors(const char *moduleIdName, SBWInteger *numberOfDescriptors)
{
	std::vector<ServiceDescriptor> *descriptors ;

	try
	{
		descriptors = SBWLowLevel::getServiceDescriptors(moduleIdName);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	*numberOfDescriptors = descriptors->size();
	SBWServiceDescriptor *result = new SBWServiceDescriptor[*numberOfDescriptors];

	for (int i = 0; i != *numberOfDescriptors; i++)
		LoadSBWServiceDescriptor(&(result[i]), (*descriptors)[i]);

	delete descriptors ;

	return result ;
}

/**
 * locates an instance of a given module, potentially starting an new instance if required.
 * @param moduleIdentificationName module identification string of the require module instance.
 * @return
 * - 0 an error has occurred and exception is pending
 * - otherwise the numeric identifier of the given module.
 */
 SBWInteger SBWGetModuleInstance(const char *moduleIdentificationName)
{
	try
	{
		return SBWLowLevel::getModuleInstance(moduleIdentificationName);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return 0;
	}
}

/**
 * returns the number of services implemented by a given module instance
 * @param moduleInstanceId numeric indentifier for a module instance
 * @return
 * - -1 an error has occurred and exception is pending
 * - otherwise the number of services implemented by a given module instance
 */
 SBWInteger SBWModuleGetNumberOfServices(SBWInteger moduleInstanceId)
{
	try
	{
		return SBWLowLevel::moduleGetNumberOfServices(moduleInstanceId);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return -1;
	}
}

/** 
 * disconnects a given module instance from the broker, potentially triggering its termination.
 * A module instance normally terminates when the broker disconnects from the module instance.
 * @param moduleInstanceId numeric module instance identifier
 * @return
 * - 0 an error has occurred and exception is pending.
 * - 1 the module was successfully disconnected.
 */
 SBWBoolean SBWModuleShutdown(SBWInteger moduleInstanceId)
{
	try
	{
		SBWLowLevel::moduleShutdown(moduleInstanceId);
		return 1 ;
	}
	catch (SBWException *e)
	{
		e->storeException();
		return 0;
	}
}

/**
 * locates a given service on a module given the service's identification string.
 * @param moduleInstanceId numeric module instance identifier
 * @param serviceName service identification string
 * @return
 * - -1 an error has occurred and exception is pending.
 * - otherwise numeric service identifier for given service.
 */
 SBWInteger SBWModuleFindServiceByName(SBWInteger moduleInstanceId, const char *serviceName)
{
	try
	{
		return SBWLowLevel::moduleFindServiceByName(moduleInstanceId, serviceName);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return -1 ;
	}
}

/**
 * locates the set of services on a module that are contained in a given category.
 * @param moduleInstanceId numeric module instance identifier
 * @param category service category
 * @param numberOfServices referenced integer is set to the number of returned services
 * @return
 * - NULL an error has occurred and an exception is pending.
 * - otherwise array of numeric service identifier.
 */
 SBWInteger *SBWModuleFindServicesByCategory(
	SBWInteger moduleInstanceId, const char *category, SBWInteger *numberOfServices)
{
	*numberOfServices = 0 ;
	SBWInteger i = SBWModuleGetNumberOfServices(moduleInstanceId);

	if (i == -1)
		return NULL ;

	SBWInteger *result = (SBWInteger *)calloc(i, sizeof(SBWInteger));

	while (i != 0)
	{
		i-- ;

		SBWServiceDescriptor *descriptor = SBWServiceGetDescriptor(moduleInstanceId, i);

		if (descriptor == NULL)
		{
			free(result);
			return NULL;
		}

		if (SBWLowLevel::isSubCategory(category, descriptor->serviceCategory))
		{
			result[*numberOfServices] = i ;
			(*numberOfServices)++ ;
		}
	}

	return result ;
}

/**
 * locates the method on a given service with a given signature
 * @param moduleInstanceId numeric module instance identifier
 * @param serviceId numeric service identifier
 * @param signature SBW method signature
 * @return
 * - -1 an error has occurred and an exception is pending.
 * - otherwise numeric method identifier.
 */
 SBWInteger SBWServiceGetMethod(
	SBWInteger moduleInstanceId, SBWInteger serviceId, const char *signature)
{
	try
	{
		return SBWLowLevel::serviceGetMethod(moduleInstanceId, serviceId, signature);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return -1 ;
	}
}

/**
 * locates the method on a given service with a given signature
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param signature SBW method signature.
 * @param oReturn
 * - -1 an error has occurred and an exception is pending.
 * - otherwise numeric method identifier.
 */
 void SBWServiceGetDescriptorA(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWServiceDescriptor *oReturn)
{
	oReturn = SBWServiceGetDescriptor(moduleInstanceId, serviceId);
}

/**
 * locates the method on a given service with a given signature
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param signature SBW method signature.
 * @return
 * - -1 an error has occurred and an exception is pending.
 * - otherwise numeric method identifier.
 */
 SBWServiceDescriptor *SBWServiceGetDescriptor(
	SBWInteger moduleInstanceId, SBWInteger serviceId)
{
	try
	{
		ServiceDescriptor handle = SBWLowLevel::serviceGetDescriptorHandle(moduleInstanceId, serviceId);
		SBWServiceDescriptor *descriptor = new SBWServiceDescriptor();

		LoadSBWServiceDescriptor(descriptor, handle);
		
		return descriptor ;
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}
}

/**
 * returns the number of SBW methods on a given service.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @return
 * - -1 an error has occurred and an exception is pending.
 * - otherwise number of SBW methods on the given service.
 */
 SBWInteger SBWServiceGetNumberOfMethods(SBWInteger moduleInstanceId, SBWInteger serviceId)
{
	try
	{
		return SBWLowLevel::serviceGetNumberOfMethods(moduleInstanceId, serviceId);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return -1 ;
	}
}

/**
 * returns the signature of a given SBW method.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @return
 * - NULL an error has occurred and an exception is pending.
 * - otherwise the SBW signture string for the given method.
 */
 char *SBWMethodGetSignatureString(SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger methodId)
{
	try
	{
		return SBWLowLevel::methodGetSignatureInCharStar(moduleInstanceId, serviceId, methodId);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}
}

/**
 * returns the name of a given SBW method.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @return
 * - NULL an error has occurred and an exception is pending.
 * - otherwise the name of the given method.
 */
 char *SBWMethodGetName(SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger methodId)
{
	try
	{
		return SBWLowLevel::methodGetName(moduleInstanceId, serviceId, methodId);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}
}

/**
 * returns the help string for a given SBW method.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @return
 * - NULL an error has occurred and an exception is pending.
 * - otherwise the help string for the given method.
 */
 char *SBWMethodGetHelp(SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger methodId)
{
	try
	{
		return SBWLowLevel::methodGetHelp(moduleInstanceId, serviceId, methodId);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}
}

/**
 * executes a given SBW method.
 * @param returnResult whether this function should block until the method has completed.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @param sig expected signature of method compatable with the following argument.
 * @param ap variable argument list consisting of argument values followed by a pointer to memory to hold the SBW method result.
 * @return
 * - 0 an error has occurred and an exception is pending.
 * - 1 the method executed successfully.
 */
SBWBoolean SBWMethodInvoke(
	SBWBoolean returnResult, SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method,
	const char *sig, va_list &ap)
{
	try
	{
		Signature signature(sig);
		DataBlockWriter args ;

		signature.getImpl()->getArguments().write(args, ap);

		if (returnResult)
		{
			DataBlockReader result = SBWLowLevel::methodCall(moduleInstanceId, serviceId, method, args);

			signature.getImpl()->getReturnType().getImpl()->read(result, ap);
		}
		else
			SBWLowLevel::methodSend(moduleInstanceId, serviceId, method, args);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}

/**
 * call an SBW Method.
 * The variable argument list consists of argument values followed by a pointer to memory to hold the SBW method result.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @param signature expected signature of method compatable with the following arguments.
 * @return
 * - 0 an error has occurred and an exception is pending.
 * - 1 the method executed successfully.
 */
 SBWBoolean SBWMethodCall(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const char *signature ...)
{
	va_list ap;
	va_start(ap, signature);
	SBWBoolean result = SBWMethodInvoke(true, moduleInstanceId, serviceId, method, signature, ap);
	va_end(ap);

	return result ;
}

/**
 * call an SBW Method using data blocks to fetch and receive data
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @return 
 * a datablock containing the return data from the method
 * - NULL => an error has occurred and an exception is pending.
 */
 SBWDataBlockReader *SBWMethodCallUsingDataBlock(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, SBWDataBlockWriter *args)
{
	DataBlockReader returnData;
	SystemsBiologyWorkbench::sbwDataBlockReader *result ;

	try
	{
		returnData = SBWLowLevel::methodCall(moduleInstanceId, serviceId, method, DataBlockConverter::convert(args));
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	result = returnData.getImpl();
	result->AddReference();

	return (SBWDataBlockReader *)result ;
}

/**
 * make a non-blocking SBW Method call.
 * The variable argument list consists of argument values.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @param signature expected signature of method compatable with the following arguments.
 * @return
 * - 0 an error has occurred and an exception is pending.
 * - 1 the send message was sent successfully.
 */
 SBWBoolean SBWMethodSend(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const char *signature ...)
{
	va_list ap;
	va_start(ap, signature);
	SBWBoolean result = SBWMethodInvoke(false, moduleInstanceId, serviceId, method, signature, ap);
	va_end(ap);

	return result ;
}

/**
 * make a non-blocking SBW Method call using data blocks to fetch and receive data
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @return 
 * a datablock containing the return data from the method
 * - NULL => an error has occurred and an exception is pending.
 */
 SBWBoolean SBWMethodSendUsingDataBlock(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, SBWDataBlockWriter *args)
{
	try
	{
		SBWLowLevel::methodSend(moduleInstanceId, serviceId, method, DataBlockConverter::convert(args));
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}
/**
 * call an SBW Method using raw argument data.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @param rawData argument data.
 * @param rawDataLength referenced integer is initially the length of rawData,
 *                      this method changes it to the length of the result data.
 * @param oReturn
 * - NULL an error has occurred and an exception is pending.
 * - otherwise the result data.
 */

 void SBWMethodCallRawA(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const unsigned char *rawDataArgs, unsigned char *oReturn, SBWInteger *rawDataLength)
{	
	int nTemp = *rawDataLength;
	unsigned char *oTemp = SBWMethodCallRaw(moduleInstanceId, serviceId, method, rawDataArgs, &nTemp);
	memcpy(oReturn, oTemp , nTemp );
	*rawDataLength=nTemp;	
}


/**
 * call an SBW Method using raw argument data.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @param rawData argument data.
 * @param rawDataLength referenced integer is initially the length of rawData,
 *                      this method changes it to the length of the result data.
 * @return
 * - NULL an error has occurred and an exception is pending.
 * - otherwise the result data.
 */
 unsigned char *SBWMethodCallRaw(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const unsigned char *rawData, SBWInteger *rawDataLength)
{
	try
	{
		DataBlockWriter args ;

		if (rawData != NULL)
			args.insertData(rawData, *rawDataLength);

		DataBlockReader result = SBWLowLevel::methodCall(moduleInstanceId, serviceId, method, args);

		return result.getRemainingDataForC(*rawDataLength);
	}
	catch (SBWException *e)
	{
		e->storeException();
		*rawDataLength = 0;
		return NULL ;
	}
	catch (...)
	{
		*rawDataLength = 0;
		return NULL ;
	}
}

/**
 * make a non-blocking SBW Method call using raw argument data.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @param rawData argument data.
 * @param rawDataLength the length of rawData.
 * @return
 * - 0 an error has occurred and an exception is pending.
 * - 1 the send message was sent successfully.
 */
 SBWBoolean SBWMethodSendRaw(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const unsigned char *rawData, SBWInteger rawDataLength)
{
	try
	{
		DataBlockWriter args ;

		if (rawData != NULL)
			args.insertData(rawData, rawDataLength);

		SBWLowLevel::methodSend(moduleInstanceId, serviceId, method, args);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}
/**
 * return the parse structure for the signature of a given method.
 * deallocate the result using SBWFreeSBWSignature.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @param oReturn
 * - NULL an error has occurred and an exception is pending.
 * - otherwise the parse tree of the given method's signature.
 */
 void SBWMethodGetSignatureA(SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger methodId, SBWSignature *oReturn)
{
	oReturn = SBWMethodGetSignature(moduleInstanceId, serviceId, methodId);
}
/**
 * return the parse structure for the signature of a given method.
 * deallocate the result using SBWFreeSBWSignature.
 * @param moduleInstanceId numeric module instance identifier.
 * @param serviceId numeric service identifier.
 * @param methodId numeric method identifier.
 * @return
 * - NULL an error has occurred and an exception is pending.
 * - otherwise the parse tree of the given method's signature.
 */
 SBWSignature *SBWMethodGetSignature(SBWInteger moduleId, SBWInteger serviceId, SBWInteger methodId)
{
	try
	{
		Signature signature = SBWLowLevel::methodGetSignature(moduleId, serviceId, methodId);
		SystemsBiologyWorkbench::sbwSignature *handle = signature.getImpl();

		handle->AddReference();

		return (SBWSignature *)handle ;
	}
	catch (SBWException *e)
	{
		e->storeException();

		return NULL ;
	}
}

/**
 * return the arguments of a given signature structure.
 * deallocate the result using SBWFreeSBWSignatureElementArray.
 * @param sig signature parse structure.
 * @param numberOfArguments referenced integer is set to the number of reutnred arguments.
 * @return array of signature arguments.
 */
 SBWSignatureElement **SBWSignatureGetArguments(SBWSignature *sig, SBWInteger *numberOfArguments)
{
	SystemsBiologyWorkbench::sbwSignature *signature = (SystemsBiologyWorkbench::sbwSignature *)sig;

	return
		(SBWSignatureElement **)SBWLowLevel::mapSignatureElements(
			signature->getArguments().copyVector(), numberOfArguments);
}

/**
 * return the result type of the given signature structure.
 * deallocate the result using SBWFreeSBWSignatureType.
 * @param sig signature parse structure.
 * @return signature type structure of the result of the signature structure.
 */
 SBWSignatureType *SBWSignatureGetReturnType(SBWSignature *sig)
{
	SystemsBiologyWorkbench::sbwSignature *signature = (SystemsBiologyWorkbench::sbwSignature *)sig;
	SystemsBiologyWorkbench::sbwSignatureType *handle = signature->getReturnType().getImpl();

	handle->AddReference();

	return (SBWSignatureType *)handle;
}

/**
 * returns the name string of the given signature structure.
 * deallocate the result using SBWFree.
 * @param sig signature parse structure.
 * @return the name string of the given signature structure
 */
 char *SBWSignatureGetName(SBWSignature *sig)
{
	SystemsBiologyWorkbench::sbwSignature *signature = (SystemsBiologyWorkbench::sbwSignature *)sig;
	return SBWLowLevel::resultStringForC(signature->getName().c_str());
}

/**
 * returns the name of a signature argument.
 * deallocate the result using SBWFree.
 * @param arg signature argument parse structure.
 * @return the name string of the given signature argument.
 */
 char *SBWSignatureElementGetName(SBWSignatureElement *arg)
{
	SystemsBiologyWorkbench::sbwSignatureElement *argument = (SystemsBiologyWorkbench::sbwSignatureElement *)arg;
	return SBWLowLevel::resultStringForC(argument->getName().c_str());
}

/**
 * returns the type structure of a signature argument.
 * deallocate the result using SBWFreeSBWSignatureType.
 * @param arg signature argument parse structure.
 * @return the type structure of a signature argument..
 */
SBWSignatureType *SBWSignatureElementGetType(SBWSignatureElement *arg)
{
	SystemsBiologyWorkbench::sbwSignatureElement *argument = (SystemsBiologyWorkbench::sbwSignatureElement *)arg;
	SignatureType type;
	
	argument->getType(type);
	
	SystemsBiologyWorkbench::sbwSignatureType *handle = type.getImpl();
	
	handle->AddReference();

	return (SBWSignatureType *)handle;
}

/**
 * returns the generic type of a signature type.
 * @param t signature type.
 * @return the generic type of the signature type.
 */
 SBWDataBlockType SBWSignatureTypeGetType(SBWSignatureType *t)
{
	SystemsBiologyWorkbench::sbwSignatureType *type = (SystemsBiologyWorkbench::sbwSignatureType *)t;
	return (SBWDataBlockType)type->getType();
}

/**
 * returns the type of object contained inside a signature type for an array.
 * deallocate the result using SBWFreeSBWSignatureType.
 * @param t signature type.
 * @return
 * - NULL error occured, exception pending
 * - otherwise the signature type of object contained inside a signature type for an array.
 */
 SBWSignatureType *SBWSignatureTypeGetArrayInnerType(SBWSignatureType *t)
{
	SystemsBiologyWorkbench::sbwSignatureType *type = (SystemsBiologyWorkbench::sbwSignatureType *)t;
	try
	{
		SystemsBiologyWorkbench::sbwSignatureType *handle = type->getArrayInnerType();

		handle->AddReference();

		return (SBWSignatureType *)handle ;
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}
}

/**
 * returns the number of dimensions of a signature type for an array.
 * @param t signature type.
 * @return
 * - -1 implies error occured, exception pending
 * - other values are the number of dimensions of a signature type for an array.
 */
 SBWInteger SBWSignatureTypeGetArrayDimensions(SBWSignatureType *t)
{
	SystemsBiologyWorkbench::sbwSignatureType *type = (SystemsBiologyWorkbench::sbwSignatureType *)t;

	try
	{
		return type->getArrayDimensions();
	}
	catch (SBWException *e)
	{
		e->storeException();
		return -1 ;
	}
}

/**
 * returns the contents of a list type.
 * @param t signature type.
 * @param numberOfArguments referenced integer is set to the number of arguments in the returned array.
 * @return
 * - NULL implies error occured, exception pending
 * - otherwise array containing signature argument structures which are the contents of the given list type.
 */
 SBWSignatureElement **SBWSignatureTypeGetListContents(SBWSignatureType *t, SBWInteger *numberOfArguments)
{
	SystemsBiologyWorkbench::sbwSignatureType *type = (SystemsBiologyWorkbench::sbwSignatureType *)t;

	try
	{
		return (SBWSignatureElement **)SBWLowLevel::mapSignatureElements(type->getContents(), numberOfArguments);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}
}

/**
 * copy raw data into a SBWDataBlockWriter structure.
 * @param writer raw data is appended to this object.
 * @param rawData data in SBW data format.
 * @param rawDataLength length of rawData.
 */
 void SBWWriteRaw(
	SBWDataBlockWriter *writer, const unsigned char *rawData, SBWInteger rawDataLength)
{
	if (rawData != NULL)
	((SystemsBiologyWorkbench::sbwDataBlockWriter *)writer)->insertData(rawData, rawDataLength);
}

/**
 * copy raw data from a SBWDataBlockReader structure into a byte array.
 * returned data is copied the SBWDataBlockReader cursor to the end of the SBWDataBlockReader structure.
 * @param reader raw data is extracted from this object.
 * @param rawDataLength referenced integer is set to the length of returned data.
 * @return raw message data in a byte array.
 */
 unsigned char *SBWReadRaw(SBWDataBlockReader *reader, SBWInteger *rawDataLength)
{
	return ((SystemsBiologyWorkbench::sbwDataBlockReader *)reader)->getRemainingDataForC(*rawDataLength);
}

/**
 * This function returns the current connection status. Used by the C# binding library to determine
 * whether or not an SBWConnect its necessary. 
 *
 */
 SBWBoolean SBWGetConnectionStatus()
{
	return SBWLowLevel::isConnected();
}

/**
 * disconnect this application from the broker.
 * This function blocks until the disconnection has occured.
 */
 void SBWDisconnect()
{
	try
	{
		SBWLowLevel::disconnect();
	}
	catch (SBWException *e)
	{
		delete e ;
	}
}

/**
 * request that the broker disconnects from this application.
 * This function doesn't block until the disconnection has occured.
 */
 void SBWSignalDisconnect()
{
	try
	{
		SBWLowLevel::signalDisconnect();
	}
	catch (SBWException *e)
	{
		delete e ;
	}
}	

/**
 * create a module implementation structure for this application.
 * @param unique the module identification string for the module.
 * @param nameForDisplay the humanly readable name for this module.
 * @param type management mode of this module.
 * @param help help string for this module.
 * @return
 * - false error occured, exception pending.
 * - true structure created successfully.
 */
 SBWBoolean SBWCreateModuleImpl(
	const char *uniqueName, const char *nameForDisplay, SBWModuleManagementType type, const char *help)
{
	try
	{
		if (moduleImpl != NULL)
			throw new SBWApplicationException("existing module implementation in place");

		moduleImpl = new ModuleImpl(uniqueName, nameForDisplay, (ModuleManagementType)type, help);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}

/**
 * Discards the current module implementation for this application
 * @return
 * - false error occured, exception pending.
 * - true structure deallocated successfully.
 */
 SBWBoolean SBWFreeModuleImpl()
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not in place");

		delete moduleImpl;
		moduleImpl = NULL;
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}

/**
 * Discards the current module implementation for this application.
 * @return
 * - false error occured, exception pending.
 * - true structure deallocated successfully.
 */
 SBWBoolean SBWModuleImplSetCommandLine(const char *cmdLine)
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not initialized");

		moduleImpl->setCommandLine(cmdLine);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}


 SBWBoolean SBWModuleImplSetHost(char* host)
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not initialized");

		moduleImpl->runOnHost(host);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;

}

/**
 * adds a new service implementation to the current module implementation for this application.
 * @param serviceName service identification string.
 * @param serviceDisplayName humanly readable name for the service.
 * @param categrory specific category for this service or empty string if not categorized.
 * @param help help string for this service.
 * @return
 * - false error occured, exception pending.
 * - true new service added successfully.
 */
 SBWBoolean SBWModuleImplAddService(
	const char *serviceName, const char *serviceDisplayName, const char *category, const char *help)
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not in place");

		moduleImpl->addService(serviceName, serviceDisplayName, category, help);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}

/**
 * adds a new SBW method to a given service implementation for this application.
 * @param serviceName service identification string.
 * @param handler function that implements the method.
 * @param userData application data associated with this method.
 * @param signature SBW method signature for this method.
 * @param synchronized indicates whether this method should be synchronized.
 * @param help help string for this method.
 * @return
 * - false error occured, exception pending.
 * - true new method added successfully.
 */
 SBWBoolean SBWModuleImplSetHandlerA(
	const char *serviceName,
	SBWCHandler handler,
	int nService, int nMethod,
	const char *signature,
	SBWBoolean synchronized,
	const char *help)
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not in place");

		moduleImpl->setHandler(
			serviceName, new HandlerForStdCall(handler, nService, nMethod), signature, synchronized != 0, help);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}


/**
 * adds a new SBW method to a given service implementation for this application.
 * @param serviceName service identification string.
 * @param handler function that implements the method.
 * @param userData application data associated with this method.
 * @param signature SBW method signature for this method.
 * @param synchronized indicates whether this method should be synchronized.
 * @param help help string for this method.
 * @return
 * - false error occured, exception pending.
 * - true new method added successfully.
 */
 SBWBoolean SBWModuleImplSetHandler(
	const char *serviceName,
	SBWHandler handler,
	void *userData,
	const char *signature,
	SBWBoolean synchronized,
	const char *help)
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not in place");

		moduleImpl->setHandler(
			serviceName, new HandlerForC(handler, userData), signature, synchronized != 0, help);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}

/**
 * connect this application to the broker indicating that this application is providing services.
 * @return
 * - false error occured, exception pending.
 * - true connection to broker made successfully.
 */
 SBWBoolean SBWModuleImplEnableServices()
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not in place");

		moduleImpl->enableModuleServices();
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}

/**
 * register this application's module implementation with the broker.
 * @return
 * - false error occured, exception pending.
 * - true registration with broker made successfully.
 */
 SBWBoolean SBWModuleImplRegister()
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not in place");

		moduleImpl->registerModule();
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}

 SBWBoolean SBWModuleImplRunA()
{
	char* argv[1];
#if  _MSC_VER >= 1400
	argv[0] = _strdup("-sbwmodule");
#else
	argv[0] = strdup("-sbwmodule");
#endif
	return SBWModuleImplRun(1, argv, 1);
}
/**
 * run this application in the SBW mode appropriate to its command line arguments.
 * If the application has the argument <code>-sbwmodule</code> then the application provides services to other modules.
 * If the application has the argument <code>-sbwregister</code> the the application registers information about its services
 * with the broker.
 * @param argc number of command line arguments + 1 (length of argv).
 * @param argv command line arguments including command itself.
 * @param waitForDisconnect indicates whether, in module mode,
 *                          this method should block until the application is disconnected from the broker.
 * @return
 * - false error occured, exception pending.
 * - true execution was successful.
 */
 SBWBoolean SBWModuleImplRun(int argc, char *argv[], SBWBoolean waitForDisconnect)
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not in place");

		moduleImpl->run(argc, argv, waitForDisconnect != 0);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}

	return true ;
}
/**
 * returns the module descriptor for a given module instance.
 * @param moduleInstanceId numeric module identifier.
 * @param oReturn
 * - NULL error occured, exception pending
 * - otherwise module descriptor for given module instance.
 */
 void SBWModuleGetModuleDescriptorA(SBWInteger moduleInstanceId, SBWModuleDescriptor *oReturn)
{
	oReturn = SBWModuleGetModuleDescriptor(moduleInstanceId);
}
/**
 * returns the module descriptor for a given module instance.
 * @param moduleInstanceId numeric module identifier.
 * @return
 * - NULL error occured, exception pending
 * - otherwise module descriptor for given module instance.
 */
 SBWModuleDescriptor *SBWModuleGetModuleDescriptor(SBWInteger moduleInstanceId)
{
	SBWModuleDescriptor *result = new SBWModuleDescriptor ;

	try
	{
		LoadSBWModuleDescriptor(
			result, SBWLowLevel::moduleGetModuleDescriptor(moduleInstanceId));
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}

	return result ;
}

/** 
 * returns the function that implements a given method on this applications module implementation.
 * @param serviceName service identification string.
 * @param methodName method name or method signature string.
 * @param userData referenced object chnaged to application data supplied with method implementation.
 * @return function that implements given SBW method.
 */
 SBWHandler SBWModuleImplGetHandler(const char *serviceName, const char *methodName, void **userData)
{
	try
	{
		if (moduleImpl == NULL)
			throw new SBWApplicationException("module implementation not in place");

		HandlerForC *handler = (HandlerForC *)moduleImpl->getHandler(serviceName, methodName);

		*userData = handler->getUserData();

		return handler->getFunction();
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}
}

/**
 * returns a new empty SBWDataBlockWriter structure.
 * @return a new empty SBWDataBlockWriter structure.
 */
 SBWDataBlockWriter *SBWCreateSBWDataBlockWriter()
{
	SystemsBiologyWorkbench::sbwDataBlockWriter *result = new SystemsBiologyWorkbench::sbwDataBlockWriter();

	result->AddReference();

	return (SBWDataBlockWriter *)result ;
}

/**
 * insert data into given SBWDataBlockWriter structure.
 * Data is appended to the structure.
 * The variable argument list consists of the data values corresponding to the types in argumentList.
 * @param w data block to which the data will be appended.
 * @param argumentList comma separated list of SBW type names corresponding to the remaining arguments.
 * @return 
 * - false error occurred, exception pending.
 * - true data inserted successfully.
 */
 SBWBoolean SBWWrite(SBWDataBlockWriter *w, const char *argumentList, ...)
{
	try
	{
		va_list ap;
		va_start(ap, argumentList);
	
		SignatureListContents args(argumentList);
		args.write(DataBlockConverter::convert(w), ap);

		va_end(ap);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}	

	return true ;
}

/**
 * extract data from the given SBWDataBlockReader structure.
 * The structure's cursor is moved past the data.
 * The variable argument list consists of pointers to objects which will be changed to the extracted values.
 * @param r data block from which the data will be extracted.
 * @param argumentList comma separated list of SBW type names corresponding to the remaining arguments.
 * @return 
 * - false error occurred, exception pending.
 * - true data extracted successfully.
  */
 SBWBoolean SBWRead(SBWDataBlockReader *r, const char *argumentList, ...)
{
	try
	{
		va_list ap;
		va_start(ap, argumentList);
	
		SignatureListContents args(argumentList);
		args.read(DataBlockConverter::convert(r), ap);

		va_end(ap);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return false ;
	}	

	return true ;
}

/**
 * returns the SBWDataBlockType for the object at the cursor in the given SBWDataBlockReader structure.
 * The cursor is not moved.
 * @return 
 * - SBWErrorType error occurred, exception pending.
 * - otherise the SBWDataBlockType for the object at the cursor in the given SBWDataBlockReader structure.
 */
 SBWDataBlockType SBWGetNextType(SBWDataBlockReader *reader)
{
	try
	{
		return (SBWDataBlockType)((SystemsBiologyWorkbench::sbwDataBlockReader *)reader)->getNextType();
	}
	catch (SBWException *e)
	{
		e->storeException();
		return SBWErrorType ;
	}	
}

/**
 * returns the SBWDataBlockType for the objects contained in the array at the cursor in the given SBWDataBlockReader structure.
 * The cursor is not moved.
 * @return 
 * - SBWErrorType error occurred, exception pending.
 * - otherise the SBWDataBlockType for the objects contained in the array at the cursor in the given SBWDataBlockReader structure.
 */
 SBWDataBlockType SBWGetNextArrayType(SBWDataBlockReader *reader)
{
	try
	{
		return (SBWDataBlockType)((SystemsBiologyWorkbench::sbwDataBlockReader *)reader)->getNextArrayType();
	}
	catch (SBWException *e)
	{
		e->storeException();
		return SBWErrorType ;
	}	
}

/**
 * returns the number of dimensions of the array at the cursor in the given SBWDataBlockReader structure.
 * The cursor is not moved.
 * @return 
 * - -1 error occurred, exception pending.
 * - otherwise the number of dimensions of the array at the cursor in the given SBWDataBlockReader structure.
 */
 SBWInteger SBWGetNextArrayDimensions(SBWDataBlockReader *reader)
{
	try
	{
		return ((SystemsBiologyWorkbench::sbwDataBlockReader *)reader)->getNextArrayDimensions();
	}
	catch (SBWException *e)
	{
		e->storeException();
		return -1 ;
	}	
}

/**
 * WIN32 specific - extracts the command line that this application was launched with.
 * @param argc referenced integer set to the number of command line arguments plus 1 - size of argc
 * @param argv referenced pointer set to array of command line arguments including the command itself.
 */
#ifdef WIN32
 void SBWWindowsExtractCommandLine(int *argc, char ***argv)
{
	SBWOS::windowsExtractCommandLine(argc, argv);
}
#endif

/**
 * returns the command line required to launch this application in module mode.
 * The result should be recovered using SBWFree
 * @param argZero the command (without arguments) which was passed to this application by the OS.
 * @return the command line required to launch this application in module mode.
 */
 char *SBWCalculateCommandLine(const char *argZero)
{
	try
	{
		return ModuleImpl::calculateCommandLineRaw(argZero);
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}
}

/**
 * returns the version string for the C/C++ SBW library (this library!!).
 * The result should be recovered using SBWFree
 * @return the version string for the C/C++ SBW library.
 */
 char *SBWGetVersion()
{
	return SBWLowLevel::resultStringForC(SBWLowLevel::getVersion().c_str());
}

/**
 * deallocates the given module descriptor.
 * @param x module descriptor to be deallocated.
 */
void SBWFreeModuleDescriptor(SBWModuleDescriptor *x)
{
	delete[] x->commandLine ;
	delete[] x->displayName ;
	delete[] x->name ;

	delete x ;
}

/**
 * returns the SBWDataBlockReader corresponding to a given SBWDataBlockWriter.
 * @param writer given data block.
 * @return the SBWDataBlockReader corresponding to the given SBWDataBlockWriter.
 */
 SBWDataBlockReader *SBWGetReaderFromWriter(SBWDataBlockWriter *writer)
{
	SystemsBiologyWorkbench::sbwDataBlockReader *reader =
		((SystemsBiologyWorkbench::sbwDataBlockWriter *)writer)->getReader();

	reader->AddReference();

	return (SBWDataBlockReader *)reader;
}

/**
 * turns on or off debug trace output depending on parameter.
 * Has no effect in the Release configuration library.
 * Use the debug configuration library to get debug trace output.
 * @param traceOn whether debug trace should be output
 */
 void SBWSetDebugTrace(SBWBoolean traceOn)
{
	SBWDebug::setTraceMode(traceOn != 0);
}

/**
 * output debug trace information to standard error stream.
 * Has no effect in the Release configuration library.
 * Use the debug configuration library to get debug trace output.
 * Use SBWSetDebugTrace to enable output from this function.
 * @param text message string to be output
 * @param file the source code file in which this call is being made 
 * @param line the line in <code>file</code> on which this call is being made
 */
 void SBWDebugTrace(const char *text, const char *file, int line)
{
	SBWDebug::trace(text, file, line);
}

/**
 * Convenience function for performing the tasks required for handling
 * the <code>-sbwdebug</code> to an application.  This method must be passed the array of command-line
 * arguments passed to the application.  This method then checks for the
 * presence of <code>-sbwdebug</code> and enables debug trace output.
 * Returns whether the application should either register itself or run as a module based on the command line.
 * @see SBWSetDebugTrace
 * @param argc an integer indicating the number of arguments passed to
 * the program on the command line
 * @param argv an array of strings comprising the set of arguments
 * passed to the program on the command line
 * @returns whether the command line contains either <code>-sbwregister</code> or <code>-sbwmodule</code>
 *          ie whether the application should either register itself or run as a module respectively.
 **/
 SBWBoolean SBWProcessArguments(int argc, char* argv[])
{
	return SBWLowLevel::processArguments(argc, argv);
}

 void *SBWMalloc(SBWInteger a, SBWInteger b)
{
	return calloc(a, b);
}

/**
 * frees string array including the strings inside
 * @param size - number of elements in array
 * @param a - array
 **/
 void SBWFreeStringArray(SBWInteger size, char **a)
{
	for (int i = 0 ; i != size; i++)
		free(a[i]);

	free(a);
}

/**
 * frees string 2D array including the strings inside
 * @param xSize - number of elements in array on x axis
 * @param ySize - number of elements in array on y axis
 * @param a - array
 **/
 void SBWFreeString2DArray(SBWInteger xSize, SBWInteger ySize, char ***a)
{
	for (int x = 0 ; x != xSize; x++)
	{
		for (int y = 0 ; y != ySize; y++)
			free(a[x][y]);

		free(a[x]);
	}

	free(a);
}


/**
 * frees data block array including the data block inside
 * @param size - number of elements in array
 * @param a - array
 **/
 void SBWFreeSBWDataBlockWriterArray(SBWInteger size, SBWDataBlockWriter **a)
{
	for (int i = 0 ; i != size; i++)
		SBWFreeSBWDataBlockWriter(a[i]);

	free(a);
}

/**
 * frees data block 2D array including the data block inside
 * @param xSize - number of elements in array on x axis
 * @param ySize - number of elements in array on y axis
 * @param a - array
 **/
 void SBWFreeSBWDataBlockWriter2DArray(SBWInteger xSize, SBWInteger ySize, SBWDataBlockWriter ***a)
{
	for (int x = 0 ; x != xSize; x++)
	{
		for (int y = 0 ; y != ySize; y++)
			SBWFreeSBWDataBlockWriter(a[x][y]);

		free(a[x]);
	}

	free(a);
}

/**
 * frees data block array including the data block inside
 * @param size - number of elements in array
 * @param a - array
 **/
 void SBWFreeSBWDataBlockReaderArray(SBWInteger size, SBWDataBlockReader **a)
{
	for (int i = 0 ; i != size; i++)
		SBWFreeSBWDataBlockReader(a[i]);

	free(a);
}

/**
 * frees data block 2D array including the data block inside
 * @param xSize - number of elements in array on x axis
 * @param ySize - number of elements in array on y axis
 * @param a - array
 **/
 void SBWFreeSBWDataBlockReader2DArray(SBWInteger xSize, SBWInteger ySize, SBWDataBlockReader ***a)
{
	for (int x = 0 ; x != xSize; x++)
	{
		for (int y = 0 ; y != ySize; y++)
			SBWFreeSBWDataBlockReader(a[x][y]);

		free(a[x]);
	}

	free(a);
}

/**
 * returns broker version string
 * @return broker version string
 **/
 char *SBWGetBrokerVersion()
{
	try
	{
		return SBWLowLevel::resultStringForC(SBWLowLevel::getBrokerVersion().c_str());
	}
	catch (SBWException *e)
	{
		e->storeException();
		return NULL ;
	}
}

/// registered incoming call thread creater for the C API
SBWIncomingCallThreadCreator incomingCallThreadCreator ;

/**
 * creates threads for the C API using the registered incvoming call thread creater for the C API
 * @param x object managing all incoming calls
 */
void SBWIncomingCallThreadCreatorForC(RPCInCall *x)
{
	incomingCallThreadCreator((SBWIncomingCallThreadData *)x);

	if (SBWException::getStoredException() != NULL)
			SBWException::throwStoredException();	
}

/**
 * registers function that creates threads that process incoming calls
 * @param x function that creates threads that process incoming calls
 */
 void SBWRegisterIncomingCallThreadCreator(SBWIncomingCallThreadCreator x)
{
	RPCInCall::registerThreadCreator(SBWIncomingCallThreadCreatorForC);
	incomingCallThreadCreator = x ;
}

/**
 * prcoesses incoming calls (body of thread for that processes incoming calls)
 * @param x object that manages all incoming calls
 */
 void SBWProcessIncomingCalls(SBWIncomingCallThreadData *x)
{
	RPCInCall::processIncomingCalls((RPCInCall *)x);
}
