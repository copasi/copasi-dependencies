/**
 * @file MethodInline.h
 * @brief  interface for inline ServiceMethod
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
#ifndef METHODINLINE
#define METHODINLINE

#include <SBW/SBWRPC.h>
#include <SBW/Signature.h>
#include <SBW/SBWLowLevel.h>
#include <SBW/Method.h>

/// creates a potential reference to an SBW Method.  This value doesn't refer to any known method.
inline SystemsBiologyWorkbench::ServiceMethod::ServiceMethod()
: moduleId(SBWErrorModule), serviceId(SBWErrorService), methodId(SBWErrorMethod)
{
}

/**
 * creates a reference to a given SBW method.
 * @param mid numeric module instance identifier.
 * @param sid numeric service identifier
 * @param methid numeric methid identifier
 */
inline SystemsBiologyWorkbench::ServiceMethod::ServiceMethod(
	SystemsBiologyWorkbench::Integer mid, SystemsBiologyWorkbench::Integer sid, SystemsBiologyWorkbench::Integer methid)
: moduleId(mid), serviceId(sid), methodId(methid)
{
}

/**
 * returns the name of the method.
 * @returns the name of the method
 */
inline std::string SystemsBiologyWorkbench::ServiceMethod::getName()
{
	return getSignature().getName();
}

/**
 * returns the help string for the method.
 * @returns the help string for the method
 */
inline std::string SystemsBiologyWorkbench::ServiceMethod::getHelp()
{
	return SBWLowLevel::methodGetHelpInString(moduleId, serviceId, methodId);
}

/**
 * returns the signature string of the method.
 * @returns the signature string of the method
 */
inline std::string SystemsBiologyWorkbench::ServiceMethod::getSignatureString()
{
	return SBWLowLevel::methodGetSignatureInString(moduleId, serviceId, methodId);
}

/**
 * returns the signature structure of the method.
 * @returns the signature structure of the method
 */
inline SystemsBiologyWorkbench::Signature SystemsBiologyWorkbench::ServiceMethod::getSignature()
{
	return SBWLowLevel::methodGetSignature(moduleId, serviceId, methodId);
}

/**
 * call the method blocking until the method completes.
 * @param args the datablock containing the arguments to the call.
 * @return the datablock containing the result of the method call.
 */
inline SystemsBiologyWorkbench::DataBlockReader SystemsBiologyWorkbench::ServiceMethod::call(
			SystemsBiologyWorkbench::DataBlockWriter args)
{
	return SBWLowLevel::methodCall(moduleId, serviceId, methodId, args);
}

/**
 * call the method returning as soon as the transmit message has been transmitted from this application.
 * @param args the datablock containing the arguments to the call.
 */
inline void SystemsBiologyWorkbench::ServiceMethod::send(DataBlockWriter args)
{
	SBWLowLevel::methodSend(moduleId, serviceId, methodId, args);
}
#endif // METHODINLINE
