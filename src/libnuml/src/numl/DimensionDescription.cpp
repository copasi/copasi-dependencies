/*
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
#include <numl/common/operationReturnValues.h>

#include <numl/AtomicDescription.h>
#include <numl/TupleDescription.h>
#include <numl/CompositeDescription.h>


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


/**
 * Creates a new CompositeDescription and add it to DimensionDescription's list inside this ResultComponent and return it.
 *
 * @return the CompositeDescription object created
 *
 * @see addCompositeDescription(const CompositeDescription *compDesc)
 */
CompositeDescription*
DimensionDescription::createCompositeDescription()
{
  CompositeDescription* desc = 0;

  try
  {
    desc = new CompositeDescription(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }

  if (desc) appendAndOwn(desc);

  return desc;
}

/**
 * creates a new tupledescription and adds it to the dimensiondescription
 * @return the created tuple description
 */
TupleDescription*
DimensionDescription::createTupleDescription()
{
  TupleDescription* desc = 0;

  try
  {
    desc = new TupleDescription(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }

  if (desc) appendAndOwn(desc);

  return desc;

}

/**
 * creates a new atomic description and adds it to the dimensiondescription
 * @return the created atomic description
 */
AtomicDescription*
DimensionDescription::createAtomicDescription()
{
  AtomicDescription* desc = 0;

  try
  {
    desc = new AtomicDescription(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }

  if (desc) appendAndOwn(desc);

  return desc;
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
DimensionDescription *
DimensionDescription::get(unsigned int n)
{
  return static_cast<DimensionDescription*>(NUMLList::get(n));
}


/* return nth item in list */
const DimensionDescription *
DimensionDescription::get(unsigned int n) const
{
  return static_cast<const DimensionDescription*>(NUMLList::get(n));
}

/**
* Used by DimensionDescription::get() to lookup an NMBase based by its id.
*/
struct IdEqS
{
  const string& id;

  IdEqS (const string& id) : id(id) { }
  bool operator() (NMBase* sb)
                   { return static_cast <DimensionDescription *> (sb)->getId() == id; }
};

/* return item by id */
DimensionDescription*
DimensionDescription::get (const std::string& sid)
{
  return const_cast<DimensionDescription*>(
      static_cast<const DimensionDescription&>(*this).get(sid) );
}


/* return item by id */
const DimensionDescription*
DimensionDescription::get (const std::string& sid) const
{
  vector<NMBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqS(sid) );
  return (result == mItems.end()) ? 0 : static_cast <DimensionDescription*> (*result);
}


/* Removes the nth item from this list */
DimensionDescription*
DimensionDescription::remove (unsigned int n)
{
  return static_cast<DimensionDescription*>(NUMLList::remove(n));
}


/* Removes item in this list by id */
DimensionDescription*
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

  return static_cast <DimensionDescription*> (item);
}


/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
DimensionDescription::getElementPosition () const
{
  return 0;
}

void DimensionDescription::writeXMLNS(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  if (getNUMLDocument() != NULL)
  {
    NMBase::writeXMLNS(stream);
    return;
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
    if (object) appendAndOwn(object);

  }
  else if(name == "tupleDescription")
  {
    try
    {

      object = new TupleDescription(getNUMLNamespaces());
    }
    catch (NUMLConstructorException*)
    {
      object = new TupleDescription(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }
    catch ( ... )
    {
      object = new TupleDescription(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }
    if (object)  appendAndOwn(object);
  }else if(name == "atomicDescription")
  {
    try
    {

      object = new AtomicDescription(getNUMLNamespaces());
    }
    catch (NUMLConstructorException*)
    {
      object = new AtomicDescription(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }
    catch ( ... )
    {
      object = new AtomicDescription(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }
    if (object)  appendAndOwn(object);
  }

  return object;
}

LIBNUML_CPP_NAMESPACE_END
