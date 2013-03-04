/**
 * @file ImplDataBlockWriter.h
 * @brief  implementation of DataBlockWriter - inline methods
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ImplDataBlockWriter.h,v 1.5 2007/02/22 22:24:53 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ImplDataBlockWriter.h,v $
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
#if !defined(AFX_IMPLDATABLOCKWRITER_H__D067544E_921D_46CF_9AE6_C892B23DFCE5__INCLUDED_)
#define AFX_IMPLDATABLOCKWRITER_H__D067544E_921D_46CF_9AE6_C892B23DFCE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <complex>
#include "DataBlockReader.h"
#include "DataBlockWriter.h"

/**
 * appends a given block of encoded data to the end of the data block.
 * @param x block of encoded data 
 * @param length number of bytes of encoded data to store
 */
inline void SystemsBiologyWorkbench::sbwDataBlockWriter::insertData(const unsigned char *x, int length)
{
	if (dataLength - end < length)
		makeSpace(length);

	memcpy(data + end, x, length);
	end += length ;
}

/**
 * append an integer to the end of the data block assuming that either the type prefix has already been appended or
 * the type prefix is not required.
 * @param x the integer to be appended
 */
inline void SystemsBiologyWorkbench::sbwDataBlockWriter::packWithoutType(const SystemsBiologyWorkbench::Integer x)
{
#ifdef __BIG_ENDIAN__
	unsigned char cTemp[4];
	memcpy(cTemp, (unsigned char *)&x, 4);
	for (int i = 3; i >= 0; i--)
		insertData(&cTemp[i],1);
#else
	insertData((unsigned char *)&x, 4);
#endif
}

/**
 * append a complex<double> to the end of the data block assuming that either the type prefix has already been appended or
 * the type prefix is not required.
 * @param x the double to be appended
 */
inline void SystemsBiologyWorkbench::sbwDataBlockWriter::packWithoutType(const std::complex<double> x)
{
	double dReal = x.real();
	double dImag = x.imag();

	packWithoutType((Double)dReal);
	packWithoutType((Double)dImag);
}

/**
 * append a SBWComplex to the end of the data block assuming that either the type prefix has already been appended or
 * the type prefix is not required.
 * @param x the double to be appended
 */
inline void SystemsBiologyWorkbench::sbwDataBlockWriter::packWithoutType(const SystemsBiologyWorkbench::SBWComplex x)
{
	packWithoutType(x.dReal);
	packWithoutType(x.dImag);
}

/**
 * append a double to the end of the data block assuming that either the type prefix has already been appended or
 * the type prefix is not required.
 * @param x the double to be appended
 */
inline void SystemsBiologyWorkbench::sbwDataBlockWriter::packWithoutType(const SystemsBiologyWorkbench::Double x)
{
#if defined __BIG_ENDIAN__
	unsigned char cTemp[8];
	memcpy(cTemp, (unsigned char *)&x, 8);
	for (int i = 7; i >= 0; i--)
		insertData(&cTemp[i],1);
#else
	insertData((unsigned char *)&x, 8);
#endif
}

/**
 * append a byte to the end of the data block assuming that either the type prefix has already been appended or
 * the type prefix is not required.
 * @param x the byte to be appended
 */
inline void SystemsBiologyWorkbench::sbwDataBlockWriter::packWithoutType(const unsigned char x)
{
	insertData((unsigned char *)&x, 1);
}

/**
 * append a bool to the end of the data block assuming that either the type prefix has already been appended or
 * the type prefix is not required.
 * @param x the bool to be appended
 */
inline void SystemsBiologyWorkbench::sbwDataBlockWriter::packWithoutType(const bool x)
{
	unsigned char b ;
	
	if (x)
		b = 1 ;
	else
		b = 0 ;

	packWithoutType(b);
}

/**
 * append a string to the end of the data block assuming that either the type prefix has already been appended or
 * the type prefix is not required.
 * @param x the string to be appended
 */
inline void SystemsBiologyWorkbench::sbwDataBlockWriter::packWithoutType(const char *const x)
{
	SystemsBiologyWorkbench::Integer l = (SystemsBiologyWorkbench::Integer)strlen(x);
	
	packWithoutType(l);
	insertData((unsigned char *)x, l);
}

/**
 * append a string to the end of the data block assuming that either the type prefix has already been appended or
 * the type prefix is not required.
 * @param x the string to be appended
 */
inline void SystemsBiologyWorkbench::sbwDataBlockWriter::packWithoutType(const std::string x)
{
	const char *y = x.c_str();

	packWithoutType(y);
}

/**
 * returns SystemsBiologyWorkbench::IntegerType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::IntegerType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(
	const SystemsBiologyWorkbench::Integer /*ignored*/)
{
	return( SystemsBiologyWorkbench::IntegerType);
}

/**
 * returns SystemsBiologyWorkbench::DoubleType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::DoubleType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(
	const SystemsBiologyWorkbench::SBWComplex /*ignored*/)
{
	return( SystemsBiologyWorkbench::ComplexType);
}
/**
 * returns SystemsBiologyWorkbench::DoubleType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::DoubleType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(
	const std::complex<double> /*ignored*/)
{
	return( SystemsBiologyWorkbench::ComplexType);
}

/**
 * returns SystemsBiologyWorkbench::DoubleType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::DoubleType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(
	const SystemsBiologyWorkbench::Double /*ignored*/)
{
	return( SystemsBiologyWorkbench::DoubleType);
}

/**
 * returns SystemsBiologyWorkbench::ByteType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::ByteType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(
	const unsigned char /*ignored*/)
{
	return( SystemsBiologyWorkbench::ByteType);
}

/**
 * returns SystemsBiologyWorkbench::BooleanType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::BooleanType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(const bool /*ignored*/)
{
	return( SystemsBiologyWorkbench::BooleanType);
}

/**
 * returns SystemsBiologyWorkbench::StringType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::StringType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(
	const char *const /*ignored*/)
{
	return( SystemsBiologyWorkbench::StringType);
}

/**
 * returns SystemsBiologyWorkbench::StringType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::StringType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(
	const std::string /*ignored*/)
{
	return( SystemsBiologyWorkbench::StringType);
}

/**
 * returns SystemsBiologyWorkbench::ListType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::ListType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(
	const DataBlockWriter /*ignored*/)
{
	return( SystemsBiologyWorkbench::ListType);
}

/**
 * returns SystemsBiologyWorkbench::ListType
 * @param ignored ignored; used to invoke correct function overload
 * @return SystemsBiologyWorkbench::ListType
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::sbwDataBlockWriter::getType(
	const sbwDataBlockWriter *const /*ignored*/)
{
	return( SystemsBiologyWorkbench::ListType);
}

/**
 * returns a string containing the type name corresponding to a given type.
 * @param x a SBW type to be translated into the corresponding SBW type string.
 * @return a string containing the type name corresponding to a given type.
 */
inline std::string SystemsBiologyWorkbench::sbwDataBlockWriter::typeString(SystemsBiologyWorkbench::DataBlockType x)
{
	if (SystemsBiologyWorkbench::TerminateType > x || SystemsBiologyWorkbench::ComplexType < x)
		return "garbage" ;

	static const char *typeStrings[] =
		{ "terminator", "byte", "integer", "double", "boolean", "string", "array", "list", "complex" };

	return typeStrings[x + 1];
}

/**
 * returns the raw data encoding of the data block.  The returned pointer is not a copy but a pointer
 * to the representation encapsulated by the sbwDataBlockWriter.
 * @param x the referenced integer is set to the number of returned bytes.
 * @return the raw data encoding of the data block.
 */
inline const unsigned char *SystemsBiologyWorkbench::sbwDataBlockWriter::getData(
	SystemsBiologyWorkbench::Integer *x) const
{
	if (x != NULL)
		*x = end ;

	return data ;
}

/**
 * returns the raw data encoding of the data block.  The returned pointer to a copy of the representation encapsulated
 * by the sbwDataBlockWriter.  This copy should freed using the delete[] operator.
 * @param length set to the number of returned bytes.
 * @return a copy of the raw data encoding of the data block.
 */
inline unsigned char *SystemsBiologyWorkbench::sbwDataBlockWriter::getDataCopy(SystemsBiologyWorkbench::Integer &length)
{
	length = end ;

	return (unsigned char *)memcpy(new unsigned char[length], data, length);
}

#endif // !defined(AFX_IMPLDATABLOCKWRITER_H__D067544E_921D_46CF_9AE6_C892B23DFCE5__INCLUDED_)
