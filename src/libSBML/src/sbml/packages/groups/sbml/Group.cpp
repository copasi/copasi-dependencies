/**
 * @file    Group.cpp
 * @brief   Implementation of Group, the SBase derived class of groups package.
 * @author  Akiya Jouraku
 *
 * $Id: Group.cpp 13541 2011-04-08 22:16:45Z fbergmann $
 * $HeadURL: https://sbml.svn.sourceforge.net/svnroot/sbml/branches/libsbml-5/src/sbml/packages/groups/sbml/Group.cpp $
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

#include <sbml/SBMLVisitor.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLToken.h>
#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>

#include <sbml/packages/groups/sbml/Group.h>
#include <sbml/packages/groups/sbml/Member.h>
#include <sbml/packages/groups/extension/GroupsExtension.h>

using namespace std;

LIBSBML_CPP_NAMESPACE_BEGIN

/*
 * Creates a new Group with the given level, version, and package version.
 */
Group::Group (unsigned int level, unsigned int version, unsigned int pkgVersion) 
  : SBase (level,version)
   ,mId("")
   ,mName("")
   ,mKind(GROUP_KIND_UNKNOWN)
   ,mMembers(level,version,pkgVersion)
{
  // set an SBMLNamespaces derived object (GroupsPkgNamespaces) of this package.
  setSBMLNamespacesAndOwn(new GroupsPkgNamespaces(level,version,pkgVersion));  

  // connect child elements to this element.
  connectToChild();
}


/*
 * Creates a new Group with the given GroupsPkgNamespaces object.
 */
Group::Group(GroupsPkgNamespaces* groupsns)
 : SBase(groupsns)
  ,mId("")
  ,mName("")
  ,mKind(GROUP_KIND_UNKNOWN)
  ,mMembers(groupsns)
{
  // set the element namespace of this object
  setElementNamespace(groupsns->getURI());

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(groupsns);
}


/*
 * Copy constructor.
 */
Group::Group(const Group& source) : SBase(source)
{
  this->mId=source.mId;
  this->mName=source.mName;
  this->mMembers=source.mMembers;
  this->mKind=source.mKind;

  // connect child elements to this element.
  connectToChild();
}

/*
 * Assignment operator.
 */
Group& Group::operator=(const Group& source)
{
  if(&source!=this)
  {
    this->SBase::operator=(source);
    this->mId = source.mId;
    this->mName=source.mName;
    this->mKind=source.mKind;
    this->mMembers=source.mMembers;

    // connect child elements to this element.
    connectToChild();
  }

  return *this;
}


/*
 * Destructor.
 */ 
Group::~Group ()
{
}


/*
  * Returns the value of the "id" attribute of this Group.
  */
const std::string& 
Group::getId () const
{
  return mId;
}


/*
  * Predicate returning @c true or @c false depending on whether this
  * Group's "id" attribute has been set.
  */
bool 
Group::isSetId () const
{
  return (mId.empty() == false);
}

/*
  * Sets the value of the "id" attribute of this Group.
  */
int 
Group::setId (const std::string& id)
{
  return SyntaxChecker::checkAndSetSId(id,mId);
}


/*
  * Unsets the value of the "id" attribute of this Group.
  */
int 
Group::unsetId ()
{
  mId.erase();
  if (mId.empty())
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Sets the name of this SBML object to a copy of name.
 */
int
Group::setName (const std::string& name)
{
  mName = name;
  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * @return the name of this SBML object.
 */
const string&
Group::getName () const
{
  return mName;
}


/*
 * @return true if the name of this SBML object has been set, false
 * otherwise.
 */
bool
Group::isSetName () const
{
  return (mName.empty() == false);
}


/*
 * Unsets the name of this SBML object.
 */
int
Group::unsetName ()
{
  mName.erase();

  if (mName.empty())
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Sets kind and returns value indicating success.
 */
int
Group::setKind(const GroupKind_t kind)
{
	if (GroupKind_isValidGroupKind(kind) == 0)
	{
    mKind = GROUP_KIND_UNKNOWN;
		return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
	else
	{
		mKind = kind;
		return LIBSBML_OPERATION_SUCCESS;
	}
}


/*
 * @return the kind of this SBML object.
 */
const GroupKind_t
Group::getKind () const
{
  return mKind;
}

/*
 * @return true if the kind of this SBML object has been set, false
 * otherwise.
 */
bool
Group::isSetKind () const
{
  return (mKind != GROUP_KIND_UNKNOWN);
}


/*
 * Returns the member object that holds all members.
 */ 
const ListOfMembers*
Group::getListOfMembers () const
{
  return &this->mMembers;
}


/*
 * Remove the member with the given id.
 * A pointer to the removed member is returned.
 * If no member has been removed, NULL is returned.
 */
Member*
Group::removeMember(const std::string& symbol)
{
  return mMembers.remove(symbol);
}


/*
 * Remove the member with the given index.
 * A pointer to the removed member is returned.
 * If no member has been removed, NULL is returned.
 */
Member*
Group::removeMember(unsigned int index)
{
  return mMembers.remove(index);
}


/*
 * Returns the member with the given index.
 * If the index is invalid, NULL is returned.
 */ 
Member* 
Group::getMember (unsigned int index)
{
  return mMembers.get(index);
}

/*
 * Returns the member with the given index.
 * If the index is invalid, NULL is returned.
 */ 
const Member* 
Group::getMember (unsigned int index) const
{
  return mMembers.get(index);
}


/*
 * Returns the member that has the given id, or NULL if no
 * member has the id.
 */
Member*
Group::getMember (const std::string& symbol) 
{
  return mMembers.get(symbol);
}


/*
 * Returns the member that has the given id, or NULL if no
 * member has the id.
 */
const Member*
Group::getMember (const std::string& symbol) const
{
  return mMembers.get(symbol);
}


/*
 * Adds a member element
 */
int
Group::addMember (const Member* member)
{
  if (member == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
//  else if (!(member->hasRequiredAttributes()) || !(member->hasRequiredElements()))
//  {
//    return LIBSBML_INVALID_OBJECT;
//  }
  else if (getLevel() != member->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != member->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (getMember(member->getId()) != NULL)
  {
    // an object with this id already exists
    return LIBSBML_DUPLICATE_OBJECT_ID;
  }
  else if (getPackageVersion() != member->getPackageVersion())
  {
    return LIBSBML_PKG_VERSION_MISMATCH;
  }
  else
  {
    mMembers.append(member);

    return LIBSBML_OPERATION_SUCCESS;
  }

}


/*
 * Returns the number of members for the group.
 */
unsigned int
Group::getNumMembers () const
{
  return this->mMembers.size();
}


/*
 * Creates a Member object, adds it to the end of the member
 * objects list and returns a reference to the newly created object.
 */
Member*
Group::createMember ()
{
  Member* m = new Member(static_cast<GroupsPkgNamespaces*>(mSBMLNamespaces));
  this->mMembers.appendAndOwn(m);
  return m;
}


/*
 * Subclasses should override this method to return XML element name of
 * this SBML object.
 */
const std::string&
Group::getElementName () const
{
  static const std::string name = "group";
  return name;
}


/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase*
Group::createObject (XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase*        object = 0;

  if (name == "listOfMembers")
  {
    object = &mMembers;
  }

  return object;
}

/*
 * Subclasses should override this method to get the list of
 * expected attributes.
 * This function is invoked from corresponding readAttributes()
 * function.
 */
void
Group::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("id");
  attributes.add("name");
  attributes.add("kind");
}


/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
Group::readAttributes (const XMLAttributes& attributes,
                        const ExpectedAttributes& expectedAttributes)
{
  SBase::readAttributes(attributes,expectedAttributes);

  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  //
  // Reads an attribute "id" (optional)
  //
  bool assigned = attributes.readInto("id", mId, getErrorLog(), false, getLine(), getColumn());

  if (assigned)
  {
    // "id" attribute is set to this group element

    if (mId.empty())
    {
      //
      // Logs an error if the "id" attribute is empty.
      //
      logEmptyString(mId, sbmlLevel, sbmlVersion, "<group>");
    }
    else if (!SyntaxChecker::isValidInternalSId(mId)) 
    {
      //
      // Logs an error if the "id" attribute doesn't
      // conform to the SBML type SId.
      //
      logError(InvalidIdSyntax);
    }
  }

  //
  // Reads an attribute "name" (optional)
  //
  attributes.readInto("name", mName, getErrorLog(), false, getLine(), getColumn());


	//
	// kind string   ( use = "required" )
	//
  std::string kind;
	assigned = attributes.readInto("kind", kind, getErrorLog(), true);

	if (assigned == true)
	{
		// check string is not empty

		if (kind.empty() == true)
		{
			logEmptyString(kind, getLevel(), getVersion(), "<Group>");
		}
    else 
    {
       mKind = GroupKind_fromString( kind.c_str() );
       if (GroupKind_isValidGroupKind(mKind) == 0)
       {
         logError(NotSchemaConformant, getLevel(), getVersion(),
           "Invalid group kind.");
       }
    }
	}
}


/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
Group::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);

  if (isSetId())
    stream.writeAttribute("id",   getPrefix(), mId);
  if (isSetName())
    stream.writeAttribute("name", getPrefix(), mName);
  if (isSetKind())
    stream.writeAttribute("kind", getPrefix(), 
                          GroupKind_toString(mKind));

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
Group::writeElements (XMLOutputStream& stream) const
{
  SBase::writeElements(stream);

  if (getNumMembers() > 0)
  {
    mMembers.write(stream);
  }

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
Group::getTypeCode () const
{
  return SBML_GROUPS_GROUP;
}

Group*
Group::clone() const
{
    return new Group(*this);
}


/*
 * Accepts the given SBMLVisitor.
 */
bool
Group::accept (SBMLVisitor& v) const
{
  return false;
}


/*
 * Sets the parent SBMLDocument of this SBML object.
 */
void
Group::setSBMLDocument (SBMLDocument* d)
{
  SBase::setSBMLDocument(d);

  mMembers.setSBMLDocument(d);
}


/*
 * Sets this SBML object to child SBML objects (if any).
 * (Creates a child-parent relationship by the parent)
  */
void
Group::connectToChild()
{
  mMembers.connectToParent(this);
}


/*
 * Enables/Disables the given package with this element and child
 * elements (if any).
 * (This is an internal implementation for enablePakcage function)
 */
void
Group::enablePackageInternal(const std::string& pkgURI,
                             const std::string& pkgPrefix, bool flag)
{
  SBase::enablePackageInternal(pkgURI,pkgPrefix,flag);

  mMembers.enablePackageInternal(pkgURI,pkgPrefix,flag);
}



/** @cond doxygen-libsbml-internal */
bool
Group::hasRequiredAttributes() const
{
  bool allPresent = true;

  
  if (isSetKind() == false)
  {
    allPresent = false;
  }
  
  return allPresent;
}
/** @endcond doxygen-libsbml-internal */


/** @cond doxygen-libsbml-internal */
/* default for components that have no required elements */
bool
Group::hasRequiredElements() const
{
  bool allPresent = true;

  
  if (mMembers.size() < 1)
  {
    allPresent = false;
  }
  
  return allPresent;
}
/** @endcond doxygen-libsbml-internal */


/*
 * Ctor.
 */
ListOfGroups::ListOfGroups(GroupsPkgNamespaces* groupsns)
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
ListOfGroups::ListOfGroups(unsigned int level, unsigned int version, unsigned int pkgVersion)
 : ListOf(level,version)
{
  setSBMLNamespacesAndOwn(new GroupsPkgNamespaces(level,version,pkgVersion));
};


/*
 * @return a (deep) copy of this ListOfUnitDefinitions.
 */
ListOfGroups*
ListOfGroups::clone () const
{
  return new ListOfGroups(*this);
}


/* return nth item in list */
Group *
ListOfGroups::get(unsigned int n)
{
  return static_cast<Group*>(ListOf::get(n));
}


/* return nth item in list */
const Group *
ListOfGroups::get(unsigned int n) const
{
  return static_cast<const Group*>(ListOf::get(n));
}


/* return item by id */
Group*
ListOfGroups::get (const std::string& sid)
{
  return const_cast<Group*>( 
    static_cast<const ListOfGroups&>(*this).get(sid) );
}


/* return item by id */
const Group*
ListOfGroups::get (const std::string& sid) const
{
  vector<SBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<Group>(sid) );
  return (result == mItems.end()) ? 0 : static_cast <Group*> (*result);
}


/* Removes the nth item from this list */
Group*
ListOfGroups::remove (unsigned int n)
{
   return static_cast<Group*>(ListOf::remove(n));
}


/* Removes item in this list by id */
Group*
ListOfGroups::remove (const std::string& sid)
{
  SBase* item = 0;
  vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<Group>(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <Group*> (item);
}


/*
 * @return the typecode (int) of SBML objects contained in this ListOf or
 * SBML_UNKNOWN (default).
 */
int
ListOfGroups::getItemTypeCode () const
{
  return SBML_GROUPS_GROUP;
}


/*
 * Subclasses should override this method to return XML element name of
 * this SBML object.
 */
const std::string&
ListOfGroups::getElementName () const
{
  static const std::string name = "listOfGroups";
  return name;
}


/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase*
ListOfGroups::createObject (XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase*        object = 0;


  if (name == "group")
  {
    GROUPS_CREATE_NS(groupsns, this->getSBMLNamespaces());
    object = new Group(groupsns);
    appendAndOwn(object);
    //mItems.push_back(object);
  }

  return object;
}


void 
ListOfGroups::writeXMLNS (XMLOutputStream& stream) const
{
  XMLNamespaces xmlns;

  std::string prefix = getPrefix();

  if (prefix.empty())
  {
    if (getNamespaces()->hasURI(GroupsExtension::getXmlnsL3V1V1()))
    {
      xmlns.add(GroupsExtension::getXmlnsL3V1V1(),prefix);
    }
  }

  stream << xmlns;
}

static
const char* GROUP_KIND_STRINGS[] =
{
    "classification"
  , "partonomy"
  , "collection"
};


LIBSBML_EXTERN
const char* 
GroupKind_toString(GroupKind_t kind)
{
  int max = GROUP_KIND_UNKNOWN;

  if (kind < GROUP_KIND_CLASSIFICATION || kind >= max)
  {
      return NULL;
  }

  return GROUP_KIND_STRINGS[kind];
}


LIBSBML_EXTERN
GroupKind_t 
GroupKind_fromString(const char* s)
{
  if (s == NULL) 
  {
    return GROUP_KIND_UNKNOWN;
  }

  int max = GROUP_KIND_UNKNOWN;
  for (int i = 0; i < max; i++)
  {
    if (strcmp(GROUP_KIND_STRINGS[i], s) == 0)
      return (GroupKind_t)i;
  }
  return GROUP_KIND_UNKNOWN;
}


LIBSBML_EXTERN
int 
GroupKind_isValidGroupKind(GroupKind_t kind)
{
  int max = GROUP_KIND_UNKNOWN;

  if (kind < GROUP_KIND_CLASSIFICATION || kind >= max)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}


LIBSBML_EXTERN
int 
GroupKind_isValidGroupKindString(const char* s)
{
  return GroupKind_isValidGroupKind(GroupKind_fromString(s));
}


LIBSBML_CPP_NAMESPACE_END

