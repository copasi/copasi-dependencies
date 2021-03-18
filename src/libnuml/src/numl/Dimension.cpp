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

#include <numl/NUMLDocument.h>
#include <numl/Dimension.h>

#include <numl/AtomicValue.h>
#include <numl/CompositeValue.h>
#include <numl/Tuple.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

Dimension::Dimension (unsigned int level, unsigned int version) :
   NUMLList ( level, version )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


Dimension::Dimension (NUMLNamespaces *numlns) :
    NUMLList                   ( numlns )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


/* constructor for validators */
Dimension::Dimension() :
  NUMLList()
{
}

/*
 * Destroys this Dimension.
 */
Dimension::~Dimension ()
{
}


/*
 * Accepts the given NUMLVisitor.
 *
 * @return the result of calling <code>v.visit()</code>, which indicates
 * whether or not the Visitor would like to visit the NUML Doc's next
 * CompositeValue (if available).
 */
bool
Dimension::accept(NUMLVisitor& v) const
{
  v.visit(*this);
 // mCompositeValue.accept(v);
  return true;
}

/*
 * @return a (deep) copy of this Dimension.
 */
Dimension*
Dimension::clone () const
{
	return new Dimension(*this);
}


/**
 * Creates a new CompositeValue and add it to Dimension's list inside this ResultComponent and return it.
 *
 * @return the CompositeValue object created
 *
 * @see addCompositeValue(const CompositeValue *compValue)
 */
CompositeValue*
Dimension::createCompositeValue ()
{
  CompositeValue* value = 0;

  try
  {
    value = new CompositeValue(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }


  if (value) appendAndOwn(value);

  return value;
}

/**
 * creates a new tuple and adds it to the dimension
 * @return the created tuple
 */
Tuple*
Dimension::createTuple()
{
  Tuple* value = 0;

  try
  {
    value = new Tuple(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }


  if (value) appendAndOwn(value);

  return value;
}

/**
 * creates a new atomic value and adds it to the dimension
 * @return the created atomic value
 */
AtomicValue*
Dimension::createAtomicValue()
{
  AtomicValue* value = 0;

  try
  {
    value = new AtomicValue(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }


  if (value) appendAndOwn(value);

  return value;
}


NUMLTypeCode_t Dimension::getTypeCode() const { return NUML_DIMENSION; }


/*
 * @return the NUMLTypeCode_t of NUML objects contained in this Dimension or
 * NUML_UNKNOWN (default).
 */
NUMLTypeCode_t
Dimension::getItemTypeCode () const
{
	return NUML_COMPOSITEVALUE;
}

/*
 * @return the annotation of this NUML object by string.
 */
/* TODO std::string
NMBase::getAnnotationString ()
{
  return XMLNode::convertXMLNodeToString(getAnnotation());
}*/

/*
 * @return the name of this element ie "Dimension".
 */
const string&
Dimension::getElementName () const
{
	static const string name = "dimension";
	return name;
}

/* return nth item in list */
Dimension *
Dimension::get(unsigned int n)
{
	return static_cast<Dimension*>(NUMLList::get(n));
}


/* return nth item in list */
const Dimension *
Dimension::get(unsigned int n) const
{
	return static_cast<const Dimension*>(NUMLList::get(n));
}

/**
* Used by Dimension::get() to lookup an NMBase based by its id.
*/
struct IdEqS
{
	const string& id;

	IdEqS (const string& id) : id(id) { }
	bool operator() (NMBase* sb)
    		{ return static_cast <Dimension *> (sb)->getId() == id; }
};


/* return item by id */
CompositeValue*
Dimension::get (const std::string& sid)
{
	return const_cast<CompositeValue*>(
			static_cast<const Dimension&>(*this).get(sid) );
}


/* return item by id */
const CompositeValue*
Dimension::get (const std::string& sid) const
{
	vector<NMBase*>::const_iterator result;

	result = find_if( mItems.begin(), mItems.end(), IdEqS(sid) );
	return (result == mItems.end()) ? 0 : static_cast <CompositeValue*> (*result);
}


/* Removes the nth item from this list */
Dimension*
Dimension::remove (unsigned int n)
{
	return static_cast<Dimension*>(NUMLList::remove(n));
}


/* Removes item in this list by id */
Dimension*
Dimension::remove (const std::string& sid)
{
	NMBase* item = 0;
	vector<NMBase*>::iterator result;

	result = find_if( mItems.begin(), mItems.end(), IdEqS(sid) );

	if (result != mItems.end())
	{
		item = *result;
		mItems.erase(result);
	}

	return static_cast <Dimension*> (item);
}


/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
Dimension::getElementPosition () const
{
	return 0;
}

/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
Dimension::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
	//const string& name   = stream.peek().getName();

  const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLToken& element = stream.peek();
	const string& name = element.getName();
	NMBase*        object = 0;

	if (name == "compositeValue") // && element.isStart())
	{

    try
		{
			object = new CompositeValue(getNUMLNamespaces());
		}
		catch (NUMLConstructorException*)
		{
		}
		catch ( ... )
		{
		}
		if (object) appendAndOwn(object);


	}
	else if(name == "tuple")
	{
    try
    {
      object = new Tuple(getNUMLNamespaces());
    }
    catch (NUMLConstructorException*)
    {
    }
    catch ( ... )
    {
    }
    if (object) appendAndOwn(object);

	}else if(name == "atomicValue")
	{
    try
    {
      object = new AtomicValue(getNUMLNamespaces());
    }
    catch (NUMLConstructorException*)
    {
    }
    catch ( ... )
    {
    }
    if (object) appendAndOwn(object);

	}

	return object;
}

/*
 * Subclasses should override this method to write out their contained
 * NUML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.
 */
/*void
Dimension::writeElements (XMLOutputStream& stream) const
{
  NUMLList::writeElements(stream);
}*/


LIBNUML_CPP_NAMESPACE_END
