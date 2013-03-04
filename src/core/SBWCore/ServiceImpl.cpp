/**
 * @file ServiceImpl.cpp
 * @brief the implementation of a service in this module
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: ServiceImpl.cpp,v 1.3 2007/03/11 05:38:45 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/ServiceImpl.cpp,v $
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
// ServiceImpl.cpp: implementation of the ServiceImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServiceImpl.h"

#include "DataBlockWriter.h"
#include "MethodImpl.h"
#include "SBWMethodNotFoundException.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * construct a service implementation object from the components of a service specification.
 * @param n identification name of service
 * @param dn humanly readable name of service
 * @param c categroy of service
 * @param h help string for service
 */
ServiceImpl::ServiceImpl(std::string n, std::string dn, std::string c, std::string h)
: mutex("service", n), uniqueName(n), displayName(dn), category(c), help(h) 
{

}

/**
 * deallocate method objects associated with this service implementation
 */
ServiceImpl::~ServiceImpl()
{
	unsigned int i = 0 ;

	while (i != methods.size())
	{
		delete methods[i];
		i++;
	}
	
}

/**
 * add an SBW method to this service.
 * @param handler method implementation.
 * @param signature SBW method signature.
 * @param synchronized indicates whether this method is synchroized
 * @param help method help string
 */
void ServiceImpl::addMethod(Handler *handler, std::string signature, bool synchronized, std::string help)
{
	methods.push_back(new MethodImpl(handler, signature, synchronized ? &mutex : NULL, help));
}

/**
 * return the method implementation for a given method on this service.
 * @param signature the given SBW method signature of the returned method implementation.
 * @return the method implementation for the method with the given signature on this service.
 */
Handler *ServiceImpl::getHandler(std::string signature)
{
	unsigned int i = 0 ;
	std::vector<std::string> sigs;

	sigs.resize(methods.size());
	while (i != methods.size())
	{
		sigs[i] = methods[i]->getSignature();
		i++;
	}

	return methods[sbwSignature::match(sigs, signature.c_str())]->getHandler();
}

/**
 * return the method object for given a numeric method identifier
 * @param methodId numeric method id.
 * @return the method object for the given numeric method identifier.
 */
MethodImpl *ServiceImpl::getMethod(Integer methodId)
{
	if (methodId >= 0 && methodId <  (int)methods.size())
		return methods[methodId];
	else
		throw new SBWMethodNotFoundException("method not found");
}

/**
 * return the service identification string for this service
 * @return the service identification string for this service
 */
std::string ServiceImpl::getName()
{
	return uniqueName ;
}

/**
 * append the signatures of all the methods implemented by this service, onto a given data block.
 * The signatures are appended as an array of strings.
 * @param writer the data block to be extended.
 */
void ServiceImpl::addSignatures(DataBlockWriter writer)
{
	std::vector<std::string> signatures(methods.size());
	unsigned int i = 0 ;

	while (i != methods.size())
	{
		signatures[i] = methods[i]->getSignature();
		i++;
	}

	writer << signatures ;
}
std::vector<std::string> ServiceImpl::getSignatureStrings()
{
	std::vector<std::string> signatures(methods.size());
	unsigned int i = 0 ;

	while (i != methods.size())
	{
		signatures[i] = methods[i]->getSignature();
		i++;
	}
	return signatures;
}
