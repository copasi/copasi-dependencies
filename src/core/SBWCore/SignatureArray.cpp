/**
 * @file SignatureArray.cpp
 * @brief represents an arry type inside a signature
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

// sbwSignatureArray.cpp: implementation of the sbwSignatureArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#ifndef WIN32
#include <stdarg.h>
//#endif


#include "SignatureArray.h"

#include "SBWUnsupportedObjectTypeException.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * create signature array object.
 * @param st the type of the elements of the array
 * @param d the number of dimensions of the array
 */
sbwSignatureArray::sbwSignatureArray(SignatureType st, Integer d) : scalarType(st), dimensions(d)
{
}

DataBlockType sbwSignatureArray::getType() { return ArrayType; }

sbwSignatureType*sbwSignatureArray::getArrayInnerType() { return scalarType.getImpl(); }

Integer sbwSignatureArray::getArrayDimensions() { return dimensions; }

/**
 * For C API : store an array as defined by this object in a given data block.
 * @param writer the data block in which the data will be stored.
 * @param ap the variable argument list that contains the array data.  If the array is one dimesional
 *           then the data consists of an integer (the number of elements) followed by a pointer to a C array of data.
 *           If the array is two dimensional the data consists of 2 integers followed by a pointer to an array of pointer to data.
 */
void sbwSignatureArray::write(DataBlockWriter writer, va_list &ap)
{
	sbwSignatureType *innerType = scalarType.getImpl();

	switch (dimensions)
	{
		case 1 :
			{
				Integer size = va_arg(ap, Integer);
				innerType->write1DArray(writer, size, ap);
				break ;
			}
		case 2 :
			{
				Integer xSize = va_arg(ap, Integer);
				Integer ySize = va_arg(ap, Integer);
				innerType->write2DArray(writer, xSize, ySize, ap);
				break ;
			}
		default :
			throw new SBWUnsupportedObjectTypeException("1 and 2 dimension arrays supported only");
	}
}

/**
 * For C API : read an array as defined by this object from a given data block.
 * The array allocated should be freed using SBWFreeArray or SBWFree2DArray depending on the array type.
 * @param reader the data block in which the data is stored.
 * @param ap the variable argument list that contains pointers to object that will contain the array data.
 *           If the array is one dimesional
 *           then the data consists of an integer pointer (the number of elements) followed by a pointer to a pointer to a C array of data.
 *           If the array is two dimensional the data consists of 2 integer pointers 
 *           followed by a pointer to a pointer to an array of pointers to data.
 */
void sbwSignatureArray::read(DataBlockReader reader, va_list &ap)
{
	sbwSignatureType *innerType = scalarType.getImpl();

	switch (dimensions)
	{
		case 1 :
			{
				Integer *size = va_arg(ap, Integer *);
				innerType->read1DArray(reader, *size, ap);
				break ;
			}
		case 2 :
			{
				Integer *xSize = va_arg(ap, Integer *);
				Integer *ySize = va_arg(ap, Integer *);
				innerType->read2DArray(reader, *xSize, *ySize, ap);
				break ;
			}
		default :
			throw new SBWUnsupportedObjectTypeException("1 and 2 dimension arrays supported only");
	}
}

/**
 * throws SBWUnsupportedObjectTypeException *
 * @throws SBWUnsupportedObjectTypeException *
 */
void sbwSignatureArray::write1DArray(DataBlockWriter /*writer*/, Integer /*size*/, va_list &/*ap*/)
{
	throw SBWUnsupportedObjectTypeException("arrays of arrays not supported");
}

/**
 * throws SBWUnsupportedObjectTypeException *
 * @throws SBWUnsupportedObjectTypeException *
 */
void sbwSignatureArray::read1DArray(DataBlockReader /*reader*/, Integer &/*size*/, va_list &/*ap*/)
{
	throw SBWUnsupportedObjectTypeException("arrays of arrays not supported");
}

/**
 * throws SBWUnsupportedObjectTypeException *
 * @throws SBWUnsupportedObjectTypeException *
 */
void sbwSignatureArray::write2DArray(DataBlockWriter /*writer*/, Integer /*xsize*/, Integer /*ysize*/, va_list &/*ap*/)
{
	throw SBWUnsupportedObjectTypeException("arrays of arrays not supported");
}

/**
 * throws SBWUnsupportedObjectTypeException *
 * @throws SBWUnsupportedObjectTypeException *
 */
void sbwSignatureArray::read2DArray(DataBlockReader /*reader*/, Integer &/*xsize*/, Integer &/*ysize*/, va_list &/*ap*/)
{
	throw SBWUnsupportedObjectTypeException("arrays of arrays not supported");
}

/**
 * returns whether a given signature type matches this type.
 * @return whether a given signature type matches this type.
 */
bool sbwSignatureArray::match(sbwSignatureType *other)
{
	return getType() == other->getType() && getArrayInnerType()->match(other->getArrayInnerType());
}
