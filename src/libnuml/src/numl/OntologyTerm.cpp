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
#include <numl/OntologyTerm.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

OntologyTerm::OntologyTerm (unsigned int level, unsigned int version) :
   NMBase ( level, version )
  , mId                       ( ""   )
  , mTerm                     ( ""   )
  , mSourceTermId          	  ( ""   )
  , mOntologyURI              ( ""   )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}


OntologyTerm::OntologyTerm (NUMLNamespaces *numlns) :
    NMBase                   ( numlns )
   ,mId                       ( ""   )
   ,mTerm                     ( ""   )
   ,mSourceTermId          	  ( ""   )
   ,mOntologyURI              ( ""   )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}

/*
 * Accepts the given NUMLVisitor.
 *
 * @return the result of calling <code>v.visit()</code>, which indicates
 * whether or not the Visitor would like to visit the NUML Doc's next
 * OntologyTerm (if available).
 */
bool
OntologyTerm::accept (NUMLVisitor& v) const
{
  return v.visit(*this);
}


/*
 * @return a (deep) copy of this OntologyTerm.
 */
OntologyTerm*
OntologyTerm::clone () const
{
  return new OntologyTerm(*this);
}

/*
 * Sets the id of this NUML object to a copy of sid.
 */
int
OntologyTerm::setId (const std::string& sid)
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


/*
 * Sets the term attribute of this NUML object to a copy of term.
 */
int OntologyTerm::setTerm(const std::string& term) {
	mTerm = term;
	return LIBNUML_OPERATION_SUCCESS;

}


/*
 * Sets the ontologyURI attribute of this NUML object to a copy of ontologyURI.
 */
int OntologyTerm::setOntologyURI(const std::string& ontologyURI) {
	mOntologyURI = ontologyURI;
	return LIBNUML_OPERATION_SUCCESS;

}


/*
 * Sets the sourceTermId attribute of this NUML object to a copy of sourceTermId.
 */
int OntologyTerm::setSourceTermId(const std::string& sourceTermId) {
	mSourceTermId = sourceTermId;
	return LIBNUML_OPERATION_SUCCESS;

}

/*
 * @return the id of this NUML object.
 */
const string&
OntologyTerm::getId () const
{
  return mId;
}


/*
 * @return the term of this NUML object.
 */
const string&
OntologyTerm::getTerm () const
{
  return mTerm;
}

/*
 * @return the sourceTermId of this NUML object.
 */
const string&
OntologyTerm::getSourceTermId () const
{
  return mSourceTermId;
}

/*
 * @return the ontologyURI of this NUML object.
 */
const string&
OntologyTerm::getOntologyURI () const
{
  return mOntologyURI;
}

/* constructor for validators */
OntologyTerm::OntologyTerm() :
  NMBase()
{
}


/*
 * Destroys this OntologyTerm.
 */
OntologyTerm::~OntologyTerm ()
{
}

/*
 * @return the NUMLTypeCode_t of this NUML object or NUML_UNKNOWN
 * (default).
 *
 * @see getElementName()
 */
NUMLTypeCode_t
OntologyTerm::getTypeCode () const
{
	return NUML_ONTOLOGYTERM;
}


/*
 * @return the name of this element ie "ontologyTerm".
 */
const string&
OntologyTerm::getElementName () const
{
	static const string ontologyTerm  = "ontologyTerm";
	return ontologyTerm;
}


/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
OntologyTerm::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
{
	NMBase::readAttributes(attributes);

	const unsigned int level   = getLevel  ();
	const unsigned int version = getVersion();

	std::vector<std::string> expectedAttributes;
	expectedAttributes.clear();
	expectedAttributes.push_back("metaid");
	expectedAttributes.push_back("id");
	expectedAttributes.push_back("term");
	expectedAttributes.push_back("sourceTermId");
	expectedAttributes.push_back("ontologyURI");

	// check that all attributes are expected
	for (int i = 0; i < attributes.getLength(); i++)
	{
		std::vector<std::string>::const_iterator end = expectedAttributes.end();
		std::vector<std::string>::const_iterator begin = expectedAttributes.begin();

		std::string name = attributes.getName(i);
		if (std::find(begin, end, name) == end)
		{
			logUnknownAttribute(name, level, version, "<ontologyTerm>");
		}
	}

	const string id = "id";
	bool assigned = attributes.readInto(id, mId, getErrorLog(), true);
	if (assigned && mId.size() == 0)
	{
		logEmptyString(id, level, version, "<ontologyTerm>");
	}
  if (!LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::isValidSBMLSId(mId)) logError(NUMLInvalidIdSyntax);

	attributes.readInto("term", mTerm);
	attributes.readInto("sourceTermId", mSourceTermId);
	attributes.readInto("ontologyURI", mOntologyURI);
}


/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
OntologyTerm::writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  NMBase::writeAttributes(stream);

  //const unsigned int level   = getLevel  ();
  //const unsigned int version = getVersion();

  stream.writeAttribute("id", mId);
  stream.writeAttribute("term", mTerm);
  stream.writeAttribute("sourceTermId", mSourceTermId);
  stream.writeAttribute("ontologyURI", mOntologyURI);

}




/*
 * @return a (deep) copy of this OntologyTerms.
 */
OntologyTerms*
OntologyTerms::clone () const
{
  return new OntologyTerms(*this);
}


/*
 * @return the NUMLTypeCode_t of NUML objects contained in this OntologyTerms or
 * NUML_UNKNOWN (default).
 * @return the annotation of this NUML object by string.
 */
/* TODO std::string
NMBase::getAnnotationString ()
{
  return XMLNode::convertXMLNodeToString(getAnnotation());
}*/

NUMLTypeCode_t
OntologyTerms::getItemTypeCode () const
{
  return NUML_ONTOLOGYTERM;
}


/*
 * @return the name of this element ie "ontologyTerms".
 */
const string&
OntologyTerms::getElementName () const
{
  static const string name = "ontologyTerms";
  return name;
}


/* return nth item in list */
OntologyTerm *
OntologyTerms::get(unsigned int n)
{
  return static_cast<OntologyTerm*>(NUMLList::get(n));
}


/* return nth item in list */
const OntologyTerm *
OntologyTerms::get(unsigned int n) const
{
  return static_cast<const OntologyTerm*>(NUMLList::get(n));
}


/**
 * Used by OntologyTerms::get() to lookup an NMBase based by its id.
 */
struct IdEqS
{
  const string& id;

  IdEqS (const string& id) : id(id) { }
  bool operator() (NMBase* sb)
       { return static_cast <OntologyTerm *> (sb)->getId() == id; }
};


/* return item by id */
OntologyTerm*
OntologyTerms::get (const std::string& sid)
{
  return const_cast<OntologyTerm*>(
    static_cast<const OntologyTerms&>(*this).get(sid) );
}


/* return item by id */
const OntologyTerm*
OntologyTerms::get (const std::string& sid) const
{
  vector<NMBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqS(sid) );
  return (result == mItems.end()) ? 0 : static_cast <OntologyTerm*> (*result);
}


/* Removes the nth item from this list */
OntologyTerm*
OntologyTerms::remove (unsigned int n)
{
   return static_cast<OntologyTerm*>(NUMLList::remove(n));
}


/* Removes item in this list by id */
OntologyTerm*
OntologyTerms::remove (const std::string& sid)
{
  NMBase* item = 0;
  vector<NMBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqS(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <OntologyTerm*> (item);
}

/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
OntologyTerms::getElementPosition () const
{
  return 1;
}


/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
OntologyTerms::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
  const string& name   = stream.peek().getName();
  NMBase*        object = 0;

  if (name == "ontologyTerm")
  {
    try
    {
      object = new OntologyTerm(getNUMLNamespaces());
    }
    catch (NUMLConstructorException*)
    {
      object = new OntologyTerm(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }
    catch ( ... )
    {
      object = new OntologyTerm(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
    }

    if (object) mItems.push_back(object);
  }

  return object;
}

LIBNUML_CPP_NAMESPACE_END
