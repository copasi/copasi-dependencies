/**
 * @file DoNothingReceiver.cpp
 * @brief a receiver (handles all messages for a module) that does bugger all
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: DoNothingReceiver.cpp,v 1.2 2006/09/08 06:04:14 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/DoNothingReceiver.cpp,v $
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

static const char rcsid[] = "$Id: DoNothingReceiver.cpp,v 1.2 2006/09/08 06:04:14 fbergmann Exp $";

// DoNothingReceiver.cpp: implementation of the DoNothingReceiver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoNothingReceiver.h"

#include "SBWRPC.h"
#include "SBWMethodNotFoundException.h"
#include "SBWServiceNotFoundException.h"
#include "ImplDataBlockWriter.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/** 
 * process a call/send message that require implementation on this module where the module actually provides no services.
 * @throws SBWException * if unsupported services or methods are called.
 * @param fromModuleId module that sent the message.
 * @param service requested service - only SBWSystemService supported.
 * @param method requested method - only SBWGetMethodsMethod and SBWGetServicesMethod supported.
 * @param args method argument data - ignored.
 * @return empty data block.
 */
DataBlockWriter DoNothingReceiver::receive(
		Integer /*fromModuleId*/, Integer service, Integer method, DataBlockReader /*args*/)
{
	DataBlockWriter result ;

	switch (service)
	{
		case SBWSystemService :
			switch (method)
			{
				case SBWGetMethodsMethod :
				case SBWGetServicesMethod :
					{
						const char *empty[] = {""} ;
						result.add(0, empty);

						return result ;
					}
				
				default :
					throw new SBWMethodNotFoundException("method doesn't exist");
			}

		default :
			throw new SBWServiceNotFoundException("service doesn't exist");
	}
}
