/**
 * @file SignatureType.h
 * @brief represents a type inside a signature
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

// SignatureType.h: interface for the SignatureType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIGNATURETYPE_H__C85CBF03_84D1_4A65_89E5_DDC44C4DEF24__INCLUDED_)
#define AFX_SIGNATURETYPE_H__C85CBF03_84D1_4A65_89E5_DDC44C4DEF24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/sbwdefs.h>
#ifndef SBW_STRICT_INCLUDES
#include <SBW/portableOS.h>
#endif
#include <SBW/DataBlockWriterClass.h>
#include <SBW/DataBlockReaderClass.h>
#include <SBW/SignatureElement.h>

#include <stdarg.h>

namespace SystemsBiologyWorkbench
{

/// underlying implementation of SignatureType and base class of signature type representations.
struct SBW_API sbwSignatureType : public SBWObject
{
public:
    /**
     * returns the DataBlockType of this type
     * @return the DataBlockType of this type
     */
    virtual DataBlockType getType() = 0 ;

    /**
     * returns the contents of this list type assuming this represents a list type.
     * This default implementation throws an exception.
     * The result should be deallocated using the delete[] operator.
     * @throws SBWTypeMismatchException - default behaviour
     * @returns the contents of this list type assuming this represents a list type
     */
    virtual std::vector<SignatureElement> *getContents();

    /**
     * returns the element type assuming this represents an array type.
     * This default implementation throws an exception.
     * @throws SBWTypeMismatchException - default behaviour
     * @returns the element type assuming this represents an array type.
     */
    virtual sbwSignatureType *getArrayInnerType();

    /**
     * returns the number of dimensions of the array type assuming this represents an array type.
     * This default implementation throws an exception.
     * @throws SBWTypeMismatchException - default behaviour
     * @returns the number of dimensions of the array type assuming this represents an array type.
     */
    virtual Integer getArrayDimensions();

    /**
     * returns whether this type is a list
     * @returns flase by default.
     */
    virtual bool isList();

    // Note the functions involving va_list below are specific to the C interface
    // the use of these functions in C++ is not recommended

    /**
     * For C API : store a value from a variable argument list in a given data block.
     * The type of the value is assumed to correspond to this sbwSignatureType.
     * @param writer data block in which the value will be stored.
     * @param ap variable argument list - the next item is assumed to be the value to be stored.
     */
    virtual void write(DataBlockWriter writer, va_list &ap) = 0 ;

    /**
     * For C API : read a value from a given data block.
     * The type of the value is assumed to correspond to this sbwSignatureType.
     * @param reader data block in which the value is stored.
     * @param ap variable argument list - the next item is assumed to be a pointer,
     *           the extracted value is stored in the referenced location.
     */
    virtual void read(DataBlockReader reader, va_list &ap) = 0 ;

    /**
     * For C API : store array in data block.
     * The type of the elements is assumed to correspond to this sbwSignatureType.
     * @param writer data block in which array to be stored
     * @param size number of elements in the array
     * @param ap variable argument list - the next item is assumed to be a pointer to an array.
     */
    virtual void write1DArray(DataBlockWriter writer , Integer size, va_list &ap) = 0 ;

    /**
     * For C API : read an array from a data block.
     * The type of the elements is assumed to correspond to this sbwSignatureType.
     * @param reader data block in which array is stored
     * @param size number of elements in the array
     * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to an array.
     *           This pointer is changed to point to an array allocated on the C Heap.
     *           Use SBWFree to deallocate this array. (unless its a string array in which case use SBWFree2DArray)
     */
    virtual void read1DArray(DataBlockReader reader, Integer &size, va_list &ap) = 0 ;

    /**
     * For C API : store 2D array in data block.
     * @param writer data block in which array to be stored
     * @param xSize number of elements in the first dimension in the array
     * @param ySize number of elements in the 2nd dimension in the array
     * @param ap variable argument list - the next item is assumed to be a pointer to a
     *           pointer array where each element points to an array.
     */
    virtual void write2DArray(DataBlockWriter, Integer xsize, Integer ysize, va_list &) = 0 ;

    /**
     * For C API : read a 2D array from a data block.
     * @param reader data block in which array is stored
     * @param xSize set to the number of elements on the first dimension of the array
     * @param ySize set to the number of elements on the first dimension of the array
     * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to a pointer array.
     *           This pointer is changed to point to a pointer array allocated on the C Heap.
     *           Use SBWFree2DArray to deallocate this array. (doesn't work for string 2D string arrays)
     */
    virtual void read2DArray(DataBlockReader, Integer &xsize, Integer &ysize, va_list &) = 0 ;

    /**
     * returns whether this type is equal to another type.
     * @param other other type.
     * @return whether this type is equal to another type.
     */
    virtual bool match(sbwSignatureType *other);
};

/// represents a type inside an SBW method signature
class SBW_API SignatureType : public SBWHandle<sbwSignatureType>
{
public:
    /// creates an empty type
    SignatureType();

    /**
     * creates a type object given an underlying implementation.
     * @param x underlying implementation.
     */
    SignatureType(sbwSignatureType *x);

    /**
     * returns the DataBlockType of this type
     * @return the DataBlockType of this type
     */
    DataBlockType getType();

    /**
     * returns the contents of this list type assuming this represents a list type.
     * This default implementation throws an exception.
     * The result should be deallocated using the delete[] operator.
     * @throws SBWTypeMismatchException - default behaviour
     * @returns the contents of this list type assuming this represents a list type
     */
    std::vector<SignatureElement> *getListContents();

    /**
     * returns the element type assuming this represents an array type.
     * This default implementation throws an exception.
     * @throws SBWTypeMismatchException - default behaviour
     * @returns the element type assuming this represents an array type.
     */
    SignatureType getArrayInnerType();

    /**
     * returns the underlying implementation.
     * @return the underlying implementation.
     */
    sbwSignatureType *getImpl();

    /**
     * returns the number of dimensions of the array type assuming this represents an array type.
     * This default implementation throws an exception.
     * @throws SBWTypeMismatchException - default behaviour
     * @returns the number of dimensions of the array type assuming this represents an array type.
     */
    Integer getArrayDimensions();

    /**
     * returns whether this type is equal to another type.
     * @param x other type.
     * @return whether this type is equal to another type.
     */
    bool match(SignatureType x);
};


} // SystemsBiologyWorkbench

#endif // !defined(AFX_SIGNATURETYPE_H__C85CBF03_84D1_4A65_89E5_DDC44C4DEF24__INCLUDED_)
