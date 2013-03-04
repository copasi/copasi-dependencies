/**
 * @file SBWListenerConsolidator.cpp
 * @brief class implementation of SBWListenerConsolidator -
 *          listener that broadcasts events to other listeners registered with it
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWListenerConsolidator.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SBWListenerConsolidator.cpp,v $
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
#include "SBWListenerConsolidator.h"

using namespace SystemsBiologyWorkbench ;

/// constructor
SBWListenerConsolidator::SBWListenerConsolidator() : mutex("listener-consolidator")
{

}

/// destructor
SBWListenerConsolidator::~SBWListenerConsolidator()
{

}

/**
 * Called when a given module connects from the broker.
 * This implementation forwards the event to the registered listeners.
 * @param otherModule module that has just connected to a the broker.
 */
void SBWListenerConsolidator::onModuleStart(Integer otherModule)
{	

	std::list<SBWRPCListener *> l;
	
	copyListeners(l);

	std::list<SBWRPCListener *>::iterator i = l.begin();

	while (i != l.end())
	{

		try { (*i)->onModuleStart(otherModule); } catch (...) {}
		i++ ;
	}
}
	
/**
 * called when the broker's module registration data changes.
 * This implementation forwards the event to the registered listeners.
 */
void SBWListenerConsolidator::onRegistrationChange()
{	

	std::list<SBWRPCListener *> l;
	
	copyListeners(l);

	std::list<SBWRPCListener *>::iterator i = l.begin();

	while (i != l.end())
	{		
		try { (*i)->onRegistrationChange(); } catch (...) {}
		i++ ;
	}
}

/**
 * called when this module/application is disconnected from the broker. 
 * This implementation forwards the event to the registered listeners.
 */
void SBWListenerConsolidator::onShutdown()
{	

	std::list<SBWRPCListener *> l;
	
	copyListeners(l);

	std::list<SBWRPCListener *>::iterator i = l.begin();

	while (i != l.end())
	{		
		try { (*i)->onShutdown(); } catch (...) {}
		i++ ;
	}
}

/**
 * Called when a given module disconnects from the broker.
 * This implementation does nothing.
 * @param x ignored; Integer instance that has disconnected from the broker.
 */
void SBWListenerConsolidator::onModuleShutdown(Integer x)
{	

	std::list<SBWRPCListener *> l;
	
	copyListeners(l);

	std::list<SBWRPCListener *>::iterator i = l.begin();

	while (i != l.end())
	{		
		try { (*i)->onModuleShutdown(x); } catch (...) {}
		i++ ;
	}
}

/**
 * adds an event listener 
 * @param x event listener
 */
void SBWListenerConsolidator::addListener(SBWRPCListener *x)
{
	SBWOSMutexLock lock(mutex);

	listeners.insert(x);
}

/**
 * removes an event listener
 * @param x event listener
 */
void SBWListenerConsolidator::removeListener(SBWRPCListener *x)
{
	SBWOSMutexLock lock(mutex);

	listeners.erase(x);
}

/**
 * inserts the set of listeners into a given list 
 * @param l list inot which the set of listeners is inserted
 */
void SBWListenerConsolidator::copyListeners(std::list<SBWRPCListener *> &l)
{
	SBWOSMutexLock lock(mutex);
	std::set<SBWRPCListener *>::iterator itr = listeners.begin();
	
	while (itr != listeners.end())
	{
		l.push_back(*itr);
		itr++;
	}
}
