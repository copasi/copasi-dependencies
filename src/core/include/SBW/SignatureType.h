/**
 * @file SignatureType.h
 * @brief represents a type inside a signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SignatureType.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SignatureType.h,v $
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
*/// SignatureType.h: interface for the SignatureType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIGNATURETYPE_H__C85CBF03_84D1_4A65_89E5_DDC44C4DEF24__INCLUDED_)
#define AFX_SIGNATURETYPE_H__C85CBF03_84D1_4A65_89E5_DDC44C4DEF24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "portableOS.h"
#include "DataBlockWriterClass.h"
#include "DataBlockReaderClass.h"
#include "SignatureElement.h"
//#ifndef WIN32
#include <stdarg.h>
//#endif
namespace SystemsBiologyWorkbench
{

/// underlying implementation of SignatureType and base class of signature type representations.
struct sbwSignatureType : public SBWObject
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
	virtual std::vector<SignatureElement> *getContents() { throw new SBWTypeMismatchException("this type is not a list");}

	/**
	 * returns the element type assuming this represents an array type.
	 * This default implementation throws an exception.
     * @throws SBWTypeMismatchException - default behaviour
	 * @returns the element type assuming this represents an array type.
	 */
	virtual sbwSignatureType *getArrayInnerType() { throw new SBWTypeMismatchException("this type is not an array"); }

	/**
	 * returns the number of dimensions of the array type assuming this represents an array type.
	 * This default implementation throws an exception.
     * @throws SBWTypeMismatchException - default behaviour
	 * @returns the number of dimensions of the array type assuming this represents an array type.
	 */
	virtual Integer getArrayDimensions() { throw new SBWTypeMismatchException("this type is not an array"); }

	/**
	 * returns whether this type is a list
	 * @returns flase by default.
	 */
	virtual bool isList() { return false; }

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
	virtual bool match(sbwSignatureType *other) { return getType() == other->getType(); }
};

/// represents a type inside an SBW method signature
class SignatureType : public SBWHandle<sbwSignatureType>
{
public:
	/// creates an empty type
	SignatureType() {} ;

	/**
	 * creates a type object given an underlying implementation.
	 * @param x underlying implementation.
	 */
	SignatureType(sbwSignatureType *x) : SBWHandle<sbwSignatureType>(x) {}

	/**
	 * returns the DataBlockType of this type
	 * @return the DataBlockType of this type
	 */
	DataBlockType getType() { return p->getType() ; }

	/**
	 * returns the contents of this list type assuming this represents a list type.
	 * This default implementation throws an exception.
	 * The result should be deallocated using the delete[] operator.
     * @throws SBWTypeMismatchException - default behaviour
	 * @returns the contents of this list type assuming this represents a list type
	 */	
	std::vector<SignatureElement> *getListContents() { return p->getContents(); }

	/**
	 * returns the element type assuming this represents an array type.
	 * This default implementation throws an exception.
     * @throws SBWTypeMismatchException - default behaviour
	 * @returns the element type assuming this represents an array type.
	 */
	SignatureType getArrayInnerType() { return p->getArrayInnerType(); }

	/**
	 * returns the underlying implementation.
	 * @return the underlying implementation.
	 */
	sbwSignatureType *getImpl() { return p ; }

	/**
	 * returns the number of dimensions of the array type assuming this represents an array type.
	 * This default implementation throws an exception.
     * @throws SBWTypeMismatchException - default behaviour
	 * @returns the number of dimensions of the array type assuming this represents an array type.
	 */
	Integer getArrayDimensions() { return p->getArrayDimensions(); }

	/**
	 * returns whether this type is equal to another type.
	 * @param x other type.
	 * @return whether this type is equal to another type.
	 */
	bool match(SignatureType x) { return p->match(x.p); }
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SIGNATURETYPE_H__C85CBF03_84D1_4A65_89E5_DDC44C4DEF24__INCLUDED_)
