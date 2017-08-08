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

#include <numl/Tuple.h>
#include <numl/AtomicValue.h>



using namespace std;
LIBNUML_CPP_NAMESPACE_BEGIN

Tuple::Tuple (unsigned int level, unsigned int version) :
   Dimension ( level, version )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


Tuple::Tuple (NUMLNamespaces *numlns) :
    Dimension                  ( numlns )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}

Tuple::Tuple() {
	// TODO Auto-generated constructor stub

}

Tuple::~Tuple() {
	// TODO Auto-generated destructor stub
}


/*
 * @return a (deep) copy of this Tuple.
 */
Tuple*
Tuple::clone () const
{
  return new Tuple(*this);
}

/*
 * @return the NUMLTypeCode_t of NUML objects contained in this Tuple or
 * NUML_UNKNOWN (default).
 */
NUMLTypeCode_t
Tuple::getItemTypeCode () const
{
  return NUML_ATOMICVALUE;
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
 * @return the name of this element ie "tuple".
 */
const string&
Tuple::getElementName () const
{
	static const string tuple  = "tuple";
	return tuple;
}

/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
Tuple::getElementPosition () const
{
  return 4;
}

/* return nth AtomicValue in the Tuple list */
AtomicValue *
Tuple::getAtomicValue(unsigned int n)
{
  return static_cast<AtomicValue*>(NUMLList::get(n));
}


/* return nth AtomicValue in the Tuple list */
const AtomicValue *
Tuple::getAtomicValue(unsigned int n) const
{
  return static_cast<const AtomicValue*>(NUMLList::get(n));
}


/* Removes the nth AtomicValue from this Tuple list */
AtomicValue*
Tuple::removeAtomicValue (unsigned int n)
{
   return static_cast<AtomicValue*>(NUMLList::remove(n));
}

/*
 * Creates a new AtomicValue inside this Tuple and returns it.
 */
AtomicValue*
Tuple::createAtomicValue ()
{
	AtomicValue* aValue = 0;

	try
	{
		aValue = new AtomicValue(getNUMLNamespaces());
	}
	catch (...)
	{
		/* here we do not create a default object as the level/version must
		 * match the parent object
		 *
		 * so do nothing
		 */
	}

	if(aValue) this->appendAndOwn(aValue);
	return aValue;
}

/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
Tuple::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
  const string& name   = stream.peek().getName();
 // NMBase*        object = 0;
  AtomicValue *aValue = 0;
  if (name == "atomicValue")
  {

	  stream.next();
	  const string value = stream.next().getCharacters();

	  try
	  {
		 aValue = new AtomicValue(getNUMLNamespaces());
		 aValue->setValue(value);
	  }
	  catch (NUMLConstructorException*)
	  {
		 aValue = new AtomicValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
		 aValue->setValue(value);
	  }
	  catch ( ... )
	  {
		 aValue = new AtomicValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
		 aValue->setValue(value);
	  }
	//  object = aValue;
	  if (aValue) appendAndOwn(aValue);
  }

  return aValue;
}

/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
Tuple::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
{
	NUMLList::readAttributes(attributes);
	//no attributes for Tuple
}

/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void Tuple::writeAttributes(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
	NUMLList::writeAttributes(stream);
	//no attributes for Tuple
}

/*
 * Subclasses should override this method to write out their contained
 * NUML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.
 */
/*void
Tuple::writeElements (XMLOutputStream& stream) const
{
  NUMLList::writeElements(stream);
  write(stream);*/

 // mAtomicValue->writeChars(stream);
  //mAtomicValue->write(stream);
//}

void
Tuple::write(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
 // NUMLList::writeElements(stream);
//  mAtomicValue->writeElements(stream);
  stream.startElement( getElementName() );

    writeAttributes( stream );
    writeElements  ( stream );

    stream.endElement( getElementName() );

 // mAtomicValue->writeChars(stream);
  //mAtomicValue->write(stream);
}



LIBNUML_CPP_NAMESPACE_END
