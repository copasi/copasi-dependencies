/**
 * @file ServiceDescriptor.h
 * @brief access data on one service - taken from the broker's module registry. 
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ServiceDescriptor.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ServiceDescriptor.h,v $
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
#if !defined(AFX_SERVICEDESCRIPTOR_H__0A7C51A4_8BF5_43DA_BAC1_87D22F4B1AF9__INCLUDED_)
#define AFX_SERVICEDESCRIPTOR_H__0A7C51A4_8BF5_43DA_BAC1_87D22F4B1AF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Service.h"
#include "ModuleDescriptor.h"

namespace SystemsBiologyWorkbench
{

/**
 * underlying implementation of ServiceDescriptor.
 * contains data on one service - taken from the broker's module registry
 */
class ImplServiceDescriptor : public SBWObject
{
public:
	SBW_API ImplServiceDescriptor(ServiceDescriptor sd);
	SBW_API ImplServiceDescriptor(DataBlockReader);
	SBW_API ImplServiceDescriptor(DataBlockReader r, ModuleDescriptor md);

	/**
	 * returns the identification name of the service.
	 * @return the identification name of the service.
	 */
	SBW_API std::string getName() { return name ; }

	/**
	 * returns the humanly readable name of the service
	 * @return the humanly readable name of the service
	 */
	SBW_API std::string getDisplayName() { return displayName; }

	/**
	 * returns the category of the service.
	 * @return the category of the service.
	 */
	SBW_API std::string getCategory() { return category; }

	/**
	 * returns the ModuleDescriptor of the module that implements this service.
	 * @return the ModuleDescriptor of the module that implements this service.
	 */
	SBW_API ModuleDescriptor getModuleDescriptor() { return module; }

	/**
	 * returns the help string for this service
	 * @return the help string for this service
	 */
	SBW_API std::string getHelp() { return help ; }
	
private:
	/// the identification name of the service
	std::string name ;

	/// the humanly readable name of the service
	std::string displayName ;

	/// the category of the service
	std::string category;

	/// the help string for this service
	std::string help ;

	/// the ModuleDescriptor of the module that implements this service
	ModuleDescriptor module ;
};

/// contains data on one service - taken from the broker's module registry
class ServiceDescriptor : public SBWHandle<ImplServiceDescriptor>
{
public:
	/// create a service descriptor containing no data
	ServiceDescriptor() {}

	SBW_API Service getServiceInModuleInstance();

	/**
	 * create a ServiceDescriptor object using data extracted from a data block.
	 * The cursor of the DataBlockReader is move to the first object after the descriptor data.
	 * @param r data block containing service descriptor data
	 */
	ServiceDescriptor(DataBlockReader r)
	: SBWHandle<ImplServiceDescriptor>(new ImplServiceDescriptor(r))
	{
	}
	ServiceDescriptor(ServiceDescriptor *sd)
	: SBWHandle<ImplServiceDescriptor>(new ImplServiceDescriptor(*sd))
	{
	}
	ServiceDescriptor(DataBlockReader r, ModuleDescriptor md)
	: SBWHandle<ImplServiceDescriptor>(new ImplServiceDescriptor(r,md))
	{
	}
	
	/**
	 * returns the identification name of the service.
	 * @return the identification name of the service.
	 */
	std::string getName() { return p->getName() ; }

	/**
	 * returns the humanly readable name of the service
	 * @return the humanly readable name of the service
	 */
	std::string getDisplayName() { return p->getDisplayName(); }

	/**
	 * returns the category of the service.
	 * @return the category of the service.
	 */
	std::string getCategory() { return p->getCategory(); }

	/**
	 * returns the ModuleDescriptor of the module that implements this service.
	 * @return the ModuleDescriptor of the module that implements this service.
	 */
	ModuleDescriptor getModuleDescriptor() { return p->getModuleDescriptor(); }

	/**
	 * returns the help string for this service
	 * @return the help string for this service
	 */
	std::string getHelp() { return p->getHelp(); }
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SERVICEDESCRIPTOR_H__0A7C51A4_8BF5_43DA_BAC1_87D22F4B1AF9__INCLUDED_)
