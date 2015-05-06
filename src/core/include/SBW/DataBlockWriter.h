/**
 * @file DataBlockWriter.h
 * @brief  encodes data in data block - inline methods
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

/// MSVC generated include loop block
#if !defined(AFX_DATABLOCKWRITER_H__55CECBFB_D441_416A_8150_04BEF26E1EDD__INCLUDED_)
#define AFX_DATABLOCKWRITER_H__55CECBFB_D441_416A_8150_04BEF26E1EDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include <SBW/DataBlockWriterClass.h>
#include <SBW/ImplDataBlockWriter.h>
#include <SBW/SBWRPC.h>

/**
 * return a DataBlockReader corresponding to this object
 * @return a DataBlockReader corresponding to this object
 */
inline SystemsBiologyWorkbench::DataBlockReader SystemsBiologyWorkbench::DataBlockWriter::getReader() const
{
	return DataBlockReader(p->getReader());
}

/**
 * returns the raw data contained in this object.
 * The returned data is owned by this object.
 * @param x referenced integer is set to the number of bytes in the returned data
 * @return the raw data contained in this object.
 */
inline const unsigned char *SystemsBiologyWorkbench::DataBlockWriter::getData(SystemsBiologyWorkbench::Integer *x) const
{
	return p->getData(x);
}

/**
 * returns a copy of the raw data in this object.
 * The returned data should deallocated using delete[].
 * @param x set to the number of bytes in the returned data
 * @return the raw data contained in this object.
 */
inline unsigned char *SystemsBiologyWorkbench::DataBlockWriter::getDataCopy(SystemsBiologyWorkbench::Integer &x) const
{
	return p->getDataCopy(x);
}

/**
 * inserts raw data into this object at the current cursor position.
 * The cursor is moved to next position after the inserted data.
 * @param data data for insertion.
 * @param length number of bytes to be inserted.
 */
inline void SystemsBiologyWorkbench::DataBlockWriter::insertData(
	const unsigned char *data, SystemsBiologyWorkbench::Integer length)
{
	p->insertData(data, length);
}

/**
 * writes to standard output the space seperated decimal values of the bytes in this object.
 */
inline void SystemsBiologyWorkbench::DataBlockWriter::dump()
{
	p->dump();
}

/**
 * returns a call message where the argument data is taken from this object.
 * Returned data should be freed with the delete[] operator.
 * @param moduleTo the numeric module instance identifier of the module being called
 * @param moduleFrom the numeric module instance identifier of the module making the call
 * @param messageId the message identifier
 * @param service the numeric service identifier of the service being called
 * @param method the numeric method identifier of the method being called
 * @param length set to the length of the returned data including the message termination byte
 * @return a complete call message where the argument data is taken from this DataBlockWriter.
 */
inline unsigned char *SystemsBiologyWorkbench::DataBlockWriter::createCall(
	SystemsBiologyWorkbench::Integer moduleTo,
	SystemsBiologyWorkbench::Integer moduleFrom,
	SystemsBiologyWorkbench::Integer messageId,
	SystemsBiologyWorkbench::Integer service,
	SystemsBiologyWorkbench::Integer method,
	SystemsBiologyWorkbench::Integer &length)
{
	return p->createCall(moduleTo, moduleFrom, messageId, service, method, length);
}

/**
 * returns a send message where the argument data is taken from this DataBlockWriter.
 * Returned data should be freed with the delete[] operator.
 * @param moduleTo the numeric module instance identifier of the module being sent to.
 * @param moduleFrom the numeric module instance identifier of the module sending the message
 * @param messageId the numeric message identifier
 * @param service the numeric service identifier of the service being called
 * @param method the numeric method identifier of the method being called
 * @param length set to the length of the returned data including the message termination byte
 * @return a complete send message where the argument data is taken from this DataBlockWriter.
 */
inline unsigned char *SystemsBiologyWorkbench::DataBlockWriter::createSend(
	SystemsBiologyWorkbench::Integer moduleTo,
	SystemsBiologyWorkbench::Integer moduleFrom,
	SystemsBiologyWorkbench::Integer service,
	SystemsBiologyWorkbench::Integer method,
	SystemsBiologyWorkbench::Integer &length)
{
	return p->createSend(moduleTo, moduleFrom, service, method, length);
}

/** 
 * returns a reply message where the result data is taken from this DataBlockWriter.
 * Returned data should be freed with the delete[] operator.
 * @param moduleTo the numeric module instance identifier of the module being sent to.
 * @param messageId numeric message identifier of the message being replied to.
 * @param length set to the length of the returned data including the message termination byte.
 * @return a complete reply message where the result data is taken from this DataBlockWriter.
 */
inline unsigned char *SystemsBiologyWorkbench::DataBlockWriter::createReply(
	SystemsBiologyWorkbench::Integer moduleTo,
	SystemsBiologyWorkbench::Integer messageId,
	SystemsBiologyWorkbench::Integer &length)
{
	return p->createReply(moduleTo, messageId, length);
}

/**
 * returns a connect message.
 * Returned data should be freed with the delete[] operator.
 * @param name the module identification string of this module instance.
 * @length set to the length of the returned data
 * @return a connect message.
 */
inline unsigned char *SystemsBiologyWorkbench::DataBlockWriter::createConnectMessage(
        const char *sessionKey, const char *name,
	SystemsBiologyWorkbench::Integer &length)
{
	sbwDataBlockWriter writer(4 + (int)strlen(sessionKey) + (int)strlen(name));

	writer.packWithoutType(sessionKey);
	writer.packWithoutType(name);

	return writer.getDataCopy(length);
}
inline unsigned char *SystemsBiologyWorkbench::DataBlockWriter::createConnectReply(int nId, Integer &length)
{
	sbwDataBlockWriter writer(4);

	writer.packWithoutType(nId);

	return writer.getDataCopy(length);
}
inline unsigned char *SystemsBiologyWorkbench::DataBlockWriter::createConnectReply(char *string, Integer &length)
{
	sbwDataBlockWriter writer(4 + (int)strlen(string));

	writer.packWithoutType(string);

	return writer.getDataCopy(length);
}


/**
* returns a disconnect message.
* Returned data should be freed with the delete[] operator.
* @length set to the length of the returned data
* @return a disconnect message.
*/
inline unsigned char *SystemsBiologyWorkbench::DataBlockWriter::createDisconnectMessage(
	SystemsBiologyWorkbench::Integer &length)
{
	sbwDataBlockWriter writer(4);
	writer.packWithoutType(SBWRPC::disconnectMessage);

	return writer.getDataCopy(length);
}

/**
* return an exception message.
* @param moduleTo destination module instance numeric identifier
* @param messageId the numeric identifier for the message that the given exception is for.
* @param type the exception code (should be Exception code).
* @param message message for user
* @param detailedMessage message for developer
* @param length set to the number of bytes in the result.
* @return the exception message
*/
inline unsigned char *SystemsBiologyWorkbench::DataBlockWriter::createException(
	SystemsBiologyWorkbench::Integer moduleTo,
	SystemsBiologyWorkbench::Integer messageId,
	unsigned char type,
	const char *message,
	const char *detailedMessage,
	SystemsBiologyWorkbench::Integer &length)
{
	sbwDataBlockWriter writer ;
	SystemsBiologyWorkbench::Integer size = (SystemsBiologyWorkbench::Integer)(24 + strlen(message) + strlen(detailedMessage));

	writer.packWithoutType(size); // no terminator
	writer.packWithoutType(moduleTo);
	writer.packWithoutType((unsigned char)SBWExceptionCode);
	writer.packWithoutType(messageId);
	writer.packWithoutType(type);
	writer.add(message);
	writer.add(detailedMessage);

	unsigned char *result = writer.getDataCopy(length);

	assert(length == size);

	return result ;
}

#endif // !defined(AFX_DATABLOCKWRITER_H__55CECBFB_D441_416A_8150_04BEF26E1EDD__INCLUDED_)
