/**
 * @file ModuleImpl.cpp
 * @brief the implementation of services in a module
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ModuleImpl.cpp,v 1.8 2008/07/25 00:01:38 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/ModuleImpl.cpp,v $
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

// ModuleImpl.cpp: implementation of the ModuleImpl class.
//
//////////////////////////////////////////////////////////////////////

#if  _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE
#pragma comment(lib,"ADVAPI32.LIB")
#endif

#include <string.h>
#include <iostream>
#include "stdafx.h"
#include "ModuleImpl.h"
#include "sbwenums.h"

#include "SBWLowLevel.h"
#include "ServiceImpl.h"
#include "DoNothingReceiver.h"
#include "SBWServiceNotFoundException.h"
#include "SBWMethodNotFoundException.h"
#include "MethodImpl.h"
#include "MethodInline.h"
#include "SBWApplicationException.h"
#include "MethodTable.h"

#ifdef LINUX
#include <unistd.h>
//fixme was <linux/limits.h>
#include <limits.h>
#endif

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * recovers the mempry allocated by this object
 */
ModuleImpl::~ModuleImpl()
{
	if (commandLine != NULL)
		free(commandLine);

	free(uniqueName);
	free(displayName);
	free(help);

	std::vector<ServiceImpl *>::iterator itr= services.begin();

	while (itr != services.end())
	{
		delete *itr ;
		itr++ ;
	}

	if (mode == ModuleMode)
		SBWLowLevel::setReceiver(new DoNothingReceiver());
}

/// Blocks until the connection between this module and the broker is broken or disconnected
void ModuleImpl::waitForDisconnect()
{
	SBWLowLevel::waitForDisconnect();
}

/** 
 * creates a new module implementation given information specific to the whole module.
 * @param un unique identification string for this module
 * @param dn humanly readable name for this module
 * @param t module management type of this module
 * @param h help string for this module
 */
#if  _MSC_VER >= 1400
ModuleImpl::ModuleImpl(std::string un, std::string dn, ModuleManagementType t, std::string h)
: uniqueName(_strdup(un.c_str())),
	displayName(_strdup(dn.c_str())),
	type(t),
	commandLine(NULL),
	help(_strdup(h.c_str())),
	host(),
	mode(NormalMode)
#else
ModuleImpl::ModuleImpl(std::string un, std::string dn, ModuleManagementType t, std::string h)
: uniqueName(strdup(un.c_str())),
	displayName(strdup(dn.c_str())),
	type(t),
	commandLine(NULL),
	help(strdup(h.c_str())),
	host(),
	mode(NormalMode)
#endif
{
	
}
#if  _MSC_VER >= 1400
ModuleImpl::ModuleImpl(const char* un, const char* dn, ModuleManagementType t, const char* h)
: uniqueName(_strdup(un)),
	displayName(_strdup(dn)),
	type(t),
	commandLine(NULL),
	help(_strdup(h)),
	host(),
	mode(NormalMode)
#else
ModuleImpl::ModuleImpl(const char* un, const char* dn, ModuleManagementType t, const char* h)
: uniqueName(strdup(un)),
	displayName(strdup(dn)),
	type(t),
	commandLine(NULL),
	help(strdup(h)),
	host(),
	mode(NormalMode)

#endif
{
	
}
/**
 * sets the command to start this module to cmd
 * @param cmd the command to start this module
 */
void ModuleImpl::setCommandLine(std::string cmd)
{
#if  _MSC_VER >= 1400
	commandLine = _strdup(cmd.c_str());
#else
	commandLine = strdup(cmd.c_str());
#endif
}

/**
 * creates a new service implementation on this module given information specific to the whole service.
 * @param serviceName unique identification string for this service (unique amongst other services on the module).
 * @param serviceDisplayName humanly readable name for the service.
 * @param category specific category (classification) that this service resides in.
 * @param help help string for this service.
 */
void ModuleImpl::addService(
	std::string serviceName, std::string serviceDisplayName, std::string category, std::string help)
{
	std::string sServiceName(serviceName);
	std::string sServiceDisplayName(serviceDisplayName);
	std::string sCategory (category);
	std::string sHelp(help);
	services.push_back(new ServiceImpl(sServiceName, sServiceDisplayName, sCategory, sHelp));
	//services.push_back(new ServiceImpl(serviceName, serviceDisplayName, category, help));
}

void ModuleImpl::addService(
	const char* serviceName, const char* serviceDisplayName, const char* category, const char* help)
{
#if (_MSC_VER > 1400)
	services.push_back(new ServiceImpl(_strdup(serviceName), _strdup(serviceDisplayName), _strdup(category), _strdup(help)));
#else
	services.push_back(new ServiceImpl(strdup(serviceName), strdup(serviceDisplayName), strdup(category), strdup(help)));
#endif
}

/**
 * connect to the broker indicating that this module is now providing services to other modules.
 * incoming method calls are serviced in seperate threads.  Terminates as soon as theis module
 * is connected.
 */
void ModuleImpl::enableModuleServices()
{
	mode = ModuleMode ;

	// TODO review how this connect stuff works
	// needs to be more flexiable
	// if aleady connected change module name!

	// changed the order of the following functions - AMF 7th Aug 02
	SBWLowLevel::setReceiver(this);
	SBWLowLevel::connect(uniqueName, host);
}
void ModuleImpl::runOnHost(char *sHost)
{
#if  _MSC_VER >= 1400
	host = _strdup(sHost);
#else
	host = strdup(sHost);
#endif
}
/**
 * pass information on this module to the broker to be stored in the broker's persistant registry of module data.
 */
void ModuleImpl::registerModule()
{
	mode = RegisterMode ;

	if (!SBWLowLevel::isConnected())
		SBWLowLevel::connect();

	if (commandLine == NULL)
		throw new SBWApplicationException("command line not set on call to register module");

 	//SBWLowLevel::getBrokerMethod(
 	//	"void registerModule(string,string,int,string, string)").call(
 	//		DataBlockWriter() << (char *)uniqueName << displayName << (int)type << commandLine << help);
 	SBWLowLevel::getBrokerMethod(
		RegisterModule).call(
 			DataBlockWriter() << (char *)uniqueName << displayName << (int)type << commandLine << help);

	std::vector<ServiceImpl *>::iterator itr = services.begin();
	//ServiceMethod registerService =
	//	SBWLowLevel::getBrokerMethod(
	//		"void registerService(string,string,string,string, string)");
	ServiceMethod registerService =
		SBWLowLevel::getBrokerMethod(
		RegisterService);

	while (itr != services.end())
	{
		ServiceImpl *service = *itr ;
		registerService.call(
			DataBlockWriter() <<
				uniqueName <<
				service->getName() <<
				service->getDisplayName() <<
				service->getCategory() <<
				service->getHelp()); 

		itr++ ;
	}
}

/**
 * return the ServiceImpl object that implements the service with the unique name given by serviceName.
 * @param serviceName the unique name of the required service.
 * @return the ServiceImpl object that implements the service with the unique name given by serviceName.
 */
ServiceImpl *ModuleImpl::getService(std::string serviceName)
{
	std::vector<ServiceImpl *>::iterator itr = services.begin();

	while (itr != services.end())
	{
		if ((*itr)->getName().compare(serviceName) == 0)
			return *itr ;

		itr++ ;
	}

	std::string message("service ");

	message += serviceName ;
	message += " not found";
	throw new SBWServiceNotFoundException(message);
}

/**
 * adds a method to a given service on this module. 
 * @param serviceName the unique identifier string for the service beign extended.
 * @param handler the object implementing this method.
 * @param signature the SBW signature for this method.
 * @param synchronized indicates whether this method should be synchronized - default false.
 * @param help help string for this method
 */
void ModuleImpl::setHandler(
	std::string serviceName, Handler *handler, std::string signature, bool synchronized, std::string help)
{
	std::string sServiceName(serviceName);
	std::string sSignature(signature);
	std::string sHelp(help);
	getService(sServiceName)->addMethod(handler, sSignature, synchronized, sHelp);
	//getService(serviceName)->addMethod(handler, signature, synchronized, help);
}

/**
 * returns the Handler object corresponding to the given method.
 * @param serviceName the unique service identifier containing the method.
 * @param methodSignature 
 */
Handler *ModuleImpl::getHandler(std::string serviceName, std::string methodSignature)
{
	return getService(serviceName)->getHandler(methodSignature);
}

/**
 * returns the ServiceImpl object corresponding to a given numeric service identifier.
 * @param queryServiceId the numeric identifier of the required service
 * @return the ServiceImpl object corresponding to the given numeric service identifier.
 */
ServiceImpl *ModuleImpl::getService(Integer queryServiceId)
{
	Integer nServices = services.size();
	if (queryServiceId >= 0 && queryServiceId < nServices )
		return services[queryServiceId];
	else
		throw new SBWServiceNotFoundException("unknown service id");
}

/** 
 * process a call/send message that require implementation on this module.
 * @throws SBWException * if unsupported services or methods are called or the invoked method throws an application error.
 * @param fromModuleId numeric module identifier for the mdoule that sent the message.
 * @param service numeric service identifier of the requested service.
 * @param method numeric method identifier of the requested method.
 * @param args method argument data.
 * @return result data block.
 */
DataBlockWriter ModuleImpl::receive(
		Integer fromModuleId, Integer serviceId, Integer methodId, DataBlockReader args)
{
	if (serviceId == SBWSystemService)
	{
		DataBlockWriter writer ;

		switch (methodId)
		{
			case SBWGetServicesMethod:
				{
					std::vector<std::string> serviceNames(services.size());
					unsigned int i = 0 ;

					while (i != services.size())
					{
						serviceNames[i] = services[i]->getName();
						i++;
					}

					writer << serviceNames ;
				}
				return writer ;

			case SBWGetMethodsMethod :
				{
					Integer queryServiceId ;

					args >> queryServiceId ;

					getService(queryServiceId)->addSignatures(writer);
				}
				return writer ;

			case SBWShutdownMethod :
				SBWLowLevel::signalDisconnect();
				return writer ;

			case SBWGetMethodHelpMethod :
				{
					int requestedServiceId, requestedMethodId ;

					args >> requestedServiceId >> requestedMethodId ;
					writer << getService(requestedServiceId)->getMethod(requestedMethodId)->getHelp();
				}
				return writer ;

			default : 
				throw new SBWMethodNotFoundException("method not found");
		}
	}
	else 
		return getService(serviceId)->getMethod(methodId)->invoke(Module(fromModuleId), args);	
}

/**
 * returns false - indicates that this object can't be deleted when SBWRPC replaces it with another receiver.
 * @return false
 */
bool ModuleImpl::canDelete()
{
	return false ;
}

/**
 * Convenience function for performing the tasks required for handling the variuos modes in which a module can run.
 * @param mode mode in which we want to run the module.
 * @param wait in RegisterMode indicates whether this function should block until the module is disconnected from the broker.
 * @param argZero the first string on the command line that invoked this module usually a reference to this excutable.
 */
void ModuleImpl::run(ModuleModeType mode, bool wait, const char *argZero)
{
	switch (mode)
	{
		case RegisterMode:
			if (commandLine == NULL)
				commandLine = calculateCommandLineRaw(argZero);

			registerModule();
			SBWLowLevel::disconnect();
			break;

		case ModuleMode:
			enableModuleServices();

			if (wait)
				waitForDisconnect();
			break;

		case NormalMode:
		    /* We do nothing in this case. */
		        break;
	}
}

/**
 * Convenience function for performing the tasks required for handling
 * the <code>-sbwdebug</code>, <code>-sbwregister</code> and <code>-sbwmodule</code> options
 * to a module.  This method must be passed the array of command-line
 * arguments passed to the application.  This then checks for the
 * presence of <code>-sbwdebug</code>, <code>-sbwregister</code> and <code>-sbwmodule</code>,
 * and acts as follows:
 * <dl>
 * <dt><code>-sbwregister</code>
 * <dd>Runs <code>ModuleImpl.registerModule()</code>
 * <dt><code>-sbwmodule</code>
 * <dd>Runs <code>ModuleImpl.enableModuleServices()</code>
 * <dt><code>-sbwdebug</code>
 * <dd>Runs <code>SBWDebug::setTraceMode()</code>
 * </dl>
 * The return behavior of this method depends on whether the optional
 * third argument, <code>wait</code>, is <code>true</code> or
 * <code>false</code>.  If <code>true</code> (the default), it waits
 * until SBW is disconnected.  If <code>false</code>, it returns
 * immediately.
 * The calling routine should perform whatever tasks it would after
 * enabling module services.
 * <p>
 * @param argc an integer indicate the number of arguments passed to
 * the program on the command line
 * @param argv an array of strings comprising the set of arguments
 * passed to the program on the command line
 * @param wait (optional) boolean argument indicating whether to wait
 * until SBW is disconnected, default value is true.
 **/
void ModuleImpl::run(int argc, char* argv[], bool wait /* = true */)
{
	int i = 0;
	char *argZero = argv[0];
	
	if (SBWLowLevel::processArguments(argc, argv))
	{
		while (i != argc)
		{
			if (strcmp(argv[i], "-sbwregister") == 0)
				run(RegisterMode, wait, argZero);
			else if (strcmp(argv[i], "-sbwmodule") == 0)
				run(ModuleMode, wait, argZero);

			i++;
		}
	}
}

/** 
 * returns the default line that can be used to invoke this application in module mode.
 * Assumes that the application can be invoked in module mode with only one argument <code>-sbwmodule</code>.
 * @return the default line that can be used to invoke this application in module mode.
 */
std::string ModuleImpl::calculateCommandLine(std::string argZero)
{
	char *cmdLine = calculateCommandLineRaw(argZero.c_str());
	std::string result(cmdLine);

	delete[] cmdLine ;

	return result ;
}

/**
 * changes s so that it is terminated at the first carriage return of its original value.
 * Has no effect if s contains no carriage returns.
 * @param s string to be modified
 * @return s - as modified
 */
char *stripcr(char *s)
{
    char *ptr;

    for (ptr = s; *ptr != '\0' && *ptr != '\n'; ptr++)
	;
    *ptr = '\0';

    return s;
}

/** 
 * returns the default line that can be used to invoke this application in module mode.
 * Assumes that the application can be invoked in module mode with only one argument <code>-sbwmodule</code>.
 * @return the default line that can be used to invoke this application in module mode - should freed using <code>delete[]</code>
 */
char *ModuleImpl::calculateCommandLineRaw(const char *argZero)
{
#if defined(WIN32)

	char *lpFilePart ;
	const char *format = "\"%s\" -sbwmodule";
	char buffer[MAX_PATH + 1];
	char *result = new char[MAX_PATH + strlen(format) - 1];

	// First try to find it with the current extension.

	if (SearchPath(
			NULL, // indicates to search for exes
			argZero,  // pointer to filename
			"", // pointer to extension
			MAX_PATH + 1, // size, in characters, of buffer
			buffer,     // pointer to buffer for found filename
			&lpFilePart))   // pointer to pointer to file component
	{
		sprintf(result, format, buffer);
		return result ;
	}

	// runthro extensions on PATHEXT
	char *pathExt = getenv("PATHEXT");
	char *ext = strtok(pathExt, ";");

	while (ext != NULL)
	{
		if (SearchPath(
				NULL, // indicates to search for exes
				argZero,  // pointer to filename
				ext, // pointer to extension
				MAX_PATH + 1, // size, in characters, of buffer
				buffer,     // pointer to buffer for found filename
				&lpFilePart))   // pointer to pointer to file component
		{
			sprintf(result, format, buffer);
			return result ;
		}

		ext = strtok(NULL, ";");
	}

	SBWOS::ThrowError();

#else

	const char *addedArg = "-sbwmodule";
	char *result = new char[PATH_MAX];
	char tmpwd[PATH_MAX];

	// Arg zero may be a relative path or a full path.  If it's a relative
	// path, we have to prepend the current directory path.

	if (argZero != NULL && argZero[0] != '/')
	{
	    if ((char *) getcwd(tmpwd, (size_t)PATH_MAX) == NULL)
	    {
		TRACE("Unable to get the current working directory.\n");
		sprintf(result, "%s %s", argZero, addedArg);
	    }
	    else
		sprintf(result, "%s/%s %s", stripcr(tmpwd), argZero, addedArg);
	}
	else				// Absolute path -- use it directly.
	{
	    sprintf(result, "%s %s", argZero, addedArg);
	}

	TRACE("Startup command: " << result);
	return result;

#endif

	return NULL ;
}
