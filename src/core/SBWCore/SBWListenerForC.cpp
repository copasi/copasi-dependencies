/**
 * @file SBWListenerForC.cpp
 * @brief SBWListenerForC class implementation
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWListenerForC.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SBWListenerForC.cpp,v $
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

