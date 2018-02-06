/**
 * @file Module.cpp
 * @brief implementation of Module class which represents a module seperate from the module implemented by this application
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
#include "Module.h"

#include "SBWLowLevel.h"
#include "ServiceInline.h"

using namespace SystemsBiologyWorkbench ;

/**
 * returns the services that are implemented by this module.
 * the result should deallocated using delete.
 * @return the services that are implemented by this module.
 */
bool Module::operator==(const Module &other) const
{
    return moduleId == other.moduleId;
}

bool Module::operator!=(const Module &other) const
{
    return moduleId != other.moduleId;
}

Integer Module::getModuleInstanceId() { return moduleId ; }

std::vector<Service> *Module::getServices()
{
    Integer n = SBWLowLevel::moduleGetNumberOfServices(moduleId);
    std::vector<Service> *services = new std::vector<Service>(n);

    while (n != 0)
    {
        n-- ;
        (*services)[n].serviceId = n;
        (*services)[n].moduleId = moduleId;
    }

    return services ;
}

/**
 * returns the services that are implemented by this module that are in a given category.
 * the result should deallocated using delete.
 * @param category the category that the returned services must be in.
 * @return the services that are implemented by this module that are in category.
 */
std::vector<Service> *Module::findServicesByCategory(std::string category)
{
    std::vector<Service> *services = getServices();
    std::vector<Service> *result = new std::vector<Service>();

    try
    {
        unsigned int i = 0 ;

        while (i != services->size())
        {
            ServiceDescriptor descriptor ;

            (*services)[i].getDescriptor(descriptor);

            if (SBWLowLevel::isSubCategory(category, descriptor.getCategory()))
                result->push_back((*services)[i]);

            i++ ;
        }
    }
    catch (SBWException *)
    {
        delete result ;
        throw ;
    }

    return result ;
}

Module::Module(Integer id) : moduleId(id) {}
