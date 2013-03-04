/**
 * @file MethodImpl.cpp
 * @brief contains a method implementation together with its signature string.
 * ServiceMethod class represents a method on a service in a potentially different module. 
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: MethodImpl.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/MethodImpl.cpp,v $
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

// MethodImpl.cpp: implementation of the MethodImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MethodImpl.h"
#include "Handler.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/// deallocates the method handler object
MethodImpl::~MethodImpl()
{
	delete handler ;
}

/**
 * constructs a service method object's implementation
 * @param h the handler object that implements the method functionality
 * @param s the method's SBW signature
 * @param m the mutex for all sychronized methods - null if this method is not synchronized - (null by default)
 * @param hp the help string for the method - (empty by default)
 */
MethodImpl::MethodImpl(Handler *h, std::string s, SBWOSMutex *m, std::string hp)
: handler(h), signature(s), mutex(m), help(hp)
{ 
	// check syntax
	Signature sig(s);
}

/** 
 * invokes/executes this method.
 * @param from the module that invoked this method.
 * @param reader the argument data for this invocation.
 * @return a DataBlockWriter containing the result data from this method invocation.
 */
DataBlockWriter MethodImpl::invoke(Module from, DataBlockReader reader)
{
	if (isSynchronised())
	{
		SBWOSMutexLock ml(*mutex);

		return handler->receive(from, reader);
	}
	else
		return handler->receive(from, reader);
}

/**
 * returns whether this method is synchronized.
 * @return whether this method is synchronized.
 */
bool MethodImpl::isSynchronised()
{
	return mutex != NULL ;
}

/**
 * returns the signature of this method.
 * @return the signature of this method.
 */
std::string MethodImpl::getSignature()
{
	return signature ;
}

/**
 * returns the handler object that implements this method.
 * @return the handler object that implements this method.
 */
Handler *MethodImpl::getHandler()
{
	return handler ;
}
