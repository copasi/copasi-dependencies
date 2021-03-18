/**
* Begin svn Header
* $Rev: 27 $:	Revision of last commit
* $Author: josephodada@gmail.com $:	Author of last commit
* $Date: 2013-04-25 10:07:15 +0100 (Thu, 25 Apr 2013) $:	Date of last commit
* $HeadURL: https://numl.googlecode.com/svn/trunk/libnuml/src/common/extern.h $
* $Id: extern.h 27 2013-04-25 09:07:15Z josephodada@gmail.com $
* End svn Header
* ***************************************************************************
* This file is part of libNUML.  Please visit http://code.google.com/p/numl/for more
* information about NUML, and the latest version of libNUML.
* Copyright (c) 2013 The University of Manchester.
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation.  A copy of the license agreement is
* provided in the file named "LICENSE.txt" included with this software
* distribution and also available online as http://www.gnu.org/licenses/lgpl.html
*
* The original code was initially developed by:
*
* 	SBML Team
* 	Control and Dynamical Systems, MC 107-81
* 	California Institute of Technology
* 	Pasadena, CA, 91125, USA
*
*  	http://www.sbml.org
* 	mailto:sbml-team@caltech.edu
*
* Adapted for NUML by:
*
* 	NUML Team
* 	Computational Systems Biology Group
* 	School of Computer Science
* 	The University of Manchester
* 	Manchester, M1 7DN UK
* 	mailto:josephodada@gmail.com
*
*   Contributor(s):
*   Joseph O. Dada, The University of Manchester - initial LIBNUML API and implementation
* ***************************************************************************
**/


#ifndef LIBNUML_EXTERN_H
#define LIBNUML_EXTERN_H

#include <numl/common/libnuml-namespace.h>

#if ( defined WIN32 && ! defined CYGWIN )

#if ( ! defined LIBNUML_STATIC )
/**
 * The following ifdef block is the standard way of creating macros which
 * make exporting from a DLL simpler. All files within this DLL are
 * compiled with the LIBNUML_EXPORTS symbol defined on the command line.
 * This symbol should not be defined on any project that uses this
 * DLL. This way any other project whose source files include this file see
 * LIBNUML_EXTERN functions as being imported from a DLL, wheras this DLL
 * sees symbols defined with this macro as being exported.
 *
 * (From Andrew Finney's sbwdefs.h, with "SBW" replaced by "LIBNUML" :)
 */
#if LIBNUML_EXPORTS
#  define LIBNUML_EXTERN __declspec(dllexport)
#else
#  define LIBNUML_EXTERN __declspec(dllimport)
#endif

#else
#  define LIBNUML_EXTERN
#endif  /* LIBNUML_STATIC */

/**
 * Disable MSVC++ warning C4800: 'const int' : forcing value to bool 'true'
 * or 'false' (performance warning).
 */
#pragma warning(disable: 4800)

/**
 * Disable MSVC++ warning C4291: no matching operator delete found.
 */
#pragma warning(disable: 4291)

/**
 * Disable MSVC++ warning C4251: class 'type' needs to have dll-interface
 * to be used by clients of class 'type2'.
 *
 * For an explanation of why this is safe, see:
 *   - http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
 */
#pragma warning(disable: 4251)

#else

/**
 * LIBNUML_EXTERN is used under Windows to simplify exporting functions
 * from a DLL.  When compiling under Windows, all files within this DLL are
 * compiled with the LIBNUML_EXPORTS symbol defined on the command line.
 * This in turn causes extern.h to define a different version of
 * LIBNUML_EXTERN that is appropriate for exporting functions to client
 * code that uses the DLL.
 */
#define LIBNUML_EXTERN

#endif  /* WIN32 */


#if !defined(BEGIN_C_DECLS) && !defined(END_C_DECLS)

#if __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif

#endif


#endif  /** LIBNUML_EXTERN_H **/
