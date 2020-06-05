/**
 * @file MethodImpl.cpp
 * @brief contains a method implementation together with its signature string.
 * ServiceMethod class represents a method on a service in a potentially different module.
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

// MethodImpl.cpp: implementation of the MethodImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MethodImpl.h"
#include "Handler.h"
#include <SBW/portableOS.h>

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/// deallocates the method handler object
MethodImpl::~MethodImpl()
{
    delete handler ;
    handler = NULL;
}

/**
 * constructs a service method object's implementation
 * @param h the handler object that implements the method functionality
 * @param s the method's SBW signature
 * @param m the mutex for all sychronized methods - null if this method is not synchronized - (null by default)
 * @param hp the help string for the method - (empty by default)
 */
MethodImpl::MethodImpl(Handler *h, std::string s, SBWOSMutex *m, std::string hp)
: handler(h), signature(s), mutex(m), help(hp)
{
    // check syntax
    Signature sig(s);
}

/**
 * invokes/executes this method.
 * @param from the module that invoked this method.
 * @param reader the argument data for this invocation.
 * @return a DataBlockWriter containing the result data from this method invocation.
 */
DataBlockWriter MethodImpl::invoke(Module from, DataBlockReader reader)
{
    if (isSynchronised())
    {
        SBWOSMutexLock ml(*mutex);

        return handler->receive(from, reader);
    }
    else
        return handler->receive(from, reader);
}

/**
 * returns whether this method is synchronized.
 * @return whether this method is synchronized.
 */
bool MethodImpl::isSynchronised() const
{
    return mutex != NULL ;
}

/**
 * returns the signature of this method.
 * @return the signature of this method.
 */
std::string MethodImpl::getSignature() const
{
    return signature ;
}

/**
 * returns the handler object that implements this method.
 * @return the handler object that implements this method.
 */
Handler *MethodImpl::getHandler()
{
    return handler ;
}

std::string MethodImpl::getHelp()  const { return help; }
