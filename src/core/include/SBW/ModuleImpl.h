/**
 * @file ModuleImpl.h
 * @brief  the implementation of a module's services
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

#include <string.h>

#include <SBW/sbwdefs.h>
#ifndef SBW_STRICT_INCLUDES
#include <SBW/portableOS.h>
#endif
#include <SBW/Receiver.h>

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
	static void windowsExtractCommandLine(int *argc, char ***argv);

#endif

	SBW_API void setCommandLine(std::string);
	SBW_API void setCommandLine(const char* cmdLine);

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
