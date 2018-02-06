/**
 * @file SBWException.cpp
 * @brief implementation of SBWException class - abstract base class for all SBW Exceptions
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


// SBWException.cpp: implementation of the SBWException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sbwplusbasictypes.h"
#include "SBWException.h"
#include <SBW/SBWOSMutexLock.h>


using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * mapping between thread identifier and pending exception
 */
std::map<long, SBWException *> SBWException::exceptionMap ;

/**
 * mutex to protect access to exceptionMap
 */
SBWOSMutex SBWException::threadStoreMutex("exception store") ;

/**
 * creates an exception with a user message but no detailed message.
 * @param c user message
 */
SBWException::SBWException(const std::string& c) :  m_szMessage(c), m_szDetailedMessage()
{
}

/**
 * creates an exception with user and detailed messages.
 * @param c user message.
 * @param d detailed message.
 */
SBWException::SBWException(const std::string& c, const std::string& d)
  : m_szMessage(c), m_szDetailedMessage(d)
{
}

/**
 * recover memory used by exception
 */
SBWException::~SBWException()
{
}

std::string SBWException::getMessage() const
{
  return m_szMessage;
}

std::string SBWException::getDetailedMessage() const
{
  return m_szDetailedMessage;
}

/**
 * output to standard output the user and detailed messages.
 */
void SBWException::log() const
{
  try
  {
    printf("%s", m_szMessage.c_str());
    printf("\n");

    if (!m_szDetailedMessage.empty())
    {
      printf("%s", m_szDetailedMessage.c_str());
      printf("\n");
    }
  } catch(...) {}
}

/**
 * output to standard output the given string.
 * @param c string to be output
 */
void SBWException::log(const std::string&  c)
{
  try
  {
    printf("%s", c.c_str());
    printf("\n");
  }catch (...) {}
}

/**
 * output to standard output the 2 given strings.
 * @param c first string to be output
 * @param d 2nd string to be output
 */
void SBWException::log(const std::string& c, const std::string& d)
{
  try
  {
    printf("%s", c.c_str());
    printf("\n");
    printf("%s", d.c_str());
    printf("\n");
  } catch(...) {}
}

/**
 * overload allocation operator to use C heap
 * method needs to be removed.
 */
void *SBWException::operator new(size_t size)
{
  return malloc(size);
}

/**
 * overload deallocation operator to use C heap
 * method needs to be removed.
 */
void SBWException::operator delete(void *x)
{
  free(x);
}

/**
 * returns the pending exception for this thread.
 * @return the pending exception for this thread.
 */
SBWException *SBWException::getStoredException()
{
  try
  {
#ifdef WIN32
    DWORD threadId = GetCurrentThreadId();
#elif defined(HAVE_LIBPTHREAD)
    long threadId = (long)pthread_self();
#endif
    SBWOSMutexLock ml(threadStoreMutex);
    std::map<long, SBWException *>::iterator itr = exceptionMap.find(threadId);

    if (itr == exceptionMap.end())
      return NULL ;
    else
      return (*itr).second ;
  } catch (...) { return NULL; }
}

/**
 * removes and deallocates the pending exception for this thread.
 * Assumes there is a stored exception for this thread.
 */
void SBWException::clearStoredException()
{
#ifdef WIN32
  DWORD threadId = GetCurrentThreadId();
#elif defined(HAVE_LIBPTHREAD)
  long threadId = (long)pthread_self();
#endif
  SBWOSMutexLock ml(threadStoreMutex);
  SBWException *exception = SBWException::getStoredException();

  delete exception ;
  exceptionMap.erase(threadId);
}

/**
 * stores this exception as the pending exception for this thread
 */
void SBWException::storeException()
{
#ifdef WIN32
  DWORD threadId = GetCurrentThreadId();
#elif defined(HAVE_LIBPTHREAD)
  long threadId = (long)pthread_self();
#endif
  SBWOSMutexLock ml(threadStoreMutex);
  SBWException *exception = SBWException::getStoredException();

  delete exception ;
  exceptionMap[threadId] = this ;
}

/**
 * throws the stored exception for this thread.
 * Assumes there is a stored exception for this thread.
 * The exception will no longer be the pending exception for this thread.
 */
void SBWException::throwStoredException()
{
#ifdef WIN32
  DWORD threadId = GetCurrentThreadId();
#elif defined(HAVE_LIBPTHREAD)
  long threadId = (long)pthread_self();
#endif
  SBWOSMutexLock ml(threadStoreMutex);
  SBWException *exception = SBWException::getStoredException();

  exceptionMap.erase(threadId);

  throw exception ;
}
/**
 * clears all exceptions, on all threads.
 * Added to support the SBW-MATLAB interface
 * (the nefarious tricky-sticky-error problem ).
 */
void SBWException::clearAllExceptions()
{
  SBWOSMutexLock ml(threadStoreMutex);
  exceptionMap.clear();
}
