/**
 * @file ServiceDescriptor.cpp
 * @brief represents a service on a potentially different module
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ServiceDescriptor.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/ServiceDescriptor.cpp,v $
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
#include "ServiceDescriptor.h"
#include "SBWLowLevel.h"
#include "SBW.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * creates a service descriptor from data contained in the given data block.
 * @param reader contains data for the service descriptor.
 */
ImplServiceDescriptor::ImplServiceDescriptor(ServiceDescriptor sd)
{
	name = sd.getName();
	displayName = sd.getDisplayName();
	category = sd.getCategory();
	help = sd.getHelp();
	
	module = sd.getModuleDescriptor();
}

ImplServiceDescriptor::ImplServiceDescriptor(DataBlockReader reader)
{
	std::string moduleName ;
	
	reader >> moduleName >> name >> displayName >> category >> help ;

	module = SBWLowLevel::getModuleDescriptor(moduleName, true);
}
ImplServiceDescriptor::ImplServiceDescriptor(DataBlockReader r, ModuleDescriptor md)
{
	std::string moduleName ;
	
	r >> moduleName >> name >> displayName >> category >> help ;

	module = md;
}
//////////////////////////////////////////////////////////////////////

/**
 * returns the service described by the service descriptor.
 * Locates a module instance that implements the given service.
 * May or may not create a new module instance to implement the service depending on the module management type of the
 * implementing module.
 * @return the service described by the service descriptor.
 */
SBW_API Service ServiceDescriptor::getServiceInModuleInstance()
{
	return SBW::getModuleInstance(
				getModuleDescriptor().getName()).findServiceByName(getName());
}
