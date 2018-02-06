/**
 * @file SBWException.h
 * @brief abstract base class of all SBW exceptions
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
#if !defined(AFX_SBWEXCEPTION_H__358F305F_1B7B_4D2D_AC38_16E43A66DDC4__INCLUDED_)
#define AFX_SBWEXCEPTION_H__358F305F_1B7B_4D2D_AC38_16E43A66DDC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/sbwplusbasictypes.h>
#include <SBW/sbwdefs.h>
#include <SBW/SBWOSMutex.h>
#ifndef SBW_STRICT_INCLUDES
#include <SBW/portableOS.h>
#endif
#include <SBW/sbwenums.h>

#include <map>
#include <exception>

namespace SystemsBiologyWorkbench
{

/// abstract base class of all exceptions thrown by the C++ API
class SBW_API SBWException
{
public:
    SBWException(const std::string& msg) ;
    SBWException(const std::string& a, const std::string& b) ;

    virtual ~SBWException() ;

    /**
     * return the user message associated with this exception.
     * @return the user message associated with this exception.
     */
    std::string getMessage() const;
    /**
     * return the detailed developer message associated with this exception.
     * @return the detailed developer message associated with this exception.
     */
    std::string getDetailedMessage() const;

    void log() const;

    /**
     * output to standard output the given string.
     * @param sc string to be output.
     */
    static void log(const std::string& sc);

    /**
     * output to standard output the 2 given strings.
     * @param sc first string to be output
     * @param sd 2nd string to be output
     */
    static void log(const std::string& sc, const std::string& sd);

    virtual unsigned char getCode() = 0;

    void *operator new(size_t size);
    void operator delete(void *);

    void storeException();
    static SBWException *getStoredException();
    static void clearStoredException();
    static void throwStoredException();
    static void clearAllExceptions();

private :
    /// user message
    std::string m_szMessage ;

    /// detailed developer message
    std::string m_szDetailedMessage ;

    static std::map<long, SBWException *> exceptionMap ;
    static SBWOSMutex threadStoreMutex;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWEXCEPTION_H__358F305F_1B7B_4D2D_AC38_16E43A66DDC4__INCLUDED_)
