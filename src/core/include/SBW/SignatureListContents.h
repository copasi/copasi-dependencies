/**
 * @file SignatureListContents.h
 * @brief represents the contents of a list type inside a signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SignatureListContents.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SignatureListContents.h,v $
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
#if !defined(AFX_SIGNATURELISTCONTENTS_H__81430855_7BDA_4CD7_AEB9_3C37024E8BDC__INCLUDED_)
#define AFX_SIGNATURELISTCONTENTS_H__81430855_7BDA_4CD7_AEB9_3C37024E8BDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#ifndef WIN32
#include <stdarg.h>
//#endif


#include "DataBlockWriterClass.h"
#include "DataBlockReaderClass.h"
#include "SignatureElement.h"
#include "SignatureType.h"

namespace SystemsBiologyWorkbench
{

class SignatureElement ;
class SignatureType ;

/// represents the contents of a list type inside a signature
class SignatureListContents : public std::vector<SignatureElement>  
{
public:
	/// create an empty list
	SignatureListContents() {} 
	SBW_API SignatureListContents(SignatureType);
	SBW_API SignatureListContents(const char *&);
	SBW_API std::vector<SignatureElement> *copyVector() const;

	// Note the functions involving va_list below are specific to the C interface
	// the use of these functions in C++ is not recommended
	SBW_API void write(DataBlockWriter, va_list &) const;
	SBW_API void read(DataBlockReader, va_list &) const;

	SBW_API bool match(const SignatureListContents &) const;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SIGNATURELISTCONTENTS_H__81430855_7BDA_4CD7_AEB9_3C37024E8BDC__INCLUDED_)
