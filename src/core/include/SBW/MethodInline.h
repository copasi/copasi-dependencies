/**
 * @file MethodInline.h
 * @brief  interface for inline ServiceMethod
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: MethodInline.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/MethodInline.h,v $
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
#ifndef METHODINLINE
#define METHODINLINE

#include "SBWRPC.h"
#include "Signature.h"
#include "SBWLowLevel.h"
#include "Method.h"

/// creates a potential reference to an SBW Method.  This value doesn't refer to any known method.
inline SystemsBiologyWorkbench::ServiceMethod::ServiceMethod()
: moduleId(SBWErrorModule), serviceId(SBWErrorService), methodId(SBWErrorMethod)
{
}

/**
 * creates a reference to a given SBW method.
 * @param mid numeric module instance identifier.
 * @param sid numeric service identifier
 * @param methid numeric methid identifier
 */
inline SystemsBiologyWorkbench::ServiceMethod::ServiceMethod(
	SystemsBiologyWorkbench::Integer mid, SystemsBiologyWorkbench::Integer sid, SystemsBiologyWorkbench::Integer methid)
: moduleId(mid), serviceId(sid), methodId(methid)
{
}

/**
 * returns the name of the method.
 * @returns the name of the method
 */
inline std::string SystemsBiologyWorkbench::ServiceMethod::getName()
{
	return getSignature().getName();
}

/**
 * returns the help string for the method.
 * @returns the help string for the method
 */
inline std::string SystemsBiologyWorkbench::ServiceMethod::getHelp()
{
	return SBWLowLevel::methodGetHelpInString(moduleId, serviceId, methodId);
}

/**
 * returns the signature string of the method.
 * @returns the signature string of the method
 */
inline std::string SystemsBiologyWorkbench::ServiceMethod::getSignatureString()
{
	return SBWLowLevel::methodGetSignatureInString(moduleId, serviceId, methodId);
}

/**
 * returns the signature structure of the method.
 * @returns the signature structure of the method
 */
inline SystemsBiologyWorkbench::Signature SystemsBiologyWorkbench::ServiceMethod::getSignature()
{
	return SBWLowLevel::methodGetSignature(moduleId, serviceId, methodId);
}

/**
 * call the method blocking until the method completes.
 * @param args the datablock containing the arguments to the call.
 * @return the datablock containing the result of the method call.
 */
inline SystemsBiologyWorkbench::DataBlockReader SystemsBiologyWorkbench::ServiceMethod::call(
			SystemsBiologyWorkbench::DataBlockWriter args)
{
	return SBWLowLevel::methodCall(moduleId, serviceId, methodId, args);
}

/**
 * call the method returning as soon as the transmit message has been transmitted from this application.
 * @param args the datablock containing the arguments to the call.
 */
inline void SystemsBiologyWorkbench::ServiceMethod::send(DataBlockWriter args)
{
	SBWLowLevel::methodSend(moduleId, serviceId, methodId, args);
}
#endif // METHODINLINE
