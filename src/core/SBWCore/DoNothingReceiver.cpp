/**
 * @file DoNothingReceiver.cpp
 * @brief a receiver (handles all messages for a module) that does bugger all
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

// DoNothingReceiver.cpp: implementation of the DoNothingReceiver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoNothingReceiver.h"

#include "SBWRPC.h"
#include "SBWMethodNotFoundException.h"
#include "SBWServiceNotFoundException.h"
#include "ImplDataBlockWriter.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * process a call/send message that require implementation on this module where the module actually provides no services.
 * @throws SBWException * if unsupported services or methods are called.
 * @param fromModuleId module that sent the message.
 * @param service requested service - only SBWSystemService supported.
 * @param method requested method - only SBWGetMethodsMethod and SBWGetServicesMethod supported.
 * @param args method argument data - ignored.
 * @return empty data block.
 */
DataBlockWriter DoNothingReceiver::receive(
        Integer /*fromModuleId*/, Integer service, Integer method, DataBlockReader /*args*/)
{
    DataBlockWriter result ;

    switch (service)
    {
        case SBWSystemService :
            switch (method)
            {
                case SBWGetMethodsMethod :
                case SBWGetServicesMethod :
                    {
                        const char *empty[] = {""} ;
                        result.add(0, empty);

                        return result ;
                    }

                default :
                    throw new SBWMethodNotFoundException("method doesn't exist");
            }

        default :
            throw new SBWServiceNotFoundException("service doesn't exist");
    }
}
