
/**
 * @file SBWLowLevel.h
 * @brief class definition of SBWLowLevel - monitors events occuring in SBW on behalf of the C API.
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

/// MSVC generated include loop block
#if !defined(AFX_SBWLOWLEVEL_H__5DD4F430_23B1_4E11_ABBA_DDA7FE35B0A2__INCLUDED_)
#define AFX_SBWLOWLEVEL_H__5DD4F430_23B1_4E11_ABBA_DDA7FE35B0A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/SBWRPC.h>
#include <SBW/DataBlockWriterClass.h>
#include <SBW/ImplDataBlockReader.h>
#include <SBW/ImplDataBlockWriter.h>
#include <SBW/ModuleDescriptor.h>
#include <SBW/ServiceDescriptor.h>
#include <SBW/Module.h>
#include <SBW/Method.h>
#include <SBW/SignatureElement.h>
#include <SBW/SBWListenerConsolidator.h>

namespace SystemsBiologyWorkbench
{

class Receiver ;
class SBWListenerForC ;

/// common implementation of API functionality used by both C and C++ APIs
class SBW_API SBWLowLevel
{
public:
    static bool processArguments(int argc, char* argv[]);

    static bool isSubCategory(std::string category, std::string subCategory);
    static void setReceiver(Receiver *r);

    static void connect(const char *moduleIdentificationName = "", const char *hostname = "");
    static void link(const char *hostname);
    static Integer getModuleInstance(const char *moduleIdentificationName);
    static bool isConnected();
    static void disconnect();

    /// notify the broker that this module wishes to disconnect.
    static void signalDisconnect();

    static void waitForDisconnect();
    static std::vector<ModuleDescriptor> *getModuleDescriptors(bool includeRunning, bool localOnly);
    static std::vector<ServiceDescriptor> *getServiceDescriptors(const char *moduleIdentificationName);
    static ModuleDescriptor getModuleDescriptor(std::string moduleName, bool includeRunning);
    static std::vector<ServiceDescriptor> *findServices(std::string category, bool recursive = true);
    static std::vector<ServiceDescriptor> *findLocalServices(std::string category, bool recursive = true);
    static std::vector<std::string> *getServiceCategories(std::string category);
    static std::vector<Module> *getExistingModuleInstances();
    static std::vector<Module> *getExistingModuleInstances(std::string moduleName);

    static void addListener(SBWListener *x) ;
    static void removeListener(SBWListener *x) ;
    static void registerShutdownListener(SimpleListener);
    static void registerModuleShutdownListener(ModuleListener);
    static void registerModuleStartupListener(ModuleListener);
    static void registerRegistrationChangeListener(SimpleListener);
    static void removeShutdownListener(SimpleListener);
    static void removeModuleShutdownListener(ModuleListener);
    static void removeModuleStartupListener(ModuleListener);
    static void removeRegistrationChangeListener(SimpleListener);

    static void registerShutdownListenerStdCall(SimpleListenerStdCall);
    static void registerModuleShutdownListenerStdCall(ModuleListenerStdCall);
    static void registerModuleStartupListenerStdCall(ModuleListenerStdCall);
    static void registerRegistrationChangeListenerStdCall(SimpleListenerStdCall);
    static void removeShutdownListenerStdCall(SimpleListenerStdCall);
    static void removeModuleShutdownListenerStdCall(ModuleListenerStdCall);
    static void removeModuleStartupListenerStdCall(ModuleListenerStdCall);
    static void removeRegistrationChangeListenerStdCall(SimpleListenerStdCall);

    static Integer getThisModule();

    static Integer moduleGetNumberOfServices(Integer moduleInstanceId);
    static Integer moduleFindServiceByName(Integer moduleInstanceId, const char *serviceName);
    static ModuleDescriptor moduleGetModuleDescriptor(Integer moduleInstanceId);
    static void moduleShutdown(Integer moduleInstanceId);

    static Integer serviceGetMethod(
            Integer moduleInstanceId, Integer serviceId, const char *signature);
    static ServiceDescriptor serviceGetDescriptorHandle(
            Integer moduleInstanceId, Integer serviceId);
    static Integer serviceGetNumberOfMethods(Integer moduleInstanceId, Integer serviceId);

    static Signature methodGetSignature(Integer moduleInstanceId, Integer serviceId, Integer methodId);
    static char *methodGetName(Integer moduleInstanceId, Integer serviceId, Integer methodId);
    static char *methodGetHelp(Integer moduleInstanceId, Integer serviceId, Integer methodId);
    static std::string methodGetSignatureInString(Integer moduleInstanceId, Integer serviceId, Integer methodId);
    static char *methodGetSignatureInCharStar(Integer moduleInstanceId, Integer serviceId, Integer methodId);
    static std::string methodGetNameInString(Integer moduleInstanceId, Integer serviceId, Integer methodId);
    static std::string methodGetHelpInString(Integer moduleInstanceId, Integer serviceId, Integer methodId);
    static DataBlockReader methodCall(
            Integer moduleInstanceId, Integer serviceId, Integer method, DataBlockWriter args);
    static void methodSend(
            Integer moduleInstanceId, Integer serviceId, Integer method, DataBlockWriter args);

    static char *resultStringForC(const char *);

    static ServiceMethod getBrokerMethod(const char *signature);
    static ServiceMethod getBrokerMethod(brokerMethods method);

    static Integer *mapModuleArrayForC(std::vector<Module> *modules, Integer *numberOfModuleInstances);
    static sbwSignatureElement **mapSignatureElements(std::vector<SignatureElement> *, Integer *);

    static std::string getVersion();
    static std::string getBrokerVersion();

    static SBWRPCListener *getListenerConsolidator();
    static void setMasterListener(SBWRPCListener *);
    static void setRPC (SBWRPC *oRPC);

private:
    static void initialiseListenerForC();
    static void deleteListenerForC();

    static SBWRPC * initializeRPC();
    static void deleteRPC();


    static std::vector<Module> *createModuleArray(const std::vector<Integer>&moduleIds);

    /// underlying RPC implementation
    static SBWRPC *rpc ;

    static SBWListenerForC *listenerForC ;
    static const std::string version ;
    static SBWListenerConsolidator listenerConsolidator ;
};
} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWLOWLEVEL_H__5DD4F430_23B1_4E11_ABBA_DDA7FE35B0A2__INCLUDED_)
