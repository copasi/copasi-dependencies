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

#include <limits>

#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>


#include <numl/NUMLDocument.h>
#include <numl/common/operationReturnValues.h>

#include <numl/NUMLVisitor.h>
#include <numl/NUMLError.h>

#include <numl/ResultComponent.h>
#include <numl/CompositeDescription.h>
#include <numl/CompositeValue.h>

#include <numl/AtomicDescription.h>
#include <numl/AtomicValue.h>

#include <numl/TupleDescription.h>
#include <numl/Tuple.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN


ResultComponent::ResultComponent (unsigned int level, unsigned int version)
  : NMBase ( level, version )
  , mId                       ( ""   )
  , mDimensionDescription(level, version)
  , mDimension(level, version)
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();

  mDimensionDescription.setParentNUMLObject(this);
  mDimension.setParentNUMLObject(this);
}


ResultComponent::ResultComponent (NUMLNamespaces *numlns)
  : NMBase                   ( numlns )
  , mId                       ( ""   )
  , mDimensionDescription(numlns)
  , mDimension(numlns)
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
  mDimensionDescription.setParentNUMLObject(this);
  mDimension.setParentNUMLObject(this);
}

/*
 * Accepts the given NUMLVisitor.
 *
 * @return the result of calling <code>v.visit()</code>, which indicates
 * whether or not the Visitor would like to visit the NUML Doc's next
 * ResultComponent (if available).
 */
bool
ResultComponent::accept (NUMLVisitor& v) const
{
	bool result = v.visit(*this);
	mDimension.accept(v);
	mDimensionDescription.accept(v);
	return result;
}


/*
 * @return a (deep) copy of this ResultComponent.
 */
ResultComponent*
ResultComponent::clone () const
{
  return new ResultComponent(*this);
}


/*
 * @return the id of this NUML object.
 */
const string&
ResultComponent::getId () const
{
  return mId;
}



/* constructor for validators */
ResultComponent::ResultComponent() :
  NMBase()
{
}


/*
 * Destroys this ResultComponent.
 */
ResultComponent::~ResultComponent ()
{
}

/*
 * @return the NUMLTypeCode_t of this NUML object or NUML_UNKNOWN
 * (default).
 *
 * @see getElementName()
 */

NUMLTypeCode_t
ResultComponent::getTypeCode () const
{
	return NUML_RESULTCOMPONENT;
}

/*
 * @return the name of this element ie "resultComponent".
 */
const string&
ResultComponent::getElementName () const
{
	static const string resultComponent  = "resultComponent";
	return resultComponent;
}

/*
 * Creates a new CompositeDescription and add it to DimensionDescription's list inside this ResultComponent and returns it.
 */
CompositeDescription*
ResultComponent::createCompositeDescription ()
{
  CompositeDescription* CompDesc = 0;

  try
  {
    CompDesc = new CompositeDescription(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }

  /* if the NUMLList is empty it doesnt know its parent */
  if (mDimensionDescription.size() == 0)
  {
	  mDimensionDescription.setNUMLDocument(this->getNUMLDocument());
	  mDimensionDescription.setParentNUMLObject(this);
  }


  if (CompDesc) mDimensionDescription.appendAndOwn(CompDesc);

  return CompDesc;
}

/*
 * Creates a new CompositeValue and add it to Dimension's list inside this ResultComponent and returns it.
 */
CompositeValue*
ResultComponent::createCompositeValue ()
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

  /* if the NUMLList is empty it doesn't know its parent */
  if (mDimension.size() == 0)
  {
	  mDimension.setNUMLDocument(this->getNUMLDocument());
	  mDimension.setParentNUMLObject(this);
  }


  if (compValue) mDimension.appendAndOwn(compValue);

  return compValue;
}


TupleDescription*
ResultComponent::createTupleDescription()
{
  TupleDescription* CompDesc = 0;

  try
  {
    CompDesc = new TupleDescription(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }

  /* if the NUMLList is empty it doesnt know its parent */
  if (mDimensionDescription.size() == 0)
  {
    mDimensionDescription.setNUMLDocument(this->getNUMLDocument());
    mDimensionDescription.setParentNUMLObject(this);
  }


  if (CompDesc) mDimensionDescription.appendAndOwn(CompDesc);

  return CompDesc;
}

Tuple*
ResultComponent::createTuple()
{
  Tuple* compValue = 0;

  try
  {
    compValue = new Tuple(getNUMLNamespaces());
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
  if (mDimension.size() == 0)
  {
    mDimension.setNUMLDocument(this->getNUMLDocument());
    mDimension.setParentNUMLObject(this);
  }


  if (compValue) mDimension.appendAndOwn(compValue);

  return compValue;
}

AtomicDescription*
ResultComponent::createAtomicDescription()
{
  AtomicDescription* CompDesc = 0;

  try
  {
    CompDesc = new AtomicDescription(getNUMLNamespaces());
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }

  /* if the NUMLList is empty it doesnt know its parent */
  if (mDimensionDescription.size() == 0)
  {
    mDimensionDescription.setNUMLDocument(this->getNUMLDocument());
    mDimensionDescription.setParentNUMLObject(this);
  }


  if (CompDesc) mDimensionDescription.appendAndOwn(CompDesc);

  return CompDesc;
}

AtomicValue*
ResultComponent::createAtomicValue()
{
  AtomicValue* compValue = 0;

  try
  {
    compValue = new AtomicValue(getNUMLNamespaces());
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
  if (mDimension.size() == 0)
  {
    mDimension.setNUMLDocument(this->getNUMLDocument());
    mDimension.setParentNUMLObject(this);
  }


  if (compValue) mDimension.appendAndOwn(compValue);

  return compValue;
}




/*
 * Creates a new DimensionDescription inside this ResultComponent and returns it.
 */
DimensionDescription*
ResultComponent::createDimensionDescription ()
{
	DimensionDescription* dDesc = 0;

	try
	{
		dDesc = new DimensionDescription(getNUMLNamespaces());
	}
	catch (...)
	{
		// here we do not create a default object as the level/version must match the parent object

	}

	// if the NUMLList is empty it doesnt know its parent
	if (mDimensionDescription.size() == 0)
	{
		mDimensionDescription.setNUMLDocument(this->getNUMLDocument());
		mDimensionDescription.setParentNUMLObject(this);
	}

	if (dDesc) mDimensionDescription.appendAndOwn(dDesc);

	return dDesc;
}

/*
 * @return the Dimension in this ResultComponent or NULL if no such
 * Dimension exists.
 */
Dimension*
ResultComponent::getDimension ()
{
	return &mDimension;
}

/*
 * @return the DimensionDescription in this ResultComponent or NULL if no
 * DimensionDescription exists.
 */
DimensionDescription*
ResultComponent::getDimensionDescription ()
{
	return &mDimensionDescription;
}



/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
ResultComponent::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
{
	NMBase::readAttributes(attributes);

	const unsigned int level   = getLevel  ();
	const unsigned int version = getVersion();

	std::vector<std::string> expectedAttributes;
	expectedAttributes.clear();
	expectedAttributes.push_back("metaid");
	expectedAttributes.push_back("id");


	// check that all attributes are expected
	for (int i = 0; i < attributes.getLength(); i++)
	{
		std::vector<std::string>::const_iterator end = expectedAttributes.end();
		std::vector<std::string>::const_iterator begin = expectedAttributes.begin();

		std::string name = attributes.getName(i);
		if (std::find(begin, end, name) == end)
		{
			logUnknownAttribute(name, level, version, "<resultComponent>");
		}
	}

	const string id = "id";
	bool assigned = attributes.readInto(id, mId, getErrorLog(), true);
	if (assigned && mId.size() == 0)
	{
		logEmptyString(id, level, version, "<resultComponent>");
	}
  if (!LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::isValidSBMLSId(mId)) logError(NUMLInvalidIdSyntax);

}

/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
ResultComponent::writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  NMBase::writeAttributes(stream);

  stream.writeAttribute("id", mId);
}

/*
 * Subclasses should override this method to write out their contained
 * NUML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.
 */
void
ResultComponent::writeElements (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  NMBase::writeElements(stream);
  if (mDimensionDescription.size()!=0){
	  mDimensionDescription.write(stream);
  }


  if (mDimension.size()!=0){
	mDimension.write(stream);
  }

}


/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
ResultComponent::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
	const string& name   = stream.peek().getName();
	NMBase*        object = 0;

	if ( name == "dimension" )
	{
	//	cout<<"Dimension Element is Here";
		if (mDimension.size() != 0)
		{
			logError(NUMLNotSchemaConformant);
		}
		object = &mDimension;
	}
	else if ( name == "dimensionDescription" ){
		//cout<<"Dimension Element is Here";

		if (mDimensionDescription.size() != 0)
		{
			logError(NUMLNotSchemaConformant);
		}
		object = &mDimensionDescription;
	}
	return object;
}



/*
 * @return a (deep) copy of this ResultComponents.
 */
ResultComponents*
ResultComponents::clone () const
{
  return new ResultComponents(*this);
}

/*
 * Sets the id of this NUML object to a copy of sid.
 */
int
ResultComponent::setId (const std::string& sid)
{

  if (!(LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::isValidSBMLSId(sid)))
  {
    return LIBNUML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mId = sid;
    return LIBNUML_OPERATION_SUCCESS;
  }
}

/**
	* Sets the parent NUMLDocument of this NUML object.
	*
	* @param d the NUMLDocument object to use
	*/
void
ResultComponent::setNUMLDocument (NUMLDocument* d)
{
  mDimensionDescription.setNUMLDocument(d);
  mDimension.setNUMLDocument(d);
}



/**
	* Sets the parent NUML object of this NUML object.
	*
	* @param sb the NUML object to use
	*/
void
ResultComponent::setParentNUMLObject (NMBase* sb)
{
  mDimensionDescription.setParentNUMLObject(sb);
  mDimension.setParentNUMLObject(sb);
}


/*
 * @return the NUMLTypeCode_t of NUML objects contained in this ResultComponents or
 * NUML_UNKNOWN (default).
 * @return the annotation of this NUML object by string.
 */
/* TODO std::string
NMBase::getAnnotationString ()
{
  return XMLNode::convertXMLNodeToString(getAnnotation());
}*/

NUMLTypeCode_t
ResultComponents::getItemTypeCode () const
{
  return NUML_RESULTCOMPONENT;
}


/*
 * @return the name of this element ie "ResultComponents".
 */
const string&
ResultComponents::getElementName () const
{
  static const string name = "resultComponents";
  return name;
}


/* return nth item in list */
ResultComponent *
ResultComponents::get(unsigned int n)
{
  return static_cast<ResultComponent*>(NUMLList::get(n));
}


/* return nth item in list */
const ResultComponent *
ResultComponents::get(unsigned int n) const
{
  return static_cast<const ResultComponent*>(NUMLList::get(n));
}


/**
 * Used by ResultComponents::get() to lookup an NMBase based by its id.
 */
struct IdEqS
{
  const string& id;

  IdEqS (const string& id) : id(id) { }
  bool operator() (NMBase* sb)
       { return static_cast <ResultComponent *> (sb)->getId() == id; }
};


/* return all the items */
ResultComponents*
ResultComponents::getResultComponents()
{
  //return &mResultComponents;
   return static_cast<ResultComponents*>(this);
//	return this;
}

/* return item by id */
ResultComponent*
ResultComponents::get (const std::string& sid)
{
  return const_cast<ResultComponent*>(
    static_cast<const ResultComponents&>(*this).get(sid) );
}


/* return item by id */
const ResultComponent*
ResultComponents::get (const std::string& sid) const
{
  vector<NMBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqS(sid) );
  return (result == mItems.end()) ? 0 : static_cast <ResultComponent*> (*result);
}


/* Removes the nth item from this list */
ResultComponent*
ResultComponents::remove (unsigned int n)
{
   return static_cast<ResultComponent*>(NUMLList::remove(n));
}


/* Removes item in this list by id */
ResultComponent*
ResultComponents::remove (const std::string& sid)
{
  NMBase* item = 0;
  vector<NMBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqS(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <ResultComponent*> (item);
}


/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
ResultComponents::getElementPosition () const
{
  return 0;
}


/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
ResultComponents::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
  const string& name   = stream.peek().getName();
  NMBase*        object = 0;


  if (name == "resultComponent")
  {
	  try
	  {
		  object = new ResultComponent(getNUMLNamespaces());
	  }
    catch (NUMLConstructorException*)
    {
      object = new ResultComponent(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }
    catch ( ... )
    {
      object = new ResultComponent(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }

    if (object) appendAndOwn(object);
  }

  return object;
}

LIBNUML_EXTERN
DimensionDescription_t *
ResultComponent_createDimensionDescription(ResultComponent_t *rComp)
{
  return rComp->createDimensionDescription();
}

LIBNUML_EXTERN
CompositeDescription_t *
ResultComponent_createCompositeDescription(ResultComponent_t *rComp)
{
  return rComp->createCompositeDescription();
}

LIBNUML_CPP_NAMESPACE_END
