/**
 * @file ImplDataBlockReader.h
 * @brief  implementation of DataBlockReader - inline methods
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
// sbwDataBlockReader.h: interface for the sbwDataBlockReader class.
//
//////////////////////////////////////////////////////////////////////

/// MSVC generated include loop block
#if !defined(AFX_IMPLDATABLOCKREADER_H__459EF159_6B8C_4C1C_8B01_AD611810D86E__INCLUDED_)
#define AFX_IMPLDATABLOCKREADER_H__459EF159_6B8C_4C1C_8B01_AD611810D86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include <complex>
#include <vector>
#include <deque>
#include <list>
#include <string>


#include  <SBW/sbwplusbasictypes.h>
#include  <SBW/SBWObject.h>
#include  <SBW/ImplDataBlockWriter.h>
#include  <SBW/SBWTypeMismatchException.h>



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
