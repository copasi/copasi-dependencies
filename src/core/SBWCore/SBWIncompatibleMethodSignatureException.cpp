/**
 * @file SBWIncompatibleMethodSignatureException.cpp
 * @brief exception thrown when a given signature doesn't match the signature supplied by the service provider
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWIncompatibleMethodSignatureException.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SBWIncompatibleMethodSignatureException.cpp,v $
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

static const char rcsid[] = "$Id: SBWIncompatibleMethodSignatureException.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $";

// SBWIncompatibleMethodSignatureException.cpp: implementation of the SBWIncompatibleMethodSignatureException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SBWIncompatibleMethodSignatureException.h"

using namespace SystemsBiologyWorkbench ;

/**
 * returns IncompatibleMethodSignatureExceptionCode
 * @return IncompatibleMethodSignatureExceptionCode
 */
SBW_API unsigned char SBWIncompatibleMethodSignatureException::encoding() { return code; } ;

/**
 * returns IncompatibleMethodSignatureExceptionCode
 * @return IncompatibleMethodSignatureExceptionCode
 */
SBW_API unsigned char SBWIncompatibleMethodSignatureException::getCode() { return code; } ;

/**
 * the exception code for SBWIncompatibleMethodSignatureException
 */
const unsigned char SBWIncompatibleMethodSignatureException::code = IncompatibleMethodSignatureExceptionCode ;
