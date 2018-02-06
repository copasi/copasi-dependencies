/**
 * @file ImplDataBlockReader.cpp
 * @brief implementation of DataBlockReader
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

#include "stdafx.h"
#include <stdio.h>
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
 void sbwDataBlockReader::checkType(DataBlockType expectedType, const char *additional)
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
 sbwDataBlockReader::~sbwDataBlockReader()
{
    delete[] data;
}

/**
 * shares or copies data between a new sbwDataBlockReader object and raw char data.
 * @param x raw data with which to construct DatablockReader object (not a zero terminated text string)
 * @param l size of x
 * @param makeCopy if true then the sbwDataBlockReader creates a copy of x otherwise takes ownership of x
 */
 sbwDataBlockReader::sbwDataBlockReader(unsigned char *x, Integer l, bool makeCopy)
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
 sbwDataBlockReader::sbwDataBlockReader(const unsigned char *x, Integer l)
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
 void sbwDataBlockReader::getWithoutType(DataBlockReader &x)
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
 void sbwDataBlockReader::dump()
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
 void sbwDataBlockReader::skipObjectWithoutType(DataBlockType type)
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
 void sbwDataBlockReader::skipArray()
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
 DataBlockType sbwDataBlockReader::getNextArrayType()
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
 Integer sbwDataBlockReader::getNextArrayDimensions()
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
