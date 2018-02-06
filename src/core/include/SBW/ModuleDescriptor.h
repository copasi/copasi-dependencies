/**
 * @file ModuleDescriptor.h
 * @brief  interface for the ModuleDescriptor class.
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
#if !defined(AFX_MODULEDESCRIPTOR_H__5464A475_237B_438E_AC35_D25B3062A5B5__INCLUDED_)
#define AFX_MODULEDESCRIPTOR_H__5464A475_237B_438E_AC35_D25B3062A5B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/ImplModuleDescriptor.h>

namespace SystemsBiologyWorkbench
{

/// data on a module returned from the broker
class SBW_API ModuleDescriptor : public SBWHandle<ImplModuleDescriptor>
{
public:
    /// create an empty module descriptor
    ModuleDescriptor();

    /** create a module descriptor from the data in the given data block reader.
     * The data is assumed to be at the cursor in the data block and the cursor is moved
     * to the first object after the module descriptor.
     * @param r data block containing module descriptor data.
     */
    ModuleDescriptor(DataBlockReader r);

    /**
     * returns the module identification string/name
     * @return the module identification string/name
     */
    std::string getName() const;

    /**
     * returns the humanly readable name for the module
     * @return the humanly readable name for the module
     */
    std::string getDisplayName() const;

    /**
     * returns the command line that is used to create an instance of the module
     * @return the command line that is used to create an instance of the module
     */
    std::string getCommandLine() const;

    /**
     * returns the mechanism used to determine how module instances are started and disconnected
     * @return the mechanism used to determine how module instances are started and disconnected
     */
    ModuleManagementType getManagementType() const;

    /**
     * returns the help string for the module
     * @return the help string for the module
     */
    std::string getHelp() const;

    /**
     * returns the service descriptors for the services registered with this module
     * @return the service descriptors for the services registered with this module
     */
    std::vector<ServiceDescriptor> *getServiceDescriptors();

    //! added functionality for use in broker
    std::vector<ServiceDescriptor*> getServices();
    void addService(ServiceDescriptor* sd);

private:

    std::vector<ServiceDescriptor*> _services;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_MODULEDESCRIPTOR_H__5464A475_237B_438E_AC35_D25B3062A5B5__INCLUDED_)
