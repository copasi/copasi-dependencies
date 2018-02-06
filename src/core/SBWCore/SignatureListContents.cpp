/**
 * @file SignatureListContents.cpp
 * @brief represents the contents of a list type inside a signature
 * 
 * This file is part of SBW.  Please visit http://sbw.sf.org for more
 * information about SBW, and the latest version of libSBW.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the New BSD license.
 *
 * Copyright (c) 2010-2014, Frank T. Bergmann and 
 *                          University of Washington
 * Copyright (c) 2008-2010, University of Washington and 
 *                          Keck Graduate Institute.
 * Copyright (c) 2005-2008, Keck Graduate Institute.
 * Copyright (c) 2001-2004, California Institute of Technology and
 *               Japan Science and Technology Corporation.
 * 
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are 
 * met: 
 * 
 * 1. Redistributions of source code must retain the above 
 *    copyright notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 
 * 3. Neither the name of the copyright holder nor the names of its 
 *    contributors may be used to endorse or promote products derived from 
 *    this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * The original code contained here was initially developed by:
 *
 *     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri
 *     The Systems Biology Workbench Development Group
 *     ERATO Kitano Systems Biology Project
 *     Control and Dynamical Systems, MC 107-81
 *     California Institute of Technology
 *     Pasadena, CA, 91125, USA
 *
 *
 * Contributor(s):
 *
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
 SignatureListContents::SignatureListContents() {}

SignatureListContents::SignatureListContents(SignatureType arg)
{
	push_back(SignatureElement(arg));
}

/**
 * creates a list structure of signature arguments given a signature argument list string.
 * @param signature string prefixed by a signature argument list.  The given string pointer is set to point to the 
 *        first character in the original string after the signature argument list.
 */
 SignatureListContents::SignatureListContents(const char *&signature)
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
 void SignatureListContents::write(DataBlockWriter writer, va_list &ap) const
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
 void SignatureListContents::read(DataBlockReader reader, va_list &ap) const
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
 std::vector<SignatureElement> *SignatureListContents::copyVector() const
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
 bool SignatureListContents::match(const SignatureListContents &other) const
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
