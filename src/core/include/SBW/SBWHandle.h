/**
 * @file SBWHandle.h
 * @brief smart pointer - based on MS ATL - base class for immediate API classes
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SBWHandle.h,v 1.2 2007/02/22 22:24:53 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWHandle.h,v $
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
// SBWHandle.h: interface for the SBWHandle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SBWHANDLE_H__3540512C_45D8_448B_9111_619845A580CA__INCLUDED_)
#define AFX_SBWHANDLE_H__3540512C_45D8_448B_9111_619845A580CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning (disable: 4312)

#endif // _MSC_VER > 1000

#include <assert.h>
#include "portableOS.h"

namespace SystemsBiologyWorkbench
{

/// smart pointer for use with classes derived from SBWObject
template <class T>
class SBW_API SBWHandle
{
public:
	typedef T _PtrClass;

	/// create pointer with null reference
	SBWHandle() {p=NULL;}

	/**
	 * create a smart pointer from a pointer.
     * Increment the reference count of the referenced object.
	 * @param lp pointer to SBWObject
	 */
	SBWHandle(T* lp)
	{
        assert(lp!=(LPVOID) 0xdddddddd);
		if ((p = lp) != NULL)
			p->AddReference();
	}

	/**
	 * create a smart pointer from another smart pointer
	 * Increment the reference count of the referenced object.
	 * @param lp smart pointer to SBWObject
	 */
	SBWHandle(const SBWHandle<T>& lp)
	{
        assert(lp.p!=(LPVOID) 0xdddddddd);
		if ((p = lp.p) != NULL)
			p->AddReference();
	}

	/**
	 * decrement the reference count of the referenced object.
	 */
	~SBWHandle() {if (p) p->Release();}

	/**
	 * decrement the reference count of the referenced object.
	 */
	void Release() {if (p) p->Release(); p=NULL;}

	/**
	 * conversion operator from smart pointer to ordinary pointer.
	 * @return equivalent ordinary pointer.
	 */
	operator T*() const {return (T*)p;}

    /**
	 * dereference or * operator.
	 * @returns a reference to the referenced object.
	 */
	T& operator*() {assert(p!=NULL); return *p; }

	/**
	 * dereference field operator or -> operator.
	 * @returns pointer to the referenced object.
	 */
	T* operator->() { assert(p!=NULL); return p; }

	// T* operator=(T* lp){return (T*)AtlComPtrAssign((IUnknown**)&p, lp);}

    /**
	 * assignment operator.
	 * Decrements reference count of previously referenced object.
	 * Increments reference count of object of newly assigned reference. 
	 * @param lp smart pointer val;ue assigned tom this smart pointer.
	 * @returns new value in ordinary pointer form. 
	 */
	T* operator=(const SBWHandle<T>& lp)
	{
		assert(lp.p!=(LPVOID) 0xdddddddd);
        // in ATL it's
		//return (T*)AtlComPtrAssign((IUnknown**)&p, lp.p);

		if (lp.p)
			lp.p->AddReference();

		if (p)
			p->Release();

		p = lp.p ;

		return (T *)p ;
	}
#if _MSC_VER>1020 || defined(LINUX)
	bool operator!(){return (p == NULL);}
#else
	BOOL operator!(){return (p == NULL) ? TRUE : FALSE;}
#endif
protected:
	/// underlying smart pointer.
	T* p;
};
} // SystemsBiologyWorkbench
#endif // !defined(AFX_SBWHANDLE_H__3540512C_45D8_448B_9111_619845A580CA__INCLUDED_)
