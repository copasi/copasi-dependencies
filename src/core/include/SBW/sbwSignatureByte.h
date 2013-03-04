/**
 * @file sbwSignatureByte.h
 * @brief represents a byte type within a parsed signature structure
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: sbwSignatureByte.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/sbwSignatureByte.h,v $
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
#if !defined(AFX_SBWSIGNATUREBYTE_H__70E3DD5E_BC0D_4C1E_8B04_891E436241F8__INCLUDED_)
#define AFX_SBWSIGNATUREBYTE_H__70E3DD5E_BC0D_4C1E_8B04_891E436241F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SignatureType.h"

namespace SystemsBiologyWorkbench
{

/// parse structure of a byte type in an SBW signature
class sbwSignatureByte : public sbwSignatureType  
{
public:
	/**
	 * returns ByteType.
	 * @see ByteType
	 * @return ByteType.
	 */
	DataBlockType getType() { return ByteType; }

	/**
	 * For C API : store a byte value from a variable argument list in a given data block.
	 * @param writer data block in which the byte will be stored.
	 * @param ap variable argument list - the next item is assumed to be a unsigned char value.
	 */
	void write(DataBlockWriter writer, va_list &ap)
	{
		// The C++ C99 standard requires that "the type used in va_arg is
		// supposed to match the actual type **after default promotions**."

		unsigned char x = (unsigned char) va_arg(ap, int /* unsigned char */);

		writer << x ;
	}

	/**
	 * For C API : read a byte value from a given data block.
	 * @param reader data block in which the byte is stored.
	 * @param ap variable argument list - the next item is assumed to be an unsigned char pointer,
	 *           the extracted value is stored in the referenced location.
	 */
	void read(DataBlockReader reader, va_list &ap)
	{
		unsigned char *x = (unsigned char *) va_arg(ap, unsigned char *);

		reader.getForC(*x) ;
	}

	/**
	 * For C API : store array of bytes in data block.
	 * @param writer data block in which byte array to be stored
	 * @param size number of byte values in the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a byte array.
	 */
	void write1DArray(DataBlockWriter writer, Integer size, va_list &ap)
	{
		unsigned char *x = (unsigned char *) va_arg(ap, unsigned char *);

		writer.add(size, x);
	}

	/**
	 * For C API : read an array of byte from a data block.
	 * @param reader data block in which byte is stored
	 * @param size number of byte values in the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to a byte array.
	 *           This pointer is changed to point to a byte array allocated on the C Heap.
	 *           Use SBWFree to deallocate this array.
	 */
	void read1DArray(DataBlockReader reader, Integer &size, va_list &ap)
	{
		unsigned char **x = (unsigned char **)va_arg(ap, unsigned char **);

		reader.getForC(size, *x);
	}

	/**
	 * For C API : store 2D array of bytes in data block.
	 * @param writer data block in which the byte array is to be stored
	 * @param xSize number of bytes in the first dimension in the array
	 * @param ySize number of bytes in the 2nd dimension in the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a unsigned char * array
	 *           where each element points to an SBWBoolean array.
	 */
	void write2DArray(DataBlockWriter writer, Integer xsize, Integer ysize, va_list &ap)
	{
		unsigned char **x = (unsigned char **)va_arg(ap, unsigned char **);

		writer.add(xsize, ysize, x);
	}

	/**
	 * For C API : read a 2D array of bytes from a data block.
	 * @param reader data block in which byte array is stored
	 * @param xSize set to the number of byte values on the first dimension of the array
	 * @param ySize set to the number of byte values on the first dimension of the array
	 * @param ap variable argument list - the next item is assumed to be a pointer to a pointer to a unsigned char *
	 *           array. This pointer is changed to point to an unsigned char * array allocated on the C Heap.
	 *           Use SBWFree2DArray to deallocate this array.
	 */
	void read2DArray(DataBlockReader reader, Integer &xsize, Integer &ysize, va_list &ap)
	{
		unsigned char ***x = (unsigned char ***)va_arg(ap, unsigned char ***);

		reader.getForC(xsize, ysize, *x);
	}
};

} // SystemsBiologyWorkbench

#endif 
