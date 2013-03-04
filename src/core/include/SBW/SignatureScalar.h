/**
 * @file SignatureScalar.h
 * @brief represents a scalar type inside a signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SignatureScalar.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SignatureScalar.h,v $
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

/// MSVC generated include loop block
#if !defined(AFX_SIGNATURESCALAR_H__8CAF0AA5_6731_4165_9C40_2CA464B2C339__INCLUDED_)
#define AFX_SIGNATURESCALAR_H__8CAF0AA5_6731_4165_9C40_2CA464B2C339__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SignatureType.h"
//#ifndef WIN32
#include <stdarg.h>
//#endif


namespace SystemsBiologyWorkbench
{

/**
 * represents a scalar type inside a signature
 * @param T the C API type corresponding to the type
 * @param X the DataBlockType corresponding to the type
 */
template<class T, DataBlockType X> class sbwSignatureScalar : public sbwSignatureType  
{
public:
	sbwSignatureScalar() {};

	/**
	 * returns the DataBlockType corresponding this type.
	 * @return the DataBlockType corresponding this type.
	 */
	DataBlockType getType() { return X; }

	/**
	 * For C API : store a scalar value from a variable argument list in a given data block.
	 * @param writer data block in which the scalar value will be stored.
	 * @param ap variable argument list - the next item is assumed to be a scalar value.
	 */
	void write(DataBlockWriter writer, va_list &ap)
	{
		T x = va_arg(ap, T);

		writer << x ;
	}

	/**
	 * For C API : read a scalar value from a given data block.
	 * @param reader data block in which the scalar value is stored.
	 * @param ap variable argument list - the next item is assumed to be an scalar pointer,
	 *           the extracted value is stored in the referenced location.
	 */
	void read(DataBlockReader reader, va_list &ap)
	{
		T *x = va_arg(ap, T *);

		reader.getForC(*x) ;
	}

	/**
	 * For C API : store array of scalar values in data block.
	 * @param writer data block in which scalar array to be stored
	 * @param size number of scalar values in the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a scalar array.
	 */
	void write1DArray(DataBlockWriter writer, Integer size, va_list &ap)
	{
		T *x = va_arg(ap, T *);

		writer.add(size, x);
	}

	/**
	 * For C API : read an array of scalar values from a data block.
	 * @param reader data block in which scalar array is stored
	 * @param size number of scalar values in the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to a scalar array.
	 *           This pointer is changed to point to a scalar array allocated on the C Heap.
	 *           Use SBWFree to deallocate this array.
	 */
	void read1DArray(DataBlockReader reader, Integer &size, va_list &ap)
	{
		T **x = va_arg(ap, T **);

		reader.getForC(size, *x);
	}

	/**
	 * For C API : store 2D array of scalar values in data block.
	 * @param writer data block in which scalar array to be stored
	 * @param xSize number of scalar values in the first dimension in the array
	 * @param ySize number of scalar values in the 2nd dimension in the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a
	 *           scalar * array where each element points to an scalar array.
	 */
	void write2DArray(DataBlockWriter writer, Integer xsize, Integer ysize, va_list &ap)
	{
		T **x = va_arg(ap, T **);

		writer.add(xsize, ysize, x);
	}

	/**
	 * For C API : read a 2D array of scalar values from a data block.
	 * @param reader data block in which scalar array is stored
	 * @param xSize set to the number of scalar values on the first dimension of the array
	 * @param ySize set to the number of scalar values on the first dimension of the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to a scalar * array.
	 *           This pointer is changed to point to a scalar * array allocated on the C Heap.
	 *           Use SBWFree2DArray to deallocate this array.
	 */
	void read2DArray(DataBlockReader reader, Integer &xsize, Integer &ysize, va_list &ap)
	{
		T ***x = va_arg(ap, T ***);

		reader.getForC(xsize, ysize, *x);
	}
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SIGNATURESCALAR_H__8CAF0AA5_6731_4165_9C40_2CA464B2C339__INCLUDED_)
