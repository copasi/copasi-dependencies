/**
 * @file DataBlockReaderClass.h
 * @brief decodes data blocks - class definition
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: DataBlockReaderClass.h,v 1.2 2006/06/19 16:06:45 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/DataBlockReaderClass.h,v $
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
#ifndef DATABLOCKREADERCLASS
#define DATABLOCKREADERCLASS

#include "SBWHandle.h"
#include "ImplDataBlockReaderClass.h"

using namespace SystemsBiologyWorkbench;

/**
 * @namespace SystemsBiologyWorkbench
 * @brief contains the whole SBW C++ API
 */
namespace SystemsBiologyWorkbench
{

/**
 * container for data passed to a module - allows data to extracted sequentially like a input stream.
 * Object is a handle which refers to a sbwDataBlockReader which does the real work.
 * This means that assignment between DataBlockReader objects simply creates a new handle refering to the same data.
 * These references share the same cursor into the data as well.
 */
class DataBlockReader : public SBWHandle<sbwDataBlockReader>
{
public:
	/**
	 * returns the data from the cursor to the end of the block in a C heap allocated block.
	 * @param length set to the length of the returned block
	 * @returns the data from the cursor to the end of the block in a C heap allocated block
	 *          not a zero terminated text string
	 */
	unsigned char *getRemainingDataForC(Integer &length) { return p->getRemainingDataForC(length); }

	/**
	 * returns the underlying sbwDataBlockReader object.
	 * @returns the underlying sbwDataBlockReader object
	 */
	sbwDataBlockReader *getImpl() { return p; }

	SBW_API DataBlockReader();
	SBW_API DataBlockReader(const DataBlockReader &);

	/**
	 * conceptually similar to the standard library io stream operator - sets x to the next object in the DataBlockReader.
	 * @throws an SBWException * if the next object is not of type T
	 * @param x set to the object at the cursor
	 * @returns a reference to this object
	 */
	template<class T> DataBlockReader &operator>> (T &x) { get(x); return *this; }

  	template<class T> DataBlockReader &operator>>(std::vector< std::vector<T> > &x) { get(x); return *this; }

	/**
	 * @deprecated use unknown should be removed
	 */
 	DataBlockReader& self() { return *this; }

	void skipObject();
	DataBlockType getNextType();
	DataBlockType getNextArrayType();
	Integer getNextArrayDimensions();

	/**
	 * sets x to the next object in the DataBlockReader.
	 * @throws an SBWException * if the next object is not of type T
	 * @param x set to the object at the cursor
	 */
	template<class T> void get(T &x) { p->get(x); }

  template<class T> void get(std::vector< std::vector<T> > &x) { p->get(x); }

	/**
	 * sets x to the next object in the DataBlockReader, using the C heap for memory allocation.
	 * @throws an SBWException * if the next object is not of type T
	 * @param x set to the object at the cursor
	 */
	template<class T> void getForC(T &x) { p->getForC(x); }


	/**
	 * sets x to the next object in the DataBlockReader, assuming the cursor has already passed the type prefix data.
	 * @param x set to the object data at the cursor
	 */
	template<class T> void getWithoutType(T &x) { p->getWithoutType(x); }

	/**
	 * sets x to the next object in the DataBlockReader, assuming the cursor has already passed the type prefix data.
	 * Allocates memory on the C heap
	 * @param x set to the object data at the cursor
     */
    template<class T> void getWithoutTypeForC(T &x) { p->getWithoutTypeForC(x); }

	/**
	 * sets x to the next 1 dimensional array object in the DataBlockReader, assuming that the next object is an array.
	 * Use delete[] x to recover the memory allocated by this method
	 * @throws an SBWException * if the next object is not an 1 dimensional array of type T
	 * @param size set to the number of elements in the array
	 * @param x set to the array data at the cursor
	 */
	template<class T> void get(Integer &size, T *&x){ p->get(size, x); }

	/**
	 * sets x to the next 2 dimensional array object in the DataBlockReader, assuming that the next object is an array.
	 * Use free2DArray(xSize, x) to recover the memory allocated by this method
	 * @throws an SBWException * if the next object is not an 2 dimensional array of type T
	 * @param xSize set to the size of the first dimension of the array
	 * @param ySize set to the size of the second dimension of the array
	 * @param x set to the array data at the cursor
	 */
	template<class T> void get(Integer &xSize, Integer &ySize, T **&x)
	{
		p->get(xSize, ySize, x);
	}

	/**
	 * Recovers the memory allocated by get(Integer &xSize, Integer &ySize, T **&x).
	 * @param xSize the size of the first dimension of the array
	 * @param x the 2D array to be deallocated
	 */
	template<class T> static void free2DArray(Integer xSize, T **x) { sbwDataBlockReader::free2DArray(xSize, x); }

	/**
	 * sets x to the next 1 dimensional array object in the DataBlockReader, assuming that the next object is an array.
	 * Memory is allocated on the C heap.
	 * Use SBWFree(x) to recover the memory allocated by this method (unless T is char * in which case use SBWFree2DArray)
	 * @throws an SBWException * if the next object is not an 1 dimensional array of type T.
	 * @param size set to the number of elements in the array
	 * @param x set to the array data at the cursor
	 */
	template<class T> void getForC(Integer &size, T *&x){ p->getForC(size, x); }

	/**
	 * sets x to the next 2 dimensional array object in the DataBlockReader, assuming that the next object is an array.
	 * Memory is allocated on the C heap.
	 * Use free2DArray(xSize, x) to recover the memory allocated by this method.
	 * @throws an SBWException * if the next object is not an 2 dimensional array of type T
	 * @param xSize set to the size of the first dimension of the array
	 * @param ySize set to the size of the second dimension of the array
	 * @param x set to the array data at the cursor
	 */
	template<class T> void getForC(Integer &xSize, Integer &ySize, T **&x)
	{
		p->getForC(xSize, ySize, x);
	}

	/**
	 * outputs the contents of this object (decimal values for each byte) to the standard output.
	 */
	void dump() { p->dump(); }

	friend class DataBlockWriter;
	friend class SBWModuleRPC;
	friend class SBWRPC;
	friend struct sbwDataBlockReader;
	friend struct sbwDataBlockWriter;
	friend class DataBlockConverter ;

	// needed for broker
	SBW_API DataBlockReader(unsigned char *x, Integer length, bool makeCopy = true);

private:

 	SBW_API DataBlockReader(sbwDataBlockReader *);
};

} // SystemsBiologyWorkbench
#endif // DATABLOCKREADERCLASS
