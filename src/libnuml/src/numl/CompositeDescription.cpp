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

#include <numl/CompositeDescription.h>
#include <numl/AtomicDescription.h>
#include <numl/TupleDescription.h>
#include <numl/NUMLDocument.h>
#include <numl/common/operationReturnValues.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

CompositeDescription::CompositeDescription (unsigned int level, unsigned int version) :
   DimensionDescription ( level, version )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


CompositeDescription::CompositeDescription (NUMLNamespaces *numlns) :
    DimensionDescription                  ( numlns )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}
CompositeDescription::CompositeDescription() {
  // TODO Auto-generated constructor stub

}

CompositeDescription::~CompositeDescription() {
  // TODO Auto-generated destructor stub
}



const std::string&
CompositeDescription::getId() const
{
  return mId;
}

int
CompositeDescription::setId(const std::string& id)
{
  mId = id;
  return LIBNUML_OPERATION_SUCCESS;
}

/*
 * @return a (deep) copy of this CompositeDescription.
 */
CompositeDescription*
CompositeDescription::clone () const
{
  return new CompositeDescription(*this);
}

/*
 * @return the name of this element ie "compositeDescription".
 */
const string&
CompositeDescription::getElementName () const
{
  static const string compDesc  = "compositeDescription";
  return compDesc;
}


/*
 * @return true if the content of CompositeDescription obejct is a CompositeDescription object, false
 * otherwise.
 */
bool
CompositeDescription::isContentCompositeDescription () const
{
  if(this->mContentType == NUML_COMPOSITEDESCRIPTION)
    return true;
  else return false;
}

/*
 * @return true if the content of CompositeDescription obejct is a TupleDescription object, false
 * otherwise.
 */
bool
CompositeDescription::isContentTupleDescription () const
{
  if(this->mContentType == NUML_TUPLEDESCRIPTION)
    return true;
  else return false;
}

/*
 * @return true if the content of CompositeDescription obejct is a AtomicDescription object, false
 * otherwise.
 */
bool
CompositeDescription::isContentAtomicDescription () const
{
  if(this->mContentType == NUML_ATOMICDESCRIPTION)
    return true;
  else return false;
}


/*
 * Accepts the given NUMLVisitor.
 *
 * @return the result of calling <code>v.visit()</code>, which indicates
 * whether or not the Visitor would like to visit the NUML Doc's next
 * TupleDescription (if available).
 */


bool
CompositeDescription::accept(NUMLVisitor& v) const
{
  v.visit(*this);
  return true;
}

/* TODO std::string
NMBase::getAnnotationString ()
{
  return XMLNode::convertXMLNodeToString(getAnnotation());
}*/

/*
 * @return the NUMLTypeCode_t of NUML objects contained in this CompositeDescription or
 * NUML_UNKNOWN (default).
 * @return the annotation of this NUML object by string.
 */
NUMLTypeCode_t
CompositeDescription::getItemTypeCode () const
{
  if (mContentType == NUML_COMPOSITEDESCRIPTION)
  {
    return NUML_COMPOSITEDESCRIPTION;
  }
  else if (mContentType == NUML_TUPLEDESCRIPTION)
  {
    return NUML_TUPLEDESCRIPTION;
  }
  else if (mContentType == NUML_ATOMICDESCRIPTION)
  {
    return NUML_ATOMICDESCRIPTION;
  }
  else
  {
    return NUML_UNKNOWN;
  }
}

/* return nth item in list */
CompositeDescription *
CompositeDescription::getCompositeDescription(unsigned int n) {
  if(this->getTypeCode()==NUML_COMPOSITEDESCRIPTION)
  return static_cast<CompositeDescription*>(NUMLList::get(n));
  else return NULL;
}

/* return nth AtomicDescription from CompositeDescription list, note that only
 * one AtomicDescription object is permitted, n = 0 always for AtomicDescription
 * contained in CompositeDescription
 */
AtomicDescription *
CompositeDescription::getAtomicDescription() {
  if(this->size() == 1 && this->mContentType == NUML_ATOMICDESCRIPTION)
  {
    return static_cast<AtomicDescription*>(NUMLList::get(0));
  }
  else return NULL;
}

/* return nth TupleDescription from CompositeDescription list, note that only
 * one TupleDescription object is permitted, n = 0 always for TupleDescription
 * contained in CompositeDescription
 */
TupleDescription *
CompositeDescription::getTupleDescription() {
  if(this->size() == 1  && this->mContentType == NUML_TUPLEDESCRIPTION)
  {
    return static_cast<TupleDescription*>(NUMLList::get(0));
  }
  else return NULL;
}

/* return nth item in list */
CompositeDescription *
CompositeDescription::get(unsigned int n)
{
  return static_cast<CompositeDescription*>(NUMLList::get(n));
}


/* return nth item in list */
const CompositeDescription *
CompositeDescription::get(unsigned int n) const
{
  return static_cast<const CompositeDescription*>(NUMLList::get(n));
}


/* Removes the nth item from this list */
CompositeDescription*
CompositeDescription::remove (unsigned int n)
{
   return static_cast<CompositeDescription*>(NUMLList::remove(n));
}

/*
 * @return the name of this NUML object.
 */
const string&
CompositeDescription::getName () const
{
  return mName;
}

/*
 * Sets the name attribute of this NUML object to a copy of name.
 */
int CompositeDescription::setName(const std::string& name) {
  mName = name;
  return LIBNUML_OPERATION_SUCCESS;
}

/*
 * Sets the indexType attribute of this NUML object to a copy of indexType.
 */
int CompositeDescription::setIndexType(const std::string& indexType) {
  mIndexType = indexType;
  return LIBNUML_OPERATION_SUCCESS;
}

/*
 * Sets the ontologyTerm attribute of this NUML object to a copy of ontologyTerm.
 */
int CompositeDescription::setOntologyTerm(const std::string& ontologyTerm) {
  mOntologyTerm = ontologyTerm;
  return LIBNUML_OPERATION_SUCCESS;

}

/*
 * @return the indexType of this NUML object.
 */
const string&
CompositeDescription::getIndexType () const
{
  return mIndexType;
}

/*
 * @return the ontologyTerm of this NUML object.
 */
const string&
CompositeDescription::getOntologyTerm () const
{
  return mOntologyTerm;
}

/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
CompositeDescription::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
{
  NMBase::readAttributes(attributes);

  const unsigned int level   = NMBase::getLevel  ();
  const unsigned int version = NMBase::getVersion();

  std::vector<std::string> expectedAttributes;
  expectedAttributes.clear();
  expectedAttributes.push_back("metaid");
  expectedAttributes.push_back("id");
  expectedAttributes.push_back("name");
  expectedAttributes.push_back("ontologyTerm");
  expectedAttributes.push_back("indexType");

  // check that all attributes are expected
  for (int i = 0; i < attributes.getLength(); i++)
  {
    std::vector<std::string>::const_iterator end = expectedAttributes.end();
    std::vector<std::string>::const_iterator begin = expectedAttributes.begin();

    std::string name = attributes.getName(i);
    if (std::find(begin, end, name) == end)
    {
      logUnknownAttribute(name, level, version, "<compositeDescription>");
    }
  }
  attributes.readInto("id", mId);
  attributes.readInto("name", mName);
  attributes.readInto("ontologyTerm", mOntologyTerm);
  attributes.readInto("indexType", mIndexType);

}

/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
CompositeDescription::writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  NMBase::writeAttributes(stream);

  stream.writeAttribute("id", mId);
  stream.writeAttribute("name", mName);
  stream.writeAttribute("ontologyTerm", mOntologyTerm);
  stream.writeAttribute("indexType", mIndexType);
}

/*
 * Creates a new CompositeDescription inside this CompositeDescription, add to its list and returns it.
 */
CompositeDescription*
CompositeDescription::createCompositeDescription ()
{
  CompositeDescription* compDescription = 0;

  try
  {
    compDescription = new CompositeDescription(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }
    if(compDescription) this->appendAndOwn(compDescription);
  return compDescription;
}

/*
 * Creates a new TupleDescription inside this CompositeDescription and returns it.
 */
TupleDescription*
CompositeDescription::createTupleDescription ()
{
  TupleDescription* tDescription = 0;

  try
  {
    tDescription = new TupleDescription(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }

  /* if the NUMLList is empty it doesn't know its parent */
  /* 	if (mTupleDescription.size() == 0) {
      mTupleDescription.setNUMLDocument(this->getNUMLDocument());
      mTupleDescription.setParentNUMLObject(this);
    }*/
    if(tDescription) this->appendAndOwn(tDescription);
  return tDescription;
}

/*
 * Creates a new AtomicDescription to this CompositeDescription and returns it.
 */
AtomicDescription*
CompositeDescription::createAtomicDescription ()
{
  AtomicDescription* aDescription = 0;

  try
  {
    //mAtomicDescription = new AtomicDescription(getNUMLNamespaces());
    aDescription = new AtomicDescription(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }

  if(aDescription) this->appendAndOwn(aDescription);
  return aDescription;
}

/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
CompositeDescription::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
  const string& name   = stream.peek().getName();
  NMBase*        object = 0;

  if (name == "compositeDescription")
  {
    this->mContentType = NUML_COMPOSITEDESCRIPTION;
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
  else if (name == "atomicDescription")
  {
    this->mContentType = NUML_ATOMICDESCRIPTION;
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

    if (object) appendAndOwn(object);
  }
  else if (name == "tupleDescription")
  {
    this->mContentType = NUML_TUPLEDESCRIPTION;

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

    if (object) appendAndOwn(object);
  /*  {
      if (mTupleDescription.size() != 0)
      {
        logError(NUMLNotSchemaConformant);
      }
      object = &mTupleDescription;
    }*/
  }

  return object;
}

LIBNUML_CPP_NAMESPACE_END
