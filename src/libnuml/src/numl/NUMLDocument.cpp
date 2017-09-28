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

#include <iostream>
#include <string.h>

#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLNamespaces.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>
#include <sbml/xml/XMLError.h>

//#include <numl/validator/ConsistencyValidator.h>
//#include <numl/validator/IdentifierConsistencyValidator.h>
//#include <numl/validator/InternalConsistencyValidator.h>


#include <numl/NUMLErrorLog.h>
#include <numl/NUMLVisitor.h>
#include <numl/NUMLError.h>
#include <numl/NUMLDocument.h>
#include <numl/NUMLReader.h>
#include <numl/NUMLWriter.h>



using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

/*
 * Function to check whether an error reported by a compatability validation
 * prior to conversion between levels/versions can be ignored.
 * Some conversions will lose information but the model will still be valid
 * when converted.
 */
//static unsigned int ignorable[] = {
//  92001,
//  92003,
//  92004,
//  92005,
//  92006,
//  93001,
//  91003,
//  91005,
//  91006,
//  91013
//};


/*
 * Predicate returning true if the errors encountered are not ignorable.
 */
bool
NUMLDocument::conversion_errors(unsigned int errors)
{
  /**
   * changed this code in line with the rest of the validation
   * errors: ie each now assigns a severity
   * Error would imply conversion not possible
   * Warning implies lose of data but conversion still possible
   */
  if (errors > 0)
  {
    if (mErrorLog.getNumFailsWithSeverity(LIBNUML_SEV_ERROR) > 0)
      return true;
    else
      return false;
  }
  else
  {
    return false;
  }

  //for (unsigned int i = 0; i < errors; i++)
  //{
  //  bool failure = true;
  //
  //  for (unsigned int n = 0; n < sizeof(ignorable)/sizeof(ignorable[0]); n++)
  //  {
  //    if (getError(i)->getErrorId() == ignorable[n])
  //    {
  //      failure = false;
  //      break;
  //    }
  //  }

  //  if (failure) return failure;
  //}

  //return false;
}



/*
 * Get the most recent Level of NUML supported by this release of
 * libNUML.
 *
 * This is the "default" level in the sense that libNUML will create
 * models of this NUML Level unless told otherwise.
 *
 * @return the number representing the most recent NUML specification level
 * (at the time this libNUML was released).
 */
unsigned int
NUMLDocument::getDefaultLevel ()
{
  return NUML_DEFAULT_LEVEL;
}


/*
 * Get the most recent Version with the most recent Level of NUML supported
 * by this release of libNUML.
 *
 * This is the "default" version in the sense that libNUML will create
 * models of this NUML Level and Version unless told otherwise.
 *
 * @return the number representing the most recent NUML specification
 * version (at the time this libNUML was released).
 */
unsigned int
NUMLDocument::getDefaultVersion ()
{
  return NUML_DEFAULT_VERSION;
}


/*
 * Creates a new NUMLDocument.  If not specified, the NUML level and
 * version attributes default to the most recent NUML specification (at the
 * time this libNUML was released).
 */
NUMLDocument::NUMLDocument (unsigned int level, unsigned int version) :
   NMBase (level, version)
 , mLevel   ( level   )
 , mVersion ( version )
{
  bool setExplicitly = (mLevel != 0 && mVersion != 0);

  mNUML = this;

  if (mLevel   == 0)  mLevel   = getDefaultLevel  ();
  if (mVersion == 0)  mVersion = getDefaultVersion();

  if (setExplicitly)
  {
    setLevelAndVersion(mLevel, mVersion, false);
  }

  mApplicableValidators = AllChecksON; // turn all validators ON
  mApplicableValidatorsForConversion = AllChecksON; // turn all validators ON

  mNUMLNamespaces->setLevel(mLevel);
  mNUMLNamespaces->setVersion(mVersion);
}


/*
 * Destroys this NUMLDocument.
 */
NUMLDocument::~NUMLDocument ()
{
 // delete mModel;
}


/*
 * Creates a copy of this NUMLDocument.
 */
NUMLDocument::NUMLDocument (const NUMLDocument& orig) :
   NMBase    ( orig          )
 , mLevel   ( orig.mLevel   )
 , mVersion ( orig.mVersion )
 , mApplicableValidators (orig.mApplicableValidators)
 , mApplicableValidatorsForConversion (orig.mApplicableValidatorsForConversion)
{
  mNUML = this;

 /* if (orig.mModel)
  {
    mModel = static_cast<Model*>( orig.mModel->clone() );
    mModel->setNUMLDocument(this);
  }
  */
  //if(orig.mNamespaces)
  //  this->mNamespaces =
  //  new XMLNamespaces(*const_cast<NUMLDocument&>(orig).mNamespaces);
  //else
  //  this->mNamespaces = 0;

}


/*
 * Accepts the given NUMLVisitor.
 */
bool
NUMLDocument::accept (NUMLVisitor& v) const
{
  v.visit(*this);
//  if (mModel) mModel->accept(v);
  v.leave(*this);

  return true;
}


/*
 * @return a (deep) copy of this NUMLDocument.
 */
NUMLDocument*
NUMLDocument::clone () const
{
  return new NUMLDocument(*this);
}


/*
 * @return the OntologyTerms contained in this NUMLDocument.
 */
const OntologyTerms*
NUMLDocument::getOntologyTerms () const
{
  return &mOntologyTerms;
}


/*
 * @return the OntologyTerms contained in this NUMLDocument.
 */
OntologyTerms*
NUMLDocument::getOntologyTerms ()
{
  return &mOntologyTerms;
}

/*
 * @return the number of OntologyTerms in this NUMLDocument.
 */
unsigned int
NUMLDocument::getNumOntologyTerms () const
{
  return mOntologyTerms.size();
}

/*
 * @return the ResultComponents contained in this NUMLDocument.
 */
const ResultComponents*
NUMLDocument::getResultComponents () const
{
  return &mResultComponents;
}


/*
 * @return the ResultComponents contained in this NUMLDocument.
 */
ResultComponents*
NUMLDocument::getResultComponents ()
{
  return &mResultComponents;
}

/*
 * @return the number of ResultComponents in this NUMLDocument.
 */
unsigned int
NUMLDocument::getNumResultComponents () const
{
  return mResultComponents.size();
}

/*
 * Sets the level and version of this NUMLDocument.  Valid
 * combinations are currently:
 *
 * @li Level 1 Version 1
 *
 *
 */
bool
NUMLDocument::setLevelAndVersion (unsigned int level, unsigned int version,
                                  bool strict)
{
  /* since this function will write to the error log we should
   * clear anything in the log first
   */
  if(getErrorLog() != NULL)
  getErrorLog()->clearLog();

  bool conversionSuccess = false;

  unsigned char origValidators = mApplicableValidators;
  mApplicableValidators = mApplicableValidatorsForConversion;
  /* if strict = true we will only convert a valid model
   * to a valid model with a valid internal representation
   */
  /* see whether the unit validator is on */


  if (strict)
  {
    /* use validators that the user has selected
    */
    /* hack to catch errors caught at read time */
    NUMLDocument *d = readNUMLFromString(writeNUMLToString(this));
    unsigned int errors = d->getNumErrors();

    for (unsigned int i = 0; i < errors; i++)
    {
      mErrorLog.add(*(d->getError(i)));
    }
    delete d;

  //  errors += checkConsistency();
    if(getErrorLog() != NULL)
    errors = getErrorLog()->getNumFailsWithSeverity(LIBNUML_SEV_ERROR);

    /* if the current model is not valid dont convert
    */
    if (errors > 0)
    {
      return conversionSuccess;
    }

    if(getErrorLog() != NULL)
    getErrorLog()->clearLog();
  }



  //mLevel   = level;
  //mVersion = version;
  //mNUMLNamespaces->setLevel(mLevel);
  //mNUMLNamespaces->setVersion(mVersion);


  /* restore original value */
  mApplicableValidators = origValidators;

  mLevel   = level;
  mVersion = version;

  if (mNUMLNamespaces == 0)
    mNUMLNamespaces = new NUMLNamespaces(mLevel, mVersion);;

  /**
   * check for the case where the numl namespace has been expicitly declared
   * as well as being the default
   */
  bool numlDecl = false;
  int index;
  for (index = 0; index < mNUMLNamespaces->getNamespaces()->getLength();
                                                                  index++)
  {
    if (!mNUMLNamespaces->getNamespaces()->getPrefix(index).empty()
      && mNUMLNamespaces->getNamespaces()->getPrefix(index)=="numl")
    {
      numlDecl = true;
      break;
    }
  }
  if (numlDecl)
  {
    LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces * copyNamespaces = mNUMLNamespaces->getNamespaces()->clone();
    mNUMLNamespaces->getNamespaces()->clear();
    for (int i = 0; i < copyNamespaces->getLength(); i++)
    {
      if ( i != index)
        mNUMLNamespaces->getNamespaces()->add(copyNamespaces->getURI(i),
                         copyNamespaces->getPrefix(i));
    }
    delete copyNamespaces;
  }

  if (mLevel == 1)
  {
    if (numlDecl)
      mNUMLNamespaces->getNamespaces()->add("http://www.numl.org/numl/level1/version1", "numl");
    mNUMLNamespaces->getNamespaces()->add("http://www.numl.org/numl/level1/version1");
  }

  mNUMLNamespaces->setLevel(mLevel);
  mNUMLNamespaces->setVersion(mVersion);

  return conversionSuccess;
}

/*
 * @return the NUML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
NMBase*
NUMLDocument::createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream)
{
    const string& name   = stream.peek().getName();
    NMBase*        object = 0;
    //unsigned int level = getLevel();
    //unsigned int version = getVersion();

    if (name == "ontologyTerms")
        {
            if (mOntologyTerms.size() != 0)
            {
                logError(NUMLNotSchemaConformant);
            }
            object = &mOntologyTerms;
        }
    else if (name == "resultComponents")
        {
            if (mResultComponents.size() != 0)
            {
                logError(NUMLNotSchemaConformant);
            }
            object = &mResultComponents;
        }
    else if (name == "resultComponent")
    {
      object = createResultComponent();
    }

    return object;
}

/*
 * Creates a new OntologyTerm inside this document and returns it.
 */
OntologyTerm*
NUMLDocument::createOntologyTerm ()
{
  OntologyTerm* oTerm = 0;

  try
  {
      oTerm = new OntologyTerm(getNUMLNamespaces());
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
  if (mOntologyTerms.size() == 0)
  {
    mOntologyTerms.setNUMLDocument(this->getNUMLDocument());
    mOntologyTerms.setParentNUMLObject(this);
  }

  if (oTerm) mOntologyTerms.appendAndOwn(oTerm);

  return oTerm;
}

/*
 * Creates a new ResultComponent inside this document and returns it.
 */
ResultComponent*
NUMLDocument::createResultComponent ()
{
  ResultComponent* oResultComp = 0;

  try
  {
      oResultComp = new ResultComponent(getNUMLNamespaces());
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
  if (mResultComponents.size() == 0)
  {
    mResultComponents.setNUMLDocument(this->getNUMLDocument());
    mResultComponents.setParentNUMLObject(this);
  }

  if (oResultComp) mResultComponents.appendAndOwn(oResultComp);

  return oResultComp;
}


/*
 * @return the nth error encountered during the parse of this
 * NUMLDocument or NULL if n > getNumErrors() - 1.
 */
const NUMLError*
NUMLDocument::getError (unsigned int n) const
{
  return mErrorLog.getError(n);
}


/*
 * @return the number of errors encountered during the parse of this
 * NUMLDocument.
 */
unsigned int
NUMLDocument::getNumErrors () const
{
  return mErrorLog.getNumErrors();
}


/*
 * Prints all errors encountered during the parse of this NUMLDocument to
 * the given stream.  If no errors have occurred, i.e.  getNumErrors() ==
 * 0, no output will be sent to stream. The format of the output is:
 *
 *   N error(s):
 *     line N: (id) message
 */
void
NUMLDocument::printErrors (std::ostream& stream) const
{
  unsigned int numErrors = getNumErrors();

  if (numErrors > 0)
  {
    for (unsigned int n = 0; n < numErrors; n++)
    {
      stream << *(getError(n));
    }
  }
}

/*
 * Sets the parent NUMLDocument of this NUML object.
 */
void
NUMLDocument::setNUMLDocument (NUMLDocument* d)
{
  mNUML = d;

  mOntologyTerms.setParentNUMLObject(d);

  mResultComponents.setParentNUMLObject(d);
}


/**
  * Sets the parent NUML object of this NUML object.
  *
  * @param sb the NUML object to use
  */
void
NUMLDocument::setParentNUMLObject (NMBase* sb)
{
  mParentNUMLObject = sb;

  mOntologyTerms.setParentNUMLObject(this);
  mResultComponents.setParentNUMLObject(this);

}

/*
 * @return the NUMLTypeCode_t of this NUML object or NUML_UNKNOWN
 * (default).
 *
 * @see getElementName()
 */
NUMLTypeCode_t
NUMLDocument::getTypeCode () const
{
  return NUML_DOCUMENT;
}


/*
 * @return the name of this element ie "numl".
 */
const string&
NUMLDocument::getElementName () const
{
  static const string name = "numl";
  return name;
}


/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
NUMLDocument::getElementPosition () const
{
  return 1;
}


/**
  * @return the Namespaces associated with this NUML object
  */
LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces*
NUMLDocument::getNamespaces() const
{
  return mNUMLNamespaces->getNamespaces();
}


/*
 * @return the NUMLErrorLog used to log errors during while reading and
 * validating NUML.
 */
NUMLErrorLog*
NUMLDocument::getErrorLog ()
{
  return &mErrorLog;
}


/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
NUMLDocument::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes)
{
  NMBase::readAttributes(attributes);
  std::vector<std::string> expectedAttributes;
  expectedAttributes.clear();
  expectedAttributes.push_back("level");
  expectedAttributes.push_back("version");
  expectedAttributes.push_back("metaid");
  expectedAttributes.push_back("schemaLocation");

  // check that all attributes are expected
  for (int i = 0; i < attributes.getLength(); i++)
  {
    std::vector<std::string>::const_iterator end = expectedAttributes.end();
    std::vector<std::string>::const_iterator begin = expectedAttributes.begin();
    std::string name = attributes.getName(i);
   if (std::find(begin, end, name) == end)
    {
      logUnknownAttribute(name, getLevel(), getVersion(), "<numl>");
    }
  }


  //
  // level: positiveInteger  { use="required" fixed="1" }  (L1v1)
  //
  attributes.readInto("level", mLevel, getErrorLog(), true);

  //
  // version: positiveInteger  { use="required" fixed="1" }  (L1v1, L2v1)
  // version: positiveInteger  { use="required" fixed="2" }  (L1v2, L2v2)
  // version: positiveInteger  { use="required" fixed="3" }  (L2v3)
  //
  attributes.readInto("version", mVersion, getErrorLog(), true);

  /* check that the level and version are valid */
  if (mLevel == 1)
  {
    if (mVersion > 2)
    {
      logError(InvalidNUMLLevelVersion);
    }
  }
  else
  {
    logError(InvalidNUMLLevelVersion);
    return;
  }

  /* check that numl namespace has been set */
  unsigned int match = 0;
  if (mNUMLNamespaces->getNamespaces() == NULL)
  {
    logError(NUMLInvalidNamespaceOnNUML);
  }
  else
  {
    for (int n = 0; n < mNUMLNamespaces->getNamespaces()->getLength(); n++)
    {
      if (!strcmp(mNUMLNamespaces->getNamespaces()->getURI(n).c_str(), "http://www.numl.org/numl/level1/version1"))
      {
        match = 1;
        if (mLevel != 1)
        {
          logError(NUMLMissingOrInconsistentLevel);
        }
        if (mVersion != 1)
        {
          logError(NUMLMissingOrInconsistentVersion);
        }
       break;
      }
    }
    if (match == 0)
    {
      logError(NUMLInvalidNamespaceOnNUML);
    }
    else
    {
      mNUMLNamespaces->setLevel(mLevel);
      mNUMLNamespaces->setVersion(mVersion);
    }

  }
}


/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
NUMLDocument::writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  if (mNUMLNamespaces->getNamespaces() == 0)
  {
     LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces xmlns;

     if (mLevel == 1)
     {
        xmlns.add("http://www.numl.org/numl/level1/version1");
     }
     stream << xmlns;

     mNUMLNamespaces->setNamespaces(&xmlns);
  }

  NMBase::writeAttributes(stream);

  //
  // level: positiveInteger  { use="required" fixed="1" }  (L1v1)
  stream.writeAttribute("level", mLevel);


  stream.writeAttribute("version", mVersion);
}


/*
 * Subclasses should override this method to write out their contained
 * NUML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.
 */
void
NUMLDocument::writeElements (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const
{
  NMBase::writeElements(stream);
  
  if (mOntologyTerms.size()!=0)
    mOntologyTerms.write(stream);

  // instead of writing a resultcomponents element that does not
  // exist, write the individual components
  for (unsigned int i = 0;i  < mResultComponents.size(); ++i)  
  {
    mResultComponents.get(i)->write(stream);  
  }

  //if (mResultComponents.size() > 0)
  //  mResultComponents.write(stream);
}



/**
 * Creates a new, empty NUMLDocument_t structure.
 *
 * The NUML Level and Version attributes default to the most recent NUML
 * specification (at the time this libNUML was released).
 *
 * @return the NUMLDocument_t structure created
 */
LIBNUML_EXTERN
NUMLDocument_t *
NUMLDocument_create ()
{
  return new(nothrow) NUMLDocument;
}


/**
 * Creates a new, empty NUMLDocument_t structure with given values for the
 * NUML Level and Version.
 *
 * If not specified, the NUML Level and Version attributes default to the
 * most recent NUML specification (at the time this libNUML was
 * released).
 *
 * @param level an integer for the NUML Level
 * @param version an integer for the Version within the NUML Level
 *
 * @return the NUMLDocument_t structure created
 */
LIBNUML_EXTERN
NUMLDocument_t *
NUMLDocument_createWithLevelAndVersion (unsigned int level, unsigned int version)
{
  return new(nothrow) NUMLDocument(level, version);
}


/**
 * Frees the given NUMLDocument_t structure.
 *
 * @param d the NUMLDocument_t structure
 */
LIBNUML_EXTERN
void
NUMLDocument_free (NUMLDocument_t *d)
{
  delete d;
}


/**
 * Creates and returns a deep copy of the given NUMLDocument_t structure
 *
 * @param d the NUMLDocument_t structure
 *
 * @return a (deep) copy of the NUMLDocument_t structure
 */
LIBNUML_EXTERN
NUMLDocument_t *
NUMLDocument_clone (const NUMLDocument_t *d)
{
  return static_cast<NUMLDocument_t*>( d->clone() );
}


/**
 * Returns the NUML Level of the given NUMLDocument_t structure.
 *
 * @param d the NUMLDocument_t structure
 *
 * @return the NUML Level number
 */
LIBNUML_EXTERN
unsigned int
NUMLDocument_getLevel (const NUMLDocument_t *d)
{
  return d->getLevel();
}


/**
 * Returns the Version within the NUML Level of the given NUMLDocument_t
 * structure.
 *
 * @param d the NUMLDocument_t structure
 *
 * @return the version number
 */
LIBNUML_EXTERN
unsigned int
NUMLDocument_getVersion (const NUMLDocument_t *d)
{
  return d->getVersion();
}


/**
 * Sets the NUML Level and Version of this NUMLDocument, attempting to
 * convert the ResultComponent as needed.
 *
 * This method is used to convert numl between Levels and Versions of
 * NUML.
 *
 * Callers can also check compatibility directly using the methods
 *
 * The valid combinations as of this release of libNUML are the
 * following:
 *
 * @li Level 1 Version 1
 *
 * @param d the NUMLDocument_t structure
 *
 * @param level the desired NUML Level
 *
 * @param version the desired Version within the NUML Level
 *
 * @note Calling this method will not @em necessarily lead to successful
 * conversion.  If the conversion fails, it will be logged in the error
 * list associated with this NUMLDocument_t structure.  Callers should
 * consult getNumErrors() to find out if the conversion succeeded without
 * problems. (If the conversion to Level 1 failed, the Level
 * of this ResultComponent will be left unchanged.)
 */
LIBNUML_EXTERN
int
NUMLDocument_setLevelAndVersion (  NUMLDocument_t *d
                                 , unsigned int    level
                                 , unsigned int    version )
{
  return static_cast <int> (d->setLevelAndVersion(level, version, false));
}


/**
 * Sets the NUML Level and Version of this NUMLDocument, attempting to
 * convert the model as needed.
 *
 * This method is used to convert models between Levels and Versions of
 * NUML.
 *
 * Callers can also check compatibility directly using the methods
 *
 * The valid combinations as of this release of libNUML are the
 * following:
 *
 * @li Level 1 Version 1
 *
 * @param d the NUMLDocument_t structure
 *
 * @param level the desired NUML Level
 *
 * @param version the desired Version within the NUML Level
 *
 * @note Calling this method will not @em necessarily lead to successful
 * conversion.  If the conversion fails, it will be logged in the error
 * list associated with this NUMLDocument_t structure.  Callers should
 * consult getNumErrors() to find out if the conversion succeeded without
 * problems.
 *
 * Strict conversion applies the additional criteria that both the source
 * and the target model must be consistent NUML. Users can control the
 * consistency checks that are applied using the
 * NUMLDocument::setConsistencyChecks function. If either the source
 * or the potential target model have validation errors, the conversion
 * is not performed.  When a strict conversion is successful, the
 * underlying NUML object model is altered to reflect the new level
 * and version.  Thus information that cannot be converted (e.g. sboTerms)
 * will be lost.
 */
LIBNUML_EXTERN
int
NUMLDocument_setLevelAndVersionStrict (  NUMLDocument_t *d
                                       , unsigned int    level
                                       , unsigned int    version )
{
  return static_cast <int> (d->setLevelAndVersion(level, version, true));
}

/**
 * Returns the nth error or warning encountered during parsing,
 * consistency checking, or attempted translation of this model.
 *
 * Callers can use method XMLError_getSeverity() on the result to assess
 * the severity of the problem.  The severity levels range from
 * informationl messages to fatal errors.
 *
 * @return the error or warning indexed by integer @p n, or return NULL
 * if n > (NUMLDocument_getNumErrors() - 1).
 *
 * @param d the NUMLDocument_t structure
 *
 * @param n the index of the error sought.
 *
 * @see NUMLDocument_getNumErrors(), NUMLDocument_setLevelAndVersion(),
 * NUMLDocument_checkConsistency(), NUMLDocument_checkL1Compatibility(),
 * NUMLReader_readNUMLFromString().
 */
LIBNUML_EXTERN
const NUMLError_t *
NUMLDocument_getError (NUMLDocument_t *d, unsigned int n)
{
  return d->getError(n);
}


/**
 * Returns the number of errors or warnings encountered during parsing,
 * consistency checking, or attempted translation of this model.
 *
 * @param d the NUMLDocument_t structure
 *
 * @return the number of errors or warnings encountered
 *
 * @see NUMLDocument_setLevelAndVersion(), NUMLDocument_checkConsistency(),
 * NUMLDocument_checkL1Compatibility(),
 * NUMLReader_readNUMLFromString().
 */
LIBNUML_EXTERN
unsigned int
NUMLDocument_getNumErrors (const NUMLDocument_t *d)
{
  return d->getNumErrors();
}


/**
 * Prints to the given output stream all the errors or warnings
 * encountered during parsing, consistency checking, or attempted
 * translation of this model.
 *
 * If no errors have occurred, i.e., NUMLDocument_getNumErrors() == 0, no
 * output will be sent to the stream.
 *
 * The format of the output is:
 *
 *   N error(s):
 *     line NNN: (id) message
 *
 * @param d the NUMLDocument_t structure
 *
 * @param stream the output stream where the messages should be printed
 */
LIBNUML_EXTERN
void
NUMLDocument_printErrors (NUMLDocument_t *d, FILE *stream)
{
  unsigned int numErrors = d->getNumErrors();

  if (numErrors > 0)
  {
    for (unsigned int n = 0; n < numErrors; n++)
    {
      XMLError_print(d->getError(n), stream);
    }
  }
}


/**
 * @return the most recent NUML specification level (at the time this
 * libNUML was released).
 */
unsigned int
NUMLDocument_getDefaultLevel ()
{
  return NUMLDocument::getDefaultLevel();
}


/**
 * @return the most recent NUML specification version (at the time this
 * libNUML was released).
 */
unsigned int
NUMLDocument_getDefaultVersion ()
{
  return NUMLDocument::getDefaultVersion();
}

/**
 * Returns a list of XMLNamespaces_t associated with the XML content
 * of this NUML document.
 *
 * @param d the NUMLDocument_t structure
 *
 * @return pointer to the XMLNamespaces_t structure associated with this NUML object
 */
LIBNUML_EXTERN
const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces_t *
NUMLDocument_getNamespaces(NUMLDocument_t *d)
{
  return d->getNamespaces();
}



ResultComponent *
NUMLDocument::getResultComponent(unsigned int index)
{
  if (index >= getNumResultComponents())
    return NULL;
  return getResultComponents()->get(index);
}

LIBNUML_CPP_NAMESPACE_END
