/**
 * @file MethodTable.h
 * @brief  object that stores method pointers into a given service implementation
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 5th November 2002
 * $Id: MethodTable.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/MethodTable.h,v $
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

/// MSVC generated include loop block
#if !defined(AFX_SERVICEREGISTER_H__EE7AC432_E4CE_4735_B5E5_6A2EC8A090F3__INCLUDED_)
#define AFX_SERVICEREGISTER_H__EE7AC432_E4CE_4735_B5E5_6A2EC8A090F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataBlockWriterClass.h"
#include "DataBlockReaderClass.h"
#include "Module.h"
#include "Handler.h"
#include "ModuleImpl.h"

namespace SystemsBiologyWorkbench
{

/**
 * Stores the methods of <code>class T</code> into a module
 * implementation (ModuleImpl) object to form the methods of a SBW service.
 */
template <class T> class MethodTable  
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
		TemplateHandler(T *s, Method m): service(s), method(m) {};

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
