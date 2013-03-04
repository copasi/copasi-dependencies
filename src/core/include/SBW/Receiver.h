/**
 * @file Receiver.h
 * @brief  implements all the methods on a module
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: Receiver.h,v 1.2 2006/09/08 06:03:18 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/Receiver.h,v $
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
*/// Receiver.h: interface for the Receiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECEIVER_H__B44E478C_4471_4D1A_98F6_81CC061EDF61__INCLUDED_)
#define AFX_RECEIVER_H__B44E478C_4471_4D1A_98F6_81CC061EDF61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataBlockWriterClass.h"
#include "DataBlockReaderClass.h"

namespace SystemsBiologyWorkbench
{

/**
 * abstract base class of objects that process incoming remote method calls
 */
class SBW_API Receiver  
{
public:
	/**
	 * process incoming call.
     * @param fromModuleId the numeric module instance identifier of the module instance that made the call.
	 * @param service the numeric service identifier of the called service.
	 * @param method the numeric method identifier of the method called.
	 * @param args data block containing the argument data for the call.
	 * @return data block containing the result data from the call.
	 */
	virtual DataBlockWriter receive(
		Integer fromModuleId, Integer service, Integer method, DataBlockReader args) = 0 ;

	/**
	 * returns whether this object can and should be deleted by the library using the delete operator
	 * @return whether this object can and should be deleted by the library using the delete operator
	 */
	virtual bool canDelete() { return true; }
	virtual ~Receiver() {}
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_RECEIVER_H__B44E478C_4471_4D1A_98F6_81CC061EDF61__INCLUDED_)
