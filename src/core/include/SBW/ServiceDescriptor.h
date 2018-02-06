/**
 * @file ServiceDescriptor.h
 * @brief access data on one service - taken from the broker's module registry.
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


/// MSVC generated include loop block
#if !defined(AFX_SERVICEDESCRIPTOR_H__0A7C51A4_8BF5_43DA_BAC1_87D22F4B1AF9__INCLUDED_)
#define AFX_SERVICEDESCRIPTOR_H__0A7C51A4_8BF5_43DA_BAC1_87D22F4B1AF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/Service.h>
#include <SBW/ModuleDescriptor.h>

namespace SystemsBiologyWorkbench
{

/**
 * underlying implementation of ServiceDescriptor.
 * contains data on one service - taken from the broker's module registry
 */
class SBW_API ImplServiceDescriptor : public SBWObject
{
public:
    ImplServiceDescriptor(ServiceDescriptor sd);
    ImplServiceDescriptor(DataBlockReader);
    ImplServiceDescriptor(DataBlockReader r, ModuleDescriptor md);

    /**
     * returns the identification name of the service.
     * @return the identification name of the service.
     */
    std::string getName() const;

    /**
     * returns the humanly readable name of the service
     * @return the humanly readable name of the service
     */
    std::string getDisplayName() const;

    /**
     * returns the category of the service.
     * @return the category of the service.
     */
    std::string getCategory() const;

    /**
     * returns the ModuleDescriptor of the module that implements this service.
     * @return the ModuleDescriptor of the module that implements this service.
     */
    ModuleDescriptor getModuleDescriptor() const;

    /**
     * returns the help string for this service
     * @return the help string for this service
     */
    std::string getHelp() const;

private:
    /// the identification name of the service
    std::string name ;

    /// the humanly readable name of the service
    std::string displayName ;

    /// the category of the service
    std::string category;

    /// the help string for this service
    std::string help ;

    /// the ModuleDescriptor of the module that implements this service
    ModuleDescriptor module ;
};

/// contains data on one service - taken from the broker's module registry
class SBW_API ServiceDescriptor : public SBWHandle<ImplServiceDescriptor>
{
public:
    /// create a service descriptor containing no data
    ServiceDescriptor();

    Service getServiceInModuleInstance();

    /**
     * create a ServiceDescriptor object using data extracted from a data block.
     * The cursor of the DataBlockReader is move to the first object after the descriptor data.
     * @param r data block containing service descriptor data
     */
    ServiceDescriptor(DataBlockReader r);
    ServiceDescriptor(ServiceDescriptor *sd);
    ServiceDescriptor(DataBlockReader r, ModuleDescriptor md);

    /**
     * returns the identification name of the service.
     * @return the identification name of the service.
     */
    std::string getName() const;

    /**
     * returns the humanly readable name of the service
     * @return the humanly readable name of the service
     */
    std::string getDisplayName() const;

    /**
     * returns the category of the service.
     * @return the category of the service.
     */
    std::string getCategory() const;

    /**
     * returns the ModuleDescriptor of the module that implements this service.
     * @return the ModuleDescriptor of the module that implements this service.
     */
    ModuleDescriptor getModuleDescriptor();

    /**
     * returns the help string for this service
     * @return the help string for this service
     */
    std::string getHelp() const;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SERVICEDESCRIPTOR_H__0A7C51A4_8BF5_43DA_BAC1_87D22F4B1AF9__INCLUDED_)
