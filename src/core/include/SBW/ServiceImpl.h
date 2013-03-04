/**
 * @file ServiceImpl.h
 * @brief implementation of a service 
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ServiceImpl.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ServiceImpl.h,v $
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
#if !defined(AFX_SERVICEIMPL_H__94F87B9F_86C2_44A1_B780_EBF139CF3E05__INCLUDED_)
#define AFX_SERVICEIMPL_H__94F87B9F_86C2_44A1_B780_EBF139CF3E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "portableOS.h"
#include "DataBlockWriter.h"

namespace SystemsBiologyWorkbench
{

class MethodImpl ;
class Handler;

/// service implementation
class ServiceImpl  
{
public:
	SBW_API ServiceImpl(std::string name, std::string displayName, std::string category, std::string help);
	virtual ~ServiceImpl();

	SBW_API void addMethod(Handler *handler, std::string signature, bool synchronized = false, std::string help = "");
	SBW_API Handler *getHandler(std::string signature);

	SBW_API MethodImpl *getMethod(Integer methodId);
	SBW_API std::string getName();
	SBW_API void addSignatures(DataBlockWriter);

	SBW_API std::vector<std::string> getSignatureStrings();

	/**
	 * returns the humanly readable name of the service.
	 * @return the humanly readable name of the service 
	 */
	SBW_API std::string getDisplayName() { return displayName ; }
	
	/**
	 * returns the categroy of the service.
	 * @return the categroy of the service
	 */
	SBW_API std::string getCategory() { return category; }

	/**
	 * returns the help string for the service
	 * @return the help string for the service
	 */
	SBW_API std::string getHelp() { return help; }

private:
	/// mutex that synchronizes the synchronized methods on this service
	SBWOSMutex mutex ;

	/// service identification name of this service
	std::string uniqueName ;

	/// humanly readable name of the service
	std::string displayName ;

	/// the category of the service
	std::string category ;

	/// the help string for the service
	std::string help ;

	/// the method implementations with their signatures
	std::vector<MethodImpl *> methods ;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SERVICEIMPL_H__94F87B9F_86C2_44A1_B780_EBF139CF3E05__INCLUDED_)
