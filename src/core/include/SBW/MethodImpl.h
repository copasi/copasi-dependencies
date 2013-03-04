/**
 * @file MethodImpl.h
 * @brief  implementation of a method together with its signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: MethodImpl.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/MethodImpl.h,v $
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
*/// MethodImpl.h: interface for the MethodImpl class.
//
//////////////////////////////////////////////////////////////////////

/// MSVC generated include loop block
#if !defined(AFX_METHODIMPL_H__71359E4A_9F49_450A_9619_6C4EB77FFA54__INCLUDED_)
#define AFX_METHODIMPL_H__71359E4A_9F49_450A_9619_6C4EB77FFA54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sbwdefs.h"
#include "Module.h"
#include <string>
#include "DataBlockWriterClass.h"
#include "DataBlockReaderClass.h"

namespace SystemsBiologyWorkbench
{

class Handler ;

/// implementation of an SBW method in this module together with associated data
class MethodImpl  
{
public:
	SBW_API MethodImpl(Handler *handler, std::string signature, SBWOSMutex *mutex = NULL, std::string help = "");
	virtual ~MethodImpl();

	SBW_API DataBlockWriter invoke(Module from, DataBlockReader);
	SBW_API bool isSynchronised();
	SBW_API std::string getSignature();
	Handler *getHandler();

	/**
	 * returns the help string for this method
	 * @return the help string for this method
	 */
	SBW_API std::string getHelp() { return help; }

private:
	/// the method implementation
	Handler *handler;

	/// the method signature
	std::string signature;

	/** mutex to be locked during any call on this method.
     * normally this is either null implying that this method doesn't need to be synchroized or
	 * the mutex is the same for all methods on the method's service.
	 */
	SBWOSMutex *mutex;

	/// the help string for this method
	std::string help;
};

} // SystemsBiologyWorkbench
#endif // !defined(AFX_METHODIMPL_H__71359E4A_9F49_450A_9619_6C4EB77FFA54__INCLUDED_)
