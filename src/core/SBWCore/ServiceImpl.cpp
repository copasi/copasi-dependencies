/**
 * @file ServiceImpl.cpp
 * @brief the implementation of a service in this module
 *
 * This file is part of SBW.  Please visit http://sbw.sf.org for more
 * information about SBW, and the latest version of libSBW.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the New BSD license.
 *
 * Copyright (c) 2010-2014, Frank T. Bergmann and
 *                          University of Washington
 * Copyright (c) 2008-2010, University of Washington and
 *                          Keck Graduate Institute.
 * Copyright (c) 2005-2008, Keck Graduate Institute.
 * Copyright (c) 2001-2004, California Institute of Technology and
 *               Japan Science and Technology Corporation.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The original code contained here was initially developed by:
 *
 *     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri
 *     The Systems Biology Workbench Development Group
 *     ERATO Kitano Systems Biology Project
 *     Control and Dynamical Systems, MC 107-81
 *     California Institute of Technology
 *     Pasadena, CA, 91125, USA
 *
 *
 * Contributor(s):
 *
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

std::string ServiceImpl::getDisplayName() const { return displayName ; }

std::string ServiceImpl::getCategory() const { return category; }

std::string ServiceImpl::getHelp() const { return help; }
