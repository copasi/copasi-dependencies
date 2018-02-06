/**
* @file MethodImpl.h
* @brief  implementation of a method together with its signature
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
// MethodImpl.h: interface for the MethodImpl class.
//
//////////////////////////////////////////////////////////////////////

/// MSVC generated include loop block
#if !defined(AFX_METHODIMPL_H__71359E4A_9F49_450A_9619_6C4EB77FFA54__INCLUDED_)
#define AFX_METHODIMPL_H__71359E4A_9F49_450A_9619_6C4EB77FFA54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#include <SBW/sbwdefs.h>
#include <SBW/Module.h>
#include <SBW/DataBlockWriterClass.h>
#include <SBW/DataBlockReaderClass.h>

namespace SystemsBiologyWorkbench
{

  class SBWOSMutex;
  class Handler ;

  /// implementation of an SBW method in this module together with associated data
  class SBW_API MethodImpl
  {
  public:
    MethodImpl(Handler *handler, std::string signature, SBWOSMutex *mutex = NULL, std::string help = "");
    virtual ~MethodImpl();

    DataBlockWriter invoke(Module from, DataBlockReader);
    bool isSynchronised() const;
    std::string getSignature() const;
    Handler *getHandler();

    /**
    * returns the help string for this method
    * @return the help string for this method
    */
    std::string getHelp() const;

  private:
    /// the method implementation
    Handler *handler;

    /// the method signature
    std::string signature;

    /** mutex to be locked during any call on this method.
    * normally this is either null implying that this method doesn't need to be synchroized or
    * the mutex is the same for all methods on the method's service.
    */
    SBWOSMutex *mutex;

    /// the help string for this method
    std::string help;
  };

} // SystemsBiologyWorkbench
#endif // !defined(AFX_METHODIMPL_H__71359E4A_9F49_450A_9619_6C4EB77FFA54__INCLUDED_)
