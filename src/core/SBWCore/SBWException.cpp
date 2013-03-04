/**
 * @file SBWException.cpp
 * @brief implementation of SBWException class - abstract base class for all SBW Exceptions
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWException.cpp,v 1.2 2007/01/28 03:29:07 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SBWException.cpp,v $
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

static const char rcsid[] = "$Id: SBWException.cpp,v 1.2 2007/01/28 03:29:07 fbergmann Exp $";

// SBWException.cpp: implementation of the SBWException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sbwplusbasictypes.h"
#include "SBWException.h"
#include <stdio.h>

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
#if  _MSC_VER >= 1400
SBWException::SBWException(const char *c) :  m_szMessage(_strdup(c)), m_szDetailedMessage(NULL)
#else
SBWException::SBWException(const char *c) :  m_szMessage(strdup(c)), m_szDetailedMessage(NULL)
#endif
{
}

/**
 * creates an exception with user and detailed messages.
 * @param c user message.
 * @param d detailed message.
 */
#if  _MSC_VER >= 1400
SBWException::SBWException(const char *c, const char *d) 
: m_szMessage(_strdup(c)), m_szDetailedMessage(_strdup(d))
#else
SBWException::SBWException(const char *c, const char *d) 
: m_szMessage(strdup(c)), m_szDetailedMessage(strdup(d))
#endif
{
}

/**
 * creates an exception with a user message but no detailed message.
 * @param sc user message
 */
#if  _MSC_VER >= 1400
SBWException::SBWException(std::string sc)  : m_szMessage(_strdup(sc.c_str())), m_szDetailedMessage(NULL)
#else
SBWException::SBWException(std::string sc)  : m_szMessage(strdup(sc.c_str())), m_szDetailedMessage(NULL)
#endif
{
}


/**
 * creates an exception with user and detailed messages.
 * @param sc user message.
 * @param sd detailed message.
 */
#if  _MSC_VER >= 1400
SBWException::SBWException(std::string sc, std::string sd) 
: m_szMessage(_strdup(sc.c_str())), m_szDetailedMessage(_strdup(sd.c_str()))
#else
SBWException::SBWException(std::string sc, std::string sd) 
: m_szMessage(strdup(sc.c_str())), m_szDetailedMessage(strdup(sd.c_str()))
#endif
{
}

/**
 * recover memory used by exception
 */
SBWException::~SBWException() 
{
	free((void *)m_szMessage);

	if (m_szDetailedMessage != NULL)
		free((void *)m_szDetailedMessage);
}

/**
 * output to standard output the user and detailed messages.
 */
void SBWException::log()
{
	try
	{
	printf("%s", m_szMessage);
	printf("\n");

	if (m_szDetailedMessage != NULL)
	{
		printf("%s", m_szDetailedMessage);
		printf("\n");
	}
	} catch(...) {}
}

/**
 * output to standard output the given string.
 * @param c string to be output
 */
void SBWException::log(const char *c)
{
	try
	{
	printf("%s", c);
	printf("\n");
	}catch (...) {}
}

/**
 * output to standard output the 2 given strings.
 * @param c first string to be output
 * @param d 2nd string to be output
 */
void SBWException::log(const char *c, const char *d)
{
	try
	{
	printf("%s", c);
	printf("\n");
	printf("%s", d);
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
