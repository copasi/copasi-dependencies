/**
 * @file SignatureBoolean.cpp
 * @brief represents a Boolean type in a signature
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

// sbwSignatureBoolean.cpp: implementation of the sbwSignatureBoolean class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#ifndef WIN32
#include <stdarg.h>
//#endif

#include "SignatureBoolean.h"
#include "ImplDataBlockWriter.h"

using namespace SystemsBiologyWorkbench ;

#include "SBWC.h"

/**
 * For C API : store a boolean value from a variable argument list in a given data block.
 * @param writer data block in which the boolean will be stored.
 * @param ap variable argument list - the next item is assumed to be an SBWBoolean value.
 */
DataBlockType sbwSignatureBoolean::getType() { return BooleanType; }

void sbwSignatureBoolean::write(DataBlockWriter writer, va_list &ap)
{
	SBWBoolean arg = va_arg(ap, SBWBoolean);
	bool x = (arg != 0) ;

	writer << x ;
}

/**
 * For C API : read a SBWBoolean value from a given data block.
 * @param reader data block in which the boolean is stored.
 * @param ap variable argument list - the next item is assumed to be an SBWBoolean pointer,
 *           the extracted value is stored in the referenced location.
 */
void sbwSignatureBoolean::read(DataBlockReader reader, va_list &ap)
{
	SBWBoolean *result = va_arg(ap, SBWBoolean *);
	bool x ;

	reader.getForC(x) ;
	*result = x ;
}

/**
 * For C API : store array of SBWBoolean in data block.
 * @param writer data block in which SBWBoolean array to be stored
 * @param size number of boolean values in the array
 * @param ap variable argument list - the next item is assumed to be a pointer to a SBWBoolean array.
 */
void sbwSignatureBoolean::write1DArray(DataBlockWriter writer, Integer size, va_list &ap)
{
	SBWBoolean *arg = va_arg(ap, SBWBoolean *);
	bool *array = new bool[size];
	int i = 0 ;

	while (i != size)
	{
		array[i] = (arg[i] != 0) ;
		i++ ;
	}

	writer.add(size, array);
	delete[] array;
}

/**
 * For C API : read an array of SBWBoolean from a data block.
 * @param reader data block in which SBWBoolean is stored
 * @param size number of boolean values in the array
 * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to a SBWBoolean array.
 *           This pointer is changed to point to a SBWBoolean array allocated on the C Heap.
 *           Use SBWFree to deallocate this array.
 */
void sbwSignatureBoolean::read1DArray(DataBlockReader reader, Integer &size, va_list &ap)
{
	SBWBoolean **result = va_arg(ap, SBWBoolean **);
	bool *array ;

	reader.get(size, array);
	*result = (SBWBoolean *)calloc(size, sizeof(SBWBoolean));

	int i = 0 ;

	while (i != size)
	{
		(*result)[i] = array[i] ;
		i++ ;
	}

	delete array ;
}

/**
 * For C API : store 2D array of SBWBoolean in data block.
 * @param writer data block in which SBWBoolean array to be stored
 * @param xSize number of boolean values in the first dimension in the array
 * @param ySize number of boolean values in the 2nd dimension in the array
 * @param ap variable argument list - the next item is assumed to be a pointer to a SBWBoolean * array where each element points
 *           to an SBWBoolean array.
 */
void sbwSignatureBoolean::write2DArray(DataBlockWriter writer, Integer xSize, Integer ySize, va_list &ap)
{
	SBWBoolean **arg = va_arg(ap, SBWBoolean **);
	bool **array = new bool *[xSize];
	int x = 0 ;

	while (x != xSize)
	{
		array[x] = new bool[ySize];

		int y = 0 ;

		while (y != ySize)
		{
			array[x][y] = (arg[x][y] != 0);
			y++ ;
		}

		x++ ;
	}

	writer.add(xSize, ySize, array);
	sbwDataBlockReader::free2DArray(xSize, array);
}

/**
 * For C API : read a 2D array of SBWBoolean from a data block.
 * @param reader data block in which SBWBoolean is stored
 * @param xSize set to the number of boolean values on the first dimension of the array
 * @param ySize set to the number of boolean values on the first dimension of the array
 * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to a SBWBoolean * array.
 *           This pointer is changed to point to a SBWBoolean * array allocated on the C Heap.
 *           Use SBWFree2DArray to deallocate this array.
 */
void sbwSignatureBoolean::read2DArray(DataBlockReader reader, Integer &xSize, Integer &ySize, va_list &ap)
{
	SBWBoolean ***result = va_arg(ap, SBWBoolean ***);
	bool **array ;

	reader.get(xSize, ySize, array);

	*result = (SBWBoolean **)calloc(xSize, sizeof(SBWBoolean *));
	int x = 0 ;

	while (x != xSize)
	{
		(*result)[x] = (SBWBoolean *)calloc(ySize, sizeof(SBWBoolean));

		int y = 0 ;

		while (y != ySize)
		{
			(*result)[x][y] = array[x][y];
			y++ ;
		}

		x++ ;
	}

	sbwDataBlockReader::free2DArray(xSize, array);
}
