/**
 * @file SBWListenerForC.cpp
 * @brief SBWListenerForC class implementation
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
#include "SBWListenerForC.h"
#include "SBWCommunicationException.h"
using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * register the given callback which is called when this application disconnects from the broker.
 * @param x callback which is called when this application disconnects from the broker
 */
void SBWListenerForC::registerShutdownListener(SimpleListener x)
{
	shutdownListeners.insert(x);
}
/**
 * register the given callback which is called when another module disconnects from the broker.
 * @param x callback which is called when another module disconnects from the broker
 */
void SBWListenerForC::registerModuleShutdownListener(ModuleListener x)
{
	moduleShutdownListeners.insert(x);
}
/**
 * register the given callback which is called when another module connects to the broker.
 * @param x callback which is called when another module connects to the broker
 */
void SBWListenerForC::registerModuleStartupListener(ModuleListener x)
{
	moduleStartupListeners.insert(x);
}
/**
 * register the given callback which is called when the broker's module registry changes
 * @param x callback which is called when the broker's module registry changes
 */
void SBWListenerForC::registerRegistrationChangeListener(SimpleListener x)
{
	registrationChangeListeners.insert(x);
}
/**
 * remove the given callback which is called when this application disconnects from the broker.
 * @param x callback which is called when this application disconnects from the broker
 */
void SBWListenerForC::removeShutdownListener(SimpleListener x)
{
	shutdownListeners.erase(x);
}
/**
 * remove the given callback which is called when another module disconnects from the broker.
 * @param x callback which is called when another module disconnects from the broker
 */
void SBWListenerForC::removeModuleShutdownListener(ModuleListener x)
{
	moduleShutdownListeners.erase(x);
}
/**
 * remove the given callback which is called when another module connects to the broker.
 * @param x callback which is called when another module connects to the broker
 */
void SBWListenerForC::removeModuleStartupListener(ModuleListener x)
{
	moduleStartupListeners.erase(x);
}
/**
 * remove the given callback which is called when the broker's module registry changes
 * @param x callback which is called when the broker's module registry changes
 */
void SBWListenerForC::removeRegistrationChangeListener(SimpleListener x)
{
	registrationChangeListeners.erase(x);
}
/**
 * calls a set of callbacks with a given numeric module instance identifier.
 * @param listeners the set of callbacks to be called.
 * @param m the numeric module instance identifier to be passed to the callbacks.
 */
void SBWListenerForC::onModuleEvent(std::set<ModuleListener> &listeners, Integer m)
{
	std::set<ModuleListener>::iterator i = listeners.begin();

	while (i != listeners.end())
	{
		(*i)(m);
		i++;
	}
}
/**
 * calls a set of simple callbacks
 * @param listeners the set of callbacks to be called.
 */
void SBWListenerForC::onEvent(std::set<SimpleListener> &listeners)
{
		std::set<SimpleListener>::iterator i = listeners.begin();

		while (i != listeners.end())
		{
			try { (*i)();	} catch(...) { }
			i++;
		}
}
/**
 * register the given callback which is called when this application disconnects from the broker.
 * @param x callback which is called when this application disconnects from the broker
 */
void SBWListenerForC::registerShutdownListenerStdCall(SimpleListenerStdCall x)
{
	shutdownListenersStdCall.insert(x);
}

/**
 * register the given callback which is called when another module disconnects from the broker.
 * @param x callback which is called when another module disconnects from the broker
 */
void SBWListenerForC::registerModuleShutdownListenerStdCall(ModuleListenerStdCall x)
{
	moduleShutdownListenersStdCall.insert(x);
}

/**
 * register the given callback which is called when another module connects to the broker.
 * @param x callback which is called when another module connects to the broker
 */
void SBWListenerForC::registerModuleStartupListenerStdCall(ModuleListenerStdCall x)
{
	moduleStartupListenersStdCall.insert(x);
}


/**
 * register the given callback which is called when the broker's module registry changes
 * @param x callback which is called when the broker's module registry changes
 */
void SBWListenerForC::registerRegistrationChangeListenerStdCall(SimpleListenerStdCall x)
{
	registrationChangeListenersStdCall.insert(x);
}

/**
 * remove the given callback which is called when this application disconnects from the broker.
 * @param x callback which is called when this application disconnects from the broker
 */
void SBWListenerForC::removeShutdownListenerStdCall(SimpleListenerStdCall x)
{
	shutdownListenersStdCall.erase(x);
}

/**
 * remove the given callback which is called when another module disconnects from the broker.
 * @param x callback which is called when another module disconnects from the broker
 */
void SBWListenerForC::removeModuleShutdownListenerStdCall(ModuleListenerStdCall x)
{
	moduleShutdownListenersStdCall.erase(x);
}

/**
 * remove the given callback which is called when another module connects to the broker.
 * @param x callback which is called when another module connects to the broker
 */
void SBWListenerForC::removeModuleStartupListenerStdCall(ModuleListenerStdCall x)
{
	moduleStartupListenersStdCall.erase(x);
}

/**
 * remove the given callback which is called when the broker's module registry changes
 * @param x callback which is called when the broker's module registry changes
 */
void SBWListenerForC::removeRegistrationChangeListenerStdCall(SimpleListenerStdCall x)
{
	registrationChangeListenersStdCall.erase(x);
}

/**
 * calls a set of callbacks with a given numeric module instance identifier.
 * @param listeners the set of callbacks to be called.
 * @param m the numeric module instance identifier to be passed to the callbacks.
 */
void SBWListenerForC::onModuleEventStdCall(std::set<ModuleListenerStdCall> &listeners, Integer m)
{
	std::set<ModuleListenerStdCall>::iterator i = listeners.begin();

	while (i != listeners.end())
	{
		try { (*i)(m); } catch(...) {}
		i++;
	}
}
/**
 * calls a set of simple callbacks
 * @param listeners the set of callbacks to be called.
 */
void SBWListenerForC::onEventStdCall(std::set<SimpleListenerStdCall> &listeners)
{
	try
	{
		std::set<SimpleListenerStdCall>::iterator i = listeners.begin();

		while (i != listeners.end())
		{
			try { (*i)(); } catch (...) {}
			i++;
		}
	}
	catch(...)
	{		
	}
}


/**
 * notify callbacks that another module has disconnected from the broker.
 * @param m the numeric module identifier for a module which has disconnected from the broker.
 */
void SBWListenerForC::onModuleShutdown(Integer m)
{
	onModuleEvent(moduleShutdownListeners, m);
	onModuleEventStdCall(moduleShutdownListenersStdCall, m);
}

/**
 * notify callbacks that this application has disconnected from the broker.
 */
void SBWListenerForC::onShutdown()
{
	onEvent(shutdownListeners);
	onEventStdCall(shutdownListenersStdCall);
}

/**
 * notify callbacks that another module has connected to the broker
 * @param m the numeric module identifier for a module which has connected to the broker.
 */
void SBWListenerForC::onModuleStart(Integer m) 
{
	onModuleEvent(moduleStartupListeners, m);
	onModuleEventStdCall(moduleStartupListenersStdCall, m);
}

/**
 * notify callbacks that the broker's module registry has changed.
 */
void SBWListenerForC::onRegistrationChange()
{
	onEvent(registrationChangeListeners);
	onEventStdCall(registrationChangeListenersStdCall);
}

