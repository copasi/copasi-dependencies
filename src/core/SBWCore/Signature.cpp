/**
 * @file Signature.cpp
 * @brief represents the structure of a signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: Signature.cpp,v 1.3 2008/07/13 19:12:53 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/Signature.cpp,v $
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
// Signature.cpp: implementation of the Signature class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Signature.h"

#include "SBWSignatureSyntaxException.h"
#include "SignatureType.h"
#include "SignatureList.h"
#include "SignatureScalar.h"
#include "SignatureArray.h"
#include "SignatureBoolean.h"
#include "ImplDataBlockReader.h"
#include "ImplDataBlockWriter.h"
#include "SignatureListContents.h"
#include "sbwSignatureVoid.h"
#include "sbwSignatureByte.h"
#include "SBWMethodNotFoundException.h"
#include "SBWMethodAmbiguousException.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * create a signature parse structure given a signature.
 * @param signature SBW method signature to be parsed.
 */
sbwSignature::sbwSignature(const char *signature)
{
	result = parseType(signature);
	name = consumeName(signature);
	consume("(", signature);
	arguments = new SignatureListContents(signature);
	consume(")", signature);
}

/**
 * recovers all the memory consumed by the signature structure
 */
sbwSignature::~sbwSignature()
{
	delete arguments ;
}

/**
 * parse a given string extracting the front substring.
 * The front substring is terminated by any of the following
 * characters: {}[](), as well as whitespace and the null terminator.  Any initial whitespace is ignored/skipped.
 * @param signature string to be parsed, this string pointer value is changed to point beyond the extracted substring
 * @return the extracted substring (doesn't include any leading whitespace)
 */
std::string sbwSignature::consumeName(const char *&signature)
{
	consumeSpace(signature);

	std::string result ;

	while (*signature != '\0' && *signature != '{' && *signature != '}' &&
				*signature != '[' && *signature != ']' &&
				*signature != '(' && *signature != ')' &&
				*signature != ',' && *signature != '\t' && *signature != ' ')
	{
		result += *signature ;
		signature++ ;
	}

	return result ;
}

/**
 * parse a given string, consuming the whitespace at the front of the string.
 * @param signature string to be parsed, this string pointer value is changed to point beyond the front whitespace.
 */
void sbwSignature::consumeSpace(const char *&signature)
{
	while (*signature == ' ' || *signature == '\t')
		signature++ ;\
}

/**
 * parse a given string, consuming an expected substring.  Any leading whitespace is ignored/skipped.
 * @throw SBWSignatureSyntaxException * if signature doesn't contain the expected string.
 * @param expected the expected content of signature after any leading whitespace
 * @param signature the string to be parsed, this string pointer value is chnaged to point beyond the expected string.
 */
void sbwSignature::consume(const char *expected, const char *&signature)
{
	if (!compareString(expected, signature))
	{
		std::string message("syntax error expecting ") ;

		message += expected ;
		message += " got ";
		message += signature ;
		throw new SBWSignatureSyntaxException(message);
	}
}

/**
 * parse a given string, consuming an expected substring.  Any leading whitespace is ignored/skipped.
 * @param expected the expected content of signature after any leading whitespace
 * @param signature the string to be parsed, this string pointer value is changed to point beyond the expected string if
 * it contains the expected string.
 * @return whether signature contains the expected string.
 */
bool sbwSignature::compareString(const char *expected, const char *&signature)
{
	consumeSpace(signature);

	int len = strlen(expected);
	
	if (strncmp(expected, signature, len))
		return false ;

	signature += len;
	
	return true ;
}

/**
 * parse a given string, extracting the initial SBW scalar type. Any leading whitespace is ignored/skipped.
 * @param signature the string to be parsed, this string pointer value is changed to point beyond the scalar type if it exists.
 * @return 
 * - NULL if the front of signature doesn't match any scalar types.
 * - otherwise a sbwSignatureType structure parsed from signature.
 */
sbwSignatureType *sbwSignature::parseScalarType(const char *&signature)
{
	consumeSpace(signature);

	if (*signature == '{')
		return new sbwSignatureList(signature);
	if (compareString("string", signature))
		return new sbwSignatureScalar<char *, StringType>();
	else if (compareString("int", signature))
		return new sbwSignatureScalar<Integer, IntegerType>();
	else if (compareString("double", signature))
		return new sbwSignatureScalar<Double, DoubleType>();
	else if (compareString("boolean", signature))
		return new sbwSignatureBoolean();
	else if (compareString("byte", signature))
		return new sbwSignatureByte();
	else if (compareString("void", signature))
		return new sbwSignatureVoid();
	else if (compareString("complex", signature))
		return new sbwSignatureScalar<SBWComplex, ComplexType>();
	else
	{
		std::string message = "Whilst parsing signature: ";
		
		message += "Expecting start of type (one of: {, string, int, double, complex, boolean, byte or void) instead got ";
		message += signature;
		throw new SBWSignatureSyntaxException(message);
	}
}

/**
 * parse a given string, extracting the initial SBW type. Any leading whitespace is ignored/skipped.
 * @param signature the string to be parsed, this string pointer value is changed to point beyond the type if it exists.
 * @return 
 * - NULL if the front of signature doesn't match any SBW types.
 * - otherwise a sbwSignatureType structure parsed from signature.
 */
sbwSignatureType *sbwSignature::parseType(const char *&signature)
{
	sbwSignatureType *scalarType = parseScalarType(signature);
	int dimensions = 0 ;

	while (compareString("[]", signature))
		dimensions++;

	if (dimensions == 0)
		return scalarType ;
	else
		return new sbwSignatureArray(scalarType, dimensions);
}

/** 
 * returns whether a string is a match for a given complete signature.
 * @param completeSignature a complete SBW method signature
 * @param partialSignature either
 * - just a method name
 * - a signature without a return type
 * - a complete signature.
 * @return whether partialSignature is a match for completeSignature.
 */
bool sbwSignature::match(const char *completeSignature, const char *partialSignature)
{

	if (strcmp(completeSignature, partialSignature) == 0)
		return true ;

	sbwSignature signature(completeSignature);

	try
	{
		sbwSignature otherSignature(partialSignature);

		return
			signature.getName().compare(otherSignature.getName()) == 0 &&
				otherSignature.getArguments().match(signature.getArguments());
	}
	catch (SBWSignatureSyntaxException *)
	{
		std::string name = consumeName(partialSignature);

		if (name.compare(signature.getName()) == 0)
		{	
			if (*partialSignature == '\0')
				return true ; 
			
			consume("(", partialSignature);
			SignatureListContents *arguments = new SignatureListContents(partialSignature);
			consume(")", partialSignature);

			bool result = arguments->match(signature.getArguments());

			delete arguments ;

			return result ;
		}
	}

	return false ;
}

/**
 * returns the index of a method in a given array which matches with a given signature.
 * @param methods array of signatures.
 * @param signature the signature of the method to be located.
 * @return the numeric method on the given service with the given signature.
 */
Integer sbwSignature::match(const std::vector<std::string> &methods, const char *signature)
{
	Integer methodId = 0, methodsFound = 0, methodFound = 0;

	while (methodId !=  (Integer)methods.size())
	{
		if (sbwSignature::match(methods[methodId].c_str(), signature))
		{
			methodsFound++ ;
			methodFound = methodId;
		}

		methodId++ ;
	}

	if (methodsFound == 0)
		throw new SBWMethodNotFoundException("method not found");

	if (methodsFound != 1)
		throw new SBWMethodAmbiguousException("more than one method matches the signature given");

	return methodFound ;
}

/////////////////////////////////////////////////////////////////////////////////////

/**
 * create a signature structure by parseing a given signature string.
 * @param signature the SBW signature string to be parsed.
 */
SBW_API Signature::Signature(const char *signature)
: SBWHandle<sbwSignature>(new sbwSignature(signature)) 
{
}

/**
 * create a signature structure by parseing a given signature string.
 * @param signature the SBW signature string to be parsed.
 */
SBW_API Signature::Signature(std::string signature)
: SBWHandle<sbwSignature>(new sbwSignature(signature.c_str())) 
{
}

/**
 * returns a vector of SignatureElement objects corresponding to the arguments of the parsed signature.
 * The returned vector should be deallocated using delete.
 * @return a vector of SignatureElement objects corresponding to the arguments of the parsed signature.
 */
SBW_API std::vector<SignatureElement> *Signature::getArguments()
{
	return p->getArguments().copyVector();
}

