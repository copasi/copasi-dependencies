/**
 * @file    Member.cpp
 * @brief   Implementation of Member, the SBase derived class of groups package.
 * @author  Akiya Jouraku
 *
 * $Id: Member.cpp 13541 2011-04-08 22:16:45Z fbergmann $
 * $HeadURL: https://sbml.svn.sourceforge.net/svnroot/sbml/branches/libsbml-5/src/sbml/packages/groups/sbml/Member.cpp $
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2009-2011 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EBML-EBI), Hinxton, UK
 *  
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 */

#include <iostream>
#include <limits>

#include <sbml/SBMLVisitor.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLToken.h>
#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>

#include <sbml/packages/groups/sbml/Member.h>
#include <sbml/packages/groups/extension/GroupsExtension.h>

using namespace std;

LIBSBML_CPP_NAMESPACE_BEGIN

/*
 * Creates a new Member with the given level, version, and package version.
 */
Member::Member (unsigned int level, unsigned int version, unsigned int pkgVersion) 
  : SBase (level,version)
   ,mSymbol("")
{
  // set an SBMLNamespaces derived object (GroupsPkgNamespaces) of this package.
  setSBMLNamespacesAndOwn(new GroupsPkgNamespaces(level,version,pkgVersion));  
}


/*
 * Creates a new Member with the given GroupsPkgNamespaces object.
 */
Member::Member(GroupsPkgNamespaces* groupsns)
 : SBase(groupsns)
  ,mSymbol("")
{
  //
  // set the element namespace of this object
  //
  setElementNamespace(groupsns->getURI());

  // load package extensions bound with this object (if any) 
  loadPlugins(groupsns);
}


/*
 * Copy constructor.
 */
Member::Member(const Member& source) : SBase(source)
{
  this->mSymbol=source.mSymbol;
}

/*
 * Assignment operator.
 */
Member& Member::operator=(const Member& source)
{
  if(&source!=this)
  {
    this->SBase::operator=(source);
    this->mSymbol = source.mSymbol;
  }
  
  return *this;
}


/*
 * Destructor.
 */ 
Member::~Member ()
{
}


/*
  * Returns the value of the "symbol" attribute of this Member.
  */
const std::string& 
Member::getSymbol () const
{
  return mSymbol;
}


/*
  * Predicate returning @c true or @c false depending on whether this
  * Member's "symbol" attribute has been set.
  */
bool 
Member::isSetSymbol () const
{
  return (mSymbol.empty() == false);
}

/*
  * Sets the value of the "symbol" attribute of this Member.
  */
int 
Member::setSymbol (const std::string& symbol)
{
  return SyntaxChecker::checkAndSetSId(symbol ,mSymbol);
}


/*
  * Unsets the value of the "symbol" attribute of this Member.
  */
int 
Member::unsetSymbol ()
{
  mSymbol.erase();
  if (mSymbol.empty())
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Subclasses should override this method to return XML element name of
 * this SBML object.
 */
const std::string&
Member::getElementName () const
{
  static const std::string name = "member";
  return name;
}


/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase*
Member::createObject (XMLInputStream& stream)
{
  return 0;
}

/*
 * Subclasses should override this method to get the list of
 * expected attributes.
 * This function is invoked from corresponding readAttributes()
 * function.
 */
void
Member::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("symbol");
}


/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
Member::readAttributes (const XMLAttributes& attributes,
                        const ExpectedAttributes& expectedAttributes)
{
  SBase::readAttributes(attributes,expectedAttributes);

  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  bool assigned = attributes.readInto("symbol", mSymbol, getErrorLog(), true, getLine(), getColumn());
  if (assigned && mSymbol.empty())
  {
    logEmptyString(mSymbol, sbmlLevel, sbmlVersion, "<member>");
  }
  if (!SyntaxChecker::isValidInternalSId(mSymbol)) logError(InvalidIdSyntax);

}


/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
Member::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);

  stream.writeAttribute("symbol",   getPrefix(), mSymbol);

  //
  // (EXTENSION)
  //
  SBase::writeExtensionAttributes(stream);
}


/*
 * Subclasses should override this method to write out their contained
 * SBML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.
 */
void
Member::writeElements (XMLOutputStream& stream) const
{
  SBase::writeElements(stream);

  //
  // (EXTENSION)
  //
  SBase::writeExtensionElements(stream);
}


/*
 * @return the typecode (int) of this SBML object or SBML_UNKNOWN
 * (default).
 *
 * @see getElementName()
 */
int
Member::getTypeCode () const
{
  return SBML_GROUPS_MEMBER;
}

Member*
Member::clone() const
{
    return new Member(*this);
}


/*
 * Accepts the given SBMLVisitor.
 */
bool
Member::accept (SBMLVisitor& v) const
{
  return false;
}


/*
 * Ctor.
 */
ListOfMembers::ListOfMembers(GroupsPkgNamespaces* groupsns)
 : ListOf(groupsns)
{
  //
  // set the element namespace of this object
  //
  setElementNamespace(groupsns->getURI());
}


/*
 * Ctor.
 */
ListOfMembers::ListOfMembers(unsigned int level, unsigned int version, unsigned int pkgVersion)
 : ListOf(level,version)
{
  setSBMLNamespacesAndOwn(new GroupsPkgNamespaces(level,version,pkgVersion));
};


/*
 * @return a (deep) copy of this ListOfUnitDefinitions.
 */
ListOfMembers*
ListOfMembers::clone () const
{
  return new ListOfMembers(*this);
}


/* return nth item in list */
Member *
ListOfMembers::get(unsigned int n)
{
  return static_cast<Member*>(ListOf::get(n));
}


/* return nth item in list */
const Member *
ListOfMembers::get(unsigned int n) const
{
  return static_cast<const Member*>(ListOf::get(n));
}


/* return item by symbol */
Member*
ListOfMembers::get (const std::string& symbol)
{
  return const_cast<Member*>( 
    static_cast<const ListOfMembers&>(*this).get(symbol) );
}


/* return item by symbol */
const Member*
ListOfMembers::get (const std::string& symbol) const
{
  vector<SBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<Member>(symbol) );
  return (result == mItems.end()) ? 0 : static_cast <Member*> (*result);
}


/* Removes the nth item from this list */
Member*
ListOfMembers::remove (unsigned int n)
{
   return static_cast<Member*>(ListOf::remove(n));
}


/* Removes item in this list by symbol */
Member*
ListOfMembers::remove (const std::string& symbol)
{
  SBase* item = 0;
  vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<Member>(symbol) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <Member*> (item);
}


/*
 * @return the typecode (int) of SBML objects contained in this ListOf or
 * SBML_UNKNOWN (default).
 */
int
ListOfMembers::getItemTypeCode () const
{
  return SBML_GROUPS_MEMBER;
}


/*
 * Subclasses should override this method to return XML element name of
 * this SBML object.
 */
const std::string&
ListOfMembers::getElementName () const
{
  static const std::string name = "listOfMembers";
  return name;
}


/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase*
ListOfMembers::createObject (XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase*        object = 0;


  if (name == "member")
  {
    GROUPS_CREATE_NS(groupsns, this->getSBMLNamespaces());
    object = new Member(groupsns);
    appendAndOwn(object);
    //mItems.push_back(object);
  }

  return object;
}


LIBSBML_CPP_NAMESPACE_END

