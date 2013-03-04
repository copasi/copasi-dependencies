/**
 * @file SBWTypeMismatchException.h
 * @brief SBWTypeMismatchException class definition - exception thrown when an object of an unexpected type is encountered while deocoding a data block
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWTypeMismatchException.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWTypeMismatchException.h,v $
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
#if !defined(AFX_SBWTYPEMISMATCHEXCEPTION_H__8C7C16EB_9497_458F_9AD9_6DCF107F486B__INCLUDED_)
#define AFX_SBWTYPEMISMATCHEXCEPTION_H__8C7C16EB_9497_458F_9AD9_6DCF107F486B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SBWException.h"

namespace SystemsBiologyWorkbench
{

/// exception thrown when an object of an unexpected type is encountered while deocoding a data block
class SBWTypeMismatchException : public SBWException  
{
public :
	/**
	 * create an exception with a user message and a detailed developer message.
	 * @param a user message.
	 * @param b detailed message.
	 */
	SBWTypeMismatchException(std::string a, std::string b) : SBWException(a, b) {}

	/**
	 * create an exception with just a user message.
	 * @param x user message.
	 */	
	SBWTypeMismatchException(std::string x) : SBWException(x) {}

	SBW_API static unsigned char encoding() ;
	SBW_API unsigned char getCode() ;

private :
	static const unsigned char code ;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWTYPEMISMATCHEXCEPTION_H__8C7C16EB_9497_458F_9AD9_6DCF107F486B__INCLUDED_)
