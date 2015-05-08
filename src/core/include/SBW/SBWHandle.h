/**
 * @file SBWHandle.h
 * @brief smart pointer - based on MS ATL - base class for immediate API classes
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
#include <SBW/sbwdefs.h>
#ifndef SBW_STRICT_INCLUDES
#include <SBW/portableOS.h>
#endif

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
      assert(lp!=(void *) 0xdddddddd);
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
      assert(lp.p!=(void *) 0xdddddddd);
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
      assert(lp.p!=(void *) 0xdddddddd);
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
