/**
 * @file ImplDataBlockReaderClass.h
 * @brief implementation of data block reader - class definition
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ImplDataBlockReaderClass.h,v 1.3 2008/07/25 00:01:42 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/ImplDataBlockReaderClass.h,v $
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
#ifndef IMPLDATABLOCKREADERCLASS
#define IMPLDATABLOCKREADERCLASS

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <complex>

#include "SBWObject.h"
#include "SBWTypeMismatchException.h"

namespace SystemsBiologyWorkbench
{

class DataBlockReader ;
class DataBlockWriter ;
class SBWOSSocket ;

/**
 * underlying implementation of DataBlockReader.
 * container for data passed to a module - allows data to extracted sequentially like a input stream.
 */
struct sbwDataBlockReader : public SBWObject
{
public:
	unsigned char *getRemainingDataForC(Integer &lengthReturned);
	SBW_API static Integer readInteger(SBWOSSocket *);

	SBW_API void dump();

	sbwDataBlockReader();
	SBW_API sbwDataBlockReader(unsigned char *x, Integer length, bool makeCopy = true);
	SBW_API sbwDataBlockReader(const unsigned char *x, Integer length);
	SBW_API virtual ~sbwDataBlockReader();

	void skipObject();
	DataBlockType getNextType();
	SBW_API DataBlockType getNextArrayType();
	SBW_API Integer getNextArrayDimensions();

	/**
	 * sets x to the next object in the sbwDataBlockReader.
	 * @throws an SBWException * if the next object is not of type T
	 * @param x set to the object at the cursor
	 */
	template<class T> void get(T &x)
	{
		consumeType(x);
		getWithoutType(x);
	}

  template<class T> void get(std::vector< std::vector<T> > &x)
	{
		//consumeType(x);
		getWithoutType(x);
	}


	/**
	 * sets x to the next object in the sbwDataBlockReader, using the C heap for memory allocation.
	 * @throws an SBWException * if the next object is not of type T
	 * @param x set to the object at the cursor
	 */
	template<class T> void getForC(T &x)
	{
		consumeType(x);
		getWithoutTypeForC(x);
	}

	/**
	 * sets x to the next 1 dimensional array object in the sbwDataBlockReader, assuming that the next object is an array.
	 * use delete[] x to recover the memory allocated by this method.
	 * @throws an SBWException * if the next object is not an 1 dimensional array of type T
	 * @param size set to the number of elements in the array
	 * @param x set to the array data at the cursor
	 */
	template<class T> void get(Integer &size, T *&x)
	{
		T y;

		checkType(ArrayType, "(1D)");
		size = get1DArrayHeader(y);
		x = new T[size];
		get1DArrayBody(y, x, size);
	}

	/**
	 * sets x to the next 1 dimensional array object in the sbwDataBlockReader, assuming that the next object is an array.
	 * Memory is allocated on the C heap.
	 * use SBWFree(x) to recover the memory allocated by this method (unless T is char * in which case use SBWFree2DArray)
	 * @throws an SBWException * if the next object is not an 1 dimensional array of type T.
	 * @param size set to the number of elements in the array
	 * @param x set to the array data at the cursor
	 */
	template<class T> void getForC(Integer &size, T *&x)
	{
		T y;

		checkType(ArrayType, "(1D)");
		size = get1DArrayHeader(y);
		x = (T *)calloc(size, sizeof(T));
		get1DArrayBodyForC(y, x, size);
	}

	/**
	 * sets x to the next 2 dimensional array object in the sbwDataBlockReader, assuming that the next object is an array.
	 * use free2DArray(xSize, array) to recover the memory allocated by this method.
	 * @throws an SBWException * if the next object is not an 2 dimensional array of type T.
	 * @param xSize set to the size of the first dimension of the array
	 * @param ySize set to the size of the second dimension of the array
	 * @param array set to the array data at the cursor
	 */
	template<class T> void get(Integer &xSize, Integer &ySize, T **&array)
	{
		T y;
		Integer dimensions ;

		checkType(ArrayType, "(2D)");
		consumeType(y, "array (2D)");
		getWithoutType(dimensions);

		if (dimensions != 2)
		{
			std::string message;
			SBW_CREATE_STRING(	"expecting 2 dimensional array, received " << dimensions << " dimensions",message);

			throw new SBWTypeMismatchException(message);
		}

		getWithoutType(xSize);
		getWithoutType(ySize);

		array = new T *[xSize];
		int xItem = 0 ;

		while (xItem != xSize)
		{
			int yItem = 0 ;

			array[xItem] = new T[ySize];

			while (yItem != ySize)
			{
				getWithoutType(y);
				array[xItem][yItem] = y ;
				yItem++ ;
			}
			xItem++ ;
		}
	}

	/**
	 * Recovers the memory allocated by get(Integer &xSize, Integer &ySize, T **&x).
	 * @param xSize the size of the first dimension of the array
	 * @param x the 2D array to be deallocated
	 */
	template<class T> static void free2DArray(Integer xSize, T **a)
	{
		while (xSize != 0)
		{
			xSize-- ;
			delete[] a[xSize];
		}

		delete[] a ;
	}

	/**
	 * sets x to the next 2 dimensional array object in the sbwDataBlockReader, assuming that the next object is an array.
	 * Memory is allocated on the C heap.
	 * use free2DArray(xSize, x) to recover the memory allocated by this method.
	 * @throws an SBWException * if the next object is not an 2 dimensional array of type T
	 * @param xSize set to the size of the first dimension of the array
	 * @param ySize set to the size of the second dimension of the array
	 * @param x set to the array data at the cursor
	 */
	template<class T> void getForC(Integer &xSize, Integer &ySize, T **&array)
	{
		T y;
		Integer dimensions ;

		checkType(ArrayType, "(2D)");
		consumeType(y, "array (2D)");
		getWithoutType(dimensions);

		if (dimensions != 2)
		{
			std::string message;
			SBW_CREATE_STRING(	"expecting 2 dimensional array, received " << dimensions << " dimensions",message);

			throw new SBWTypeMismatchException(message);
		}

		getWithoutType(xSize);
		getWithoutType(ySize);

		array = (T **)calloc(xSize, sizeof(T *));
		int xItem = 0 ;

		while (xItem != xSize)
		{
			int yItem = 0 ;

			array[xItem] = (T *)calloc(ySize, sizeof(T));

			while (yItem != ySize)
			{
				getWithoutTypeForC(y);
				array[xItem][yItem] = y ;
				yItem++ ;
			}
			xItem++ ;
		}
	}

	void getWithoutType(unsigned char & x);
	void getWithoutType(Integer& x);
	void getWithoutType(sbwDataBlockReader *&x);
	void getWithoutType(Double& x);
	void getWithoutType(SBWComplex& x);
	void getWithoutType(std::complex<double>& x);
	void getWithoutType(bool& x);
	void getWithoutType(std::string &x);
	void getWithoutType(char *&);
private:
	/**
	 * moves the cursor over the type prefix of the next object checking it matches T.
	 * @throws SBWException * if T doesn't match the type prefix of the next type
	 * @param x ignored
	 * @param s string containing context information for use when generating exceptions
	 */
	template<class T> void consumeType(T &x, const char *s = "")
	{
		checkType(getType(x), s);
	}

	SBW_API void checkType(DataBlockType x, const char *context = "");

	static DataBlockType getType(Integer &);
	static DataBlockType getType(Double &);
	static DataBlockType getType(SBWComplex &);
	static DataBlockType getType(std::complex<double> &);
	static DataBlockType getType(unsigned char &);
	static DataBlockType getType(bool &);
	static DataBlockType getType(char *&);
	static DataBlockType getType(std::string &);
	static DataBlockType getType(DataBlockReader &);
	static DataBlockType getType(sbwDataBlockReader *&);

	/**
	 * returns ArrayType.
	 * @param ignored ignored
	 * @returns ArrayType
	 */
	template<class T> DataBlockType getType(std::vector<T> & /*ignored*/)
	{
		return( ArrayType);
	}

	/**
	 * returns ArrayType.
	 * @param ignored ignored
	 * @returns ArrayType
	 */
	template<class T> DataBlockType getType(std::list<T> & /*ignored*/)
	{
		return( ArrayType);
	}

	/**
	 * returns ArrayType.
	 * @param ignored ignored
	 * @returns ArrayType
	 */
	template<class T> DataBlockType getType(std::deque<T> & /*ignored*/)
	{
		return( ArrayType);
	}

	void extract(unsigned char *x, Integer l);
	SBW_API void getWithoutType(DataBlockReader &x);


	/**
	 * sets x to the next object in the sbwDataBlockReader, assuming the cursor has already passed the type prefix data.
	 * This is the default method for types that don't need heap memory allocated.
	 * @param x set to the object data at the cursor
     */
	template<class T> void getWithoutTypeForC(T &x) { getWithoutType(x); }

	void getWithoutTypeForC(char *&);

	/**
	 * sets x to the next sbwDataBlockReader in the sbwDataBlockReader.
	 * Assumes the cursor has already passed the type prefix data.
	 * The memory allocated should freed using SBWFreeSBWDataBlockWriter.
	 * @param x set to the sbwDataBlockReader at the cursor
     */
	void getWithoutTypeForC(sbwDataBlockReader *&x)
	{
		getWithoutType(x);
		x->AddReference(); // in C readers have to explictly freed
						   // add extra reference to ensure they aren't freed
						   // till then
	}

	/**
	 * sets v to the next array in the sbwDataBlockReader.
	 * Assumes the cursor has already passed the array type prefix data.
	 * @param v set to the array at the cursor
     */
	template<class T> void getWithoutType(std::vector<T> &v)
	{
		T x;
		Integer size = get1DArrayHeader(x);

		v.reserve(size);
		v.resize(size);
		get1DArrayBody(x, v.begin(), size);
	}

  /**
	 * sets v to the next array in the sbwDataBlockReader.
	 * Assumes the cursor has already passed the array type prefix data.
	 * @param v set to the array at the cursor
     */
  template<class T> void getWithoutType(std::vector< std::vector<T> > &v)
	{
    Integer x, y; T** array;
    get(x, y, array);

    v.reserve(x); v.resize(x);

    for (int i = 0; i < x; i++)
    {
      std::vector<T> row;
      row.reserve(y);
      row.resize(y);
      for (int j = 0; j < y; j++)
      {
        row[j] = array[i][j];
      }
      v[i] = row;
    }

    free2DArray(x, array);
	}


	/**
	 * sets d to the next array in the sbwDataBlockReader.
	 * Assumes the cursor has already passed the type prefix data.
	 * @param d set to the array at the cursor
     */
	template<class T> void getWithoutType(std::deque<T> &d)
	{
		T x;
		Integer size = get1DArrayHeader(x);

		d.clear();
		while (size != 0)
		{
			getWithoutType(x);
			d.push_back(x);
			size-- ;
		}
	}

	/**
	 * sets l to the next array in the sbwDataBlockReader.
	 * Assumes the cursor has already passed the type prefix data.
	 * @param l set to the array at the cursor
     */
	template<class T> void getWithoutType(std::list<T> &l)
	{
		T x;
		Integer size = get1DArrayHeader(x);

		l.clear();
		while (size != 0)
		{
			getWithoutType(x);
			l.push_back(x);
			size-- ;
		}
	}

	/**
	 * Moves the cursor over the 1 dimensional array header assumed to be at the cursor.
	 * Checks that the array header is for a 1 dimension array containing type T objects.
	 * Assumes the cursor has already passed the initial array type prefix data.
	 * @throws an SBWTypeMismatchException * if the array header at the cursor is not a 1 dimensional array of T objects.
	 * @param x ignored
	 * @return size of the array
	 */
	template<class T> Integer get1DArrayHeader(T &x)
	{
		Integer dimensions ;
		Integer size ;

		consumeType(x);
		getWithoutType(dimensions);

		if (dimensions != 1)
			throw new SBWTypeMismatchException("expecting 1 dimensional array");

		getWithoutType(size);

		return size ;
	}

	/**
	 * loads the 1 dimensional array data of type T at the cursor into the container "pointed to" by the iterator i.
	 * @param x initial value ignored and changed - please ignore value set.
	 * @param i iterator pointing to the start of a container containing objects of type T.
	 * @param size number of element in the array to be read.
	 */
	template<class T, class I> void get1DArrayBody(T &x, I i, Integer size)
	{
		int item = 0 ;

		while (item != size)
		{
			getWithoutType(x);
			*i = x ;
			i++;
			item++ ;
		}
	}

	/**
	 * loads the 1 dimensional array data of type T at the cursor into the container "pointed to" by the iterator i.
	 * Where necessary the individual elements are allocated on the C heap.
	 * @param x initial value ignored and changed - please ignore value set.
	 * @param i iterator pointing to the start of a container containing objects of type T.
	 * @param size number of element in the array to be read.
	 */
	template<class T, class I> void get1DArrayBodyForC(T &x, I i, Integer size)
	{
		int item = 0 ;

		while (item != size)
		{
			getWithoutTypeForC(x);
			*i = x ;
			i++;
			item++ ;
		}
	}

	/**
	 * moves the cursor over the object at the cursor assuming the type prefix has already been consumed.
	 * @param t type of the object at the cursor
	 */
	SBW_API void skipObjectWithoutType(DataBlockType t);

	/**
	 * moves the cursor over the array data at the cursor
	 */
	SBW_API void skipArray();

	/**
	 * the raw data of the datablock
	 */
	unsigned char *data ;

	/// the index of the cursor
	int end ;

	/// the length of the datablock
	int length ;
};

}// SystemsBiologyWorkbench

#endif // IMPLDATABLOCKREADERCLASS
