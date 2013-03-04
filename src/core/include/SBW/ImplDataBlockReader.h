/**
 * @file ImplDataBlockReader.h
 * @brief  implementation of DataBlockReader - inline methods
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ImplDataBlockReader.h,v 1.5 2006/09/10 03:55:47 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ImplDataBlockReader.h,v $
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
// sbwDataBlockReader.h: interface for the sbwDataBlockReader class.
//
//////////////////////////////////////////////////////////////////////

/// MSVC generated include loop block
#if !defined(AFX_IMPLDATABLOCKREADER_H__459EF159_6B8C_4C1C_8B01_AD611810D86E__INCLUDED_)
#define AFX_IMPLDATABLOCKREADER_H__459EF159_6B8C_4C1C_8B01_AD611810D86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SBWObject.h"
#include "ImplDataBlockWriter.h"
#include "SBWTypeMismatchException.h"

#include <complex>
#include <vector>
#include <deque>
#include <list>
#include <string>

/**
 * returns the type of the object at the cursor in the datablock ie the type of the next object to be read for the block.
 * @return the type of the object at the cursor in the datablock
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getNextType()
{
	if (end > length)
		throw new SBWTypeMismatchException("reached end of data");

	int x = (char)data[end];
	return (SystemsBiologyWorkbench::DataBlockType)x ;
}

/**
 * copies the encoded data in the data block into a given buffer.
 * @param x buffer into which the encoded data is copied
 * @param l the number of bytes to copy
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::extract(unsigned char *x, SystemsBiologyWorkbench::Integer l)
{
	if (end + l > length)
		throw new SBWTypeMismatchException("reached end of data");

	memcpy(x, data + end, l);
	end += l ;
}

/**
 * returns a copy of the encoded data from the cursor to the end of the datablock.
 * The returned data should be freed using SBWFree.
 * @param lengthReturned set to the number of bytes of returned data.
 * @return the copy of the encoded data
 */
inline unsigned char *SystemsBiologyWorkbench::sbwDataBlockReader::getRemainingDataForC(
	SystemsBiologyWorkbench::Integer &lengthReturned)
{
	lengthReturned = length - end ;
	unsigned char *result = (unsigned char *)malloc(lengthReturned);

	extract(result, lengthReturned);

	return result ;
}

/**
 * returns SystemsBiologyWorkbench::IntegerType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::IntegerType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(
	SystemsBiologyWorkbench::Integer & /*ignored*/)
{
	return( SystemsBiologyWorkbench::IntegerType);
}

/**
 * returns SystemsBiologyWorkbench::DoubleType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::DoubleType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(SystemsBiologyWorkbench::Double &/*ignored*/)
{
	return( SystemsBiologyWorkbench::DoubleType);
}

/**
 * returns SystemsBiologyWorkbench::ComplexType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::ComplexType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(SystemsBiologyWorkbench::SBWComplex &/*ignored*/)
{
	return( SystemsBiologyWorkbench::ComplexType);
}
/**
 * returns SystemsBiologyWorkbench::ComplexType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::ComplexType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(std::complex<double> &/*ignored*/)
{
	return( SystemsBiologyWorkbench::ComplexType);
}


/**
 * returns SystemsBiologyWorkbench::ByteType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::ByteType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(unsigned char &/*ignored*/)
{
	return( SystemsBiologyWorkbench::ByteType);
}

/**
 * returns SystemsBiologyWorkbench::BooleanType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::BooleanType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(bool &/*ignored*/)
{
	return( SystemsBiologyWorkbench::BooleanType);
}

/**
 * returns SystemsBiologyWorkbench::StringType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::StringType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(char *&/*ignored*/)
{
	return( SystemsBiologyWorkbench::StringType);
}

/**
 * returns SystemsBiologyWorkbench::StringType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::StringType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(std::string &/*ignored*/)
{
	return( SystemsBiologyWorkbench::StringType);
}

/**
 * returns SystemsBiologyWorkbench::ListType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::ListType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(
	DataBlockReader &/*ignored*/)
{
	return( SystemsBiologyWorkbench::ListType);
}

/**
 * returns SystemsBiologyWorkbench::ListType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::ListType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockReader::getType(
	sbwDataBlockReader *&/*ignored*/)
{
	return( SystemsBiologyWorkbench::ListType);
}

/**
 * Reads a byte from the DataBlockReader assuming that the cursor is beyond the type prefix for the byte object.
 * Moves the cursor to the beginning of the next object
 * @param x set to the byte data at the cursor
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutType(unsigned char & x)
{
	extract(&x, 1);
}

/**
 * Reads an integer from the DataBlockReader assuming that the cursor is beyond the type prefix for the integer object.
 * Moves the cursor to the beginning of the next object
 * @param x set to the integer data at the cursor
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutType(SystemsBiologyWorkbench::Integer& x)
{
#ifdef __BIG_ENDIAN__
	char cTemp[4];
	extract((unsigned char *)&cTemp[3], 1);
	extract((unsigned char *)&cTemp[2], 1);
	extract((unsigned char *)&cTemp[1], 1);
	extract((unsigned char *)&cTemp[0], 1);
	memcpy((unsigned char *)&x, cTemp, 4);	
#else
	extract((unsigned char *)&x, 4);
#endif
}

/**
 * Reads a double from the DataBlockReader assuming that the cursor is beyond the type prefix for the double object.
 * Moves the cursor to the beginning of the next object
 * @param x set to the double data at the cursor
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutType(SystemsBiologyWorkbench::Double& x)
{
#ifdef __BIG_ENDIAN__
	char cTemp[8];
	extract((unsigned char *)&cTemp[7], 1);
	extract((unsigned char *)&cTemp[6], 1);
	extract((unsigned char *)&cTemp[5], 1);
	extract((unsigned char *)&cTemp[4], 1);
	extract((unsigned char *)&cTemp[3], 1);
	extract((unsigned char *)&cTemp[2], 1);
	extract((unsigned char *)&cTemp[1], 1);
	extract((unsigned char *)&cTemp[0], 1);
	memcpy((unsigned char *)&x, cTemp, 8);
#else
	extract((unsigned char *)&x, 8);
#endif
}

/**
 * Reads a SBWComplex from the DataBlockReader assuming that the cursor is beyond the type prefix for the double object.
 * Moves the cursor to the beginning of the next object
 * @param x set to the SBWComplex data at the cursor
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutType(SystemsBiologyWorkbench::SBWComplex& x)
{
	double dReal; double dImag;
	getWithoutType(dReal);
	getWithoutType(dImag);
	x.dReal = dReal;
	x.dImag = dImag;
}
/**
 * Reads a complex<double> from the DataBlockReader assuming that the cursor is beyond the type prefix for the double object.
 * Moves the cursor to the beginning of the next object
 * @param x set to the complex<double> data at the cursor
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutType(std::complex<double>& x)
{
	double dReal; double dImag;
	getWithoutType(dReal);
	getWithoutType(dImag);
#ifdef WIN32
	x.real(dReal);
	x.imag(dImag);
#else
	x = std::complex<double>(dReal,dImag);
#endif
}


/**
 * Reads a bool from the DataBlockReader assuming that the cursor is beyond the type prefix for the bool object.
 * Moves the cursor to the beginning of the next object
 * @param x set to the bool data at the cursor
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutType(bool& x)
{
	extract((unsigned char *)&x, 1);
}

/**
 * Reads a string from the DataBlockReader assuming that the cursor is beyond the type prefix for the string object.
 * Moves the cursor to the beginning of the next object
 * @param x set to the string data at the cursor
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutType(std::string &x)
{
	SystemsBiologyWorkbench::Integer l ;

	getWithoutType(l);
	
	std::string result((char *)data + end, l);

	x = result ;
	end += l ;
}

/**
 * Reads a string from the DataBlockReader assuming that the cursor is beyond the type prefix for the string object.
 * Moves the cursor to the beginning of the next object.
 * @param x set to a new char array containing the string data at the cursor. 
 *          This new array should be freed using the delete[] operator.
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutType(char *&s)
{
	SystemsBiologyWorkbench::Integer l ;

	getWithoutType(l);
	
	s = new char[l + 1];
	extract((unsigned char *)s, l);
	s[l] = '\0';
}

/**
 * Reads a string from the DataBlockReader assuming that the cursor is beyond the type prefix for the string object.
 * Moves the cursor to the beginning of the next object.
 * @param x set to a new char array containing the string data at the cursor. 
 *          This new array should be freed using SBWFree.
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutTypeForC(char *&s)
{
	SystemsBiologyWorkbench::Integer l ;

	getWithoutType(l);
	
	s = (char *)malloc(l + 1);
	extract((unsigned char *)s, l);
	s[l] = '\0';
}

/**
 * Reads a list from the DataBlockReader assuming that the cursor is beyond the type prefix for the list object.
 * Moves the cursor to the beginning of the next object.
 * @param x set to a sbwDataBlockReader containing the list data at the cursor. 
 */
inline void SystemsBiologyWorkbench::sbwDataBlockReader::getWithoutType(sbwDataBlockReader *&x)
{
	SystemsBiologyWorkbench::Integer items ;

	getWithoutType(items);

	SystemsBiologyWorkbench::Integer start = end ;

	while (items != 0)
	{
		skipObject();
		items-- ;
	}

	// trick put temporary terminator into sequence for new DataBlockReader to copy
	unsigned char temp = data[end] ;
	data[end] = (unsigned char) TerminateType ;

	x = new sbwDataBlockReader(data + start, end - start + 1);

	data[end] = temp ;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/// create an empty data block.
inline SystemsBiologyWorkbench::sbwDataBlockReader::sbwDataBlockReader() : data(NULL), end(0), length(0)
{
}

/// move the cursor over the object at the cursor.
inline void SystemsBiologyWorkbench::sbwDataBlockReader::skipObject()
{
	SystemsBiologyWorkbench::DataBlockType type = getNextType();

	end++;
	skipObjectWithoutType(type);
}

#endif // !defined(AFX_IMPLDATABLOCKREADER_H__459EF159_6B8C_4C1C_8B01_AD611810D86E__INCLUDED_)
