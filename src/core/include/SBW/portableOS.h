/**
 * @file portableOS.h
 * @brief  OS independant classes and methods
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: portableOS.h,v 1.7 2008/10/17 22:45:53 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/portableOS.h,v $
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

/// include loop block
#ifndef PORTABLE_OS_H
#define PORTABLE_OS_H

#ifdef WIN32
#include "winsock2.h"
#include "windows.h"
#if  _MSC_VER >= 1400
#pragma comment(lib,"user32.lib")
#endif
#endif

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "sbwplusbasictypes.h"

#if !defined(WIN32) 
#include "config.h"
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef WIN32
/// 64 bit unsigned integer
#define SBW64UINT ULONGLONG
#endif

#ifdef LINUX
#define LPVOID void *
#define HAVE_STRERROR
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
class SBWOS
{
public:
#ifdef WIN32
	SBW_API	static void windowsExtractCommandLine(int *argc, char ***argv);
#endif // WIN32

	SBW_API	static void startProcess(char *commandLine, bool bWait = false);

	static void ThrowError();
	static void ThrowError(int error);

	/**
	 * returns the character used to seperate paths to JAR files in a string.
	 * (This is not actually the standard separator character under Linux, but this
	 * method is only used for breaking up lists of JAR files.)
	 * @return path seperator character.
	 */
	static char PathSeparator() { return ';'; }

	/**
	 * returns the directory seperator character in file and directory paths.
	 * @return directory seperator character.
	 */
	static char DirectorySeparator()
	{
#ifdef WIN32
		return '\\' ;
#else
		return '/' ;
#endif
	}

	/**
	 * returns the file suffix for dynamic libraries on this OS
	 * @return the file suffix for dynamic libraries on this OS
	 */
    static const char *DynamicLibraryExt()
	{
#ifdef WIN32
		return "dll" ;
#else
		return "so" ;
#endif
	}

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
// SBWOSMutex
// -----------------------------------------------------------------------------
/**
 * portable interface to operating system thread mutex functionality.
 * (windows implementation uses critical sections)
 * Not for use for synchronising processes.
 */
class SBWOSMutex
{
public :
	SBW_API SBWOSMutex(std::string name);
	SBW_API SBWOSMutex(std::string name, std::string suffix);
	SBW_API ~SBWOSMutex();

	SBW_API void lock();
	SBW_API void unLock();

	/**
	 * returns the name of this mutex.
	 * @return the name of this mutex.
	 */
	SBW_API std::string getName() { return name ; }

private:
#if defined(WIN32)
	CRITICAL_SECTION criticalSection;
#else
	pthread_mutex_t mutex;
#endif
	std::string name ;
};


// -----------------------------------------------------------------------------
// SBWOSMutexLock
// -----------------------------------------------------------------------------
/**
 * represents a lock on a SBWOSMutex
 */
class SBWOSMutexLock
{
public :
	/**
	 * lock the given mutex and keep a pointer to the mutex.
	 * @param m mutex to be locked for the lifetime of this object.
	 */
	SBWOSMutexLock(SBWOSMutex &m) : mutex(m) { m.lock(); }

	/// unlock the associated mutex
	~SBWOSMutexLock() { mutex.unLock(); }

	//SBWOSMutexLock & operator=( const SBWOSMutexLock & ) {}
	SBWOSMutexLock & operator=( const SBWOSMutexLock & ) {return *this;}

private:
	/// associated mutex - locked for the lifetime of this object.
	SBWOSMutex &mutex ;
};


// -----------------------------------------------------------------------------
// SBWThread
// -----------------------------------------------------------------------------

/// @deprecated
typedef void (*SBWOSThread)(void *userData);

/** portable interface to operating system thread functionality.
 * Represents one thread running inside this process.
 */
class SBWThread 
{
public :
	SBW_API SBWThread(std::string name);
	SBW_API virtual ~SBWThread();

	SBW_API void start();

	/// function that is run iside the thread.
	SBW_API virtual void run() = 0 ;

	SBW_API void join();
	SBW_API bool isThisThread();

	SBW_API static unsigned long myThreadId();
	SBW_API static void sleep(Integer milliSec);


private:

#if defined(WIN32)

	SBW_API static DWORD WINAPI threadProc(LPVOID lpParameter);

	/// handle to the windows thread object
	HANDLE thread ;

	/// windows numeric thread identifier
	unsigned long threadId;

#elif defined(HAVE_LIBPTHREAD)

	static void* threadProc(void* lpParameter);

	/// unix thread identifier
	pthread_t threadId;

#endif
	/// thread name
	std::string name ;

	/// has this thread terminated.
	bool joined;
};


// -----------------------------------------------------------------------------
// SBWOSEvent
// -----------------------------------------------------------------------------

/**
 * portable interface to operating system event objects for synchronizing threads.
 */
class SBWOSEvent
{
public:
	SBW_API SBWOSEvent(std::string name);
	SBW_API ~SBWOSEvent();

	SBW_API void wait();
	SBW_API void notify();
	SBW_API std::string getName();

private:
#if defined(WIN32)
	/// windows reference to windows event object
	HANDLE event;
#elif defined(HAVE_LIBPTHREAD)
	/// linux reference to event mutex
	pthread_mutex_t mutex;

	/// linux reference to event
	pthread_cond_t event;

#endif
	/// usage unknown
	bool wakeup;
	/// name of event
	std::string name ;
};

// -----------------------------------------------------------------------------
// Misc. utils.
// -----------------------------------------------------------------------------

#if defined(WIN32) && !defined(CYGWIN)
SBW_API void SBWAssertMessage();
#endif

/**
 * programming facilities avaliable only in the debug library.
 * methods in this class have no effect in the release build
 */
class SBWDebug
{
public:

	// empty inline for release configuration
#ifndef _DEBUG
	SBW_API static void trace(const char *, const char *, int)
	{}
#else
	SBW_API static void trace(const char *x, const char *file, int line);
#endif

	SBW_API static void setTraceMode(bool mode = true);

private:
	static bool traceOn;
	static SBWOSMutex traceMutex ;
};

} // SystemsBiologyWorkbench

#endif // PORTABLE_OS_H
