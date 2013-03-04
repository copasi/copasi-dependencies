/**
 * @file SBWRPCListener.h
 * @brief base class of RPC event processors
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWRPCListener.h,v 1.2 2006/09/07 19:51:02 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWRPCListener.h,v $
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
#if !defined(AFX_SBWRPCLISTENER_H__18E1AA14_F0B3_4827_AB87_8B6472952118__INCLUDED_)
#define AFX_SBWRPCLISTENER_H__18E1AA14_F0B3_4827_AB87_8B6472952118__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sbwplusbasictypes.h"

// don't understand why I need this here but...
#ifdef WIN32
// disable silly messages about debug identifier sizes being to big, typically caused by the STL.
//#pragma warning(disable: 4786 4788)
#endif

namespace SystemsBiologyWorkbench
{

/// base class of RPC event processors
class SBWRPCListener  
{
public:
	/// does nothing
	SBWRPCListener() {}

	/// does nothing
	virtual ~SBWRPCListener() {}

	/**
	 * called when another module disconnects fom the broker.
	 * @param x module instance identifier of module instance that's disconnected.
	 */
	virtual void onModuleShutdown(Integer x) = 0;

	/**
	 * called when another module connects fom the broker.
	 * @param x module instance identifier of module instance that's connected.
	 */
	virtual void onModuleStart(Integer x) = 0;

	/// called when the broker's module registration data has changed
	virtual void onRegistrationChange() = 0;

	/// called when this module has disconnected from the broker.
	virtual void onShutdown() = 0;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWRPCLISTENER_H__18E1AA14_F0B3_4827_AB87_8B6472952118__INCLUDED_)
