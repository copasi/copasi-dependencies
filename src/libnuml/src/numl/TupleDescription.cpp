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

#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>

#include <numl/NUMLVisitor.h>
#include <numl/NUMLDocument.h>
#include <numl/NUMLError.h>
#include <numl/common/operationReturnValues.h>


#include <numl/TupleDescription.h>
#include <numl/AtomicDescription.h>

using namespace std;
LIBNUML_CPP_NAMESPACE_BEGIN

TupleDescription::TupleDescription (unsigned int level, unsigned int version) :
   DimensionDescription ( level, version )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


TupleDescription::TupleDescription (NUMLNamespaces *numlns) :
    DimensionDescription                  ( numlns )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}

TupleDescription::TupleDescription() {
  // TODO Auto-generated constructor stub

}

TupleDescription::~TupleDescription() {
  // TODO Auto-generated destructor stub
}


const std::string&
TupleDescription::getId() const
{
  return mId;
}

int
TupleDescription::setId(const std::string& id)
{
  mId = id;
  return LIBNUML_OPERATION_SUCCESS;
}


/*
 * @return a (deep) copy of this TupleDescription.
 */
TupleDescription*
TupleDescription::clone () const
{
  return new TupleDescription(*this);
}

/*
 * @return the NUMLTypeCode_t of NUML objects contained in this TupleDescription or
 * NUML_UNKNOWN (default).
 */
NUMLTypeCode_t
TupleDescription::getItemTypeCode () const
{
  return NUML_ATOMICDESCRIPTION;
}

/*
 * @return the annotation of this NUML object by string.
 *
 * TODO std::string
NMBase::getAnnotationString ()
{
  return XMLNode::convertXMLNodeToString(getAnnotation());
}*/

/*
 * @return the name of this element ie "compositeValue".
 */
const string&
TupleDescription::getElementName () const
{
  static const string tuple  = "tupleDescription";
  return tuple;
}

/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
TupleDescription::getElementPosition () const
{
  return 4;
}

/* return nth AtomicDescription in the TupleDescription list */
AtomicDescription *
TupleDescription::getAtomicDescription(unsigned int n)
{
  return static_cast<AtomicDescription*>(NUMLList::get(n));
}


/* return nth AtomicDescription in the TupleDescription list */
const AtomicDescription *
TupleDescription::getAtomicDescription(unsigned int n) const
{
  return static_cast<const AtomicDescription*>(NUMLList::get(n));
}


/* Removes the nth AtomicDescription from this TupleDescription list */
AtomicDescription*
TupleDescription::removeAtomicDescription (unsigned int n)
{
   return static_cast<AtomicDescription*>(NUMLList::remove(n));
}

/*
 * Creates a new AtomicDescription inside this TupleDescription and returns it.
 */
AtomicDescription*
TupleDescription::createAtomicDescription ()
{
  AtomicDescription* aDescription = 0;

  try
  {
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
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
TupleDescription::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
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

  // check that all attributes are expected
  for (int i = 0; i < attributes.getLength(); i++)
  {
    std::vector<std::string>::const_iterator end = expectedAttributes.end();
    std::vector<std::string>::const_iterator begin = expectedAttributes.begin();

    std::string name = attributes.getName(i);
    if (std::find(begin, end, name) == end)
    {
      logUnknownAttribute(name, level, version, "<atomicDescription>");
    }
  }
  attributes.readInto("id", mId);
  attributes.readInto("name", mName);
  attributes.readInto("ontologyTerm", mOntologyTerm);

}

/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
TupleDescription::writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  NMBase::writeAttributes(stream);

  stream.writeAttribute("id", mId);
  stream.writeAttribute("name", mName);
  stream.writeAttribute("ontologyTerm", mOntologyTerm);
}

/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
TupleDescription::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
  const string& name   = stream.peek().getName();
  AtomicDescription *aDescription = NULL;

  if (name == "atomicDescription")
  {

    try
    {
      aDescription = new AtomicDescription(getNUMLNamespaces());
    }
    catch (NUMLConstructorException*)
    {
      aDescription = new AtomicDescription(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }
    catch ( ... )
    {
      aDescription = new AtomicDescription(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }

    if (aDescription) appendAndOwn(aDescription);
  }

  return aDescription;
}


void
TupleDescription::write(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
 // NUMLList::writeElements(stream);
//  mAtomicDescription->writeElements(stream);
  stream.startElement( getElementName() );

    writeAttributes( stream );
    writeElements  ( stream );

    stream.endElement( getElementName() );

}

LIBNUML_CPP_NAMESPACE_END
