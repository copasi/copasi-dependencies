/**
 * @file Signature.h
 * @brief the parse structure of a signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: Signature.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/Signature.h,v $
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
#if !defined(AFX_SIGNATURE_H__C4D435A6_EA6F_4516_8F72_178367345321__INCLUDED_)
#define AFX_SIGNATURE_H__C4D435A6_EA6F_4516_8F72_178367345321__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SignatureListContents.h"
#include "SignatureType.h"

namespace SystemsBiologyWorkbench
{

/// signature parse structure - underlying implementation of Signature
struct sbwSignature : public SBWObject 
{
public:
	sbwSignature(const char *signature);
	virtual ~sbwSignature();

	static Integer match(const std::vector<std::string> &methods, const char *signature);
	static bool match(const char *completeSignature, const char *partialSignature);

	/** 
	 * returns the argument structures of this signature
	 * @return the argument structures of this signature
	 */
	const SignatureListContents &getArguments() { return *arguments ; }

	/**
	 * returns the result type structure of this signature
	 * @return the result type structure of this signature
	 */
	SignatureType getReturnType() { return result ; }

	/**
	 * returns the name of the method defined by the signature
	 * @return the name of the method defined by the signature
	 */
	std::string getName() const { return name ; }

	static sbwSignatureType *parseType(const char *&);
	static bool compareString(const char *, const char *&);
	static void consume(const char *, const char *&);
	static void consumeSpace(const char *&);
	static std::string consumeName(const char *&);

private:
	static sbwSignatureType *parseScalarType(const char *&);

	/// the name of the method defined by the signature
	std::string name ;

	/// the argument structures of this signature
	SignatureListContents *arguments;

    /// the result type structure of this signature
	SignatureType result ;
};

/// signature parse structure
class Signature : public SBWHandle<sbwSignature>
{
public:
	SBW_API Signature(const char *signature);
	SBW_API Signature(std::string signature);

	SBW_API std::vector<SignatureElement> *getArguments();

	/**
	 * returns the result type structure of this signature
	 * @return the result type structure of this signature
	 */
	SignatureType getReturnType() { return p->getReturnType() ; }

	/**
	 * returns the name of the method defined by the signature
	 * @return the name of the method defined by the signature
	 */
	std::string getName() const { return p->getName() ; }

	/**
	 * returns the underlying implementation
	 * @return the underlying implementation
	 */
	sbwSignature *getImpl() { return p; }
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SIGNATURE_H__C4D435A6_EA6F_4516_8F72_178367345321__INCLUDED_)
