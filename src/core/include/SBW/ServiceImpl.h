/**
 * @file ServiceImpl.h
 * @brief implementation of a service 
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
#if !defined(AFX_SERVICEIMPL_H__94F87B9F_86C2_44A1_B780_EBF139CF3E05__INCLUDED_)
#define AFX_SERVICEIMPL_H__94F87B9F_86C2_44A1_B780_EBF139CF3E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/sbwdefs.h>
#ifndef SBW_STRICT_INCLUDES
#include <SBW/portableOS.h>
#endif
#include <SBW/DataBlockWriter.h>

namespace SystemsBiologyWorkbench
{

class MethodImpl ;
class Handler;

/// service implementation
class ServiceImpl  
{
public:
	SBW_API ServiceImpl(std::string name, std::string displayName, std::string category, std::string help);
	virtual ~ServiceImpl();

	SBW_API void addMethod(Handler *handler, std::string signature, bool synchronized = false, std::string help = "");
	SBW_API Handler *getHandler(std::string signature);

	SBW_API MethodImpl *getMethod(Integer methodId);
	SBW_API std::string getName();
	SBW_API void addSignatures(DataBlockWriter);

	SBW_API std::vector<std::string> getSignatureStrings();

	/**
	 * returns the humanly readable name of the service.
	 * @return the humanly readable name of the service 
	 */
	SBW_API std::string getDisplayName() { return displayName ; }
	
	/**
	 * returns the categroy of the service.
	 * @return the categroy of the service
	 */
	SBW_API std::string getCategory() { return category; }

	/**
	 * returns the help string for the service
	 * @return the help string for the service
	 */
	SBW_API std::string getHelp() { return help; }

private:
	/// mutex that synchronizes the synchronized methods on this service
	SBWOSMutex mutex ;

	/// service identification name of this service
	std::string uniqueName ;

	/// humanly readable name of the service
	std::string displayName ;

	/// the category of the service
	std::string category ;

	/// the help string for the service
	std::string help ;

	/// the method implementations with their signatures
	std::vector<MethodImpl *> methods ;
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_SERVICEIMPL_H__94F87B9F_86C2_44A1_B780_EBF139CF3E05__INCLUDED_)
