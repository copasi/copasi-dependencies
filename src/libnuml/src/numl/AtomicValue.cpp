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

#include <numl/Dimension.h>
#include <numl/NUMLVisitor.h>
#include <numl/NUMLDocument.h>
#include <numl/NUMLError.h>


#include <numl/AtomicValue.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

AtomicValue::AtomicValue (unsigned int level, unsigned int version) :
   Dimension ( level, version )
  , mValue                 ( ""   )

{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


AtomicValue::AtomicValue (NUMLNamespaces *numlns) :
    Dimension                   ( numlns )
   ,mValue                      ( ""   )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}

/*
 * Accepts the given NUMLVisitor.
 *
 * @return the result of calling <code>v.visit()</code>, which indicates
 * whether or not the Visitor would like to visit the NUML Doc's next
 * AtomicValue (if available).
 */
bool
AtomicValue::accept (NUMLVisitor& v) const
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
AtomicValue::getTypeCode () const
{
  return NUML_ATOMICVALUE;
}

/*
 * @return the name of this element ie "atomicValue".
 */
const string&
AtomicValue::getElementName () const
{
  static const string atomicValue  = "atomicValue";
  return atomicValue;
}

/*
 * @return the value held by atomicValue.
 */
const string&
AtomicValue::getValue () const{

  return mValue;
}

/*
 * @return the value held by atomicValue converted to double value.
 */
double
AtomicValue::getDoubleValue () {

  double value = 0.0;
  stringstream isreal;
  isreal.str(mValue);
  isreal >> value;
  return value;
}

/*
 * Sets the value of this NUML object to a copy of value.
 */
int
AtomicValue::setValue (const std::string& value)
{
  mValue = value;
  return LIBNUML_OPERATION_SUCCESS;

}

/*
 * @return a (deep) copy of this AtomicValue.
 */
AtomicValue*
AtomicValue::clone () const
{
  return new AtomicValue(*this);
}

AtomicValue::AtomicValue() {
  // TODO Auto-generated constructor stub

}

AtomicValue::~AtomicValue() {
  // TODO Auto-generated destructor stub
}

/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
/*NMBase*
AtomicValue::createObject (XMLInputStream& stream)
{
  const XMLToken  element  = stream.peek();
  const string& name = element.getName();
  NMBase*        object = 0;


  if (name == "atomicValue")
  {
    stream.next();
    mValue = stream.next().getCharacters();
    try
    {
      object = new AtomicValue(getNUMLNamespaces());
      //	static_cast<AtomicValue*>(object)->setValue(mValue);

    }
    catch (NUMLConstructorException*)
    {
      object = new AtomicValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
      //	static_cast<AtomicValue*>(object)->setValue(mValue);

    }
    catch ( ... )
    {
      object = new AtomicValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
      //	static_cast<AtomicValue*>(object)->setValue(mValue);
    }
  }

  return object;
}*/

/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
AtomicValue::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
{
  NMBase::readAttributes(attributes);
}
/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
AtomicValue::writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  NMBase::writeAttributes(stream);

  //const unsigned int level   = getLevel  ();
  //const unsigned int version = getVersion();

  //TODO

}

void
AtomicValue::writeChars(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  stream << mValue;
}

/*void
AtomicValue::write(XMLOutputStream& stream) const
{
  //NMBase::writeElements(stream);
  stream.startElement( getElementName() );
  //writeAttributes( stream );
  stream <<mValue;
//	writeChars  ( stream );
  //writeElements  ( stream );
  stream.endElement( getElementName() );
}*/

/*
 * Subclasses should override this method to write out their contained
 * NUML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.
 */
/*void
AtomicValue::writeElements (XMLOutputStream& stream) const
{
  //NMBase::writeElements(stream);
//	stream << mValue;

}*/




LIBNUML_CPP_NAMESPACE_END
