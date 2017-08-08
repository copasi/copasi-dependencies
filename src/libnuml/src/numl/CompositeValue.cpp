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

#include <numl/NUMLDocument.h>
#include <numl/CompositeValue.h>
#include <numl/AtomicValue.h>
#include <numl/Tuple.h>
#include <numl/common/operationReturnValues.h>


using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

CompositeValue::CompositeValue (unsigned int level, unsigned int version) :
   Dimension ( level, version )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


CompositeValue::CompositeValue (NUMLNamespaces *numlns) :
    Dimension                  ( numlns )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}

CompositeValue::CompositeValue() {
	// TODO Auto-generated constructor stub

}

CompositeValue::~CompositeValue() {
	// TODO Auto-generated destructor stub
}

/*
 * @return a (deep) copy of this CompositeValue.
 */
CompositeValue*
CompositeValue::clone () const
{
  return new CompositeValue(*this);
}

/*
 * Accepts the given NUMLVisitor.
 *
 * @return the result of calling <code>v.visit()</code>, which indicates
 * whether or not the Visitor would like to visit the NUML Doc's next
 * Tuple (if available).
 */
bool
CompositeValue::accept(NUMLVisitor& v) const
{
  v.visit(*this);
  //mTuple.accept(v);
  return true;
}

/*
 * @return the AtomicValue in this CompositeValue or NULL if no such
 * AtomicValue exists.
 */
/*AtomicValue*
CompositeValue::getAtomicValue ()
{*/
	/*if (mContentType == NUML_ATOMICVALUE)
		return static_cast<AtomicValue*> (NUMLList::get(0));
	else
		return NULL;*/
//	return mAtomicValue;
//}*/

/*
 * @return the Tuple in this CompositeValue or NULL if no such
 * AtomicValue exists.
 */
/*Tuple*
CompositeValue::getTuple ()
{
	//if (mContentType == NUML_TUPLE)
	//	return static_cast<Tuple*> (NUMLList::get(0)); //return the only Tuple if there is one
	//else
	//	return NULL;
	return &mTuple;
}*/

/*
 * Adds a copy of the given compositeValue (CompositeValue) to this CompositeValue.
 */
int
CompositeValue::addCompositeValue (const CompositeValue* compValue)
{
  if (compValue == NULL)
  {
    return LIBNUML_OPERATION_FAILED;
  }
  else if (!(compValue->hasRequiredAttributes()) || !(compValue->hasRequiredElements()))
  {
    return LIBNUML_INVALID_OBJECT;
  }
  else if (getLevel() != compValue->getLevel())
  {
    return LIBNUML_LEVEL_MISMATCH;
  }
  else if (getVersion() != compValue->getVersion())
  {
    return LIBNUML_VERSION_MISMATCH;
  }
  else
  {
    this->append(compValue);

    return LIBNUML_OPERATION_SUCCESS;
  }
}

/*
 * set the AtomicValue for this CompositeValue.
 */
/*void
CompositeValue::setAtomicValue(AtomicValue* atomicValue)
{
	mAtomicValue = atomicValue; //introduce error
}*/

/*
 * set the Tuple in this CompositeValue.
 */
/*void
CompositeValue::setTuple(Tuple* tuple)
{

	mTuple = *tuple;
}*/


/* TODO std::string
NMBase::getAnnotationString ()
{
  return XMLNode::convertXMLNodeToString(getAnnotation());
}*/

/*
 * @return the NUMLTypeCode_t of NUML objects contained in this CompositeValue or
 * NUML_UNKNOWN (default).
 * @return the annotation of this NUML object by string.
 */
NUMLTypeCode_t
CompositeValue::getItemTypeCode () const
{
	if (mContentType == NUML_COMPOSITEVALUE)
	{
		return NUML_COMPOSITEVALUE;
	}
	else if (mContentType == NUML_TUPLE)
	{
		return NUML_TUPLE;
	}
	else if (mContentType == NUML_ATOMICVALUE)
	{
		return NUML_ATOMICVALUE;
	}
	else
	{
		return NUML_UNKNOWN;
	}
}

//CompositeValue section

/*
 * @return the name of this element ie "compositeValue".
 */
const string&
CompositeValue::getElementName () const
{
	static const string compValue  = "compositeValue";
	return compValue;
}

/*
 * @return true if the content of CompositeValue obejct is a CompositeValue object, false
 * otherwise.
 */
bool
CompositeValue::isContentCompositeValue () const
{
	if(this->mContentType == NUML_COMPOSITEVALUE)
		return true;
	return false;
}

/*
 * @return true if the content of CompositeValue obejct is a Tuple object, false
 * otherwise.
 */
bool
CompositeValue::isContentTuple () const
{
	if(this->mContentType == NUML_TUPLE)
		return true;
	return false;
}

/*
 * @return true if the content of CompositeValue obejct is a AtomicValue object, false
 * otherwise.
 */
bool
CompositeValue::isContentAtomicValue () const
{
	if(this->mContentType == NUML_ATOMICVALUE)
		return true;
	return false;
}

/* return nth item in list */
CompositeValue *
CompositeValue::getCompositeValue(unsigned int n) {
	if(this->getTypeCode() == NUML_COMPOSITEVALUE)
	return static_cast<CompositeValue*>(NUMLList::get(n));
	return NULL;
}

/* return nth AtomicValue from CompositeValue list, note that only
 * one AtomicValue object is permitted, n = 0 always for atomicValue
 * contained in CompositeValue
 */
AtomicValue *
CompositeValue::getAtomicValue() {
	if(this->size() == 1 && this->mContentType == NUML_ATOMICVALUE)
	{
		return static_cast<AtomicValue*>(NUMLList::get(0));
	}
	return NULL;
}

/* return nth Tuple from CompositeValue list, note that only
 * one Tuple object is permitted, n = 0 always for tuple
 * contained in CompositeValue
 */
Tuple *
CompositeValue::getTuple() {
	if(this->size() == 1  && this->mContentType == NUML_TUPLE)
	{
		return static_cast<Tuple*>(NUMLList::get(0));
	}
	return NULL;
}


/* return nth item in list */
const CompositeValue *
CompositeValue::get(unsigned int n) const
{
	if(this->getTypeCode() == NUML_COMPOSITEVALUE)
	{
   return static_cast<const CompositeValue*>(NUMLList::get(n));
	}
	return NULL;
}

/* return nth item in list */
CompositeValue *CompositeValue::get(unsigned int n)
{
	if(this->getTypeCode() == NUML_COMPOSITEVALUE)
	{
		return static_cast<CompositeValue*>(NUMLList::get(n));
	}

  return NULL;
}


/* Removes the nth item from this list */
CompositeValue*
CompositeValue::remove (unsigned int n)
{
   return static_cast<CompositeValue*>(NUMLList::remove(n));
	//else return NULL;
}

/*
 * Sets the indexValue attribute of this NUML object to a copy of indexValue.
 */
int CompositeValue::setIndexValue(const std::string& indexValue) {
	mIndexValue = indexValue;
	return LIBNUML_OPERATION_SUCCESS;

}


/*
 * @return the indexValue of this NUML object.
 */
const string&
CompositeValue::getIndexValue () const
{
  return mIndexValue;
}

/*
 * Sets the indexValue attribute of this NUML object to a copy of indexValue.
 */
int CompositeValue::setDescription(const std::string& description) {
	mDescription = description;
	return LIBNUML_OPERATION_SUCCESS;

}
/*
 * @return the description of this NUML object.
 */
const string&
CompositeValue::getDescription () const
{
  return mDescription;
}

/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
CompositeValue::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
{
	NUMLList::readAttributes(attributes);

	const unsigned int level   = getLevel  ();
	const unsigned int version = getVersion();

	std::vector<std::string> expectedAttributes;
	expectedAttributes.clear();
	expectedAttributes.push_back("metaid");
	expectedAttributes.push_back("indexValue");
	expectedAttributes.push_back("description");

	// check that all attributes are expected
	for (int i = 0; i < attributes.getLength(); i++)
	{
		std::vector<std::string>::const_iterator end = expectedAttributes.end();
		std::vector<std::string>::const_iterator begin = expectedAttributes.begin();

		std::string name = attributes.getName(i);
		if (std::find(begin, end, name) == end)
		{
			logUnknownAttribute(name, level, version, "<compositeValue>");
		}
	}
	attributes.readInto("indexValue", mIndexValue);
	attributes.readInto("description", mDescription);
}

/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
CompositeValue::writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  NUMLList::writeAttributes(stream);

  //const unsigned int level   = getLevel  ();
  //const unsigned int version = getVersion();

  stream.writeAttribute("indexValue", mIndexValue);
  stream.writeAttribute("description", mDescription);
}

/*
 * Creates a new CompositeValue inside this CompositeValue, add to its list and returns it.
 */
CompositeValue*
CompositeValue::createCompositeValue ()
{
  CompositeValue* compValue = 0;

  try
  {
    compValue = new CompositeValue(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }
   	if(compValue) this->appendAndOwn(compValue);
  return compValue;
}

/*
 * Creates a new Tuple inside this CompositeValue and returns it.
 */
Tuple*
CompositeValue::createTuple ()
{
	Tuple* tuple = 0;

	try
	{
		tuple = new Tuple(getNUMLNamespaces());
	}
	catch (...)
	{
		/* here we do not create a default object as the level/version must
		 * match the parent object
		 *
		 * so do nothing
		 */
	}
	if(tuple) this->appendAndOwn(tuple);
	return tuple;
}

/*
 * Creates a new AtomicValue to this CompositeValue and returns it.
 */
AtomicValue*
CompositeValue::createAtomicValue ()
{
	AtomicValue* aValue = 0;

	try
	{
		//mAtomicValue = new AtomicValue(getNUMLNamespaces());
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
CompositeValue::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
  const string& name   = stream.peek().getName();
  NMBase*        object = 0;

  if (name == "compositeValue")
  {
	  this->mContentType = NUML_COMPOSITEVALUE;
	 try
	  {
		 object = new CompositeValue(getNUMLNamespaces());
	  }
	  catch (NUMLConstructorException*)
	  {
		   object = new CompositeValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
	  }
	  catch ( ... )
	  {
		  object = new CompositeValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
	  }

	 if (object) appendAndOwn(object);

  }
  else if (name == "atomicValue")
  {

	  this->mContentType = NUML_ATOMICVALUE;
	  stream.next();
	  const string value = stream.next().getCharacters();
	  try
	  {

		 object = new AtomicValue(getNUMLNamespaces());
		 static_cast<AtomicValue*>(object)->setValue(value);

	  }
	  catch (NUMLConstructorException*)
	  {
		 object = new AtomicValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
		 static_cast<AtomicValue*>(object)->setValue(value);
	  }
	  catch ( ... )
	  {

		  object = new AtomicValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
		  static_cast<AtomicValue*>(object)->setValue(value);
	  }

	 if (object) appendAndOwn(object);
  }
  else if (name == "tuple")
  {

	  this->mContentType = NUML_TUPLE;

	  try
	 	  {

	 	  object = new Tuple(getNUMLNamespaces());
	 	  }
	 	  catch (NUMLConstructorException*)
	 	  {
	 		  object = new Tuple(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());

	 	  }
	 	  catch ( ... )
	 	  {
	 		  object = new Tuple(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
	 	  }

	 	 if (object) appendAndOwn(object);
	/*  {
		  if (mTuple.size() != 0)
		  {
			  logError(NUMLNotSchemaConformant);
		  }
		  object = &mTuple;
	  }*/
  }
  return object;
}

LIBNUML_CPP_NAMESPACE_END
