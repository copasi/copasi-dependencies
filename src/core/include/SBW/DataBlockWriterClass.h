/**
 * @file DataBlockWriterClass.h
 * @brief  encodes data in datablock - class definition
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

#ifndef DATABLOCKWRITERCLASS
#define DATABLOCKWRITERCLASS

#if    _MSC_VER > 1000
#pragma once
#pragma warning (disable: 4800)
#endif

#include <SBW/ImplDataBlockWriterClass.h>
#include <SBW/SBWHandle.h>

namespace SystemsBiologyWorkbench
{

/// encodes data in a datablock ultimately for transmission to another module or the broker
class SBW_API DataBlockWriter : public SBWHandle<sbwDataBlockWriter>
{
public:
	DataBlockWriter(const DataBlockWriter &);
	DataBlockWriter();

	/// @deprecated
  DataBlockWriter& self();

	/**
	 * returns underlying sbwDataBlockWriter that implements the data block writer.
	 * @return underlying sbwDataBlockWriter
	 */
  sbwDataBlockWriter *getImpl();

	/**
	 * adds an object to the DataBlockWriter.
	 * moves the cursor so that the next call places the next object after the object given in this call.
	 * example usage: Integer a; Double b; DataBlockWriter() << a << b;
	 * @param x object to be added to the datablock.
	 * @return a reference to the DataBlockWriter.
	 */
	template<class T> DataBlockWriter &operator<<(T x) { add(x); return *this; }

	/**
	 * adds an object to the DataBlockWriter.
	 * moves the cursor so that the next call places the next object after the object given in this call.
	 * @param x object to be added to the datablock.
	 */
	template<class T> void add(T x) { p->add(x); }

	/**
	 * adds an 1D array to the DataBlockWriter.
	 * moves the cursor so that the next call places the next object after the array.
	 * @param size number of elements in array.
	 * @param x array to be added to the datablock.
	 */
	template<class T> void add(Integer size, T *x) { p->add(size, x); }

	/**
	 * adds an 2D array to the DataBlockWriter.
	 * moves the cursor so that the next call places the next object after the array.
	 * @param x 2d vector to be added to the datablock.
	 */
	template<class T> void add(const std::vector< std::vector<T> >& x) { p->add(x); }

	/**
	 * adds an object to the DataBlockWriter.
	 * moves the cursor so that the next call places the next object after the object given in this call.
	 * example usage: Integer a; Double b; DataBlockWriter() << a << b;
	 * @param x object to be added to the datablock.
	 * @return a reference to the DataBlockWriter.
	 */
	template<class T> DataBlockWriter &operator<<(const std::vector< std::vector<T> >& x) { add(x); return *this; }

	/**
	 * adds a 2D array to the DataBlockWriter.
	 * moves the cursor so that the next call places the next object after the array.
	 * @param sizeX number of elements in the first dimension of the array.
	 * @param sizeY number of elements in the second dimension of the array.
	 * @param x array to be added to the datablock.
	 */
	template<class T> void add(Integer sizeX, Integer sizeY, T **x) { p->add(sizeX, sizeY, x); }

	DataBlockReader getReader() const;
	const unsigned char *getData(Integer *x = NULL) const;
	unsigned char *getDataCopy(Integer &x) const;
	void insertData(const unsigned char *data, Integer length);
	void dump();

	unsigned char *createCall(
		Integer moduleTo, Integer moduleFrom, Integer messageId, Integer service, Integer method,
		Integer &length);
	unsigned char *createSend(
		Integer moduleTo, Integer moduleFrom, Integer service, Integer method, Integer &length);
	unsigned char *createReply(Integer moduleTo, Integer messageId, Integer &length);

	static unsigned char *createConnectMessage(const char *key, const char *name, Integer &length);
	static unsigned char *createDisconnectMessage(Integer &length);
	static unsigned char *createConnectReply(int nId, Integer &length);
	static unsigned char *createConnectReply(char *string, Integer &length);
	static unsigned char *createException(
		Integer moduleTo, Integer messageId, unsigned char type, const char *message, const char *detailedMessage,
		Integer &length);

	friend class DataBlockConverter ;

private:
	DataBlockWriter(sbwDataBlockWriter *);

};

} // SystemsBiologyWorkbench

#endif // DATABLOCKWRITERCLASS
