/**
 * @file ImplDataBlockWriterClass.h
 * @brief  implementation of DataBlockWriter - class definition
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

/// include loop block
#ifndef IMPLDATABLOCKWRITERCLASS
#define IMPLDATABLOCKWRITERCLASS

#include <SBW/SBWObject.h>
#include <SBW/sbwenums.h>
#include <SBW/SBWUnsupportedObjectTypeException.h>

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <complex>

namespace SystemsBiologyWorkbench
{

/// the types of SBW RPC messages using the standard SBW using in the APIs and messages.
enum SBWMessageType
{
	SBWReplyCode = 0, /**< message sent back from a call with the call result */
	SBWSendCode = 1, /**< method invocation which doesn't require reply */
	SBWCallCode = 2, /**< method invocation that does require reply */
	SBWExceptionCode = 3 /**< message sent back from a call indicating an exception occured */
};

struct sbwDataBlockReader;
class DataBlockReader;
class DataBlockWriter;

/**
 * underlying implementation of DataBlockWriter and SBWDataBlockWriter.
 * Enables the encoding of objects into the SBW DataBlock format used to transmit data between modules.
 */
struct SBW_API sbwDataBlockWriter : public SBWObject
{
public:
	unsigned char *createCall(
		Integer moduleTo, Integer moduleFrom, Integer messageId, Integer service, Integer method,
		Integer &length);
	unsigned char *createSend(
		Integer moduleTo, Integer moduleFrom, Integer service, Integer method,
		Integer &length);
	unsigned char *createReply(Integer moduleTo, Integer messageId, Integer &length);

	unsigned char *getDataCopy(Integer &length);
	void dump();

	sbwDataBlockWriter();
	sbwDataBlockWriter(Integer size);
	virtual ~sbwDataBlockWriter();

	/**
	 * adds an object to the sbwDataBlockWriter.
	 * moves the cursor so that the next call places the next object after the object given in this call.
	 * @param x object to be added to the datablock.
	 */
	template<class T> void add(const T x)
	{
		packType(x);
		packWithoutType(x);
	}

	/**
	 * adds an 1D array to the sbwDataBlockWriter.
	 * moves the cursor so that the next call places the next object after the array.
	 * @param size number of elements in array.
	 * @param x array to be added to the datablock.
	 */
	template<class T> void add(Integer size, T *x)
	{
		packWithoutType((unsigned char)ArrayType);
		packWithoutType(*x, x, size);
	}

  template<class T> void add(const std::vector< std::vector<T> >& data)
  {
    packWithoutType((unsigned char)ArrayType);
    if (data.size() > 0 && data[0].size() > 0)
    {
      packType(data[0][0]);
    }
    else
    {
      throw new SBWUnsupportedObjectTypeException(
        "Empty two dimensional arrays of type are not supported.");
    }

    packWithoutType((Integer)2);
    packWithoutType((Integer)data.size());
    packWithoutType((Integer)data[0].size());

		Integer x = 0 ;

    while (x != (Integer)data.size())
    {
      Integer y = 0 ;

      while (y != (Integer)data[x].size())
      {
        packWithoutType(data[x][y]);
        ++y;
      }
      ++x;
    }
  }

	/**
	 * adds a 2D array to the sbwDataBlockWriter.
	 * moves the cursor so that the next call places the next object after the array.
	 * @param sizeX number of elements in the first dimension of the array.
	 * @param sizeY number of elements in the second dimension of the array.
	 * @param d array to be added to the datablock.
	 */
	template<class T> void add(Integer sizeX, Integer sizeY, T **d)
	{
		packWithoutType((unsigned char)ArrayType);

		try
		{
					packType(**d);
		}
		catch(...)
		{
			throw new SBWUnsupportedObjectTypeException(
				"Empty two dimensional arrays of type ** are not supported.",
				"Initialize the array with size 1, but use add(0,0,array).");
		}

		packWithoutType((Integer)2);
		packWithoutType(sizeX);
		packWithoutType(sizeY);

		Integer x = 0 ;

		while (x != sizeX)
		{
			Integer y = 0 ;

			while (y != sizeY)
			{
				packWithoutType(d[x][y]);
				y++;
			}
			x++;
		}
	}

	sbwDataBlockReader *getReader() const ;
	const unsigned char *getData(Integer *x = NULL) const;

	static DataBlockType getType(const Integer);
	static DataBlockType getType(const Double);
	static DataBlockType getType(const SBWComplex);
	static DataBlockType getType(const std::complex<double>);
	static DataBlockType getType(const unsigned char);
	static DataBlockType getType(const bool);
	static DataBlockType getType(const char *const);
	static DataBlockType getType(const std::string);
	static DataBlockType getType(const DataBlockWriter);
	static DataBlockType getType(const sbwDataBlockWriter *const);

	/**
	 * returns SystemsBiologyWorkbench::ArrayType
	 * @param ignored ignored; used to invoke correct function overload
	 * @return SystemsBiologyWorkbench::ArrayType
	 */
	template<class T> DataBlockType getType(const std::vector<T> &/*ignored*/)
	{
		return( ArrayType);
	}

	/**
	 * returns SystemsBiologyWorkbench::ArrayType
	 * @param ignored ignored; used to invoke correct function overload
	 * @return SystemsBiologyWorkbench::ArrayType
	 */
	template<class T> DataBlockType getType(const std::deque<T> &/*ignored*/)
	{
		return( ArrayType);
	}

	/**
	 * returns SystemsBiologyWorkbench::ArrayType
	 * @param ignored ignored; used to invoke correct function overload
	 * @return SystemsBiologyWorkbench::ArrayType
	 */
	template<class T> DataBlockType getType(const std::list<T> &/*ignored*/)
	{
		return( ArrayType);
	}

	static std::string typeString(DataBlockType);

	void packWithoutType(const char *const);
	void packWithoutType(const Integer);
	void packWithoutType(const unsigned char);

	void insertData(const unsigned char *x, int length);

private:
	unsigned char *createInvoke(
		unsigned char type,
		Integer moduleTo,
		Integer moduleFrom,
		Integer messageId,
		Integer service,
		Integer method,
		Integer &length);

	unsigned char *assembleMessage(sbwDataBlockWriter &header);

	/**
	 * append an array to the end of the data block assuming that either the type prefix has already been appended or
	 * the type prefix is not required.
	 * @param ignored ignored; used to invoke correct function overload.
	 * @param begin iterator pointing at the first element of type T in array.
	 * @param size number of elements in array.
	 */
	template<class T, class I> void packWithoutType(T &ignored, I begin, Integer size)
	{
		packType(ignored);
		packWithoutType((Integer)1);
		packWithoutType(size);

		while (size != 0)
		{
			packWithoutType(*begin);
			begin++ ;
			size-- ;
		}
	}

	void makeSpace(int space);
	void packWithoutType(const Double);
	void packWithoutType(const SBWComplex);
	void packWithoutType(const std::complex<double>);
	void packWithoutType(const bool);
	void packWithoutType(const std::string);
	void packWithoutType(const DataBlockWriter);
	void packWithoutType(const sbwDataBlockWriter *const);

	/**
	 * append an array to the end of the data block assuming that either the type prefix has already been appended or
	 * the type prefix is not required.
	 * @param x the array to be appended
	 */
	template<class T> void packWithoutType(const std::vector<T> &x)
	{
		T y;

		packWithoutType(y, x.begin(), (int)x.size());
	}

	/**
	 * append an array to the end of the data block assuming that either the type prefix has already been appended or
	 * the type prefix is not required.
	 * @param x the array to be appended
	 */
	template<class T> void packWithoutType(const std::deque<T> &x)
	{
		T y;

		packWithoutType(y, x.begin(), x.size());
	}

	/**
	 * append an array to the end of the data block assuming that either the type prefix has already been appended or
	 * the type prefix is not required.
	 * @param x the array to be appended
	 */
	template<class T> void packWithoutType(const std::list<T> &x)
	{
		T y;

		packWithoutType(y, x.begin(), x.size());
	}

	/**
	 * append the type prefix for a given object to the end of the data block.
	 * @param x ignored; used to invoke correct function overload
	 */
	template<class T> void packType(const T x)
	{
		packWithoutType((unsigned char)getType(x));
	}

	/// the raw encoded data of the datablock
	unsigned char *data ;

	/// the number of bytes encoded in the datablock
	int end;

	/// the number of bytes allocated for the datablock
	int dataLength;
};
} // SystemsBiologyWorkbench
#endif //IMPLDATABLOCKWRITERCLASS
