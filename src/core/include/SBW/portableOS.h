/**
 * @file portableOS.h
 * @brief  OS independent classes and methods
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
#ifndef PORTABLE_OS_H
#define PORTABLE_OS_H

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <string>

#if !defined(WIN32)
#include <SBW/config.h>
#endif

#if defined(WIN32) && !defined(SBW_STRICT_INCLUDES)
#include <winsock2.h>
# ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
# endif // WIN32_LEAN_AND_MEAN
#include <windows.h>
#if  _MSC_VER >= 1400
#pragma comment(lib,"user32.lib")
#endif
#endif

#include <SBW/sbwplusbasictypes.h>
#include <SBW/SBWOSMutexLock.h>
#include <SBW/SBWOSEvent.h>
#include <SBW/SBWThread.h>


#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif



#ifndef TRACE
#ifdef _DEBUG
/** writes a given string to standard error prefixing the string with the current thread id and
 * terminating the string the string with a carriage return.
 * The parameter is an expression that can occur on the right hand side of a << output stream operator.
 * valid expression include "foo" and 1 << 0.2 << "freddy" for example.
 * @param output concatenation expression
 */
#define TRACE(x) \
{\
  std::ostringstream out;\
\
  out << x << "\n";\
  SystemsBiologyWorkbench::SBWDebug::trace(out.str().c_str(), __FILE__, __LINE__);\
  std::cout << x << "\n";\
  std::cout.flush();\
}

#else
#define TRACE(x) {;}
#endif
#endif

namespace SystemsBiologyWorkbench
{
// -----------------------------------------------------------------------------
// Class SBWOS
// -----------------------------------------------------------------------------
/// simple portable interface to operating system functionality
class SBW_API	SBWOS
{
public:
#ifdef WIN32
  static void windowsExtractCommandLine(int *argc, char ***argv);
#endif // WIN32

  static void startProcess(const char *commandLine, bool bWait = false);

  static void ThrowError();
  static void ThrowError(int error);

  /**
   * returns the character used to seperate paths to JAR files in a string.
   * (This is not actually the standard separator character under Linux, but this
   * method is only used for breaking up lists of JAR files.)
   * @return path seperator character.
   */
  static char PathSeparator();

  /**
   * returns the directory seperator character in file and directory paths.
   * @return directory seperator character.
   */
  static char DirectorySeparator();

  /**
   * returns the file suffix for dynamic libraries on this OS
   * @return the file suffix for dynamic libraries on this OS
   */
    static const char *DynamicLibraryExt();

};

// -----------------------------------------------------------------------------
// SBWOSLibrary
// -----------------------------------------------------------------------------

#if 0
/// @deprecated
class SBWOSLibrary
{
public:
  SBWOSLibrary(const char *szLibrary)
#ifdef WIN32
  {
    char *str = strdup(szLibrary);
    char *szup = strupr(str);
    m_handle = LoadLibrary(szup);
    //char buffer[1000];
    //sprintf(buffer, "loaded %s with handle %d", szLibrary, m_handle);
        //MessageBox(NULL, buffer, "In SBWOSLibrary", MB_OK);
    free(str);
    if (m_handle == NULL)
      SBWOS::ThrowError();
  }
#endif

  ~SBWOSLibrary()
#ifdef WIN32
  {
    //char buffer[1000];
    //sprintf(buffer, "about to free library with handle %d", m_handle);
        //MessageBox(NULL, buffer, "In SBWOSLibrary", MB_OK);
    //FreeLibrary(m_handle);
  }
#endif

  void *getFunction(char *szFunction)
#ifdef WIN32
  {
    FARPROC pFunction = GetProcAddress(m_handle, szFunction);

    if (pFunction == NULL)
      SBWOS::ThrowError();

    return (void *)pFunction ;
  }
#endif

private:

#ifdef WIN32
  HINSTANCE m_handle ;
#endif
};

#endif


// -----------------------------------------------------------------------------
// Misc. utils.
// -----------------------------------------------------------------------------

/**
 * programming facilities available only in the debug library.
 * methods in this class have no effect in the release build
 */
class SBW_API SBWDebug
{
public:

  // empty inline for release configuration
#ifndef _DEBUG
  static void trace(const char *, const char *, int)
  {}
#else
  static void trace(const char *x, const char *file, int line);
#endif

  static void setTraceMode(bool mode = true);

private:
  static bool traceOn;
  static SBWOSMutex traceMutex ;
};

} // SystemsBiologyWorkbench

#endif // PORTABLE_OS_H