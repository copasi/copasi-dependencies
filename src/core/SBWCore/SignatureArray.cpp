/**
 * @file SignatureArray.cpp
 * @brief represents an arry type inside a signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SignatureArray.cpp,v 1.2 2006/09/08 06:04:14 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SignatureArray.cpp,v $
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
