/**
 * @file SBWC.h
 * @brief   SBW C API
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

/// include loop block
#ifndef SBWC
#define SBWC

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef TRACE
#ifdef _DEBUG
/** writes a given string to standard error prefixing the string with the current thread id and terminating the string
 * the string with a carriage return.
 * @param x string to be output.
 */
#define TRACE(x) \
{\
	SBWDebugTrace(x, __FILE__, __LINE__);\
}

#else
#define TRACE(x) {;}
#endif
#endif

#include <SBW/sbwbasictypes.h>
#include <SBW/DataBlockType.h>

#ifndef SWIG

/// contains module data on one module taken from the broker's persistant registry of modules
typedef struct 
{
	char *name ; /**< module identification name */
	char *displayName ; /**< humanly readable name */
	SBWModuleManagementType managementType; /**< mechanism used to launch module instances */
	char *commandLine ; /**< command line used to launch instances of module */
	char *help ; /**< help string (documentation) for module */
} SBWModuleDescriptor ;

/// contains service data on one service taken from the broker's persistant registry of modules
typedef struct 
{
	char *serviceName ; /**< service identification name */
	char *serviceDisplayName ; /**< humanly readable service name */
	char *serviceCategory ; /**< category of service */
	SBWModuleDescriptor module ; /**< module that implements service */
	char *help ; /**< help string (documentation) for service */
} SBWServiceDescriptor ;

#endif // not SWIG

/// data block structure oriented to encoding data.
typedef struct sbwDataBlockWriter SBWDataBlockWriter ;

/// data block structure oriented to decoding data.
typedef struct sbwDataBlockReader SBWDataBlockReader ;

/// the parse structure of a SBW method signature.
typedef struct sbwSignature SBWSignature ;

/// the parse structure of an argument in a SBW method signature.
typedef struct sbwSignatureElement SBWSignatureElement ;

/// the parse structure of a type in a SBW method structure.
typedef struct sbwSignatureType SBWSignatureType ;

/// boolean type corresponding to the SBWBooleanType enum value.
typedef int SBWBoolean ;

/**
 * callback function which implements an SBW method.
 * @param from numeric module instance identifier of the module that called this function.
 * @param args data block containing the arguments to this SBW method.
 * @param result data block in which the result of this SBW method should be stored.
 * @param userData data suppied when this function was attached to the SBW C API.
 */
typedef void (*SBWHandler)(
			SBWInteger from, SBWDataBlockReader *args, SBWDataBlockWriter *result, void *userData);


typedef void (STDCALL *SBWCHandler)(
			SBWInteger from, unsigned char* args, SBWInteger nArgLenght, unsigned char* result, int* nResultLenght, int nService, int nMethod);

SBW_API void SBWFree(void *);
SBW_API void SBWFreeArray(void *); // deprecated
SBW_API void SBWFreeObject(void *x); // deprecated
SBW_API void SBWFree2DArray(SBWInteger xSize, void **inArray);
SBW_API void SBWFreeServiceDescriptor(SBWServiceDescriptor *);
#ifndef SWIG
SBW_API void SBWFreeServiceDescriptorArray(SBWInteger size, SBWServiceDescriptor *inArray);
#endif
SBW_API void SBWFreeModuleDescriptor(SBWModuleDescriptor *);
#ifndef SWIG
SBW_API void SBWFreeModuleDescriptorArray(SBWInteger size, SBWModuleDescriptor *inArray);
#endif
SBW_API void SBWFreeSBWDataBlockWriter(SBWDataBlockWriter *);
SBW_API void SBWFreeSBWDataBlockReader(SBWDataBlockReader *);
SBW_API void SBWFreeSBWSignature(SBWSignature *x);
SBW_API void SBWFreeSBWSignatureType(SBWSignatureType *x);
#ifndef SWIG
SBW_API void SBWFreeSBWSignatureElementArray(SBWInteger size, SBWSignatureElement **inArray);
SBW_API void SBWFreeStringArray(SBWInteger size, char **);
SBW_API void SBWFreeString2DArray(SBWInteger xSize, SBWInteger ySize, char ***);
SBW_API void SBWFreeSBWDataBlockWriterArray(SBWInteger size, SBWDataBlockWriter **);
SBW_API void SBWFreeSBWDataBlockWriter2DArray(SBWInteger xSize, SBWInteger ySize, SBWDataBlockWriter ***);
SBW_API void SBWFreeSBWDataBlockReaderArray(SBWInteger size, SBWDataBlockReader **);
SBW_API void SBWFreeSBWDataBlockReader2DArray(SBWInteger xSize, SBWInteger ySize, SBWDataBlockReader ***);
#endif

SBW_API char *SBWGetVersion();
SBW_API char *SBWGetBrokerVersion();

SBW_API char *SBWExceptionGetMessage();
SBW_API char *SBWExceptionGetDetailedMessage();
SBW_API SBWInteger SBWExceptionGetCode();
SBW_API void SBWSetException(const char *userMessage, const char *detailedMessage);
SBW_API void SBWExceptionClear(); 

#ifdef WIN32
SBW_API void SBWWindowsExtractCommandLine(int *argc, char ***argv);
#endif

SBW_API SBWBoolean SBWConnect();
SBW_API SBWBoolean SBWLink(const char *);
SBW_API SBWBoolean SBWConnectOnHost(const char *hostname);
SBW_API char *SBWCalculateCommandLine(const char *argZero);
SBW_API SBWInteger SBWGetModuleInstance(const char *moduleIdentificationName);
SBW_API void SBWDisconnect();
SBW_API void SBWSignalDisconnect();
SBW_API void SBWWaitForDisconnect();

#ifndef SWIG
SBW_API void SBWFindServicesA(
	const char *serviceCategory, SBWInteger *numberOfServices, SBWBoolean recursive, SBWServiceDescriptor* oReturn);
SBW_API SBWServiceDescriptor* SBWFindServices(
	const char *serviceCategory, SBWInteger *numberOfServices, SBWBoolean recursive);
SBW_API SBWServiceDescriptor* SBWFindLocalServices(
	const char *serviceCategory, SBWInteger *numberOfServices, SBWBoolean recursive);
SBW_API SBWBoolean SBWGetServiceInModuleInstance(
	SBWServiceDescriptor *service, SBWInteger *moduleIdentifier, SBWInteger *serviceIdentifier);
SBW_API char ** SBWGetServiceCategories(const char *serviceCategory, SBWInteger *numberOfCategories);
SBW_API SBWInteger *SBWGetExistingNamedModuleInstances(const char *moduleIdName, SBWInteger *numberOfModuleInstances);
SBW_API SBWInteger *SBWGetExistingModuleInstances(SBWInteger *numberOfModuleInstances);
SBW_API void SBWGetModuleDescriptorsA(SBWBoolean includeRunning, SBWBoolean localOnly, SBWInteger *numberOfDescriptors, SBWModuleDescriptor *oReturn);
SBW_API SBWModuleDescriptor *SBWGetModuleDescriptors(SBWBoolean includeRunning, SBWBoolean localOnly, SBWInteger *numberOfDescriptors);
SBW_API void SBWGetServiceDescriptorsA(const char *moduleIdName, SBWInteger *numberOfDescriptors, SBWServiceDescriptor *oReturn);
SBW_API SBWServiceDescriptor *SBWGetServiceDescriptors(const char *moduleIdName, SBWInteger *numberOfDescriptors);
#endif /* SWIG */
SBW_API void SBWGetModuleDescriptorA(const char *moduleIdName, SBWBoolean includeRunning, SBWModuleDescriptor *oReturn);
SBW_API SBWModuleDescriptor *SBWGetModuleDescriptor(const char *moduleIdName, SBWBoolean includeRunning);
SBW_API void SBWRegisterShutdownListener(SBWSimpleListener);
SBW_API void SBWRegisterModuleShutdownListener(SBWModuleListener);
SBW_API void SBWRegisterModuleStartupListener(SBWModuleListener);
SBW_API void SBWRegisterRegistrationChangeListener(SBWSimpleListener);
SBW_API void SBWRemoveShutdownListener(SBWSimpleListener);
SBW_API void SBWRemoveModuleShutdownListener(SBWModuleListener);
SBW_API void SBWRemoveModuleStartupListener(SBWModuleListener);
SBW_API void SBWRemoveRegistrationChangeListener(SBWSimpleListener);

SBW_API void SBWRegisterShutdownListenerA(SBWSimpleListenerStdCall);
SBW_API void SBWRegisterModuleShutdownListenerA(SBWModuleListenerStdCall);
SBW_API void SBWRegisterModuleStartupListenerA(SBWModuleListenerStdCall);
SBW_API void SBWRegisterRegistrationChangeListenerA(SBWSimpleListenerStdCall);
SBW_API void SBWRemoveShutdownListenerA(SBWSimpleListenerStdCall);
SBW_API void SBWRemoveModuleShutdownListenerA(SBWModuleListenerStdCall);
SBW_API void SBWRemoveModuleStartupListenerA(SBWModuleListenerStdCall);
SBW_API void SBWRemoveRegistrationChangeListenerA(SBWSimpleListenerStdCall);

SBW_API SBWInteger SBWGetThisModule();
SBW_API SBWBoolean SBWGetConnectionStatus();

SBW_API SBWBoolean SBWCreateModuleImpl(
	const char *uniqueName, const char *nameForDisplay, SBWModuleManagementType type, const char *help);
SBW_API SBWBoolean SBWFreeModuleImpl();
SBW_API SBWBoolean SBWModuleImplSetCommandLine(const char *);
SBW_API SBWBoolean SBWModuleImplSetHost(char* host);
SBW_API SBWBoolean SBWModuleImplAddService(
	const char *serviceName, const char *serviceDisplayName, const char *category, const char *help);
SBW_API SBWBoolean SBWModuleImplSetHandler(
	const char *serviceName, SBWHandler, void *userData, const char *signature, SBWBoolean synchronized, const char *help);
SBW_API SBWBoolean SBWModuleImplSetHandlerA(
	const char *serviceName, SBWCHandler, int nService, int nMethod, const char *signature, SBWBoolean synchronized, const char *help);
SBW_API SBWBoolean SBWModuleImplEnableServices();
SBW_API SBWBoolean SBWModuleImplRegister();
SBW_API SBWBoolean SBWModuleImplRun(int argc, char* argv[], SBWBoolean waitForDisconnect);
SBW_API SBWBoolean SBWModuleImplRunA();
SBW_API SBWHandler SBWModuleImplGetHandler(const char *serviceName, const char *methodName, void **userData);

SBW_API SBWInteger SBWModuleGetNumberOfServices(SBWInteger moduleInstanceId);
SBW_API SBWInteger SBWModuleFindServiceByName(SBWInteger moduleInstanceId, const char *serviceName);
SBW_API void SBWModuleGetModuleDescriptorA(SBWInteger moduleInstanceId, SBWModuleDescriptor *oReturn);
SBW_API SBWModuleDescriptor *SBWModuleGetModuleDescriptor(SBWInteger moduleInstanceId);
#ifndef SWIG
SBW_API SBWInteger *SBWModuleFindServicesByCategory(SBWInteger moduleInstanceId, const char *category, SBWInteger *numberOfServices);
#endif
SBW_API SBWBoolean SBWModuleShutdown(SBWInteger moduleInstanceId);

SBW_API SBWInteger SBWServiceGetMethod(
	SBWInteger moduleInstanceId, SBWInteger serviceId, const char *signature);
SBW_API void SBWServiceGetDescriptorA(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWServiceDescriptor *oReturn);

SBW_API SBWServiceDescriptor *SBWServiceGetDescriptor(
	SBWInteger moduleInstanceId, SBWInteger serviceId);
SBW_API SBWInteger SBWServiceGetNumberOfMethods(SBWInteger moduleInstanceId, SBWInteger serviceId);	

#ifndef SWIG
SBW_API SBWBoolean SBWMethodCall(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const char *signature, ...);
SBW_API SBWBoolean SBWMethodSend(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const char *signature, ...);
#endif

SBW_API SBWDataBlockReader *SBWMethodCallUsingDataBlock(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, SBWDataBlockWriter *args);
SBW_API SBWBoolean SBWMethodSendUsingDataBlock(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, SBWDataBlockWriter *args);
SBW_API char *SBWMethodGetSignatureString(SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger methodId);
SBW_API char *SBWMethodGetName(SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger methodId);
SBW_API char *SBWMethodGetHelp(SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger methodId);

SBW_API void SBWMethodGetSignatureA(SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger methodId, SBWSignature *oReturn);
SBW_API SBWSignature *SBWMethodGetSignature(SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger methodId);

#ifndef SWIG
SBW_API SBWSignatureElement **SBWSignatureGetArguments(SBWSignature *, SBWInteger *numberOfArguments);
#endif
SBW_API SBWSignatureType *SBWSignatureGetReturnType(SBWSignature *signature);
SBW_API char *SBWSignatureGetName(SBWSignature *);

SBW_API char *SBWSignatureElementGetName(SBWSignatureElement *);
SBW_API SBWSignatureType *SBWSignatureElementGetType(SBWSignatureElement *);

SBW_API SBWDataBlockType SBWSignatureTypeGetType(SBWSignatureType *);
SBW_API SBWSignatureType *SBWSignatureTypeGetArrayInnerType(SBWSignatureType *type);
SBW_API SBWInteger SBWSignatureTypeGetArrayDimensions(SBWSignatureType *);

#ifndef SWIG
SBW_API SBWSignatureElement **SBWSignatureTypeGetListContents(SBWSignatureType *type, SBWInteger *numberOfArguments);
#endif

SBW_API SBWDataBlockWriter *SBWCreateSBWDataBlockWriter();

#ifndef SWIG
SBW_API SBWBoolean SBWWrite(SBWDataBlockWriter *, const char *argumentList, ...);
SBW_API SBWBoolean SBWRead(SBWDataBlockReader *, const char *argumentList, ...);
#endif

SBW_API SBWDataBlockType SBWGetNextType(SBWDataBlockReader *);
SBW_API SBWDataBlockType SBWGetNextArrayType(SBWDataBlockReader *);
SBW_API SBWInteger SBWGetNextArrayDimensions(SBWDataBlockReader *);

SBW_API void SBWSetDebugTrace(SBWBoolean traceOn);

#ifndef SWIG
SBW_API void SBWDebugTrace(const char *text, const char *file, int line);
SBW_API SBWBoolean SBWProcessArguments(int argc, char* argv[]);
SBW_API void SBWMethodCallRawA(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const unsigned char *rawDataArgs, unsigned char *oReturn, SBWInteger *rawDataLength);
SBW_API unsigned char *SBWMethodCallRaw(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const unsigned char *rawDataArgs, SBWInteger *rawDataLength);
SBW_API SBWBoolean SBWMethodSendRaw(
	SBWInteger moduleInstanceId, SBWInteger serviceId, SBWInteger method, const unsigned char *rawDataArgs, SBWInteger rawDataLength);
SBW_API void SBWWriteRaw(SBWDataBlockWriter *, const unsigned char *rawData, SBWInteger rawDataLength);
SBW_API unsigned char *SBWReadRaw(SBWDataBlockReader *, SBWInteger *rawDataLength);
#endif

SBW_API SBWDataBlockReader *SBWGetReaderFromWriter(SBWDataBlockWriter *);

SBW_API void *SBWMalloc(SBWInteger, SBWInteger);

/**
 * object that manages all incoming calls
 */
typedef struct sbwIncomingCallThreadData SBWIncomingCallThreadData ;

/**
 * function that creates threads that process incoming calls.
 * Must call SBWSetException if thread creation fails for any reason.
 * @param x object that manages all incoming calls must be passed to 
 *          SBWProcessIncomingCalls
 */
typedef void (*SBWIncomingCallThreadCreator) (SBWIncomingCallThreadData *x);

SBW_API void SBWRegisterIncomingCallThreadCreator(SBWIncomingCallThreadCreator x);

SBW_API void SBWProcessIncomingCalls(SBWIncomingCallThreadData *x);

#ifdef __cplusplus
}
#endif

#endif /* SBWC */
