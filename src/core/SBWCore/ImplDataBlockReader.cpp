/**
 * @file ImplDataBlockReader.cpp
 * @brief implementation of DataBlockReader
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ImplDataBlockReader.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/ImplDataBlockReader.cpp,v $
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

static const char rcsid[] = "$Id: ImplDataBlockReader.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $";

#include "stdafx.h"
#include "ImplDataBlockReader.h"
#include "SBWOSSocket.h"

using namespace SystemsBiologyWorkbench ;

/**
 * consumes the type prefix of the object at the cursor checking that it matches expectedType.
 * @throws SBWTypeMismatchException if the cursor is at the end of the data block or if
 *         expectedType doesn't match the type prefix at the cursor.
 * @param expectedType the type expected in the type prefix at the cursor.
 * @param additional context string that is added to any thrown exception message.
 */
SBW_API void sbwDataBlockReader::checkType(DataBlockType expectedType, const char *additional)
{
	if (end >= length)
	{
	    std::string message("reached end of data when expecting ");
	    message += sbwDataBlockWriter::typeString(expectedType);
	    message += " ";
	    message += additional;

	    throw new SBWTypeMismatchException(message);
	}

	if (getNextType() != expectedType)
	{
		std::string message("encountered ");

		message += sbwDataBlockWriter::typeString(getNextType());
		message += " required ";
		message += sbwDataBlockWriter::typeString(expectedType);
		message += " ";
		message += additional;

		throw new SBWTypeMismatchException(message);
	}

	end++;
}

/**
 * deallocates the data representing the datablock
 */
SBW_API sbwDataBlockReader::~sbwDataBlockReader()
{
	delete[] data;
}

/** 
 * shares or copies data between a new sbwDataBlockReader object and raw char data.
 * @param x raw data with which to construct DatablockReader object (not a zero terminated text string)
 * @param l size of x
 * @param makeCopy if true then the sbwDataBlockReader creates a copy of x otherwise takes ownership of x
 */
SBW_API sbwDataBlockReader::sbwDataBlockReader(unsigned char *x, Integer l, bool makeCopy)
: end(0), length(l)
{
	if (makeCopy)
	{
		data = new unsigned char[l]; 
		memcpy(data, x, l);
	}
	else
		data = x ;
}

/** 
 * copies raw char data into a new sbwDataBlockReader object.
 * @param x raw data with which to construct DatablockReader object (not a zero terminated text string)
 * @param l size of x
 */
SBW_API sbwDataBlockReader::sbwDataBlockReader(const unsigned char *x, Integer l)
: end(0), length(l)
{
	data = new unsigned char[l]; 
	memcpy(data, x, l);
}

/**
 * consumes the list data at the cursor storing the data in x.
 * Assumes that type prefix has been consumed
 * @param x set to the list data at the cursor
 */
SBW_API void sbwDataBlockReader::getWithoutType(DataBlockReader &x)
{
	sbwDataBlockReader *impl;

	getWithoutType(impl);
	DataBlockReader actual(impl);

	x = actual ;
}

/** 
 * outputs to standard output the contents of the data block.
 * Data is output in space seperated decimal byte values.
 * A '>' character indicates the position of the cursor.
 */
SBW_API void sbwDataBlockReader::dump()
{
	printf("\n");

	int x = 0 ;

	while (x != end)
	{
		printf("%d ", data[x]);
		x++;
	}

	printf(">");

	while (x != length)
	{
		printf("%d ", data[x]);
		x++;
	}

	printf("\n");
}

/**
 * consumes the data part of the object at the cursor.
 * Assumes the type prefix of the object has already been consumed.
 * @param type the type of the object data at the cursor.
 */
SBW_API void sbwDataBlockReader::skipObjectWithoutType(DataBlockType type)
{
	switch (type)
	{
		case IntegerType :
			{
				Integer a;

				getWithoutType(a) ;
				break ;
			}
		case DoubleType :
			{
				Double a;

				getWithoutType(a) ;
				break ;
			}
		case StringType :
			{
				std::string a;

				getWithoutType(a) ;
				break ;
			}
		case ArrayType :
			skipArray();
			break;
		case ComplexType:
			{
				Double a;

				getWithoutType(a) ;
				getWithoutType(a) ;
				break ;

			}
		case ListType :
			{
				DataBlockReader r;

				getWithoutType(r) ;
				break;
			}
		
		case BooleanType :
			{
				bool z ;

				getWithoutType(z) ;
				break;
			}

		case ByteType :
			{
				unsigned char x ;

				getWithoutType(x) ;
				break;
			}

		case VoidType :
		case TerminateType :
		case ErrorType :
		    /* These are only here to quiet some compilers. */
			{
			        break;
			}
	}
}

/**
 * consumes the array data at the cursor.
 * Assumes the type prefix but not the array header for the array has already been consumed.
 */
SBW_API void sbwDataBlockReader::skipArray()
{
	Integer dimensions ;
	unsigned char type ;
	int dimension = 0 ;
	int size = 1;
	int item = 0;

	getWithoutType(type);
	getWithoutType(dimensions);

	while (dimension != dimensions)
	{
		Integer dimSize ;

		getWithoutType(dimSize);
		size *= dimSize ;
		dimension++;
	}

	while (item != size)
	{
		skipObjectWithoutType((DataBlockType)type);
		item++;
	}
}

/**
 * reads an integer in SBW format from sock.
 * Blocks until an integer value has been sent to the socket.
 * Assumes that the type prefix for the integer either is not present or has already been consumed.
 * @param sock the socket from which to read the integer value.
 * @return the next integer value read from sock.
 */
Integer sbwDataBlockReader::readInteger(SBWOSSocket *sock)
{
	Integer result ;
	unsigned char *buffer = new unsigned char[4];
	sbwDataBlockReader reader(buffer, 4, false);  // this free's buffer on its destruction
	
	sock->receive(buffer, 4);
	reader.getWithoutType(result);
	
	return result ;
}

/**
 * returns the type of the next object in the sbwDataBlockReader.
 * @return the type of the next object in the sbwDataBlockReader
 */
SBW_API DataBlockType sbwDataBlockReader::getNextArrayType()
{
	if (getNextType() != ArrayType)
		throw new SBWTypeMismatchException("next type is not an array");

	if (end + 1 > length)
		throw new SBWTypeMismatchException("reached end of data");

	int x = (char)data[end + 1];
	return (DataBlockType)x ;
}

/**
 * returns the number of dimensions of the next array object in the sbwDataBlockReader.
 * @throws an SBWException * if the next object is not an array
 * @return the number of dimensions of the next array object in the sbwDataBlockReader
 */
SBW_API Integer sbwDataBlockReader::getNextArrayDimensions()
{
	if (getNextType() != ArrayType)
		throw new SBWTypeMismatchException("next type is not an array");

	// move end up to array dimensions just for this function
	end += 2;

	Integer result ;

	getWithoutType(result);
	end -= 6 ; // 1 byte for Array Type, 1 byte for Inner Type and 4 bytes for dimensions

	return result ;
}
