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

#include <sstream>

#include <sbml/xml/XMLError.h>
#include <sbml/xml/XMLErrorLog.h>
#include <sbml/xml/XMLOutputStream.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLToken.h>
#include <sbml/xml/XMLNode.h>

#include <sbml/util/util.h>

/*#include <numl/xml/XMLError.h>
#include <numl/xml/XMLErrorLog.h>
#include <numl/xml/XMLOutputStream.h>
#include <numl/xml/XMLInputStream.h>
#include <numl/xml/XMLToken.h>
#include <numl/xml/XMLNode.h>

#include <numl/util/util.h> */

#include <numl/NUMLError.h>
#include <numl/NUMLErrorLog.h>
#include <numl/NUMLDocument.h>
#include <numl/ResultComponent.h>
#include <numl/DimensionDescription.h>
#include <numl/NUMLList.h>
#include <numl/NMBase.h>

/*#ifdef USE_LAYOUT
 #include <numl/layout/LineSegment.h>
#endif
*/
/** @cond doxygen-ignored */

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

/** @endcond doxygen-ignored */

/**
 * elements permitted on the body element of xhtml
 */

/** @cond doxygen-libnuml-internal */

NUMLConstructorException::NUMLConstructorException() :
      std::invalid_argument("Level/version/namespaces combination is invalid")
{
}


/** @endcond doxygen-libnuml-internal */

/** @cond doxygen-libnuml-internal */
/*
 * Only subclasses may create NMBase objects.
 */
NMBase::NMBase (const std::string& id, const std::string& name) :
   mNUML      ( 0 )
 , mNUMLNamespaces (0)
 , mLine      ( 0 )
 , mColumn    ( 0 )
 , mParentNUMLObject (0)
 , mHasBeenDeleted (false)
 , mEmptyString ("")
{
  mNUMLNamespaces = new NUMLNamespaces();
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Creates a new NMBase object with the given sboTerm.
 * Only subclasses may create NMBase objects.
 */
NMBase::NMBase (unsigned int level, unsigned int version) :
   mNUML      ( 0 )
 , mNUMLNamespaces (0)
 , mLine      ( 0 )
 , mColumn    ( 0 )
 , mDept      ( 0 )
 , mParentNUMLObject (0)
 , mHasBeenDeleted (false)
 , mEmptyString ("")
{
  mNUMLNamespaces = new NUMLNamespaces(level, version);
}
/*
 * Creates a new NMBase object with the given NUMLNamespaces.
 * Only subclasses may create NMBase objects.
 */
NMBase::NMBase (NUMLNamespaces *numlns) :
   mNUML      ( 0 )
 , mNUMLNamespaces (0)
 , mLine      ( 0 )
 , mColumn    ( 0 )
 , mDept     ( 0 )
 , mParentNUMLObject (0)
 , mHasBeenDeleted (false)
 , mEmptyString ("")
{
  if (!numlns) throw NUMLConstructorException();
  mNUMLNamespaces = numlns->clone();
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Copy constructor. Creates a copy of this NMBase object.
 */
NMBase::NMBase(const NMBase& orig)
{
  this->mMetaId = orig.mMetaId;
  
  this->mNUML       = NULL;
  this->mLine       = orig.mLine;
  this->mColumn     = orig.mColumn;
  this->mParentNUMLObject = NULL;

  if(orig.mNUMLNamespaces)
    this->mNUMLNamespaces =
    new NUMLNamespaces(*const_cast<NMBase&>(orig).mNUMLNamespaces);
  else
    this->mNUMLNamespaces = 0;

  this->mHasBeenDeleted = false;

}
/** @endcond doxygen-libnuml-internal */


/*
 * Destroy this NMBase object.
 */
NMBase::~NMBase ()
{
  if (mNUMLNamespaces)  delete mNUMLNamespaces;

  mHasBeenDeleted = true;
}

/*
 * Assignment operator
 */
NMBase& NMBase::operator=(const NMBase& orig)
{
  if(&orig!=this)
  {
    this->mMetaId = orig.mMetaId;

    this->mNUML       = orig.mNUML;
    this->mLine       = orig.mLine;
    this->mColumn     = orig.mColumn;
    this->mParentNUMLObject = orig.mParentNUMLObject;

    delete this->mNUMLNamespaces;

    if(orig.mNUMLNamespaces)
      this->mNUMLNamespaces =
      new NUMLNamespaces(*const_cast<NMBase&>(orig).mNUMLNamespaces);
    else
      this->mNUMLNamespaces = 0;

    this->mHasBeenDeleted = orig.mHasBeenDeleted;
  }

  return *this;
}


/*
 * @return the metaid of this NUML object.
 */
const string&
NMBase::getMetaId () const
{
  return mMetaId;
}


/*
 * @return the metaid of this NUML object.
 */
string&
NMBase::getMetaId ()
{
  return mMetaId;
}


/** @cond doxygen-libnuml-internal */

/*
 * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
 *
 * @return the id of this NUML object.
 */
const string&
NMBase::getId () const
{
  return mEmptyString;
}



/*
 * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
 *
 * @return the name of this NUML object.
 */
const string&
NMBase::getName () const
{
  return mEmptyString;
}

/*
 * @return the Namespaces associated with this NUML object
 */
LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces*
NMBase::getNamespaces() const
{
  if (mNUML)
    return mNUML->getNUMLNamespaces()->getNamespaces();
  else
    return mNUMLNamespaces->getNamespaces();
}


/*
 * @return the parent NUMLDocument of this NUML object.
 */
const NUMLDocument*
NMBase::getNUMLDocument () const
{
  if (mNUML != NULL)
  {
    // if the doc object has been deleted the pointer is
    // still valid but points to nothing
    try
    {
      if (mNUML->getHasBeenDeleted())
      {
        return NULL;
      }
      else
      {
        return mNUML;
      }
    }
    catch ( ... )
    {
      return NULL;
    }
  }
  return mNUML;
}

/*
 * @return the parent NUMLDocument of this NUML object.
 */
NUMLDocument*
NMBase::getNUMLDocument ()
{
  if (mNUML != NULL)
  {
    // if the doc object has been deleted the pointer is
    // still valid but points to nothing
    try
    {
      if (mNUML->getHasBeenDeleted())
      {
        return NULL;
      }
      else
      {
        return mNUML;
      }
    }
    catch ( ... )
    {
      return NULL;
    }
  }
  return mNUML;
}
NMBase*
NMBase::getParentNUMLObject ()
{
  if (mParentNUMLObject != NULL)
  {
    // if the parent object has been deleted the pointer is
    // still valid but points to nothing
    try 
    {
      if (mParentNUMLObject->getHasBeenDeleted())
      {
        return NULL;
      }
      else
      {
        return mParentNUMLObject;
      }
    }
    catch ( ... )
    {
      return NULL;
    }
  }

  return mParentNUMLObject;
}

/*
 * @return the line number of this NUML object.
 */
unsigned int
NMBase::getLine () const
{
  return mLine;
}


/*
 * @return the column number of this NUML object.
 */
unsigned int
NMBase::getColumn () const
{
  return mColumn;
}


/*
 * @return the list of CVTerms for this NUML object.
 */
LIBSBML_CPP_NAMESPACE_QUALIFIER List*
NMBase::getCVTerms()
{
  return mCVTerms;
}


/*
 * @return the list of CVTerms for this NUML object.
 */
LIBSBML_CPP_NAMESPACE_QUALIFIER List*
NMBase::getCVTerms() const
{
  return mCVTerms;
}

/*
 * @return true if the metaid of this NUML object has been set, false
 * otherwise.
 */
bool
NMBase::isSetMetaId () const
{
  return (mMetaId.empty() == false);
}

/*
 * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
 *
 * @return true if the name of this NUML object has been set, false
 * otherwise.
 */
bool
NMBase::isSetName () const
{
  return (getName().empty() == false);
}

/*
 * Sets the metaid field of the given NUML object to a copy of metaid.
 */
int
NMBase::setMetaId (const std::string& metaid)
{
  if (getLevel() == 1)
  {
    return LIBNUML_UNEXPECTED_ATTRIBUTE;
  }
  else if (metaid.empty())
  {
    mMetaId.erase();
    return LIBNUML_OPERATION_SUCCESS;
  }
  else if (!(LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::isValidXMLID(metaid)))
  {
    return LIBNUML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mMetaId = metaid;
    return LIBNUML_OPERATION_SUCCESS;
  }
}


/** @cond doxygen-libnuml-internal */

/*
 * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
 *
 * Sets the id of this NUML object to a copy of sid.
 */
int
NMBase::setId (const std::string& sid)
{
  return LIBNUML_UNEXPECTED_ATTRIBUTE;
}

/*
 * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
 * Sets the name of this NUML object to a copy of name.
 */
/*int
NMBase::setName (const std::string& name)
{
  NUMLTypeCode_t type = this->getTypeCode();

  return static_cast <Model *> (this)->setName(name);
}
*/

/** @cond doxygen-libnuml-internal */

/*
 * Sets the parent NUMLDocument of this NUML object.
 */
void
NMBase::setNUMLDocument (NUMLDocument* d)
{
  mNUML = d;
}

bool
NMBase::hasValidLevelVersionNamespaceCombination()
{
  bool valid = true;
  bool numlDeclared = false;
  std::string declaredURI("");
  //unsigned int index = 0;
  //unsigned int version = getVersion();
  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces *xmlns = getNamespaces();
  if (xmlns)
  {
    //
    // checks defined NUML XMLNamespace
    // returns false if different NUML XMLNamespaces
    //
    int numNS = 0;

    if (xmlns->hasURI(NUML_XMLNS_L1))
    {
      // checks different NUML XMLNamespaces
      if (numNS > 0) return false;
      ++numNS;
      declaredURI.assign(NUML_XMLNS_L1);
    }

    // checks if the NUML Namespace is explicitly defined.
    for (int i=0; i < xmlns->getLength(); i++)
    {
      if (!xmlns->getPrefix(i).empty() &&
                      xmlns->getURI(i) == declaredURI)
      {
        numlDeclared = true;
        break;
      }
    }

  }
  return valid;
}

/** @cond doxygen-libnuml-internal */


/**
  * Sets the parent NUML object of this NUML object.
  *
  * @param sb the NUML object to use
  */
void 
NMBase::setParentNUMLObject (NMBase* sb)
{
  mParentNUMLObject = sb;
}
/** @endcond doxygen-libnuml-internal */

NMBase*
NMBase::getAncestorOfType(NUMLTypeCode_t type)
{
  if (type == NUML_DOCUMENT)
    return getNUMLDocument();

  NMBase *child = this;
  NMBase *parent = getParentNUMLObject();

  while (parent != NULL && parent->getTypeCode() != NUML_DOCUMENT)
  {
    if (parent->getTypeCode() == type)
      return parent;
    else
    {
      child = parent;
      parent = child->getParentNUMLObject();
    }
  }

  // if we get here we havent found an ancestor of this type
  return NULL;

}


/*
 * Sets the namespaces relevant of this NUML object.
 *
 * @param xmlns the namespaces to set
 */
int 
NMBase::setNamespaces(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* xmlns)
{
  if (xmlns == NULL)
  {
    mNUMLNamespaces->setNamespaces(NULL);
    return LIBNUML_OPERATION_SUCCESS;
  }
  else
  {
    mNUMLNamespaces->setNamespaces(xmlns);
    return LIBNUML_OPERATION_SUCCESS;
  }
}



/*
 * Unsets the metaid of this NUML object.
 */
int
NMBase::unsetMetaId ()
{
  /* only in L2 onwards */
  if (getLevel() < 2)
  {
    return LIBNUML_UNEXPECTED_ATTRIBUTE;
  }

  mMetaId.erase();

  if (mMetaId.empty())
  {
    return LIBNUML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBNUML_OPERATION_FAILED;
  }
}


/*
 * @return the NUML level of this NUML object.
 */
unsigned int
NMBase::getLevel () const
{
  if (mNUML)
    return mNUML->mLevel;
  else if (mNUMLNamespaces != 0)
    return mNUMLNamespaces->getLevel();
  else
    return NUMLDocument::getDefaultLevel();
}


/*
 * @return the NUML version of this NUML object.
 */
unsigned int
NMBase::getVersion () const
{
  if (mNUML)
    return mNUML->mVersion;
  else if (mNUMLNamespaces != 0)
    return mNUMLNamespaces->getVersion();
  else
    return NUMLDocument::getDefaultVersion();
}

/*
 * @return the typecode (int) of this NUML object orNUML_UNKNOWN
 * (default).
 *
 * This method MAY return the typecode of this NUML object or it MAY
 * returnNUML_UNKNOWN.  That is, subclasses of NMBase are not required to
 * implement this method to return a typecode.  This method is meant
 * primarily for the LibNUML C interface where class and subclass
 * information is not readily available.
 *
 * @see getElementName()
 */
/*int
NMBase::getTypeCode () const
{
  return NUML_UNKNOWN;
}*/

/*
 * @return the NUMLTypeCode_t of this NUML object or NUML_UNKNOWN
 * (default).
 *
 * This method MAY return the typecode of this NUML object or it MAY
 * return NUML_UNKNOWN.  That is, subclasses of NMBase are not required to
 * implement this method to return a typecode.  This method is meant
 * primarily for the LibNUML C interface where class and subclass
 * information is not readily available.
 *
 * @see getElementName()
 */
NUMLTypeCode_t
NMBase::getTypeCode () const
{
  return NUML_UNKNOWN;
}


/** @cond doxygen-libnuml-internal */

/* sets the NUMLnamespaces - internal use only*/
void 
NMBase::setNUMLNamespaces(NUMLNamespaces * numlns)
{
  delete mNUMLNamespaces;
  if (numlns)
    mNUMLNamespaces = numlns->clone();
  else
    mNUMLNamespaces = 0;
}

/* gets the NUMLnamespaces - internal use only*/
NUMLNamespaces *
NMBase::getNUMLNamespaces() const
{
  if (mNUML)
    return mNUML->mNUMLNamespaces;
  else if (mNUMLNamespaces != 0)
    return mNUMLNamespaces;
  else
    return new NUMLNamespaces();
}

/** @endcond doxygen-libnuml-internal */



/*
 * @return the partial NUML that describes this NUML object.
 */
char*
NMBase::toNUML ()
{
  ostringstream    os;
  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream  stream(os, "UTF-8", false);

  write(stream);

  return LIBSBML_CPP_NAMESPACE_QUALIFIER safe_strdup( os.str().c_str() );
}


/** @cond doxygen-libnuml-internal */
/*
 * Reads (initializes) this NUML object by reading from XMLInputStream.
 */
void
NMBase::read (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
  if ( !stream.peek().isStart() ) return;

  const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLToken  element  = stream.next();
  int             position =  0;

  setNMBaseFields( element );
  readAttributes( element.getAttributes() );

  if (element.getName() == "numl")
  {
  }
  else
  {
    //
    // checks if the given default namespace (if any) is a valid
    // NUML namespace
    //
    checkDefaultNamespace(mNUMLNamespaces->getNamespaces(), element.getName());
  }

  if ( element.isEnd() ) return;

  while ( stream.isGood() )
  {
    stream.skipText();
    const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLToken& next = stream.peek();

    // Re-check stream.isGood() because stream.peek() could hit something.
    if ( !stream.isGood() ) break;

    if ( next.isEndFor(element) )
    {
      stream.next();
      break;
    }
    else if ( next.isStart() )
    {
      NMBase * object = createObject(stream);

      if (object)
      {
        //TODO
      //  checkOrderAndLogError(object, position);
        position = object->getElementPosition();

        object->setNUMLDocument(mNUML);
        object->setParentNUMLObject(static_cast <NMBase*>(this));

        object->read(stream);

        if ( !stream.isGood() ) break;

      /*  if (object->getTypeCode() == NUML_SPECIES_REFERENCE
            && object->getLevel() > 1)
        {
          static_cast <SpeciesReference *> (object)->sortMath();
        }*/
        checkNUMLListPopulated(object);
      }
      else if ( !( readOtherXML(stream)
          || readAnnotation(stream)
          || readNotes(stream) ))
      {
        logUnknownElement(next.getName(), getLevel(), getVersion());
        stream.skipPastEnd( stream.next() );
      }
    }
    else
    {
      stream.skipPastEnd( stream.next() );
    }
  }
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Writes (serializes) this NUML object by writing it to XMLOutputStream.
 */
void
NMBase::write (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  stream.startElement( getElementName() );

  writeXMLNS( stream );
  writeAttributes( stream );
  writeElements  ( stream );

  //check if the NUML object is AtomicValue and write the data as character to output
  NUMLTypeCode_t type = this->getTypeCode();
  if(type==NUML_ATOMICVALUE) static_cast<const AtomicValue*>(this)->writeChars(stream);

  stream.endElement( getElementName() );
}

/** @endcond doxygen-libnuml-internal */


void 
NMBase::writeXMLNS(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{

}

/** @cond doxygen-libnuml-internal */
/*
 * Subclasses should override this method to write out their contained
 * NUML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.
 */
void
NMBase::writeElements (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& ) const
{
//  if ( mNotes      ) stream << *mNotes;

  /*
   * NOTE: CVTerms on a model have already been dealt with
   */

//TODO  const_cast <NMBase *> (this)->syncAnnotation();
 // if (mAnnotation) stream << *mAnnotation;
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Subclasses should override this method to create, store, and then
 * return an NUML object corresponding to the next XMLToken in the
 * XMLInputStream.
 *
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
NMBase::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream&)
{
  return 0;
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * @return true if read an <annotation> element from the stream
 */
bool
NMBase::readAnnotation (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
  const string& name = stream.peek().getName();

  if (name == "annotation"
    || (getLevel() == 1 && getVersion() == 1 && name == "annotations"))
  {
//    XMLNode* new_annotation = NULL;
    // If this is a level 1 document then annotations are not allowed on
    // the numl container
    if (getLevel() == 1 && getTypeCode() == NUML_DOCUMENT)
    {
      logError(NUMLAnnotationNotesNotAllowedLevel1);
    }


    // If an annotation already exists, log it as an error and replace
    // the content of the existing annotation with the new one.

    if (mAnnotation)
    {
      logError(NUMLNotSchemaConformant, getLevel(), getVersion(),
         "Only one <annotation> element is permitted inside any "
         "particular containing element.");
    }

    delete mAnnotation;
    mAnnotation = new LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode(stream);
    checkAnnotation();

    //TODO
   /* if(mCVTerms)
    {
      unsigned int size = mCVTerms->getSize();
      while (size--) delete static_cast<CVTerm*>( mCVTerms->remove(0) );
      delete mCVTerms;
    }
    mCVTerms = new List();
    RDFAnnotationParser::parseRDFAnnotation(mAnnotation, mCVTerms);  */
//    new_annotation = RDFAnnotationParser::deleteRDFAnnotation(mAnnotation);
//    delete mAnnotation;
//    mAnnotation = new_annotation;
    return true;
  }

  return false;
}
/** @endcond doxygen-libnuml-internal */

/** @cond doxygen-libnuml-internal */
/*
 * Checks that the XHTML is valid.
 * If the xhtml does not conform to the specification of valid xhtml within
 * an numl document, an error is logged.
 */
void
NMBase::checkXHTML(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode * xhtml)
{
  if (!xhtml) return;

  const string&  name = xhtml->getName();
  unsigned int i, errorNS, errorXML, errorDOC, errorELEM;

  if (name == "notes")
  {
    errorNS   = NUMLNotesNotInXHTMLNamespace;
    errorXML  = NUMLNotesContainsXMLDecl;
    errorDOC  = NUMLNotesContainsDOCTYPE;
    errorELEM = NUMLInvalidNotesContent;
  }
  else if (name == "message")
  {
    errorNS   = NUMLConstraintNotInXHTMLNamespace;
    errorXML  = NUMLConstraintContainsXMLDecl;
    errorDOC  = NUMLConstraintContainsDOCTYPE;
    errorELEM = NUMLInvalidConstraintContent;
  }
  else                                  // We shouldn't ever get to this point.
  {
    logError(NUMLUnknownError);
    return;
  }

  /*
  * errors relating to a misplaced XML or DOCTYPE declaration
  * will also cause a parser error.
  * since parsing will terminate at this error, then if it has occurred
  * it will be in the XML currently being checked and so a more
  * informative message can be added
  */
  for (i = 0; i < getErrorLog()->getNumErrors(); i++)
  {
    if (getErrorLog()->getError(i)->getErrorId() == LIBSBML_CPP_NAMESPACE_QUALIFIER BadXMLDeclLocation)
    {
      logError(errorXML);
    }
    if (getErrorLog()->getError(i)->getErrorId() == LIBSBML_CPP_NAMESPACE_QUALIFIER BadlyFormedXML)
    {
      logError(errorDOC);
    }
  }

  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* toplevelNS = (mNUML) ? mNUML->getNamespaces() : 0;

  /*
  * namespace declaration is variable
  * if a whole html tag has been used
  * or a whole body tag then namespace can be implicitly declared
  */
  unsigned int children = xhtml->getNumChildren();

  if (children > 1)
  {
    for (i=0; i < children; i++)
    {
      if (LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::isAllowedElement(xhtml->getChild(i)))
      {
        if (!LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::hasDeclaredNS(xhtml->getChild(i),
                                                  toplevelNS))
        {
          logError(errorNS);
        }
      }
      else
      {
        logError(errorELEM);
      }
    }
  }
  else
  {
    /* only one element which can be html or body with either implicit/explicit
    * namespace declaration
    * OR could be one of the listed elements.
    */

    const string& top_name = xhtml->getChild(0).getName();

    if (top_name != "html" && top_name != "body"
      && !LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::isAllowedElement(xhtml->getChild(0)))
    {
      logError(errorELEM);
    }
    else
    {
      if (!LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::hasDeclaredNS(xhtml->getChild(0), toplevelNS))
      {
        logError(errorNS);
      }
      if (top_name == "html"
        && !LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::isCorrectHTMLNode(xhtml->getChild(0)))
      {
        logError(errorELEM);
      }
    }
  }
}

/** @endcond doxygen-libnuml-internal */
/**
  * Checks the annotation does not declare an numl namespace.
  * If the annotation declares an numl namespace an error is logged.
  */
void
NMBase::checkAnnotation()
{
  unsigned int nNodes = 0;
  unsigned int match = 0;
  int n = 0;
  std::vector<std::string> prefixes;
  prefixes.clear();

  if (!mAnnotation) return;

  //
  // checks if the given default namespace (if any) is a valid
  // NUML namespace
  //
  const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces &xmlns = mAnnotation->getNamespaces();
  checkDefaultNamespace(&xmlns,"annotation");

  while (nNodes < mAnnotation->getNumChildren())
  {
    LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode topLevel = mAnnotation->getChild(nNodes);

    std::string prefix = topLevel.getPrefix();

    // cannot be other toplevel element with this prefix
    if (!prefix.empty())
    {
      if (find(prefixes.begin(), prefixes.end(), prefix)
                                               != prefixes.end())
      {
        logError(NUMLDuplicateAnnotationNamespaces);
      }
      prefixes.push_back(prefix);
    }

    match = 0;
    n = 0;

    bool implicitNSdecl = false;
   // must have a namespace
    if (topLevel.getNamespaces().getLength() == 0)
    {
      // not on actual element - is it explicit ??
      if( mNUML->getNamespaces() != NULL)
      /* check for implicit declaration */
      {
        for (n = 0; n < mNUML->getNamespaces()->getLength(); n++)
        {
        /* TODO  if (!strcmp(mNUML->getNamespaces()->getPrefix(n).c_str(),
                        prefix.c_str()))
          {
            implicitNSdecl = true;
            break;
          }*/
        }
     }


      if (!implicitNSdecl)
      {
        logError(NUMLMissingAnnotationNamespace);
      }
    }
    // cannot declare numl namespace
 /* TODO   while(!match && n < topLevel.getNamespaces().getLength())
    {
      match += !strcmp(topLevel.getNamespaces().getURI(n).c_str(),
                                       "http://www.numl.org/numl/level1/version1");
      n++;
    }*/
    if (match > 0)
    {
      logError(NUMLNamespaceInAnnotation);
      break;
    }

    if (implicitNSdecl && prefix.empty())
    {
      logError(NUMLMissingAnnotationNamespace);
      logError(NUMLNamespaceInAnnotation);
    }
    nNodes++;
  }
}
/** @endcond doxygen-libnuml-internal */

/** @cond doxygen-libnuml-internal */
/*
 * @return true if read a <notes> element from the stream
 */
bool
NMBase::readNotes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
  const string& name = stream.peek().getName();

  if (name == "notes")
  {
    // If this is a level 1 document then notes are not allowed on
    // the numl container
    if (getLevel() == 1 && getTypeCode() == NUML_DOCUMENT)
    {
      logError(NUMLAnnotationNotesNotAllowedLevel1);
    }

    // If a notes element already exists, then it is an error.
    // If an annotation element already exists, then the ordering is wrong.
    // In either case, replace existing content with the new notes read.

    if (mNotes)
    {
      logError(NUMLNotSchemaConformant, getLevel(), getVersion(),
               "Only one <notes> element is permitted inside a "
         "particualr containing element.");
    }
    else if (mAnnotation)
    {
      logError(NUMLNotSchemaConformant, getLevel(), getVersion(),
               "Incorrect ordering of <annotation> and <notes> elements -- "
         "<notes> must come before <annotation> due to the way that "
         "the XML Schema for NUML is defined.");
    }

    delete mNotes;
    mNotes = new LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode(stream);

    //
    // checks if the given default namespace (if any) is a valid
    // NUML namespace
    //
    const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces &xmlns = mNotes->getNamespaces();
    checkDefaultNamespace(&xmlns,"notes");

    if (getNUMLDocument() != NULL)
    {
      if (getNUMLDocument()->getNumErrors() == 0)
      {
        checkXHTML(mNotes);
      }
    }
    return true;
  }

  return false;
}


/** @cond doxygen-libnuml-internal */
/*
 * Subclasses should override this method to read (and store) XHTML,
 * MathML, etc. directly from the XMLInputStream.
 *
 * @return true if the subclass read from the stream, false otherwise.
 */
bool
NMBase::readOtherXML (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream&)
{
  return false;
}
/** @endcond doxygen-libnuml-internal */


bool
NMBase::getHasBeenDeleted()
{
  return mHasBeenDeleted;
}

/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
NMBase::getElementPosition () const
{
  return -1;
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * @return the NUMLErrorLog used to log errors during while reading and
 * validating NUML.
 */
NUMLErrorLog*
NMBase::getErrorLog ()
{
  return (mNUML != 0) ? mNUML->getErrorLog() : 0;
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Helper to log a common type of error.
 */
void
NMBase::logUnknownAttribute( string attribute,
                            const unsigned int level,
                            const unsigned int version,
                            string element )
{
  ostringstream msg;

  msg << "Attribute '" << attribute << "' is not part of the "
      << "definition of an NUML Level " << level
      << " Version " << version << " " << element << " element.";
      
  getErrorLog()->logError(NUMLNotSchemaConformant, level, version, msg.str());
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Helper to log a common type of error.
 */
void
NMBase::logUnknownElement( string element,
        const unsigned int level,
        const unsigned int version )
{
  ostringstream msg;

  msg << "Element '" << element << "' is not part of the definition of "
      << "NUML Level " << level << " Version " << version << ".";
      
  getErrorLog()->logError(NUMLUnrecognizedElement,
        level, version, msg.str());
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Helper to log a common type of error.
 */
void
NMBase::logEmptyString( string attribute,
                       const unsigned int level,
                       const unsigned int version,
                       string element )
                       
{
  ostringstream msg;

  msg << "Attribute '" << attribute << "' on an "
    << element << " must not be an empty string.";
      
  getErrorLog()->logError(NUMLNotSchemaConformant,
        level, version, msg.str());
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Convenience method for easily logging problems from within method
 * implementations.
 *
 * This is essentially a short form of getErrorLog()->logError(...)
 */
void
NMBase::logError (  unsigned int       id
                 , const unsigned int level
                 , const unsigned int version
                 , const std::string& details )
{
  if ( NMBase::getErrorLog() )
    getErrorLog()->logError(id, getLevel(), getVersion(), details);
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
NMBase::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
{
  const_cast<LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes&>(attributes).setErrorLog(getErrorLog());

  bool assigned = attributes.readInto("metaid", mMetaId);

  if (assigned && mMetaId.empty())
  {
    logEmptyString("metaid", getLevel(), getVersion(), 
      NUMLTypeCode_toString(getTypeCode()));
  }

  if (isSetMetaId())
  {
    if (!LIBSBML_CPP_NAMESPACE_QUALIFIER SyntaxChecker::isValidXMLID(mMetaId))
      logError(NUMLInvalidMetaidSyntax, getLevel(), getVersion());
  }
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
NMBase::writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  if (getTypeCode() == NUML_DOCUMENT)
  {
    if (this->getNamespaces()) stream << *(this->getNamespaces());
  }
  if ( getLevel() > 1 && !mMetaId.empty() )
  {
    stream.writeAttribute("metaid", mMetaId);
  }
}

/** @endcond doxygen-libnuml-internal */

/** @cond doxygen-libnuml-internal */
/*
 * Checks that NUML element has been read in the proper order.  If object
 * is not in the expected position, an error is logged.
 */
void
NMBase::checkOrderAndLogError (NMBase* object, int expected)
{
  int actual = object->getElementPosition();

  if (actual != -1 && actual < expected)
  {
    NUMLErrorCode_t error = NUMLIncorrectOrderInModel;

    /* TODO
    if (object->getTypeCode() == NUML_NUMLLIST)
    {
      NUMLTypeCode_t tc = static_cast<NUMLList*>(object)->getItemTypeCode();

      if (tc == NUML_ || tc == NUML_)
      {
        error = IncorrectOrderInReaction;
      }
    }    */

    logError(error, getLevel(), getVersion());
  }
}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */
/**
  * Checks that an NUML NUMLList element has been populated.
  * If a NUMLList element has been declared with no elements,
  * an error is logged.
  */
void
NMBase::checkNUMLListPopulated(NMBase* object)
{
  if (object->getTypeCode() == NUML_NUMLLIST)
  {
    // Check that the list has at least one element.
    if (static_cast <NUMLList*> (object)->size() == 0)
    {
      NUMLTypeCode_t tc = static_cast<NUMLList*>(object)->getItemTypeCode();
      NUMLErrorCode_t error = NUMLEmptyListElement;

      // By default, the error will be the NUMLEmptyListElement error, unless
      // we have a special case for which NUML has a separate error code.
      switch (tc)
      {
      case NUML_ONTOLOGYTERMS:
    //    error = EmptyNUMLListOperations;
        break;

      case NUML_RESULTCOMPONENTS:
        //error = EmptyNUMLListOfResultComponents;
        break;

      default: ;
      }

      logError(error, getLevel(), getVersion());
    }
  }

}
/** @endcond doxygen-libnuml-internal */


/** @cond doxygen-libnuml-internal */

void 
NMBase::checkDefaultNamespace(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* xmlns, const std::string& elementName)
{
  //
  // checks if the given default namespace (if any) is a valid
  // NUML namespace
  //
  if (xmlns && xmlns->getLength() > 0)
  {
    unsigned int level   = getLevel();
    unsigned int version = getVersion();
    const std::string currentURI = NUMLNamespaces::getNUMLNamespaceURI(level,version);
    const std::string defaultURI = xmlns->getURI();
    if (!defaultURI.empty() && currentURI != defaultURI)
    {
      static ostringstream errMsg;
      errMsg.str("");
      errMsg << "xmlns=\"" << defaultURI << "\" in <" << elementName
             << "> element is an invalid namespace." << endl;
      
      logError(NUMLNotSchemaConformant, level, version, errMsg.str());
    }
  }
}

/** @endcond doxygen-libnuml-internal */
/** @cond doxygen-libnuml-internal */
/* default for components that have no required attributes */
bool
NMBase::hasRequiredAttributes() const
{
  return true;
}

/* default for components that have no required elements */
bool
NMBase::hasRequiredElements() const
{
  return true;
}


/** @endcond doxygen-libnuml-internal */

/** @cond doxygen-libnuml-internal */
/*
 * Stores the location (line and column) and any XML namespaces (for
 * roundtripping) declared on this NUML (XML) element.
 */
void
NMBase::setNMBaseFields (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLToken& element)
{
  mLine   = element.getLine  ();
  mColumn = element.getColumn();

  if (element.getNamespaces().getLength() > 0)
  {
    LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces tmpxmlns(element.getNamespaces());
    setNamespaces(&tmpxmlns);
  }
  else
  {
    setNamespaces(NULL);
  }
}


/**
 * Returns the value of the "metaid" attribute of the given NMBase_t
 * structure.
 *
 * @param sb the NMBase_t structure
 * 
 * @return the value of the "metaid" attribute of @p sb
 */
LIBNUML_EXTERN
const char *
NMBase_getMetaId (NMBase_t *sb)
{
  return sb->isSetMetaId() ? sb->getMetaId().c_str() : NULL;
}


///**
// * Returns the value of the "id" attribute of the given NMBase_t
// * structure.
// *
// * @param sb the NMBase_t structure
// * 
// * @return the value of the "id" attribute of @p sb
// */
//LIBNUML_EXTERN
//const char *
//NMBase_getId (const NMBase_t *sb)
//{
//  return sb->isSetId() ? sb->getId().c_str() : NULL;
//}
//
//
///**
// * Returns the value of the "name" attribute of the given NMBase_t
// * structure.
// *
// * @param sb the NMBase_t structure
// * 
// * @return the value of the "name" attribute of @p sb
// */
//LIBNUML_EXTERN
//const char *
//NMBase_getName (const NMBase_t *sb)
//{
//  return sb->isSetName() ? sb->getName().c_str() : NULL;
//}


/**
 * Returns the parent NUMLDocument_t structure of the given NMBase_t
 * structure.
 *
 * @param sb the NMBase_t structure
 * 
 * @return the parent NUMLDocument of this NUML object.
 */
LIBNUML_EXTERN
const NUMLDocument_t *
NMBase_getNUMLDocument (NMBase_t *sb)
{
  return sb->getNUMLDocument();
}


/**
 * Returns the parent NMBase_t structure of the given NMBase_t
 * structure.
 *
 * @param sb the NMBase_t structure
 * 
 * @return the parent NMBase  of this NUML object.
 */
LIBNUML_EXTERN
const NMBase_t *
NMBase_getParentNUMLObject (NMBase_t *sb)
{
  return sb->getParentNUMLObject();
}

/**
 * Returns the NUML Level of the overall NUML document.
 *
 * @param sb the NMBase_t structure to query
 * 
 * @return the NUML level of the given object.
 * 
 * @see getVersion()
 */
LIBNUML_EXTERN
unsigned int
NMBase_getLevel (const NMBase_t *sb)
{
  return sb->getLevel();
}


/**
 * Returns the Version within the NUML Level of the overall NUML document.
 *
 * @param sb the NMBase_t structure to query
 * 
 * @return the NUML version of the given object.
 *
 * @see getLevel()
 */
LIBNUML_EXTERN
unsigned int
NMBase_getVersion (const NMBase_t *sb)
{
  return sb->getVersion();
}

/**
 * Predicate returning nonzero true or false depending on whether the given
 * structure's "metaid" attribute has been set.
 *
 * @param sb the NMBase_t structure to query
 * 
 * @return nonzero (for true) if the "metaid" attribute of this NUML object
 * has been set, zero (for false) otherwise.
 */
LIBNUML_EXTERN
int
NMBase_isSetMetaId (const NMBase_t *sb)
{
  return static_cast<int>( sb->isSetMetaId() );
}


/**
 * Sets the value of the "metaid" attribute of the given object.
 *
 * The string @p metaid is copied.  The value of @p metaid must be an
 * identifier conforming to the syntax defined by the XML 1.0 data type
 * ID.  Among other things, this type requires that a value is unique
 * among all the values of type XML ID in an NUMLDocument.  Although NUML
 * only uses XML ID for the "metaid" attribute, callers should be careful
 * if they use XML ID's in XML portions of a model that are not defined
 * by NUML, such as in the application-specific content of the
 * "annotation" subelement.
 *
 * @param sb the NMBase_t structure
 *
 * @param metaid the identifier string to use as the value of the
 * "metaid" attribute
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif The possible values
 * returned by this function are:
 * @li LIBNUML_OPERATION_SUCCESS
 * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
 * @li LIBNUML_UNEXPECTED_ATTRIBUTE
 *
 * @note Using this function with the metaid set to NULL is equivalent to
 * unsetting the "metaid" attribute.
 */
LIBNUML_EXTERN
int
NMBase_setMetaId (NMBase_t *sb, const char *metaid)
{
  return (metaid == NULL) ? sb->unsetMetaId() : sb->setMetaId(metaid);
}


///**
// * Sets the value of the "id" attribute of this NUML object.
// *
// * The string @p sid is copied.  Note that NUML has strict requirements
// * for the syntax of identifiers.  The following is summary of the
// * definition of the NUML identifier type @c SId (here expressed in an
// * extended form of BNF notation):
// * @code
// *   letter ::= 'a'..'z','A'..'Z'
// *   digit  ::= '0'..'9'
// *   idChar ::= letter | digit | '_'
// *   SId    ::= ( letter | '_' ) idChar*
// * @endcode
// * The equality of NUML identifiers is determined by an exact character
// * sequence match; i.e., comparisons must be performed in a
// * case-sensitive manner.  In addition, there are a few conditions for
// * the uniqueness of identifiers in an NUML model.  Please consult the
// * NUML specifications for the exact formulations.
// *
// * @param sb the NMBase_t structure
// *
// * @param sid the string to use as the identifier of this object
// *
// * @return integer value indicating success/failure of the
// * function.  @if clike The value is drawn from the
// * enumeration #OperationReturnValues_t. @endif The possible values
// * returned by this function are:
// *
// * @li LIBNUML_OPERATION_SUCCESS
// * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
// *
// * @note Using this function with an id of NULL is equivalent to
// * unsetting the "id" attribute.
// */
//LIBNUML_EXTERN
//int
//NMBase_setId (NMBase_t *sb, const char *sid)
//{
//  return (sid == NULL) ? sb->unsetId() : sb->setId(sid);
//}
//
//
///**
// * Sets the value of the "name" attribute of this NUML object.
// *
// * The string in @p name is copied.
// *
// * @param sb the NMBase_t structure
// *
// * @param name the new name for the object
// *
// * @return integer value indicating success/failure of the
// * function.  @if clike The value is drawn from the
// * enumeration #OperationReturnValues_t. @endif The possible values
// * returned by this function are:
// *
// * @li LIBNUML_OPERATION_SUCCESS
// * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
// *
// * @note Using this function with the name set to NULL is equivalent to
// * unsetting the "name" attribute.
// */
//LIBNUML_EXTERN
//int
//NMBase_setName (NMBase_t *sb, const char *name)
//{
//  return (name == NULL) ? sb->unsetName() : sb->setName(name);
//}


/**
 * Sets the namespaces relevant of this NUML object.
 *
 * @param sb the NMBase_t structure
 *
 * @param xmlns the namespaces to set
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif The possible values
 * returned by this function are:
 * @li LIBNUML_OPERATION_SUCCESS
 */
LIBNUML_EXTERN
int
NMBase_setNamespaces (NMBase_t *sb, LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces_t *xmlns)
{
  return sb->setNamespaces(xmlns);
}

/**
 * Unsets the "metaid" attribute of the given object.
 *
 * @param sb the NMBase_t structure
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif The possible values
 * returned by this function are:
 * @li LIBNUML_OPERATION_SUCCESS
 * @li LIBNUML_OPERATION_FAILED
 */
LIBNUML_EXTERN
int
NMBase_unsetMetaId (NMBase_t *sb)
{
  return sb->unsetMetaId();
}


/**
 * Returns the libNUML type code for the given structure.
 *
 * This method MAY return the typecode of this NUML object or it MAY
 * return NUML_UNKNOWN.  That is, subclasses of NMBase are not required to
 * implement this method to return a typecode.  This method is meant
 * primarily for the LibNUML C interface where class and subclass
 * information is not readily available.
 *
 * @param sb the NMBase_t structure
 *
 * @return the NUMLTypeCode_t of this NUML object or NUML_UNKNOWN
 * (default).
 *
 * @see getElementName()
 */
LIBNUML_EXTERN
NUMLTypeCode_t
NMBase_getTypeCode (const NMBase_t *sb)
{
  return (sb != NULL) ? sb->getTypeCode() :NUML_UNKNOWN;
  //return sb->getTypeCode();
}


/**
 * Returns the XML element name of the given structure.
 *
 * This is overridden by subclasses to return a string appropriate to the
 * NUML component.  For example, Model defines it as returning "model",
 * CompartmentType defines it as returning "compartmentType", etc.
 *
 * @param sb the NMBase_t structure
 */
LIBNUML_EXTERN
const char *
NMBase_getElementName (const NMBase_t *sb)
{
  return sb->getElementName().empty() ? NULL : sb->getElementName().c_str();
}


/**
 * Returns the line number on which the given object first appears in the
 * XML representation of the NUML document.
 *
 * @param sb the NMBase_t structure
 * 
 * @return the line number of the given structure
 *
 * @see getColumn().
 */
LIBNUML_EXTERN
unsigned int
NMBase_getLine (const NMBase_t *sb)
{
  return sb->getLine();
}


/**
 * Returns the column number on which the given object first appears in the
 * XML representation of the NUML document.
 *
 * @param sb the NMBase_t structure
 * 
 * @return the column number of this NUML object.
 * 
 * @see getLine().
 */
LIBNUML_EXTERN
unsigned int
NMBase_getColumn (const NMBase_t *sb)
{
  return sb->getColumn();
}


/**
  * Predicate returning nonzero true or false depending on whether the
  * object's level/version and namespace values correspond to a valid
  * NUML specification.
  *
  * The valid combinations of NUML Level, Version and Namespace as of this release
  * of libNUML are the following:
  * <ul>
  * <li> Level&nbsp;1 Version&nbsp;1 "http://www.numl.org/numl/level1/version1"
  * </ul>
  *
  * @param sb the NMBase_t structure
  *
  * @return nonzero (true) if the level, version and namespace values of this 
  * NUML object correspond to a valid set of values, zero (false) otherwise.
  */
LIBNUML_EXTERN
int
NMBase_hasValidLevelVersionNamespaceCombination(NMBase_t *sb)
{
  return static_cast <int> (sb->hasValidLevelVersionNamespaceCombination());
}




/** @endcond doxygen-c-only */

LIBNUML_CPP_NAMESPACE_END
