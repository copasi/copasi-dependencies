/**
* Begin svn Header
* $Rev$:	Revision of last commit
* $Author$:	Author of last commit
* $Date$:	Date of last commit
* $HeadURL$
* $Id$
* End svn Header
* ****************************************************************************
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
* Contributors:
* Joseph O. Dada, The University of Manchester - initial API and implementation
* ****************************************************************************
**/

//#include <limits>


#include <numl/NUMLDocument.h>
#include <numl/DimensionDescription.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

DimensionDescription::DimensionDescription (unsigned int level, unsigned int version) :
NUMLList ( level, version )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


DimensionDescription::DimensionDescription (NUMLNamespaces *numlns) :
    NUMLList                  ( numlns )
                {
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
                }

DimensionDescription::DimensionDescription() {
  // TODO Auto-generated constructor stub

}

DimensionDescription::~DimensionDescription() {
  // TODO Auto-generated destructor stub
}

/** @endcond doxygen-libnuml-internal */

/*
 * Accepts the given NUMLVisitor.
 *
 * @return the result of calling <code>v.visit()</code>, which indicates
 * whether or not the Visitor would like to visit the NUML Doc's next
 * DimensionDescription (if available).
 */
bool
DimensionDescription::accept (NUMLVisitor& v) const
{
  return v.visit(*this);
}

/*
 * @return the name of this element ie "dimensionDescription".
 */
const string&
DimensionDescription::getElementName () const
{
  static const string dimensionDescription  = "dimensionDescription";
  return dimensionDescription;
}

/*
 * @return a (deep) copy of this DimensionDescription.
 */
DimensionDescription*
DimensionDescription::clone () const
{
  return new DimensionDescription(*this);
}


/*
 * @return the NUMLTypeCode_t of NUML objects contained in this DimensionDescription or
 * NUML_UNKNOWN (default).
 */
NUMLTypeCode_t
DimensionDescription::getItemTypeCode () const
{
  return NUML_COMPOSITEDESCRIPTION;
}

/* return nth item in list */
CompositeDescription *
DimensionDescription::get(unsigned int n)
{
  return static_cast<CompositeDescription*>(NUMLList::get(n));
}


/* return nth item in list */
const CompositeDescription *
DimensionDescription::get(unsigned int n) const
{
  return static_cast<const CompositeDescription*>(NUMLList::get(n));
}

/*
 * @return the CompositeDescription in this DimensionDescription or NULL if no such
 * CompositeDescription exists.
 */
CompositeDescription*
DimensionDescription::getCompositeDescription()
{
  return 0; //&mCompositeDescription;
}

/**
* Used by DimensionDescription::get() to lookup an NMBase based by its id.
*/
struct IdEqS : public unary_function<NMBase*, bool>
{
  const string& id;

  IdEqS (const string& id) : id(id) { }
  bool operator() (NMBase* sb)
                   { return static_cast <CompositeDescription *> (sb)->getId() == id; }
};


/* return all the items */
/*DimensionDescription*
DimensionDescription::getDimensionDescription()
{
  //return &mDimensionDescription;
   return static_cast<DimensionDescription*>(this);
//	return this;
}*/

/* return item by id */
CompositeDescription*
DimensionDescription::get (const std::string& sid)
{
  return const_cast<CompositeDescription*>(
      static_cast<const DimensionDescription&>(*this).get(sid) );
}


/* return item by id */
const CompositeDescription*
DimensionDescription::get (const std::string& sid) const
{
  vector<NMBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqS(sid) );
  return (result == mItems.end()) ? 0 : static_cast <CompositeDescription*> (*result);
}


/* Removes the nth item from this list */
CompositeDescription*
DimensionDescription::remove (unsigned int n)
{
  return static_cast<CompositeDescription*>(NUMLList::remove(n));
}


/* Removes item in this list by id */
CompositeDescription*
DimensionDescription::remove (const std::string& sid)
{
  NMBase* item = 0;
  vector<NMBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqS(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <CompositeDescription*> (item);
}


/** @cond doxygen-libnuml-internal */
/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
DimensionDescription::getElementPosition () const
{
  return 0;
}
/** @endcond doxygen-libnuml-internal */

/** @cond doxygen-libnuml-internal */


void DimensionDescription::writeXMLNS(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  if (getNUMLDocument() != NULL)
  {
    NMBase::writeXMLNS(stream);
  }

  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* ns = getNamespaces();

  if (ns != NULL)
  {
    stream << *getNamespaces();
  }
  else
  {
    LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces ns;
    ns.add(NUML_XMLNS_L1V1);
    stream << ns;
  }
}

/** @endcond doxygen-libnuml-internal */


const std::string& 
DimensionDescription::getId() const
{
  return mId;
}

int 
DimensionDescription::setId(const std::string& id)
{
  mId = id;
  return LIBNUML_OPERATION_SUCCESS;
}

const std::string& 
DimensionDescription::getName() const
{
  return mName;
}

int 
DimensionDescription::setName(const std::string& name)
{
  mName = name;
  return LIBNUML_OPERATION_SUCCESS;
}


/** @cond doxygen-libnuml-internal */
/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
DimensionDescription::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{

  //const string& name   = stream.peek().getName();

  const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLToken& element = stream.peek();
  const string& name = element.getName();
  NMBase*        object = 0;

  if (name == "compositeDescription")
  {
    try
    {

      object = new CompositeDescription(getNUMLNamespaces());
    }
    catch (NUMLConstructorException*)
    {
      object = new CompositeDescription(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }
    catch ( ... )
    {
      object = new CompositeDescription(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }
    if (object) mItems.push_back(object);

  }
  else if(name == "tupleDescription")
  {

  }else if(name == "atomicDescription")
  {

  }

  return object;
}

LIBNUML_CPP_NAMESPACE_END
