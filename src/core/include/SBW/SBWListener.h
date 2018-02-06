/**
 * @file SBWListener.h
 * @brief class definition of SBWListener - callback interface for monitoring events occuring in SBW.
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
#if !defined(AFX_SBWLISTENER_H__65AD3A17_B880_4E7B_8898_390E08A358FD__INCLUDED_)
#define AFX_SBWLISTENER_H__65AD3A17_B880_4E7B_8898_390E08A358FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SBW/Module.h>
#include <SBW/SBWRPCListener.h>

namespace SystemsBiologyWorkbench
{

/// callback interface for monitoring events occuring in SBW
class SBW_API SBWListener  : public SBWRPCListener
{
public:

	/// does nothing
	SBWListener();

	/// does nothing
	virtual ~SBWListener();

	/**
	 * Called when a given module disconnects from the broker.
	 * This implementation does nothing.
	 * @param x ignored; Module instance that has disconnected from the broker.
	 */
	virtual void onModuleShutdown(Module /*x*/);

	/**
	 * Called when a given module connects from the broker.
	 * This implementation does nothing.
	 * @param x ignored; Module instance that has connected from the broker.
	 */
	virtual void onModuleStart(Module);

	/// called when the broker's module registration data changes.  This implementation does nothing.
	virtual void onRegistrationChange();

	/// called when this module/application is disconnected from the broker. 
	virtual void onShutdown();

	/**
	 * Called when a given module disconnects from the broker.
	 * This implementation calls onModuleShutdown(Module x).
	 * @see onModuleShutdown(Module x)
	 * @param x ignored; numeric module instance identifier of module instance that has disconnected from the broker.
	 */	
	void onModuleShutdown(Integer x);

	/**
	 * Called when a given module connects from the broker.
	 * This implementation calls onModuleStart(Module x).
	 * @see onModuleStart(Module x)
	 * @param x ignored; numeric module instance identifier of module instance that has connected from the broker.
	 */	
	void onModuleStart(Integer x);
};

} // SystemsBiologyWorkbench
 
#endif // !defined(AFX_SBWLISTENER_H__65AD3A17_B880_4E7B_8898_390E08A358FD__INCLUDED_)
