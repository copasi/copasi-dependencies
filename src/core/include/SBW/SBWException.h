/**
 * @file SBWException.h
 * @brief abstract base class of all SBW exceptions
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWException.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWException.h,v $
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

/// MSVC generated include loop block
#if !defined(AFX_SBWEXCEPTION_H__358F305F_1B7B_4D2D_AC38_16E43A66DDC4__INCLUDED_)
#define AFX_SBWEXCEPTION_H__358F305F_1B7B_4D2D_AC38_16E43A66DDC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sbwplusbasictypes.h"
#include "portableOS.h"
#include "sbwenums.h"
#include <map>
#include <exception>

namespace SystemsBiologyWorkbench
{

/// abstract base class of all exceptions thrown by the C++ API
	class SBWException 
{
public:
	SBW_API SBWException(const char *c) ;
	SBW_API SBWException(const char *c, const char *d) ;
	SBW_API SBWException(std::string) ;
	SBW_API SBWException(std::string a, std::string b) ;

	SBW_API virtual ~SBWException() ;

	/**
	 * return the user message associated with this exception.
     * @return the user message associated with this exception.
	 */
	std::string getMessage() { return m_szMessage ? m_szMessage : ""; }
	/**
	 * return the detailed developer message associated with this exception.
     * @return the detailed developer message associated with this exception.
	 */
	std::string getDetailedMessage() {return m_szDetailedMessage ? m_szDetailedMessage : "" ; }

	SBW_API void log();
	SBW_API static void log(const char *c);
	SBW_API static void log(const char *c, const char *d);

	/**
	 * output to standard output the given string.
	 * @param sc string to be output.
	 */
	SBW_API static void log(std::string sc) { log(sc.c_str()); }

	/**
	 * output to standard output the 2 given strings.
	 * @param sc first string to be output
	 * @param sd 2nd string to be output
	 */
	SBW_API static void log(std::string sc, std::string sd) { log(sc.c_str(), sd.c_str()); }

/*
	virtual unsigned char getCode() { return -1 ; } ;
*/
	SBW_API virtual unsigned char getCode() = 0;

	SBW_API void *operator new(size_t size);
	SBW_API void operator delete(void *);

	SBW_API void storeException();
	SBW_API static SBWException *getStoredException();
	SBW_API static void clearStoredException();
	SBW_API static void throwStoredException();
	SBW_API static void clearAllExceptions(); 

private :
	/// user message
	const char *m_szMessage ;

	/// detailed developer message
	const char *m_szDetailedMessage ;

	static std::map<long, SBWException *> exceptionMap ;
	static SBWOSMutex threadStoreMutex;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SBWEXCEPTION_H__358F305F_1B7B_4D2D_AC38_16E43A66DDC4__INCLUDED_)
