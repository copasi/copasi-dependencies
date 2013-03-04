/**
 * @file SignatureListContents.cpp
 * @brief represents the contents of a list type inside a signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SignatureListContents.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SignatureListContents.cpp,v $
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
// SignatureListContents.cpp: implementation of the SignatureListContents class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SignatureListContents.h"
#include "SignatureElement.h"
#include "Signature.h"
#include "SignatureType.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * creates a list of singature arguments given an initial first argument type.
 * The created list contains one argument without a name.
 * @param arg The type of the initial argument.
 */
SBW_API SignatureListContents::SignatureListContents(SignatureType arg)
{
	push_back(SignatureElement(arg));
}

/**
 * creates a list structure of signature arguments given a signature argument list string.
 * @param signature string prefixed by a signature argument list.  The given string pointer is set to point to the 
 *        first character in the original string after the signature argument list.
 */
SBW_API SignatureListContents::SignatureListContents(const char *&signature)
{
	sbwSignature::consumeSpace(signature);

	while (*signature != '\0' && *signature != '}' && *signature != ')')
	{
		push_back(SignatureElement(signature));

		sbwSignature::compareString(",", signature); // don't care if its not there
		sbwSignature::consumeSpace(signature);
	}
}

/**
 * For the C API: stores the data supplied in a variable argument list, in a datablock.
 * This signature type list is used to determine the types of the objects in the variable argument list and
 * types of the corresponding data as it is stored in the datablock.
 * @param writer the datablock in which the data will be stored.
 * @param ap the variable argument list.  This pointer is changed to point to the n+1th argument where n
 *           is the number of arguments in this argument list.
 */
SBW_API void SignatureListContents::write(DataBlockWriter writer, va_list &ap) const
{
	std::vector<SignatureElement>::const_iterator itr = begin();

	while (itr != end())
	{
		SignatureType type ;

		(*itr).getType(type);
		type.getImpl()->write(writer, ap);
		itr++;
	}

}

/**
 * For the C API: reads the data supplied in a datablock into objects referenced by a variable argument list.
 * The signature type list object is used to determine the types of the objects in the variable argument list and the
 * types of the corresponding data as it is stored in the datablock.
 * Each item in the variable argument list is a pointer to either
 * - a scalar or
 * - a pointer to an object.
 * For each item in the type list, data is extracted from the datablock and either
 * - stored directly in the corresponding scalar or
 * - a new object is allocated on the C heap and the corresponding pointer is changed to point to this new object.
 * The variable argument list pointer is changed to point to the n+1th argument
 * where n is the number of arguments in the type list.
 * @param reader the datablock in which the data is stored.
 * @param ap the variable argument list.  
 */
SBW_API void SignatureListContents::read(DataBlockReader reader, va_list &ap) const
{
	std::vector<SignatureElement>::const_iterator itr = begin();

	while (itr != end())
	{
		SignatureType type ;

		(*itr).getType(type);
		type.getImpl()->read(reader, ap);
		itr++;
	}

}

/**
 * returns an array signature argument objects corresponding to the type list.
 * @return an array signature argument objects corresponding to the type list.
 *         The returned value should be deallocated using the delete operator.
 */
SBW_API std::vector<SignatureElement> *SignatureListContents::copyVector() const
{
	std::vector<SignatureElement> *result = new std::vector<SignatureElement>(size());

	result->assign(begin(), end());

	return result ;
}

/**
 * returns whether the type sequence contained in this object corresponds to another type sequence.
 * @param other another type sequence.
 * @return whether the type sequence contained in this object corresponds to the other type sequence.
 */
SBW_API bool SignatureListContents::match(const SignatureListContents &other) const
{
	std::vector<SignatureElement>::const_iterator itr = begin();
	std::vector<SignatureElement>::const_iterator otherItr = other.begin();

	while (itr != end())
	{
		if (otherItr == other.end())
			return false ;

		SignatureType type, otherType ;

		(*otherItr).getType(otherType);
		(*itr).getType(type);

		if (!type.match(otherType))
			return false ;

		itr++;
		otherItr++;
	}

	return otherItr == other.end();
}
