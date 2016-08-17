/**
* Begin svn Header
* $Rev$:	Revision of last commit
* $Author$:	Author of last commit
* $Date$:	Date of last commit
* $HeadURL$
* $Id$
* End svn Header
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

/** @cond doxygen-libnuml-internal */

/* constructor for validators */
Dimension::Dimension() :
  NUMLList()
{
}

/** @endcond doxygen-libnuml-internal */

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
/*CompositeValue *
Dimension::get(unsigned int n)
{
	return static_cast<CompositeValue*>(NUMLList::get(n));
}
*/

/* return nth item in list */
/*const CompositeValue *
Dimension::get(unsigned int n) const
{
	return static_cast<const CompositeValue*>(NUMLList::get(n));
}*/

/* return nth item in list */
CompositeValue *
Dimension::get(unsigned int n)
{
	return static_cast<CompositeValue*>(NUMLList::get(n));
}


/* return nth item in list */
const CompositeValue *
Dimension::get(unsigned int n) const
{
	return static_cast<const CompositeValue*>(NUMLList::get(n));
}

/*
 * @return the CompositeValue in this Dimension or NULL if no such
 * CompositeValue exists.
 */
CompositeValue*
Dimension::getCompositeValue()
{
	return 0; //&mCompositeValue;
}

/**
* Used by Dimension::get() to lookup an NMBase based by its id.
*/
struct IdEqS : public unary_function<NMBase*, bool>
{
	const string& id;

	IdEqS (const string& id) : id(id) { }
	bool operator() (NMBase* sb)
    		{ return static_cast <CompositeValue *> (sb)->getId() == id; }
};


/* return all the items */
/*Dimension*
Dimension::getDimension()
{
  //return &mDimension;
   return static_cast<Dimension*>(this);
//	return this;
}*/

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
CompositeValue*
Dimension::remove (unsigned int n)
{
	return static_cast<CompositeValue*>(NUMLList::remove(n));
}


/* Removes item in this list by id */
CompositeValue*
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

	return static_cast <CompositeValue*> (item);
}


/** @cond doxygen-libnuml-internal */
/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
Dimension::getElementPosition () const
{
	return 0;
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
Dimension::createObject (XMLInputStream& stream)
{
	//const string& name   = stream.peek().getName();

	const XMLToken& element = stream.peek();
	const string& name = element.getName();
	NMBase*        object = 0;

	if (name == "compositeValue") // && element.isStart())
	{
	//	while(name == "compositeValue" && element.isEndFor(element)){}
		try
		{
		//	object = &mCompositeValue;
			object = new CompositeValue(getNUMLNamespaces());
		}
		catch (NUMLConstructorException*)
		{
		//	object = new CompositeValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
		}
		catch ( ... )
		{
		//	object = new CompositeValue(NUMLDocument::getDefaultLevel(), NUMLDocument::getDefaultVersion());
		}
		if (object) mItems.push_back(object);

		/*{
				  if (mCompositeValue.size() != 0)
				  {
					  logError(NUMLNotSchemaConformant);
				  }
				  object = &mCompositeValue;
			  }*/

	}
	else if(name == "tuple")
	{

	}else if(name == "atomicValue")
	{

	}

	return object;
}
/** @endcond doxygen-libnuml-internal */

/** @cond doxygen-libnuml-internal */
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
/** @endcond doxygen-libnuml-internal */


LIBNUML_CPP_NAMESPACE_END
