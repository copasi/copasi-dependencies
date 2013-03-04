/**
 * @file Module.cpp
 * @brief implementation of Module class which represents a module seperate from the module implemented by this application
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: Module.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/Module.cpp,v $
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
#include "Module.h"

#include "SBWLowLevel.h"
#include "ServiceInline.h"

using namespace SystemsBiologyWorkbench ;

/**
 * returns the services that are implemented by this module.
 * the result should deallocated using delete.
 * @return the services that are implemented by this module.
 */
std::vector<Service> *Module::getServices()
{
	Integer n = SBWLowLevel::moduleGetNumberOfServices(moduleId);
	std::vector<Service> *services = new std::vector<Service>(n);

	while (n != 0)
	{
		n-- ;
		(*services)[n].serviceId = n;
		(*services)[n].moduleId = moduleId;
	}

	return services ;
}

/**
 * returns the services that are implemented by this module that are in a given category.
 * the result should deallocated using delete.
 * @param category the category that the returned services must be in.
 * @return the services that are implemented by this module that are in category.
 */
std::vector<Service> *Module::findServicesByCategory(std::string category)
{
	std::vector<Service> *services = getServices();
	std::vector<Service> *result = new std::vector<Service>();

	try
	{
		unsigned int i = 0 ;

		while (i != services->size())
		{
			ServiceDescriptor descriptor ;

			(*services)[i].getDescriptor(descriptor);
			
			if (SBWLowLevel::isSubCategory(category, descriptor.getCategory()))
				result->push_back((*services)[i]);

			i++ ;
		}
	}
	catch (SBWException *)
	{
		delete result ;
		throw ;
	}

	return result ;
}
