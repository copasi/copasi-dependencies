/**
 * @file SBWHighLevel.h
 * @brief (contains SystemsBiologyWorkbench::SBW class) static class for accessing the C++ OO API
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
#ifndef SBWHIGHLEVEL
#define SBWHIGHLEVEL

#include <SBW/Module.h>

namespace SystemsBiologyWorkbench
{

/// top level class for accessing the C++ OO API
class SBW
{
public :
	static std::string getVersion();
	static std::string getBrokerVersion();
	static void addListener(SBWListener *);
	static void removeListener(SBWListener *);
	static void connect();
	static void connect(std::string hostname);
	static Module getModuleInstance(std::string moduleName);
	static std::vector<ModuleDescriptor> *getModuleDescriptors(bool includeRunningModules = false, bool localOnly = false);
	static ModuleDescriptor getModuleDescriptor(std::string moduleName, bool includeRunning = false);
	static void disconnect();

	/// notify the broker that this module wishes to disconnect.
	static void signalDisconnect() { SystemsBiologyWorkbench::SBWLowLevel::signalDisconnect(); }

	static std::vector<ServiceDescriptor> *findServices(
		std::string serviceCategory, bool recursive = true);
	static std::vector<std::string> *getServiceCategories(std::string category);
	static std::vector<Module> *getExistingModuleInstances();
	static std::vector<Module> *getExistingModuleInstances(std::string moduleName);
	static Module getThisModule();
	static void waitForDisconnect();
	static bool processArguments(int argc, char* argv[]);
	static void link(std::string hostname);
};
} // SystemsBiologyWorkbench

/**
 * returns an object representing this module instance.
 * @return an object representing this module instance.
 */
inline SystemsBiologyWorkbench::Module SystemsBiologyWorkbench::SBW::getThisModule()
{
	return SystemsBiologyWorkbench::Module(SystemsBiologyWorkbench::SBWLowLevel::getThisModule());
}

/**
 * returns a module descriptor for a given module.
 * @param moduleName module identification string.
 * @param includeRunning whether a module descriptor for a module which isn't registered but is running will be returned.
 * @return module descriptor for the given module
 */
inline SystemsBiologyWorkbench::ModuleDescriptor SystemsBiologyWorkbench::SBW::getModuleDescriptor(
	std::string moduleName, bool includeRunning)
{
	return SystemsBiologyWorkbench::SBWLowLevel::getModuleDescriptor(moduleName, includeRunning);
}


/**
 * adds a given listener object to the C++ API to receive notification of SBW events.
 * @param x listener object
 */
inline void SystemsBiologyWorkbench::SBW::addListener(SBWListener *x)
{
	SystemsBiologyWorkbench::SBWLowLevel::addListener(x);
}

/**
 * removes a listener object from the C++ API.
 * @param x listener object
 */
inline void SystemsBiologyWorkbench::SBW::removeListener(SBWListener *x)
{
	SystemsBiologyWorkbench::SBWLowLevel::removeListener(x);
}

/**
 * connect this application to the broker as a module instance of a given module
 */
inline void SystemsBiologyWorkbench::SBW::connect()
{
	SystemsBiologyWorkbench::SBWLowLevel::connect();
}

/**
 * connect this application to a remote broker as a module instance of a given module
 * @see SBW::link
 * @param hostname the hostname of machine on which you wish to connect.
 */
inline void SystemsBiologyWorkbench::SBW::connect(std::string hostname)
{
	SystemsBiologyWorkbench::SBWLowLevel::connect("", hostname.c_str());
}

/**
 * request that the broker connects to a remote broker.  This ensures
 * that the SBW environment appears to be a combination of the facilities
 * of all the linked brokers.  This is the prefered mechanism for accessing
 * remote resources.
 * @param hostname the hostname of machine on which you wish to connect.
 */
inline void SystemsBiologyWorkbench::SBW::link(std::string hostname)
{
	SystemsBiologyWorkbench::SBWLowLevel::link(hostname.c_str());
}

/**
 * Convenience function for performing the tasks required for handling
 * the <code>-sbwdebug</code> to an application.  This method must be passed the array of command-line
 * arguments passed to the application.  This method then checks for the
 * presence of <code>-sbwdebug</code> and runs <code>SBWDebug::setTraceMode()</code>.
 * Returns whether the application should either register itself or run as a module based on the command line.
 * @param argc an integer indicating the number of arguments passed to
 * the program on the command line
 * @param argv an array of strings comprising the set of arguments
 * passed to the program on the command line
 * @returns whether the command line contains either <code>-sbwregister</code> or <code>-sbwmodule</code>
 *          ie whether the application should either register itself or run as a module respectively.
 **/
inline bool SystemsBiologyWorkbench::SBW::processArguments(int argc, char* argv[])
{
	return SystemsBiologyWorkbench::SBWLowLevel::processArguments(argc, argv);
}

/**
 * disconnects this application from the broker.
 * Blocks until the disconnection has occured.
 */
inline void SystemsBiologyWorkbench::SBW::disconnect()
{
	SystemsBiologyWorkbench::SBWLowLevel::disconnect();
}

/**
 * locates an instance of a given module, creating a new module instance where necessary.
 * Whether an new module instance is created depends on the management type of the module.
 * @param moduleName module identification string
 * @return instance of module with given identification string.
 */
inline SystemsBiologyWorkbench::Module SystemsBiologyWorkbench::SBW::getModuleInstance(std::string moduleName)
{
	return SystemsBiologyWorkbench::Module(SystemsBiologyWorkbench::SBWLowLevel::getModuleInstance(moduleName.c_str()));
}

/**
 * returns module descriptors for all the modules known to the broker.
 * the result should be deallocated using delete.
 * @param includeRunning the result should include running but unregistered modules.
 * @param localOnly the result should include only those modules register with the broker on this host
 * @return module descriptors for all the modules known to the broker.
 */
inline std::vector<SystemsBiologyWorkbench::ModuleDescriptor> *SystemsBiologyWorkbench::SBW::getModuleDescriptors(
	bool includeRunning, bool localOnly)
{
	return SystemsBiologyWorkbench::SBWLowLevel::getModuleDescriptors(includeRunning, localOnly);
}

/**
 * returns an array of service descriptors that are in a given category.
 * @param categroy a service category
 * @param recursive include service in subcategories of the given categroy (default true)
 * @return an array of service descriptors that are in a given category
 */
inline std::vector<SystemsBiologyWorkbench::ServiceDescriptor> *SystemsBiologyWorkbench::SBW::findServices(
		std::string serviceCategory, bool recursive /*= true */)
{
	return SystemsBiologyWorkbench::SBWLowLevel::findServices(serviceCategory, recursive);
}

/**
 * returns the top level categories or the sub categrories of a given category.
 * the result should be deallocated using delete.
 * @param category if its empty then this function returns the top level categories
 *                 otherwise returns the sub categrories of a given category.
 */
inline std::vector<std::string> *SystemsBiologyWorkbench::SBW::getServiceCategories(std::string category)
{
	return SystemsBiologyWorkbench::SBWLowLevel::getServiceCategories(category);
}

/** 
 * returns an array of module instance objects representing all the module instances connected to the broker.
 * Deallocate the result using delete.
 * @return an array of module instance objects representing all the module instances connected to the broker.
 */
inline std::vector<SystemsBiologyWorkbench::Module> *SystemsBiologyWorkbench::SBW::getExistingModuleInstances()
{
	return SystemsBiologyWorkbench::SBWLowLevel::getExistingModuleInstances();
}

/**
 * returns an array of module instance objects representing all the instances of a given module connected to the broker.
 * Deallocate the result using delete.
 * @param moduleName module identification string
 * @return an array of module instance objects representing all the instances of the given connected to the broker.
 */
inline std::vector<SystemsBiologyWorkbench::Module> *SystemsBiologyWorkbench::SBW::getExistingModuleInstances(
	std::string moduleName)
{
	return SystemsBiologyWorkbench::SBWLowLevel::getExistingModuleInstances(moduleName);
}

/**
 * blocks until this application has disconnected from the broker.
 */
inline void SystemsBiologyWorkbench::SBW::waitForDisconnect()
{ 
	SystemsBiologyWorkbench::SBWLowLevel::waitForDisconnect();
}

/**
 * returns the version of the C & C++ library
 * @return the version of the C & C++ library
 */
inline std::string SystemsBiologyWorkbench::SBW::getVersion()
{
	return SystemsBiologyWorkbench::SBWLowLevel::getVersion();
}

/**
 * returns the version of the Broker
 * @return the version of the Broker
 */
inline std::string SystemsBiologyWorkbench::SBW::getBrokerVersion()
{
	return SystemsBiologyWorkbench::SBWLowLevel::getBrokerVersion();
}
#endif // SBWHIGHLEVEL
