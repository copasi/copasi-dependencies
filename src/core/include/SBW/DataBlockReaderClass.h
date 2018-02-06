/**
 * @file DataBlockReaderClass.h
 * @brief decodes data blocks - class definition
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
#ifndef DATABLOCKREADERCLASS
#define DATABLOCKREADERCLASS

#include <SBW/SBWHandle.h>
#include <SBW/ImplDataBlockReaderClass.h>

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
class SBW_API DataBlockReader : public SBWHandle<sbwDataBlockReader>
{
public:
  /**
   * returns the data from the cursor to the end of the block in a C heap allocated block.
   * @param length set to the length of the returned block
   * @returns the data from the cursor to the end of the block in a C heap allocated block
   *          not a zero terminated text string
   */
  unsigned char *getRemainingDataForC(Integer &length);

  /**
   * returns the underlying sbwDataBlockReader object.
   * @returns the underlying sbwDataBlockReader object
   */
  sbwDataBlockReader *getImpl();

  DataBlockReader();
  DataBlockReader(const DataBlockReader &);

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
  DataBlockReader& self();

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
  void dump();

  friend class DataBlockWriter;
  friend class SBWModuleRPC;
  friend class SBWRPC;
  friend struct sbwDataBlockReader;
  friend struct sbwDataBlockWriter;
  friend class DataBlockConverter ;

  // needed for broker
  DataBlockReader(unsigned char *x, Integer length, bool makeCopy = true);

private:

  DataBlockReader(sbwDataBlockReader *);
};

} // SystemsBiologyWorkbench
#endif // DATABLOCKREADERCLASS
