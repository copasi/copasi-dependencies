/**
 * @file SignatureBoolean.cpp
 * @brief represents a Boolean type in a signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SignatureBoolean.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SignatureBoolean.cpp,v $
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
	delete array;
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
