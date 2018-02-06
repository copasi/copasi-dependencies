/**
 * @file sbwSignatureByte.h
 * @brief represents a byte type within a parsed signature structure
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

/// MSVC generated include loop block
#if !defined(AFX_SBWSIGNATUREBYTE_H__70E3DD5E_BC0D_4C1E_8B04_891E436241F8__INCLUDED_)
#define AFX_SBWSIGNATUREBYTE_H__70E3DD5E_BC0D_4C1E_8B04_891E436241F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/SignatureType.h>

namespace SystemsBiologyWorkbench
{

/// parse structure of a byte type in an SBW signature
class SBW_API sbwSignatureByte : public sbwSignatureType
{
public:
	/**
	 * returns ByteType.
	 * @see ByteType
	 * @return ByteType.
	 */
  DataBlockType getType();

	/**
	 * For C API : store a byte value from a variable argument list in a given data block.
	 * @param writer data block in which the byte will be stored.
	 * @param ap variable argument list - the next item is assumed to be a unsigned char value.
	 */
  void write(DataBlockWriter writer, va_list &ap);

	/**
	 * For C API : read a byte value from a given data block.
	 * @param reader data block in which the byte is stored.
	 * @param ap variable argument list - the next item is assumed to be an unsigned char pointer,
	 *           the extracted value is stored in the referenced location.
	 */
  void read(DataBlockReader reader, va_list &ap);

	/**
	 * For C API : store array of bytes in data block.
	 * @param writer data block in which byte array to be stored
	 * @param size number of byte values in the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a byte array.
	 */
  void write1DArray(DataBlockWriter writer, Integer size, va_list &ap);

	/**
	 * For C API : read an array of byte from a data block.
	 * @param reader data block in which byte is stored
	 * @param size number of byte values in the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to a byte array.
	 *           This pointer is changed to point to a byte array allocated on the C Heap.
	 *           Use SBWFree to deallocate this array.
	 */
  void read1DArray(DataBlockReader reader, Integer &size, va_list &ap);

	/**
	 * For C API : store 2D array of bytes in data block.
	 * @param writer data block in which the byte array is to be stored
	 * @param xSize number of bytes in the first dimension in the array
	 * @param ySize number of bytes in the 2nd dimension in the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a unsigned char * array
	 *           where each element points to an SBWBoolean array.
	 */
  void write2DArray(DataBlockWriter writer, Integer xsize, Integer ysize, va_list &ap);

	/**
	 * For C API : read a 2D array of bytes from a data block.
	 * @param reader data block in which byte array is stored
	 * @param xSize set to the number of byte values on the first dimension of the array
	 * @param ySize set to the number of byte values on the first dimension of the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to a unsigned char *
	 *           array. This pointer is changed to point to an unsigned char * array allocated on the C Heap.
	 *           Use SBWFree2DArray to deallocate this array.
	 */
  void read2DArray(DataBlockReader reader, Integer &xsize, Integer &ysize, va_list &ap);
};

} // SystemsBiologyWorkbench

#endif 
