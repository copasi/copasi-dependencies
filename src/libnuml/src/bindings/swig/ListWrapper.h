/**
* Begin svn Header
* $Rev$:	Revision of last commit
* $Author$:	Author of last commit
* $Date$:	Date of last commit
* $HeadURL$
* $Id$
* End svn Header
* ***************************************************************************
* This file is part of libNUML.  Please visit http://code.google.com/p/numl/for more
* information about NUML, and the latest version of libNUML.
* Copyright (c) 2013 The University of Manchester.
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation.  A copy of the license agreement is
* provided in the file named "LICENSE.txt" included with this software
* distribution and also available online as http://www.gnu.org/licenses/lgpl.html
*
* The original code was initially developed by:
*
* 	SBML Team
* 	Control and Dynamical Systems, MC 107-81
* 	California Institute of Technology
* 	Pasadena, CA, 91125, USA
*
*  	http://www.sbml.org
* 	mailto:sbml-team@caltech.edu
*
* Adapted for NUML by:
*
* 	NUML Team
* 	Computational Systems Biology Group
* 	School of Computer Science
* 	The University of Manchester
* 	Manchester, M1 7DN UK
* 	mailto:josephodada@gmail.com
*
*   Contributor(s):
*   Joseph O. Dada, The University of Manchester - initial LIBNUML API and implementation
* ***************************************************************************
**/

#ifndef ListWrapper_h
#define ListWrapper_h

#include <sbml/util/List.h>
#include <numl/common/extern.h>

LIBSBML_CPP_NAMESPACE_USE
LIBNUML_CPP_NAMESPACE_USE

/**
 *
 * ListWrapper : a wrapper template class for List class.
 *
 * Currently (2009-05-26), this template class is used for wrapping
 * the following functions in each language bindings.
 *
 *  - List* ModelHistory::getListCreators()
 *  - List* ModelHistory::getListModifiedDates()
 *  - List* SBase::getCVTerms()
 *  - List* ASTNode::getListOfNodes()
 */
template<typename IType>
class LIBNUML_EXTERN ListWrapper
{
  private:
    List *mList;
    bool mMemOwn;

  public:
    /**
     * Creates a new List.
     *
     * A ListXXX object is newly created (i.e. owned by the caller) and 
     * deleted by the destructor below if the constructor of this class 
     * invoked without an argument. 
     */
    ListWrapper() : mList(new List()), mMemOwn(true) {}


#ifndef SWIG
    /**
     * Creates a new List.
     * (internal implementation)
     *
     * An existing List object is given (i.e. not owned by the caller)
     * and stored.
     *
     */
    ListWrapper(List* list, bool memown = false) : mList(list), mMemOwn(memown) {}

    List* getList() { return mList; }
#endif


    /**
     * destructor
     */
    virtual ~ListWrapper() { if (mMemOwn) delete mList; }


    /**
     * Adds @p item to the end of this List.
     *
     * @param item a pointer to the item to be added.
     */
    void add(IType* item) 
    { 
      if (mList) mList->add(static_cast<void*>(item)); 
    }


    /**
     * Get the nth item in this List.
     *
     * If @p n > <code>listobj.size()</code>, this method returns @c 0.
     *
     * @return the nth item in this List.
     *
     * @see remove()
     *
     */
    IType* get(unsigned int n) const 
    { 
      return (mList) ? static_cast<IType*>(mList->get(n)) : 0; 
    }


    /**
     * Adds @p item to the beginning of this List.
     *
     * @param item a pointer to the item to be added.
     */
    void prepend(IType* item) 
    { 
      if (mList) mList->prepend(static_cast<void*>(item)); 
    }


    /**
     * Removes the nth item from this List and returns a pointer to it.
     *
     * If @p n > <code>listobj.size()</code>, this method returns @c 0.
     *
     * @return the nth item in this List.
     *
     * @see get()
     */
     IType* remove(unsigned int n) 
    { 
      return (mList) ? static_cast<IType*>(mList->remove(n)) : 0; 
    }


    /**
     * Get the number of items in this List.
     * 
     * @return the number of elements in this List.
     */
    unsigned int getSize() const 
    { 
      return (mList) ? mList->getSize() : 0; 
    }
};

#endif // ListWrapper_h
