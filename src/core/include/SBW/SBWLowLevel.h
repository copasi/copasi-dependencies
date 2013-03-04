
/**
 * @file SBWLowLevel.h
 * @brief class definition of SBWLowLevel - monitors events occuring in SBW on behalf of the C API.
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWLowLevel.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWLowLevel.h,v $
 */

/*
** Copyright 2001 California Institute of Technology and
** Japan Science and Technology Corporation.
** 
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 2.1 of the License, or
** any later version.
** 
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
** MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software and
** documentation provided hereunder is on an "as is" basis, and the
** California Institute of Technology and Japan Science and Technology
** Corporation have no obligations to provide maintenance, support,
** updates, enhancements or modifications.  In no event shall the
** California Institute of Technology or the Japan Science and Technology
** Corporation be liable to any party for direct, indirect, special,
** incidental or consequential damages, including lost profits, arising
** out of the use of this software and its documentation, even if the
** California Institute of Technology and/or Japan Science and Technology
** Corporation have been advised of the possibility of such damage.  See
** the GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with this library; if not, write to the Free Software Foundation,
** Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
**
** The original code contained here was initially developed by:
**
**     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri
**     The Systems Biology Workbench Development Group
**     ERATO Kitano Systems Biology Project
**     Control and Dynamical Systems, MC 107-81
**     California Institute of Technology
**     Pasadena, CA, 91125, USA
**
**     http://www.cds.caltech.edu/erato
**     mailto:sysbio-team@caltech.edu
**
** Contributor(s):
**
*/

/// MSVC generated include loop block
#if !defined(AFX_SBWLOWLEVEL_H__5DD4F430_23B1_4E11_ABBA_DDA7FE35B0A2__INCLUDED_)
#define AFX_SBWLOWLEVEL_H__5DD4F430_23B1_4E11_ABBA_DDA7FE35B0A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SBWRPC.h"
#include "DataBlockWriterClass.h"
#include "ImplDataBlockReader.h"
#include "ImplDataBlockWriter.h"
#include "ModuleDescriptor.h"
#include "ServiceDescriptor.h"
#include "Module.h"
#include "Method.h"
#include "SignatureElement.h"
#include "SBWListenerConsolidator.h"

namespace SystemsBiologyWorkbench
{

class Receiver ;
class SBWListenerForC ;

/// common implementation of API functionality used by both C and C++ APIs
class SBWLowLevel  
{
public:
	SBW_API static bool processArguments(int argc, char* argv[]);

	SBW_API static bool isSubCategory(std::string category, std::string subCategory);
	SBW_API static void setReceiver(Receiver *r);

	SBW_API static void connect(const char *moduleIdentificationName = "", const char *hostname = "");
	SBW_API static void link(const char *hostname);
	SBW_API static Integer getModuleInstance(const char *moduleIdentificationName);
	SBW_API static bool isConnected();
	SBW_API static void disconnect();

	/// notify the broker that this module wishes to disconnect.
	SBW_API static void signalDisconnect();

	SBW_API static void waitForDisconnect();
	SBW_API static std::vector<ModuleDescriptor> *getModuleDescriptors(bool includeRunning, bool localOnly);
	SBW_API static std::vector<ServiceDescriptor> *getServiceDescriptors(const char *moduleIdentificationName);
	SBW_API static ModuleDescriptor getModuleDescriptor(std::string moduleName, bool includeRunning);
	SBW_API static std::vector<ServiceDescriptor> *findServices(std::string category, bool recursive = true);
	SBW_API static std::vector<ServiceDescriptor> *findLocalServices(std::string category, bool recursive = true);
	SBW_API static std::vector<std::string> *getServiceCategories(std::string category);
	SBW_API static std::vector<Module> *getExistingModuleInstances();
	SBW_API static std::vector<Module> *getExistingModuleInstances(std::string moduleName);

	SBW_API static void addListener(SBWListener *x) ;
	SBW_API static void removeListener(SBWListener *x) ;
	SBW_API static void registerShutdownListener(SimpleListener);
	SBW_API static void registerModuleShutdownListener(ModuleListener);
	SBW_API static void registerModuleStartupListener(ModuleListener);
	SBW_API static void registerRegistrationChangeListener(SimpleListener);
	SBW_API static void removeShutdownListener(SimpleListener);
	SBW_API static void removeModuleShutdownListener(ModuleListener);
	SBW_API static void removeModuleStartupListener(ModuleListener);
	SBW_API static void removeRegistrationChangeListener(SimpleListener);

	SBW_API static void registerShutdownListenerStdCall(SimpleListenerStdCall);
	SBW_API static void registerModuleShutdownListenerStdCall(ModuleListenerStdCall);
	SBW_API static void registerModuleStartupListenerStdCall(ModuleListenerStdCall);
	SBW_API static void registerRegistrationChangeListenerStdCall(SimpleListenerStdCall);
	SBW_API static void removeShutdownListenerStdCall(SimpleListenerStdCall);
	SBW_API static void removeModuleShutdownListenerStdCall(ModuleListenerStdCall);
	SBW_API static void removeModuleStartupListenerStdCall(ModuleListenerStdCall);
	SBW_API static void removeRegistrationChangeListenerStdCall(SimpleListenerStdCall);

	SBW_API static Integer getThisModule();

	SBW_API static Integer moduleGetNumberOfServices(Integer moduleInstanceId);
	SBW_API static Integer moduleFindServiceByName(Integer moduleInstanceId, const char *serviceName);
	SBW_API static ModuleDescriptor moduleGetModuleDescriptor(Integer moduleInstanceId);
	SBW_API static void moduleShutdown(Integer moduleInstanceId);

	SBW_API static Integer serviceGetMethod(
		Integer moduleInstanceId, Integer serviceId, const char *signature);
	SBW_API static ServiceDescriptor serviceGetDescriptorHandle(
		Integer moduleInstanceId, Integer serviceId);
	SBW_API static Integer serviceGetNumberOfMethods(Integer moduleInstanceId, Integer serviceId);	

	SBW_API static Signature methodGetSignature(Integer moduleInstanceId, Integer serviceId, Integer methodId);
	SBW_API static char *methodGetName(Integer moduleInstanceId, Integer serviceId, Integer methodId);
	SBW_API static char *methodGetHelp(Integer moduleInstanceId, Integer serviceId, Integer methodId);
	SBW_API static std::string methodGetSignatureInString(Integer moduleInstanceId, Integer serviceId, Integer methodId);
	SBW_API static char *methodGetSignatureInCharStar(Integer moduleInstanceId, Integer serviceId, Integer methodId);
	SBW_API static std::string methodGetNameInString(Integer moduleInstanceId, Integer serviceId, Integer methodId);
	SBW_API static std::string methodGetHelpInString(Integer moduleInstanceId, Integer serviceId, Integer methodId);
	SBW_API static DataBlockReader methodCall(
		Integer moduleInstanceId, Integer serviceId, Integer method, DataBlockWriter args);
	SBW_API static void methodSend(
		Integer moduleInstanceId, Integer serviceId, Integer method, DataBlockWriter args);

	SBW_API static char *resultStringForC(const char *);

	SBW_API static ServiceMethod getBrokerMethod(const char *signature);
	static ServiceMethod getBrokerMethod(brokerMethods method);

	static Integer *mapModuleArrayForC(std::vector<Module> *modules, Integer *numberOfModuleInstances);
	static sbwSignatureElement **mapSignatureElements(std::vector<SignatureElement> *, Integer *);

	SBW_API static std::string getVersion();
	SBW_API static std::string getBrokerVersion();

	static SBWRPCListener *getListenerConsolidator();
	static void setMasterListener(SBWRPCListener *);
	static void setRPC (SBWRPC *oRPC);

private:
	static void initialiseListenerForC();
	static std::vector<Module> *createModuleArray(const std::vector<Integer>&moduleIds);

	/// underlying RPC implementation
	static SBWRPC *rpc ;

	static SBWListenerForC *listenerForC ;
	static const std::string version ;
	static SBWListenerConsolidator listenerConsolidator ;
};
} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWLOWLEVEL_H__5DD4F430_23B1_4E11_ABBA_DDA7FE35B0A2__INCLUDED_)
