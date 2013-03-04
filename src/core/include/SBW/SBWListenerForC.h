/**
 * @file SBWListenerForC.h
 * @brief class definition of SBWListenerForC - monitors events occuring in SBW on behalf of the C API.
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWListenerForC.h,v 1.2 2006/09/07 19:51:02 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWListenerForC.h,v $
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
#if !defined(AFX_SBWLISTENERFORC_H__93B8AA99_0E82_47C7_A989_50316A7127CB__INCLUDED_)
#define AFX_SBWLISTENERFORC_H__93B8AA99_0E82_47C7_A989_50316A7127CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SBWRPCListener.h"
#include <set>

#ifdef WIN32
// disable silly messages about debug identifier sizes being to big, typically caused by the STL.
//#pragma warning(disable: 4786 4788)
#endif

namespace SystemsBiologyWorkbench
{

/// monitors events occuring in SBW on behalf of the C API
class SBWListenerForC : public SBWRPCListener  
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
