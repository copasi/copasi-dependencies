/**
 * @file SignatureList.cpp
 * @brief represents a list type inside a signature
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

// sbwSignatureList.cpp: implementation of the sbwSignatureList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#ifndef WIN32
#include <stdarg.h>
//#endif

#include "SignatureList.h"

#include "SignatureListContents.h"
#include "Signature.h"
#include "ImplDataBlockWriter.h"
#include "ImplDataBlockReader.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/// deallocates the content list
sbwSignatureList::~sbwSignatureList()
{
	delete contents;
	contents = NULL;
}

/**
 * constructs a list type object given a string pointing to the start of a list type definition.
 * @param signature string prefixed with list type definition.  The string pointer is chnaged to point
 *        to the first character after the type definition
 *
 */
bool sbwSignatureList::isList() { return true; }

sbwSignatureList::sbwSignatureList(const char *&signature)
{
	sbwSignature::consume("{", signature);
	contents = new SignatureListContents(signature);
	sbwSignature::consume("}", signature);
}

/**
 * For the C API :stores a sbwDataBlockWriter object in a datablock.
 * @param writer datablock in which list will be stored.
 * @param ap variable argument list containing a pointer to a sbwDataBlockerWriter which will be stored in
 *			 the data block as a list object.  The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::write(DataBlockWriter writer, va_list &ap)
{
	sbwDataBlockWriter *list = va_arg(ap, sbwDataBlockWriter *);

	writer << list ;
}

/**
 * For the C API :reads a list from a datablock.  The list is stored in a new sbwDataBlockWriter object 
 * @param reader datablock from which list will be read.
 * @param ap variable argument list containing a pointer to a pointer to a sbwDataBlockerWriter which will be set
 *           to a new sbwDataBlockWriter object allocated on the C Heap.  
 *			 The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::read(DataBlockReader reader, va_list &ap)
{
	sbwDataBlockReader **list = va_arg(ap, sbwDataBlockReader **);

	reader.getForC(*list) ;
}

/**
 * For the C API :stores an array of sbwDataBlockWriter objects in a datablock as an array of lists
 * @param writer datablock in which array will be stored.
 * @param ap variable argument list containing a pointer to a sbwDataBlockerWriter array which will be stored in
 *			 the data block as a list array.  The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::write1DArray(DataBlockWriter writer, Integer size, va_list &ap)
{
	sbwDataBlockWriter **x = va_arg(ap, sbwDataBlockWriter **);

	writer.add(size, x);
}

/**
 * For the C API :reads a list array from a datablock.  The array is stored in a new sbwDataBlockWriter * array 
 * @param reader datablock from which array will be read.
 * @param ap variable argument list containing a pointer to a pointer to a sbwDataBlockerWriter array which will be set
 *           to a new sbwDataBlockWriter array allocated on the C Heap.  
 *			 The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::read1DArray(DataBlockReader reader, Integer &size, va_list &ap)
{
	sbwDataBlockReader ***x = va_arg(ap, sbwDataBlockReader ***);

	reader.getForC(size, *x);
}


/**
 * For the C API :stores an array of sbwDataBlockWriter objects in a datablock as an array of lists
 * @param writer datablock in which array will be stored.
 * @param ap variable argument list containing a pointer to a sbwDataBlockerWriter array which will be stored in
 *			 the data block as a list array.  The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::write2DArray(DataBlockWriter writer, Integer xsize, Integer ysize, va_list &ap)
{
	sbwDataBlockWriter ***x = va_arg(ap, sbwDataBlockWriter ***);

	writer.add(xsize, ysize, x);
}

/**
 * For the C API :reads a list 2D array from a datablock.  The array is stored in a new sbwDataBlockWriter ** array 
 * @param reader datablock from which array will be read.
 * @param ap variable argument list containing a pointer to a pointer to a sbwDataBlockerWriter * array which will be
 *           set to a new sbwDataBlockWriter * array allocated on the C Heap.  
 *			 The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::read2DArray(DataBlockReader reader, Integer &xsize, Integer &ysize, va_list &ap)
{
	sbwDataBlockReader ****x = va_arg(ap, sbwDataBlockReader ****);

	reader.getForC(xsize, ysize, *x);
}

/**
 * destroy this object returning the contents as a SignatureListContents object.
 * @return the contents as a SignatureListContents object.  
 *         The returned object should deleted using the delete operator.
 */
SignatureListContents *sbwSignatureList::discardShell()
{
	SignatureListContents *result = contents ;
	contents = NULL ;
	delete this ;

	return result ;
}

/**
 * return the contents of this list type definition.
 * @return the contents of this list type definition.
 *         The returned vector object should deleted using the delete operator.
 */
std::vector<SignatureElement> *sbwSignatureList::getContents()
{
	return contents->copyVector();
}

/**
 * returns whether this type is equal to another type.
 * This implementation checks to see if the list contents match if neither are empty.
 * @param other other type.
 * @return whether this type is equal to another type.
 */
bool sbwSignatureList::match(sbwSignatureType *other)
{
	if (other->getType() != ListType)
		return false;

	sbwSignatureList *otherList = (sbwSignatureList *)other ;

	return otherList->contents->size() == 0 || contents->size() == 0 || contents->match(*(otherList->contents));
}
