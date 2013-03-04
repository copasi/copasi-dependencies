/**
 * @file ModuleDescriptor.h
 * @brief  interface for the ModuleDescriptor class.
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ModuleDescriptor.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ModuleDescriptor.h,v $
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
#if !defined(AFX_MODULEDESCRIPTOR_H__5464A475_237B_438E_AC35_D25B3062A5B5__INCLUDED_)
#define AFX_MODULEDESCRIPTOR_H__5464A475_237B_438E_AC35_D25B3062A5B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImplModuleDescriptor.h"

namespace SystemsBiologyWorkbench
{

/// data on a module returned from the broker
class ModuleDescriptor : public SBWHandle<ImplModuleDescriptor> 
{
public:
	/// create an empty module descriptor
	ModuleDescriptor() { }

	/** create a module descriptor from the data in the given data block reader.
	 * The data is assumed to be at the cursor in the data block and the cursor is moved
	 * to the first object after the module descriptor.
	 * @param r data block containing module descriptor data.
	 */
	ModuleDescriptor(DataBlockReader r)
		: SBWHandle<ImplModuleDescriptor>(new ImplModuleDescriptor(r))
	{
	}

	/**
	 * returns the module identification string/name
	 * @return the module identification string/name
	 */
	std::string getName() { return p->getName(); }

	/**
	 * returns the humanly readable name for the module
	 * @return the humanly readable name for the module
	 */
	std::string getDisplayName() { return p->getDisplayName(); }

	/**
	 * returns the command line that is used to create an instance of the module
	 * @return the command line that is used to create an instance of the module
	 */
	std::string getCommandLine() { return p->getCommandLine(); }

	/**
	 * returns the mechanism used to determine how module instances are started and disconnected
	 * @return the mechanism used to determine how module instances are started and disconnected
	 */
	ModuleManagementType getManagementType() { return p->getType(); }

	/**
	 * returns the help string for the module
	 * @return the help string for the module
	 */
	std::string getHelp() { return p->getHelp(); }

	/**
	 * returns the service descriptors for the services registered with this module
	 * @return the service descriptors for the services registered with this module
	 */
	std::vector<ServiceDescriptor> *getServiceDescriptors() { return p->getServiceDescriptors(); }

	//! added functionality for use in broker
	SBW_API std::vector<ServiceDescriptor*> getServices() { return _services; }
	SBW_API void addService(ServiceDescriptor* sd); 

private:

	std::vector<ServiceDescriptor*> _services;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_MODULEDESCRIPTOR_H__5464A475_237B_438E_AC35_D25B3062A5B5__INCLUDED_)
