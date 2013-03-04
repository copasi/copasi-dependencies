/**
 * @file ModuleInline.h
 * @brief  the implementation of the Module class
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ModuleInline.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ModuleInline.h,v $
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

/// include loop block
#ifndef MODULEINLINE
#define MODULEINLINE

/**
 * returns the module descriptor for this module instance
 * @return the module descriptor for this module instance
 */
inline SystemsBiologyWorkbench::ModuleDescriptor SystemsBiologyWorkbench::Module::getDescriptor()
{
	return SystemsBiologyWorkbench::SBWLowLevel::moduleGetModuleDescriptor(moduleId);
}

/**
 * returns the service that has a given identification name, on this module instance, 
 * @param x service identification name of requested service
 * @return the service on this module instance that has the given identification name
 */
inline SystemsBiologyWorkbench::Service SystemsBiologyWorkbench::Module::findServiceByName(std::string x)
{
	return Service(moduleId, SystemsBiologyWorkbench::SBWLowLevel::moduleFindServiceByName(moduleId, x.c_str()));
}

/// disconnects the broker from the module instance
inline void SystemsBiologyWorkbench::Module::shutdown()
{
	SystemsBiologyWorkbench::SBWLowLevel::moduleShutdown(moduleId);
}
#endif // MODULEINLINE
