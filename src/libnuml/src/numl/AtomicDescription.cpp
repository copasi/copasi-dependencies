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

#include <numl/common/extern.h>
#include <numl/common/libnuml-namespace.h>
#include <numl/common/operationReturnValues.h>

#include <numl/common/numlfwd.h>

#include <numl/AtomicDescription.h>
#include <numl/NUMLVisitor.h>
#include <numl/NUMLDocument.h>
#include <numl/NUMLError.h>



using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

AtomicDescription::AtomicDescription (unsigned int level, unsigned int version) 
  : DimensionDescription ( level, version )
  , mId                 ( ""   )
  , mName               ( ""   )
  , mOntologyTerm       ( ""   )
  , mValueType          ( ""   )

{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


AtomicDescription::AtomicDescription (NUMLNamespaces *numlns) :
    DimensionDescription                   ( numlns )
  , mId                 ( ""   )
  , mName               ( ""   )
  , mOntologyTerm       ( ""   )
  , mValueType          ( ""   )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}



const std::string&
AtomicDescription::getId() const
{
  return mId;
}

int
AtomicDescription::setId(const std::string& id)
{
  mId = id;
  return LIBNUML_OPERATION_SUCCESS;
}

/*
 * Accepts the given NUMLVisitor.
 *
 * @return the result of calling <code>v.visit()</code>, which indicates
 * whether or not the Visitor would like to visit the NUML Doc's next
 * AtomicDescription (if available).
 */
bool
AtomicDescription::accept (NUMLVisitor& v) const
{
  return v.visit(*this);
}

/*
 * @return the NUMLTypeCode_t of this NUML object or NUML_UNKNOWN
 * (default).
 *
 * @see getElementName()
 */
NUMLTypeCode_t
AtomicDescription::getTypeCode () const
{
  return NUML_ATOMICDESCRIPTION;
}


/*
 * @return the name of this element ie "atomicDescription".
 */
const string&
AtomicDescription::getElementName () const
{
  static const string atomicDesc  = "atomicDescription";
  return atomicDesc;
}

/*
 * Sets the name attribute of this NUML object to a copy of name.
 */
int AtomicDescription::setName(const std::string& name) {
  mName = name;
  return LIBNUML_OPERATION_SUCCESS;
}

/*
 * Sets the valueType attribute of this NUML object to a copy of valueType.
 */
int AtomicDescription::setValueType(const std::string& valueType) {
  mValueType = valueType;
  return LIBNUML_OPERATION_SUCCESS;
}

/*
 * Sets the ontologyTerm attribute of this NUML object to a copy of ontologyTerm.
 */
int AtomicDescription::setOntologyTerm(const std::string& ontologyTerm) {
  mOntologyTerm = ontologyTerm;
  return LIBNUML_OPERATION_SUCCESS;

}


/*
 * @return the value held by atomicDescription.
 */
const string&
AtomicDescription::getValueType () const{

  return mValueType;
}


/*
 * @return the value held by atomicDescription.
 */
const string&
AtomicDescription::getOntologyTerm () const{

  return mOntologyTerm;
}
/*
 * @return the value held by atomicDescription.
 */
const string&
AtomicDescription::getName () const{

  return mName;
}

/*
 * @return a (deep) copy of this AtomicDescription.
 */
AtomicDescription*
AtomicDescription::clone () const
{
  return new AtomicDescription(*this);
}

AtomicDescription::AtomicDescription() 
{

}

AtomicDescription::~AtomicDescription() 
{
}

/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
AtomicDescription::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
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
  expectedAttributes.push_back("valueType");

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
  attributes.readInto("valueType", mValueType);

}
/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void AtomicDescription::writeAttributes(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  NMBase::writeAttributes(stream);

  //const unsigned int level = getLevel();
  //const unsigned int version = getVersion();

  stream.writeAttribute("id", mId);
  stream.writeAttribute("name", mName);
  stream.writeAttribute("ontologyTerm", mOntologyTerm);
  stream.writeAttribute("valueType", mValueType);

}


LIBNUML_CPP_NAMESPACE_END
