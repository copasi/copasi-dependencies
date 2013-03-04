/**
 * @file DataBlockWriterClass.h
 * @brief  encodes data in datablock - class definition
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: DataBlockWriterClass.h,v 1.3 2007/02/22 22:24:53 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/DataBlockWriterClass.h,v $
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

#ifndef DATABLOCKWRITERCLASS
#define DATABLOCKWRITERCLASS

#if    _MSC_VER > 1000
#pragma once
#pragma warning (disable: 4800)
#endif

#include "ImplDataBlockWriterClass.h"
#include "SBWHandle.h"
using namespace SystemsBiologyWorkbench;
namespace SystemsBiologyWorkbench
{

/// encodes data in a datablock ultimately for transmission to another module or the broker
class DataBlockWriter : public SBWHandle<sbwDataBlockWriter>
{
public:
	SBW_API DataBlockWriter(const DataBlockWriter &);
	SBW_API DataBlockWriter();

	/// @deprecated
	DataBlockWriter& self() { return *this; }

	/**
	 * returns underlying sbwDataBlockWriter that implements the data block writer.
	 * @return underlying sbwDataBlockWriter
	 */
	sbwDataBlockWriter *getImpl() { return p; }

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
	SBW_API DataBlockWriter(sbwDataBlockWriter *);

};

} // SystemsBiologyWorkbench

#endif // DATABLOCKWRITERCLASS
