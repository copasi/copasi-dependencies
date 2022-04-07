/**
 * @file Signature.cpp
 * @brief represents the structure of a signature
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
    arguments=NULL;
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

const SignatureListContents &sbwSignature::getArguments() { return *arguments ; }

SignatureType sbwSignature::getReturnType() const { return result ; }

std::string sbwSignature::getName() const { return name ; }

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
 Signature::Signature(const char *signature)
: SBWHandle<sbwSignature>(new sbwSignature(signature))
{
}

/**
 * create a signature structure by parseing a given signature string.
 * @param signature the SBW signature string to be parsed.
 */
 Signature::Signature(std::string signature)
: SBWHandle<sbwSignature>(new sbwSignature(signature.c_str()))
{
}

/**
 * returns a vector of SignatureElement objects corresponding to the arguments of the parsed signature.
 * The returned vector should be deallocated using delete.
 * @return a vector of SignatureElement objects corresponding to the arguments of the parsed signature.
 */
 std::vector<SignatureElement> *Signature::getArguments()
{
    return p->getArguments().copyVector();
}

SignatureType Signature::getReturnType() { return p->getReturnType() ; }

std::string Signature::getName() const { return p->getName() ; }

sbwSignature *Signature::getImpl() { return p; }

