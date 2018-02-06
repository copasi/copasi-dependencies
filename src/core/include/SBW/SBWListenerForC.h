/**
 * @file SBWListenerForC.h
 * @brief class definition of SBWListenerForC - monitors events occuring in SBW on behalf of the C API.
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
#if !defined(AFX_SBWLISTENERFORC_H__93B8AA99_0E82_47C7_A989_50316A7127CB__INCLUDED_)
#define AFX_SBWLISTENERFORC_H__93B8AA99_0E82_47C7_A989_50316A7127CB__INCLUDED_

#ifdef WIN32
#pragma warning(disable: 4251)
#endif

#include <SBW/sbwdefs.h>
#include <SBW/SBWRPCListener.h>

#include <set>

namespace SystemsBiologyWorkbench
{

/// monitors events occuring in SBW on behalf of the C API
class SBW_API SBWListenerForC : public SBWRPCListener
{
public:
	// register cdecl listeners
	void registerModuleShutdownListener(ModuleListener);
	void registerShutdownListener(SimpleListener);
	void registerModuleStartupListener(ModuleListener);
	void registerRegistrationChangeListener(SimpleListener);
	// remove cdecl listeners
	void removeShutdownListener(SimpleListener);
	void removeModuleShutdownListener(ModuleListener);
	void removeModuleStartupListener(ModuleListener);
	void removeRegistrationChangeListener(SimpleListener);

	// register stdcall listeners
	void registerModuleShutdownListenerStdCall(ModuleListenerStdCall);
	void registerShutdownListenerStdCall(SimpleListenerStdCall);
	void registerModuleStartupListenerStdCall(ModuleListenerStdCall);
	void registerRegistrationChangeListenerStdCall(SimpleListenerStdCall);
	// remove stdcall listeners
	void removeShutdownListenerStdCall(SimpleListenerStdCall);
	void removeModuleShutdownListenerStdCall(ModuleListenerStdCall);
	void removeModuleStartupListenerStdCall(ModuleListenerStdCall);
	void removeRegistrationChangeListenerStdCall(SimpleListenerStdCall);

	void onModuleShutdown(Integer moduleInstanceId) ;
	void onModuleStart(Integer moduleInstanceId) ;
	void onRegistrationChange() ;
	void onShutdown();

private:
	// cdecl events
	void onModuleEvent(std::set<ModuleListener> &listeners, Integer m);
	void onEvent(std::set<SimpleListener> &listeners);

	// stdcall events
	void onModuleEventStdCall(std::set<ModuleListenerStdCall> &listeners, Integer m);
	void onEventStdCall(std::set<SimpleListenerStdCall> &listeners);

	/// callback functions that are called when this module disconnects from the broker.
	std::set<SimpleListener> shutdownListeners ;
	/// callback functions that are called when another module disconnects from the broker.
	std::set<ModuleListener> moduleShutdownListeners ;
	/// callback functions that are called when another module connects to the broker. 
	std::set<ModuleListener> moduleStartupListeners ;
	/// callback functions that are called when the broker's module registry changes.
	std::set<SimpleListener> registrationChangeListeners ;

	/// callback functions that are called when this module disconnects from the broker.
	std::set<SimpleListenerStdCall> shutdownListenersStdCall ;
	/// callback functions that are called when another module disconnects from the broker.
	std::set<ModuleListenerStdCall> moduleShutdownListenersStdCall ;
	/// callback functions that are called when another module connects to the broker. 
	std::set<ModuleListenerStdCall> moduleStartupListenersStdCall ;
	/// callback functions that are called when the broker's module registry changes.
	std::set<SimpleListenerStdCall> registrationChangeListenersStdCall ;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWLISTENERFORC_H__93B8AA99_0E82_47C7_A989_50316A7127CB__INCLUDED_)
