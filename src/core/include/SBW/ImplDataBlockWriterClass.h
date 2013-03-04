/**
 * @file ImplDataBlockWriterClass.h
 * @brief  implementation of DataBlockWriter - class definition
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ImplDataBlockWriterClass.h,v 1.3 2007/02/22 22:24:53 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ImplDataBlockWriterClass.h,v $
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

/// include loop block
#ifndef IMPLDATABLOCKWRITERCLASS
#define IMPLDATABLOCKWRITERCLASS

#include "SBWObject.h"
#include "sbwenums.h"
#include "SBWUnsupportedObjectTypeException.h"

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
struct sbwDataBlockWriter : public SBWObject
{
public:
	SBW_API unsigned char *createCall(
		Integer moduleTo, Integer moduleFrom, Integer messageId, Integer service, Integer method,
		Integer &length);
	SBW_API unsigned char *createSend(
		Integer moduleTo, Integer moduleFrom, Integer service, Integer method,
		Integer &length);
	SBW_API unsigned char *createReply(Integer moduleTo, Integer messageId, Integer &length);

	unsigned char *getDataCopy(Integer &length);
	SBW_API void dump();

	SBW_API sbwDataBlockWriter();
	SBW_API sbwDataBlockWriter(Integer size);
	SBW_API virtual ~sbwDataBlockWriter();

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

	SBW_API sbwDataBlockReader *getReader() const ;
	SBW_API const unsigned char *getData(Integer *x = NULL) const;

	SBW_API static DataBlockType getType(const Integer);
	SBW_API static DataBlockType getType(const Double);
	SBW_API static DataBlockType getType(const SBWComplex);
	SBW_API static DataBlockType getType(const std::complex<double>);
	SBW_API static DataBlockType getType(const unsigned char);
	SBW_API static DataBlockType getType(const bool);
	SBW_API static DataBlockType getType(const char *const);
	SBW_API static DataBlockType getType(const std::string);
	SBW_API static DataBlockType getType(const DataBlockWriter);
	SBW_API static DataBlockType getType(const sbwDataBlockWriter *const);

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

	SBW_API void packWithoutType(const char *const);
	SBW_API void packWithoutType(const Integer);
	SBW_API void packWithoutType(const unsigned char);

	SBW_API void insertData(const unsigned char *x, int length);

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

	SBW_API void makeSpace(int space);
	SBW_API void packWithoutType(const Double);
	SBW_API void packWithoutType(const SBWComplex);
	SBW_API void packWithoutType(const std::complex<double>);
	SBW_API void packWithoutType(const bool);
	SBW_API void packWithoutType(const std::string);
	SBW_API void packWithoutType(const DataBlockWriter);
	SBW_API void packWithoutType(const sbwDataBlockWriter *const);

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
