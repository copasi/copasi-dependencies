/**
 * @file sbwdefs.h
 * @brief simple type definitions that are common to both C and C++
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: sbwdefs.h,v 1.7 2009/04/05 21:12:02 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/sbwdefs.h,v $
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
#ifndef SBWDEFS
#define SBWDEFS

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

#if defined (LINUX) || defined(CYGWIN) || defined(MINGW) || defined(NO_API)
#define SBW_API
#else

// The following ifdef block is the standard way of creating macros which
// make exporting from a DLL simpler. All files within this DLL are
// compiled with the SBW_EXPORTS symbol defined on the command line. this
// symbol should not be defined on any project that uses this DLL. This way
// any other project whose source files include this file see SBW_API
// functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef SBW_EXPORTS
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

#endif /* SBWDEFS */
