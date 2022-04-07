/**
 * @file sbwdefs.h
 * @brief simple type definitions that are common to both C and C++
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
#ifndef SBWDEFS
#define SBWDEFS


/**
 * SBW_DOTTED_VERSION:
 *
 * A version string of the form "1.2.3".
 */
#define SBW_DOTTED_VERSION	"@PACKAGE_VERSION@"


/**
 * SBW_VERSION:
 *
 * The version as an integer: version 1.2.3 becomes 10203.  Since the major
 * number comes first, the overall number will always increase when a new
 * libSBML is released, making it easy to use less-than and greater-than
 * comparisons when testing versions numbers.
 */
#define SBW_VERSION		@SBW_VERSION_NUMERIC@


/**
 * SBW_VERSION_STRING:
 *
 * The numeric version as a string: version 1.2.3 becomes "10203".
 */
#define SBW_VERSION_STRING	"@SBW_VERSION_NUMERIC@"



#ifdef __cplusplus
#include <iosfwd>
#define SBW_CREATE_STRING(x,target)\
{\
std::stringstream strstr;\
strstr << x;\
target = strstr.str();\
}\

#else
#define SBW_CREATE_STRING(x,target);
#endif 

#ifndef __LITTLE_ENDIAN__
#ifndef __BIG_ENDIAN__
#define __LITTLE_ENDIAN__
#endif
#endif

#if  _MSC_VER >= 1400
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

// COPASI ONLY: use always static version
#if !defined(SBW_STATIC)
#if !defined(SBW_FORCE_EXPORT)
#define SBW_STATIC
#endif
#endif


#ifndef STDCALL
#ifdef WIN32
#define STDCALL __stdcall
#else
#ifdef GCC_STDCALL
#define STDCALL __attribute__((stdcall)) 
#else
#define STDCALL
#endif
#endif  // not Win32
#endif  // not STDCALL

// There is no DLL business under Linux, so we just use "extern".

#if !defined(SBW_FORCE_EXPORT) && ( defined (LINUX) || defined(CYGWIN) || defined(MINGW) ||  defined(NO_API) || defined(SBW_STATIC))
#define SBW_API
#else

// The following ifdef block is the standard way of creating macros which
// make exporting from a DLL simpler. All files within this DLL are
// compiled with the SBW_EXPORTS symbol defined on the command line. this
// symbol should not be defined on any project that uses this DLL. This way
// any other project whose source files include this file see SBW_API
// functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#if defined(SBW_EXPORTS) || defined(SBW_FORCE_EXPORT)
#define SBW_API __declspec(dllexport)
#else 
#define SBW_API __declspec(dllimport)
#endif


#include <assert.h>

#if defined(WIN32) && ! defined(CYGWIN)
// disable silly messages about debug identifier sizes being too big, typically caused by the STL.
//#pragma warning(disable: 4786 4788)
#endif


#endif

#endif
