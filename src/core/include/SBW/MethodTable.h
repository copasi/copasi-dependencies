/**
 * @file MethodTable.h
 * @brief  object that stores method pointers into a given service implementation
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
#if !defined(AFX_SERVICEREGISTER_H__EE7AC432_E4CE_4735_B5E5_6A2EC8A090F3__INCLUDED_)
#define AFX_SERVICEREGISTER_H__EE7AC432_E4CE_4735_B5E5_6A2EC8A090F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <SBW/DataBlockWriterClass.h>
#include <SBW/DataBlockReaderClass.h>
#include <SBW/Module.h>
#include <SBW/Handler.h>
#include <SBW/ModuleImpl.h>

namespace SystemsBiologyWorkbench
{

/**
 * Stores the methods of <code>class T</code> into a module
 * implementation (ModuleImpl) object to form the methods of a SBW service.
 */
template <class T> class SBW_API MethodTable
{
public:
    /**
     * creates a MethodTable object for a given service
     * @param mi module implementation object that the service is being added to
     * @param s object implementing the service
     * @param name the SBW unique name of the service
     */
    MethodTable(ModuleImpl &mi, T *s, std::string name) : moduleImpl(mi), serviceName(name), service(s) {}

    /// the function type for SBW methods on the object implementing the service
    typedef DataBlockWriter (T:: * Method) (Module from, DataBlockReader reader);

    /// a Handler (SBW method implementation) for a method on <code>class T</code>
    class TemplateHandler : public Handler
    {
    public:
        /**
         * creates a Handler for a method on <code>class T</code>
         * @param s object implementing the service
         * @param m SBW method on <code>s</code>
         */
        TemplateHandler(T *s, Method m): service(s), method(m) {}

        /**
         * process an incoming method call for this particular method.
         * @param from module which called this method
         * @param reader data block containing the argument data for the method call.
         * @return datablock containing the results of the method call.
         */
        virtual DataBlockWriter receive(Module from, DataBlockReader reader)
        {
            return (service->*method)(from, reader);
        }

    private:
        /// object implementing the service
        T *service;

        /// SBW method on service
        Method method;
    };

    /**
     * adds a given method for <code>class T</code> to a module implementation
     * @param method to be added to the module implementation
     * @param signature SBW signature of the method
     * @param synchronized indicates whether this method should be synchronized - default false.
     * @param help help string for this method - empty by default
     */
    void addMethod(Method method, std::string signature, bool synchronized = false, std::string help = "")
    {
        moduleImpl.setHandler(serviceName, new TemplateHandler(service, method), signature, synchronized, help);
    }

  const std::string& getServiceName() const;

private:
    /// the module implementation object that service is being added to
    ModuleImpl &moduleImpl;

    /// the SBW unique service identifier for service
    std::string serviceName;

    /// the object that is implementing the service
    T *service;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SERVICEREGISTER_H__EE7AC432_E4CE_4735_B5E5_6A2EC8A090F3__INCLUDED_)
