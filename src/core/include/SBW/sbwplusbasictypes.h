/**
 * @file sbwplusbasictypes.h
 * @brief simple type definitions for C++
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: sbwplusbasictypes.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/sbwplusbasictypes.h,v $
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
#ifndef SBWPLUSBASICTYPES
#define SBWPLUSBASICTYPES

#ifndef __cplusplus
#error can only be used in C++
#endif

#include "sbwdefs.h"

namespace SystemsBiologyWorkbench
{

/// 32 bit signed integer - corresponds to the enum type value IntegerType
typedef int Integer ;

/// 64 bit IEEE floating point number - corresponds to the enum type vale DoubleType
typedef double Double ;

struct SBWComplex 
{
	double dReal;
	double dImag;
};
	
/**
 * callback function called when an event occurs on a given module instance.
 * @param x numeric module instance identifier of module that the event occurred on.
 */
typedef void (*ModuleListener)(Integer x);

/// callback function called when an event occurs in SBW
typedef void (*SimpleListener)();

/**
 * callback function called when an event occurs on a given module instance.
 * @param x numeric module instance identifier of module that the event occurred on.
 */
typedef void (STDCALL *ModuleListenerStdCall)(Integer x);

/// callback function called when an event occurs in SBW
typedef void (STDCALL  *SimpleListenerStdCall)();
}

#endif /* SBWPLUSBASICTYPES */
