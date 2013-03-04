/**
 * @file ImplModuleDescriptor.h
 * @brief  interface for the ImplModuleDescriptor class
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ImplModuleDescriptor.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ImplModuleDescriptor.h,v $
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

/// MSVC include loop block
#if !defined(AFX_IMPLMODULEDESCRIPTOR_H__32214EC0_E254_4ECF_B790_42D81B7E2A3B__INCLUDED_)
#define AFX_IMPLMODULEDESCRIPTOR_H__32214EC0_E254_4ECF_B790_42D81B7E2A3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SBWObject.h"
#include "DataBlockReaderClass.h"

namespace SystemsBiologyWorkbench
{

class ServiceDescriptor ;

/** implementation uderlying the ModuleDescriptor class.
 * Contains information stored in the Brokers repository about a given module.
 */
class ImplModuleDescriptor : public SBWObject  
{
public:
	SBW_API ImplModuleDescriptor(DataBlockReader r);

	/**
	 * returns the module identification string/name
	 * @return the module identification string/name
	 */
	SBW_API std::string getName() { return name; }

	/**
	 * returns the humanly readable name for the module
	 * @return the humanly readable name for the module
	 */
	SBW_API std::string getDisplayName() { return displayName; }

	/**
	 * returns the command line that is used to create an instance of the module
	 * @return the command line that is used to create an instance of the module
	 */
	SBW_API std::string getCommandLine() { return commandLine; }

	/**
	 * returns the mechanism used to determine how module instances are started and disconnected
	 * @return the mechanism used to determine how module instances are started and disconnected
	 */
	SBW_API ModuleManagementType getType() { return type; }

	/**
	 * returns the help string for the module
	 * @return the help string for the module
	 */
	SBW_API std::string getHelp() { return help ; }

	SBW_API std::vector<ServiceDescriptor> *getServiceDescriptors();

private:
	/// the module identification string/name
	std::string name ;

	/// the humanly readable name for the module
	std::string displayName ;

	/// the command line that is used to create an instance of the module
	std::string commandLine ;

	/// the mechanism used to determine how module instances are started and disconnected
	ModuleManagementType type;

	/// the help string for the module
	std::string help;
};

} // SystemsBiologyWorkbench
#endif // !defined(AFX_IMPLMODULEDESCRIPTOR_H__32214EC0_E254_4ECF_B790_42D81B7E2A3B__INCLUDED_)
