/**
 * @file SBWListener.h
 * @brief class definition of SBWListener - callback interface for monitoring events occuring in SBW.
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWListener.h,v 1.2 2006/09/08 06:03:18 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWListener.h,v $
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
#if !defined(AFX_SBWLISTENER_H__65AD3A17_B880_4E7B_8898_390E08A358FD__INCLUDED_)
#define AFX_SBWLISTENER_H__65AD3A17_B880_4E7B_8898_390E08A358FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Module.h"
#include "SBWRPCListener.h"

namespace SystemsBiologyWorkbench
{

/// callback interface for monitoring events occuring in SBW
class SBWListener  : public SBWRPCListener
{
public:

	/// does nothing
	SBWListener() {}

	/// does nothing
	virtual ~SBWListener() {}

	/**
	 * Called when a given module disconnects from the broker.
	 * This implementation does nothing.
	 * @param x ignored; Module instance that has disconnected from the broker.
	 */
	virtual void onModuleShutdown(Module /*x*/) {}

	/**
	 * Called when a given module connects from the broker.
	 * This implementation does nothing.
	 * @param x ignored; Module instance that has connected from the broker.
	 */
	virtual void onModuleStart(Module) {}

	/// called when the broker's module registration data changes.  This implementation does nothing.
	virtual void onRegistrationChange() {}

	/// called when this module/application is disconnected from the broker. 
	virtual void onShutdown() {}

	/**
	 * Called when a given module disconnects from the broker.
	 * This implementation calls onModuleShutdown(Module x).
	 * @see onModuleShutdown(Module x)
	 * @param x ignored; numeric module instance identifier of module instance that has disconnected from the broker.
	 */	
	void onModuleShutdown(Integer x) { onModuleShutdown(Module(x)); }

	/**
	 * Called when a given module connects from the broker.
	 * This implementation calls onModuleStart(Module x).
	 * @see onModuleStart(Module x)
	 * @param x ignored; numeric module instance identifier of module instance that has connected from the broker.
	 */	
	void onModuleStart(Integer x) { onModuleStart(Module(x)); }
};

} // SystemsBiologyWorkbench
 
#endif // !defined(AFX_SBWLISTENER_H__65AD3A17_B880_4E7B_8898_390E08A358FD__INCLUDED_)
