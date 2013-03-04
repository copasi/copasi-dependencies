/**
 * @file ModuleImpl.h
 * @brief  the implementation of a module's services
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ModuleImpl.h,v 1.3 2008/07/25 00:01:42 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ModuleImpl.h,v $
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
#if !defined(AFX_MODULEIMPL_H__C0B706AC_09B1_4572_A6A3_5696ACE71683__INCLUDED_)
#define AFX_MODULEIMPL_H__C0B706AC_09B1_4572_A6A3_5696ACE71683__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _MSC_VER > 1400
#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include "portableOS.h"
#include "Receiver.h"

namespace SystemsBiologyWorkbench
{

class Handler ;
class ServiceImpl ;
class ServiceImplementation;

template <class T> class MethodTable;

/// represents the module implementation in this module
class ModuleImpl : private Receiver 
{
public:
	SBW_API ModuleImpl(
		std::string uniqueName, std::string displayName, ModuleManagementType type, std::string help = "");
	SBW_API ModuleImpl(
		const char* uniqueName, const char* displayName, ModuleManagementType type, const char* help = "");

	SBW_API virtual ~ModuleImpl();
	
	SBW_API static std::string calculateCommandLine(std::string argZero);
	SBW_API static char *calculateCommandLineRaw(const char *argZero);

#ifdef WIN32
	/**
	 * WIN32 specific - extracts the command line that this application was launched with.
	 * @param argc referenced integer set to the number of command line arguments plus 1 - size of argc
	 * @param argv referenced pointer set to array of command line arguments including the command itself.
	 */
	static void windowsExtractCommandLine(int *argc, char ***argv)
	{ SBWOS::windowsExtractCommandLine(argc, argv); }
#endif

	SBW_API void setCommandLine(std::string);
	SBW_API void setCommandLine(const char*cmdLine) { setCommandLine(std::string(cmdLine)); } 

	SBW_API void addService(
		std::string serviceName, std::string serviceDisplayName, std::string category, std::string help = "");

	SBW_API void addService(
		const char* serviceName, const char* serviceDisplayName, const char* category, const char* help = "");


	/**
	 * adds a service implementation on this module given information specific to the whole service.
	 * @param serviceName unique identification string for this service (unique amongst other services on the module).
	 * @param serviceDisplayName humanly readable name for the service.
	 * @param category specific category (classification) that this service resides in.
	 * @param service object implementing the service
	 * @param help help string for this service.
	 */
	template<class T> void addServiceObject(
		std::string serviceName, std::string serviceDisplayName, std::string category, T *service, std::string help = "")
	{
		MethodTable<T> table(*this, service, serviceName);

		addService(serviceName, serviceDisplayName, category, help);
		service->registerMethods(table);
	}
	template<class T> void addServiceObject(
		const char* serviceName, const char* serviceDisplayName, const char* category, T *service, const char* help = "")
	{
#if _MSC_VER > 1400
		MethodTable<T> table(*this, service, _strdup(serviceName));

		addService(_strdup(serviceName), _strdup(serviceDisplayName), _strdup(category), _strdup(help));
#else
		MethodTable<T> table(*this, service, strdup(serviceName));

		addService(strdup(serviceName), strdup(serviceDisplayName), strdup(category), strdup(help));
#endif
		service->registerMethods(table);
	}


	SBW_API void enableModuleServices();
	SBW_API void registerModule();

	SBW_API void setHandler(
		std::string serviceName, Handler *handler, std::string signature,
		bool synchronized = false, std::string help = "");

	SBW_API Handler *getHandler(std::string serviceName, std::string methodSignature);
	SBW_API void waitForDisconnect();
	SBW_API void run(int argc, char* argv[], bool waitForDisconnect = true);
	SBW_API void runOnHost(char *sHost);

private:
	void run(ModuleModeType mode, bool waitForDisconnect, const char *argZero);
	ServiceImpl *getService(std::string serviceName);
	ServiceImpl *getService(Integer i);
	bool canDelete();

	DataBlockWriter receive(
		Integer fromModuleId, Integer service, Integer method, DataBlockReader args);

	/// the implementations of the services on this module instance
	std::vector<ServiceImpl *> services ;

	/// the module identification string for this module
	char *uniqueName ;

	/// the humanly readable name for this module
	char *displayName ;

	/// mechanism for creating and disconnecting instances for this module
	ModuleManagementType type ;

	/// the command line used to create instances of this module
	char *commandLine ;

	/// the help string for this module
	char *help ;

	/// the host this module should run on ... 
	char *host ;

	/// the mode in which this application is operating in.
	ModuleModeType mode ;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_MODULEIMPL_H__C0B706AC_09B1_4572_A6A3_5696ACE71683__INCLUDED_)
