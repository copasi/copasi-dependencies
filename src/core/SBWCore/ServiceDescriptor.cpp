/**
 * @file ServiceDescriptor.cpp
 * @brief represents a service on a potentially different module
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


#include "stdafx.h"
#include <SBW/ServiceDescriptor.h>
#include <SBW/SBWLowLevel.h>
#include <SBW/SBW.h>
#include <SBW/SBWHighLevel.h>

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * creates a service descriptor from data contained in the given data block.
 * @param reader contains data for the service descriptor.
 */
ImplServiceDescriptor::ImplServiceDescriptor(ServiceDescriptor sd)
{
    name = sd.getName();
    displayName = sd.getDisplayName();
    category = sd.getCategory();
    help = sd.getHelp();

    module = sd.getModuleDescriptor();
}

ImplServiceDescriptor::ImplServiceDescriptor(DataBlockReader reader)
{
    std::string moduleName ;

    reader >> moduleName >> name >> displayName >> category >> help ;

    module = SBWLowLevel::getModuleDescriptor(moduleName, true);
}
ImplServiceDescriptor::ImplServiceDescriptor(DataBlockReader r, ModuleDescriptor md)
{
    std::string moduleName ;

    r >> moduleName >> name >> displayName >> category >> help ;

    module = md;
}

std::string ImplServiceDescriptor::getName() const { return name ; }

std::string ImplServiceDescriptor::getDisplayName() const { return displayName; }

std::string ImplServiceDescriptor::getCategory() const { return category; }

ModuleDescriptor ImplServiceDescriptor::getModuleDescriptor() const { return module; }

std::string ImplServiceDescriptor::getHelp() const { return help ; }
//////////////////////////////////////////////////////////////////////

/**
 * returns the service described by the service descriptor.
 * Locates a module instance that implements the given service.
 * May or may not create a new module instance to implement the service depending on the module management type of the
 * implementing module.
 * @return the service described by the service descriptor.
 */
ServiceDescriptor::ServiceDescriptor() {}

Service ServiceDescriptor::getServiceInModuleInstance()
{
    return SBW::getModuleInstance(
                getModuleDescriptor().getName()).findServiceByName(getName());
}

ServiceDescriptor::ServiceDescriptor(DataBlockReader r)
  : SBWHandle<ImplServiceDescriptor>(new ImplServiceDescriptor(r))
{
}

ServiceDescriptor::ServiceDescriptor(ServiceDescriptor* sd)
  : SBWHandle<ImplServiceDescriptor>(new ImplServiceDescriptor(*sd))
{
}

SystemsBiologyWorkbench::ServiceDescriptor::ServiceDescriptor(SystemsBiologyWorkbench::DataBlockReader r, SystemsBiologyWorkbench::ModuleDescriptor md)
  : SBWHandle<ImplServiceDescriptor>(new ImplServiceDescriptor(r,md))
{
}

std::string ServiceDescriptor::getName() const { return p->getName() ; }

std::string ServiceDescriptor::getDisplayName() const { return p->getDisplayName(); }

std::string ServiceDescriptor::getCategory() const { return p->getCategory(); }

ModuleDescriptor ServiceDescriptor::getModuleDescriptor() { return p->getModuleDescriptor(); }

std::string ServiceDescriptor::getHelp() const { return p->getHelp(); }
