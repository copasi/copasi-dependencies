/**
 * @file ImplDataBlockWriter.cpp
 * @brief Implementation of DataBlockWriter
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ImplDataBlockWriter.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/ImplDataBlockWriter.cpp,v $
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

static const char rcsid[] = "$Id: ImplDataBlockWriter.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $";

// sbwDataBlockWriter.cpp: implementation of the sbwDataBlockWriter class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ImplDataBlockWriter.h"
#include "ImplDataBlockReader.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/** 
 * creates an empty data block of initial length 1000 bytes
 */
SBW_API sbwDataBlockWriter::sbwDataBlockWriter()
{
    end = 0;
    dataLength = 1000;
    data = new unsigned char[1000]; 
}

/**
 * creates an empty data block of initial length x bytes.
 * @param x Initial length of data block.
 */
SBW_API sbwDataBlockWriter::sbwDataBlockWriter(Integer x)
{
    end = 0;
    dataLength = x;
    data = new unsigned char[x];
}

/**
 * deallocates the memory used to represent the datablock.
 */
SBW_API sbwDataBlockWriter::~sbwDataBlockWriter()
{
	delete[] data ;
}

/**
 * outputs the datablock to standard output.
 * Data is output in space seperated decimal byte format.
 */
SBW_API void sbwDataBlockWriter::dump()
{
	int byte = 0 ;

	printf("\n");
	while (byte != end)
	{
		printf("%d ", data[byte]);
		byte++ ;
	}

	printf("\n");
}

/**
 * ensures that this data block can store an additional number of bytes beyond the last stored object.
 * Replaces the existing datablock representation with a new memory allocation.
 * @param space number of new bytes required.
 */
SBW_API void sbwDataBlockWriter::makeSpace(int space)
{
	dataLength = (end + space) * 2;

	unsigned char *newData = new unsigned char[dataLength];
	
	memcpy(newData, data, end);
	delete[] data ;
	data = newData ;
}

/**
 * stores writer as a list without a list type prefix.
 * @param writer contains the list data to be stored in this DataBlockWriter.
 */
SBW_API void sbwDataBlockWriter::packWithoutType(const DataBlockWriter writer)
{
	Integer items = 0;
	DataBlockReader reader = writer.getReader();

	while (reader.getNextType() != TerminateType)
	{
		reader.skipObject();
		items++;
	}	

	packWithoutType(items);
	
	Integer length ;
	const unsigned char *listData = writer.getData(&length);

	insertData(listData, length);
}


/**
 * stores writer as a list without a list type prefix.
 * @param writer contains the list data to be stored in this DataBlockWriter.
 */
SBW_API void sbwDataBlockWriter::packWithoutType(const sbwDataBlockWriter *const writer)
{
	Integer items = 0;
	DataBlockReader reader(writer->getReader());

	while (reader.getNextType() != TerminateType)
	{
		reader.skipObject();
		items++;
	}	

	packWithoutType(items);
	
	Integer length ;
	const unsigned char *listData = writer->getData(&length);

	insertData(listData, length);
}

/** 
 * returns a sbwDataBlockReader containing the data that is in this sbwDataBlockWriter.
 * @return a sbwDataBlockReader containing the data that is in this sbwDataBlockWriter terminated by a byte of
 *         value TerminateType.
 */
SBW_API sbwDataBlockReader *sbwDataBlockWriter::getReader() const
{
	sbwDataBlockWriter writer(end + 1); // length + terminator

	writer.insertData(data, end);
	writer.packWithoutType((unsigned char)TerminateType);

	return new sbwDataBlockReader(writer.getData(), writer.end);
}

/**
 * returns a raw data block comprised of header appended to the data in this sbwDataBlockWriter.
 * Returned data should be freed with the delete[] operator.
 * @return a raw data block comprised of header appended to the data in this sbwDataBlockWriter terminated by a byte of
 *         value TerminateType. 
 */
unsigned char *sbwDataBlockWriter::assembleMessage(sbwDataBlockWriter &header)
{
	Integer headerLength ;
	const unsigned char *headerData = header.getData(&headerLength);
	unsigned char *result = new unsigned char[headerLength + end + 1] ;

	memcpy(result, headerData, headerLength);
	memcpy(result + headerLength, data, end);
	result[headerLength + end] = (unsigned char)TerminateType ;

	return result ;
}

/**
 * returns a call or send message where the argument data is taken from this sbwDataBlockWriter.
 * Returned data should be freed with the delete[] operator.
 * @param type the code indicating whether this is a call or send message.
 * @param moduleTo the numeric module instance identifier of the module being called
 * @param moduleFrom the numeric module instance identifier of the module making/sending the call/message
 * @param messageId the message identifier
 * @param service the numeric service identifier of the service being called
 * @param method the numeric method identifier of the method being called
 * @param length set to the length of the returned data including the message termination byte
 * @return a complete call or send message where the argument data is taken from this sbwDataBlockWriter.
 */
unsigned char *sbwDataBlockWriter::createInvoke(
	unsigned char type, Integer moduleTo, Integer moduleFrom, Integer messageId, Integer service,
	Integer method, Integer &length)
{
	sbwDataBlockWriter header(25);

	length = end + 26;
	header.packWithoutType(length); // extra byte for terminator
	header.packWithoutType(moduleTo);
	header.packWithoutType(type);
	header.packWithoutType(messageId);
	header.packWithoutType(moduleFrom);
	header.packWithoutType(service);
	header.packWithoutType(method);

	return assembleMessage(header);
}

/**
 * returns a call message where the argument data is taken from this sbwDataBlockWriter.
 * Returned data should be freed with the delete[] operator.
 * @param moduleTo the numeric module instance identifier of the module being called
 * @param moduleFrom the numeric module instance identifier of the module making the call
 * @param messageId the message identifier
 * @param service the numeric service identifier of the service being called
 * @param method the numeric method identifier of the method being called
 * @param length set to the length of the returned data including the message termination byte
 * @return a complete call message where the argument data is taken from this sbwDataBlockWriter.
 */
SBW_API unsigned char *sbwDataBlockWriter::createCall(
	Integer moduleTo, Integer moduleFrom, Integer messageId, Integer service, Integer method,
	Integer &length)
{
	return createInvoke(SBWCallCode, moduleTo, moduleFrom, messageId, service, method, length);
}

/**
 * returns a send message where the argument data is taken from this sbwDataBlockWriter.
 * Returned data should be freed with the delete[] operator.
 * @param moduleTo the numeric module instance identifier of the module being sent to.
 * @param moduleFrom the numeric module instance identifier of the module sending the message
 * @param messageId the numeric message identifier
 * @param service the numeric service identifier of the service being called
 * @param method the numeric method identifier of the method being called
 * @param length set to the length of the returned data including the message termination byte
 * @return a complete send message where the argument data is taken from this sbwDataBlockWriter.
 */
SBW_API unsigned char *sbwDataBlockWriter::createSend(
	Integer moduleTo, Integer moduleFrom, Integer service, Integer method,
	Integer &length)
{
	return createInvoke(SBWSendCode, moduleTo, moduleFrom, ~0, service, method, length);
}

/** 
 * returns a reply message where the result data is taken from this sbwDataBlockWriter.
 * Returned data should be freed with the delete[] operator.
 * @param moduleTo the numeric module instance identifier of the module being sent to.
 * @param messageId numeric message identifier of the message being replied to.
 * @param length set to the length of the returned data including the message termination byte.
 * @return a complete reply message where the result data is taken from this sbwDataBlockWriter.
 */
SBW_API unsigned char *sbwDataBlockWriter::createReply(Integer moduleTo, Integer messageId, Integer &length)
{
	sbwDataBlockWriter header(13);

	length = end + 14;
	header.packWithoutType(length);
	header.packWithoutType(moduleTo);
	header.packWithoutType((unsigned char)SBWReplyCode);
	header.packWithoutType(messageId);

	return assembleMessage(header);
}
