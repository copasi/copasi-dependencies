/**
 * @file    SpeciesReference.cpp
 * @brief   Implementation of SpeciesReference and ListOfSpeciesReferences. 
 * @author  Ben Bornstein
 *
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2020 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *     3. University College London, London, UK
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *  
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/


#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>

#include <sbml/annotation/RDFAnnotation.h>
#include <sbml/math/FormulaParser.h>
#include <sbml/math/MathML.h>
#include <sbml/math/ASTNode.h>

#include <sbml/SBO.h>
#include <sbml/SBMLVisitor.h>
#include <sbml/SBMLError.h>
#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>
#include <sbml/SpeciesReference.h>
#include <sbml/SimpleSpeciesReference.h>
#include <sbml/ModifierSpeciesReference.h>
#include <sbml/extension/SBasePlugin.h>

/** @cond doxygenIgnored */
using namespace std;
/** @endcond */

LIBSBML_CPP_NAMESPACE_BEGIN
#ifdef __cplusplus


SpeciesReference::SpeciesReference (unsigned int level, unsigned int version) :
   SimpleSpeciesReference( level, version)
 , mStoichiometry        ( 1.0 )
 , mDenominator          ( 1   )
 , mStoichiometryMath    ( NULL   )
 , mConstant             (false)
 , mIsSetConstant        (false)
 , mIsSetStoichiometry   (false)
 , mExplicitlySetStoichiometry (false)
 , mExplicitlySetDenominator   (false)
{

  if (!hasValidLevelVersionNamespaceCombination())
    throw SBMLConstructorException();

  // if level 3 values have no defaults
  if (level == 3)
  {
    mStoichiometry = numeric_limits<double>::quiet_NaN();
  }
}

SpeciesReference::SpeciesReference (SBMLNamespaces *sbmlns) :
   SimpleSpeciesReference( sbmlns )
 , mStoichiometry        ( 1.0 )
 , mDenominator          ( 1   )
 , mStoichiometryMath    ( NULL             )
 , mConstant             (false)
 , mIsSetConstant        (false)
 , mIsSetStoichiometry   (false)
 , mExplicitlySetStoichiometry (false)
 , mExplicitlySetDenominator   (false)
{
  if (!hasValidLevelVersionNamespaceCombination())
  {
    throw SBMLConstructorException(SpeciesReference::getElementName(), sbmlns);
  }

  loadPlugins(sbmlns);

  // if level 3 values have no defaults
  if (sbmlns->getLevel() == 3)
  {
    mStoichiometry = numeric_limits<double>::quiet_NaN();
  }
}


/*
 * Destroys this SpeciesReference.
 */
SpeciesReference::~SpeciesReference ()
{
  delete mStoichiometryMath;
}


/*
 * Copy constructor. Creates a copy of this SpeciesReference.
 */
SpeciesReference::SpeciesReference (const SpeciesReference& orig)
 : SimpleSpeciesReference( orig                )
 , mStoichiometry        ( orig.mStoichiometry )
 , mDenominator          ( orig.mDenominator   )
 , mStoichiometryMath    ( NULL                )
 , mConstant             ( orig.mConstant)
 , mIsSetConstant        ( orig.mIsSetConstant)
 , mIsSetStoichiometry   ( orig.mIsSetStoichiometry)
 , mExplicitlySetStoichiometry ( orig.mExplicitlySetStoichiometry)
 , mExplicitlySetDenominator   ( orig.mExplicitlySetDenominator)
{
  if (orig.mStoichiometryMath != NULL)
  {
    mStoichiometryMath = new StoichiometryMath(*orig.getStoichiometryMath());
    mStoichiometryMath->connectToParent(this);
  }
}


/*
 * Assignment operator
 */
SpeciesReference& SpeciesReference::operator=(const SpeciesReference& rhs)
{
  if(&rhs!=this)
  {
    this->SBase::operator =(rhs);
    this->SimpleSpeciesReference::operator = ( rhs );
    mStoichiometry = rhs.mStoichiometry ;
    mDenominator = rhs.mDenominator   ;
    mConstant = rhs.mConstant;
    mIsSetConstant = rhs.mIsSetConstant;
    mIsSetStoichiometry = rhs.mIsSetStoichiometry;
    mExplicitlySetStoichiometry = rhs.mExplicitlySetStoichiometry;
    mExplicitlySetDenominator = rhs.mExplicitlySetDenominator;

    delete mStoichiometryMath;
    if (rhs.mStoichiometryMath != NULL)
    {
      mStoichiometryMath = new StoichiometryMath(*rhs.getStoichiometryMath());
      mStoichiometryMath->connectToParent(this);
    }
    else
    {
      mStoichiometryMath = NULL;
    }
  }

  return *this;
}


/** @cond doxygenLibsbmlInternal */
bool
SpeciesReference::accept (SBMLVisitor& v) const
{
  bool result = v.visit(*this);
  
  if (mStoichiometryMath != NULL) mStoichiometryMath->accept(v);
  
  return result;
}
/** @endcond */


/*
 * @return a (deep) copy of this SpeciesReference.
 */
SpeciesReference*
SpeciesReference::clone () const
{
  return new SpeciesReference(*this);
}


/*
 * Initializes the fields of this SpeciesReference to their defaults:
 *
 *   - stoichiometry = 1
 *   - denominator   = 1
 */
void
SpeciesReference::initDefaults ()
{
  setStoichiometry(1.0);
  setDenominator(1);

  // mark as not explicitly set
  mExplicitlySetStoichiometry = false;
  mExplicitlySetDenominator   = false;
}


/*
 * @return the stoichiometry of this SpeciesReference.
 */
double
SpeciesReference::getStoichiometry () const
{
  return mStoichiometry;
}


/*
 * @return the stoichiometryMath of this SpeciesReference.
 */
const StoichiometryMath*
SpeciesReference::getStoichiometryMath () const
{
  return mStoichiometryMath;
}


/*
 * @return the stoichiometryMath of this SpeciesReference.
 */
StoichiometryMath*
SpeciesReference::getStoichiometryMath ()
{
  return mStoichiometryMath;
}


/*
 * @return the denominator of this SpeciesReference.
 */
int
SpeciesReference::getDenominator () const
{
  return mDenominator;
}


/*
 * Get the value of the "constant" attribute.
 */
bool 
SpeciesReference::getConstant () const
{
  return mConstant;
}


/*
 * @return @c true if the stoichiometryMath of this SpeciesReference is 
 * set, false otherwise.
 */
bool
SpeciesReference::isSetStoichiometryMath () const
{
  return (mStoichiometryMath != NULL);
}


/*
 * @return @c true if the constant of this SpeciesReference is 
 * set, false otherwise.
 */
bool
SpeciesReference::isSetConstant () const
{
  return mIsSetConstant;
}


/*
 * @return @c true if the stoichiometry of this SpeciesReference is 
 * set, false otherwise.
 */
bool
SpeciesReference::isSetStoichiometry () const
{
  return mIsSetStoichiometry;
}


/*
 * Sets the stoichiometry of this SpeciesReference to value.
 */
int
SpeciesReference::setStoichiometry (double value)
{
   unsetStoichiometryMath();

   mStoichiometry = value;
   mIsSetStoichiometry = true;
   mExplicitlySetStoichiometry = true;
  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * Sets the stoichiometryMath of this SpeciesReference to a copy of the
 * given ASTNode.
 */
int
SpeciesReference::setStoichiometryMath (const StoichiometryMath* math)
{
  if ( getLevel() != 2 )
  {
    return LIBSBML_UNEXPECTED_ATTRIBUTE;
  }

  int returnValue = checkCompatibility(static_cast<const SBase *>(math));
  
  if (returnValue == LIBSBML_OPERATION_FAILED && math == NULL)
  {
    return unsetStoichiometryMath();
  }
  else if (returnValue != LIBSBML_OPERATION_SUCCESS)
  {
    return returnValue;
  }
  
  if (mStoichiometryMath == math) 
  {
    mIsSetStoichiometry = false;
    mExplicitlySetStoichiometry = false;
    mStoichiometry = 1.0;
    mDenominator = 1;
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    mIsSetStoichiometry = false;
    mExplicitlySetStoichiometry = false;
    mStoichiometry      = 1.0;
    mDenominator        = 1;

    delete mStoichiometryMath;
    mStoichiometryMath = static_cast<StoichiometryMath*>(math->clone());
    if (mStoichiometryMath != NULL) mStoichiometryMath->connectToParent(this);
    
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Sets the denominator of this SpeciesReference to value.
 */
int
SpeciesReference::setDenominator (int value)
{
  // this attribute was removed in l2 but we were able to capture it
  // by creating a stoichiometryMath element for the speciesReference
  // however stoichiometryMath was removed in l3 and so would require
  // an initialAssignment which would only work if the SpeciesReference
  // was already placed within a model
  // so we return unexpected attribute in L3
  if (getLevel() < 3)
  {
    mDenominator = value;
    mExplicitlySetDenominator = true;
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    mDenominator = value;
    return LIBSBML_UNEXPECTED_ATTRIBUTE;
  }
}


/*
 * Sets the constant field of this SpeciesReference to value.
 */
int
SpeciesReference::setConstant (bool flag)
{
  if ( getLevel() < 3 )
  {
    mConstant = flag;
    return LIBSBML_UNEXPECTED_ATTRIBUTE;
  }
  else
  {
    mConstant = flag;
    mIsSetConstant = true;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Unsets the "stoichiometryMath" subelement of this SpeciesReference.
 */
int 
SpeciesReference::unsetStoichiometryMath ()
{
  delete mStoichiometryMath;
  mStoichiometryMath = NULL;

  if ( getLevel() != 2 )
  {
    return LIBSBML_UNEXPECTED_ATTRIBUTE;
  }
  else if (!mIsSetStoichiometry)
  {
    // 
    // In SBML Level2, "stoichiometry" attribute is set to 1 (default value)
    // if neither the "stoichiometry" attribute and the "stoichiometryMath" 
    // element are present.
    //
    mIsSetStoichiometry = true;
    mStoichiometry = 1.0;
    mDenominator = 1;
  }

  if (mStoichiometryMath == NULL)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}

/* unset the stoichiometry */
int
SpeciesReference::unsetStoichiometry ()
{
  const unsigned int level = getLevel();

  if ( level > 2 )
  {
    mStoichiometry      = numeric_limits<double>::quiet_NaN();
    mDenominator = 1;
    mIsSetStoichiometry = false;
    mExplicitlySetStoichiometry = false;
    if (!isSetStoichiometry())
    {
      return LIBSBML_OPERATION_SUCCESS;
    }
    else
    {
      return LIBSBML_OPERATION_FAILED;
    }
  }
  else
  {
    mStoichiometry      = 1.0;
    mDenominator = 1;

    if ( level == 2 ) 
    {
      // 
      // In SBML Level2, "stoichiometry" attribute is set to 1 (default value)
      // if neither the "stoichiometry" attribute and the "stoichiometryMath" 
      // element are present.
      //
      if (!isSetStoichiometryMath())
      {
        mIsSetStoichiometry = true;
      }
      else
      {
        mIsSetStoichiometry = false;
        mExplicitlySetStoichiometry = false;
      }
    }
    else
    {
      //
      // In SBML Level 1, "stoichiometry" is always set (default is 1.0).
      //
      mIsSetStoichiometry = true;
    }
  }

  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * Sets the constant field of this SpeciesReference to value.
 */
int
SpeciesReference::unsetConstant ()
{
  if ( getLevel() < 3 )
  {
    mConstant = false;
    mIsSetConstant = false;
    return LIBSBML_UNEXPECTED_ATTRIBUTE;
  }
  else
  {
    // no default
    mIsSetConstant = false;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Creates a new StoichiometryMath, adds it to this SpeciesReference
 * and returns it.
 */
StoichiometryMath*
SpeciesReference::createStoichiometryMath ()
{
  delete mStoichiometryMath;
  mStoichiometryMath = NULL;

  try
  {
    mStoichiometryMath = new StoichiometryMath(getSBMLNamespaces());
  }
  catch (SBMLConstructorException)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * so do nothing
     */
  }

  if (mStoichiometryMath != NULL)
  {
    mStoichiometryMath->connectToParent(this);
    /* this should unset the stoichiometry */
    mStoichiometry = 1.0;
    mDenominator = 1;
    mIsSetStoichiometry = false;
    mExplicitlySetStoichiometry = false;
  }

  return mStoichiometryMath;
}


/*
 * @return the typecode (int) of this SBML object or SBML_UNKNOWN
 * (default).
 *
 * @see getElementName()
 */
int
SpeciesReference::getTypeCode () const
{
  return SBML_SPECIES_REFERENCE;
}


bool 
SpeciesReference::hasRequiredAttributes() const
{
  bool allPresent = SimpleSpeciesReference::hasRequiredAttributes();

  if (getLevel() > 2 && !isSetConstant())
    allPresent = false;

  return allPresent;
}







/** @cond doxygenLibsbmlInternal */

/*
 * Returns the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::getAttribute(const std::string& attributeName,
                               bool& value) const
{
  int return_value = SimpleSpeciesReference::getAttribute(attributeName,
    value);

  if (return_value == LIBSBML_OPERATION_SUCCESS)
  {
    return return_value;
  }

  if (attributeName == "constant")
  {
    value = getConstant();
    return_value = LIBSBML_OPERATION_SUCCESS;
  }

  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Returns the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::getAttribute(const std::string& attributeName,
                               int& value) const
{
  int return_value = SimpleSpeciesReference::getAttribute(attributeName,
    value);

  if (attributeName == "stoichiometry")
  {
    value = (int)(getStoichiometry());
    return_value = LIBSBML_OPERATION_SUCCESS;
  }
  else if (attributeName == "denominator")
  {
    value = getDenominator();
    return_value = LIBSBML_OPERATION_SUCCESS;
  }

  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Returns the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::getAttribute(const std::string& attributeName,
                               double& value) const
{
  int return_value = SimpleSpeciesReference::getAttribute(attributeName,
    value);

  if (return_value == LIBSBML_OPERATION_SUCCESS)
  {
    return return_value;
  }

  if (attributeName == "stoichiometry")
  {
    value = getStoichiometry();
    return_value = LIBSBML_OPERATION_SUCCESS;
  }

  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Returns the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::getAttribute(const std::string& attributeName,
                               unsigned int& value) const
{
  int return_value = SimpleSpeciesReference::getAttribute(attributeName,
    value);

  if (return_value == LIBSBML_OPERATION_SUCCESS)
  {
    return return_value;
  }

  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Returns the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::getAttribute(const std::string& attributeName,
                               std::string& value) const
{
  int return_value = SimpleSpeciesReference::getAttribute(attributeName,
    value);

  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Predicate returning @c true if this SpeciesReference's attribute
 * "attributeName" is set.
 */
bool
SpeciesReference::isSetAttribute(const std::string& attributeName) const
{
  bool value = SimpleSpeciesReference::isSetAttribute(attributeName);

  if (attributeName == "stoichiometry")
  {
    value = isSetStoichiometry();
  }
  else if (attributeName == "constant")
  {
    value = isSetConstant();
  }
  else if (attributeName == "denominator")
  {
    value = true;
  }

  return value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Sets the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::setAttribute(const std::string& attributeName, bool value)
{
  int return_value = SimpleSpeciesReference::setAttribute(attributeName,
    value);

  if (attributeName == "constant")
  {
    return_value = setConstant(value);
  }

  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Sets the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::setAttribute(const std::string& attributeName, int value)
{
  int return_value = SimpleSpeciesReference::setAttribute(attributeName,
    value);

  if (attributeName == "stoichiometry")
  {
    return_value = setStoichiometry(value);
  }
  else if (attributeName == "denominator")
  {
    return_value = setDenominator(value);
  }
  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Sets the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::setAttribute(const std::string& attributeName, double value)
{
  int return_value = SimpleSpeciesReference::setAttribute(attributeName,
    value);

  if (attributeName == "stoichiometry")
  {
    return_value = setStoichiometry(value);
  }

  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Sets the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::setAttribute(const std::string& attributeName,
                               unsigned int value)
{
  int return_value = SimpleSpeciesReference::setAttribute(attributeName,
    value);

  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Sets the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::setAttribute(const std::string& attributeName,
                               const std::string& value)
{
  int return_value = SimpleSpeciesReference::setAttribute(attributeName,
    value);

  return return_value;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Unsets the value of the "attributeName" attribute of this SpeciesReference.
 */
int
SpeciesReference::unsetAttribute(const std::string& attributeName)
{
  int value = SimpleSpeciesReference::unsetAttribute(attributeName);

  if (attributeName == "stoichiometry")
  {
    value = unsetStoichiometry();
  }
  else if (attributeName == "constant")
  {
    value = unsetConstant();
  }
  else if (attributeName == "denominator")
  {
    mDenominator = 1;
    value = LIBSBML_OPERATION_SUCCESS;
  }

  return value;
}

/** @endcond */

/** @cond doxygenLibsbmlInternal */

/*
 * Creates and returns an new "elementName" object in this SpeciesReference.
 */
SBase*
SpeciesReference::createChildObject(const std::string& elementName)
{
  SBase* obj = NULL;

  if (elementName == "stoichiometryMath")
  {
    return createStoichiometryMath();
  }

  return obj;
}

/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Adds an new "elementName" object in this SpeciesReference.
 */
int
SpeciesReference::addChildObject(const std::string& elementName, const SBase* element)
{
  if (elementName == "stoichiometryMath" && element->getTypeCode() == SBML_STOICHIOMETRY_MATH)
  {
    return setStoichiometryMath((const StoichiometryMath*)(element));
  }

  return LIBSBML_OPERATION_FAILED;
}

/** @endcond */


/** @cond doxygenLibsbmlInternal */

/*
 * Adds an new "elementName" object in this SpeciesReference.
 */
SBase*
SpeciesReference::removeChildObject(const std::string& elementName, const std::string& id)
{

  if (elementName == "stoichiometryMath")
  {
    unsetStoichiometryMath();
  }
  return NULL;
}

/** @endcond */


/** @cond doxygenLibsbmlInternal */

/*
 * Returns the number of "elementName" in this SpeciesReference.
 */
unsigned int
SpeciesReference::getNumObjects(const std::string& elementName)
{
  unsigned int n = 0;

  if (elementName == "stoichiometryMath" && isSetStoichiometryMath())
  {
    n=1;
  }

  return n;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Returns the nth object of "objectName" in this SpeciesReference.
 */
SBase*
SpeciesReference::getObject(const std::string& elementName, unsigned int index)
{
  SBase* obj = NULL;

  if (elementName == "stoichiometryMath")
  {
    return getStoichiometryMath();
  }

  return obj;
}

/** @endcond */




/*
 * Sets the annotation of this SBML object to a copy of annotation.
 */
int
SpeciesReference::setAnnotation (const XMLNode* annotation)
{
  int success = SBase::setAnnotation(annotation);

  return success;
}


/*
 * Sets the annotation (by string) of this SBML object to a copy of annotation.
 */
int
SpeciesReference::setAnnotation (const std::string& annotation)
{
  int success = LIBSBML_OPERATION_FAILED;

  if(annotation.empty())
  {
    unsetAnnotation();
    return LIBSBML_OPERATION_SUCCESS;
  }

  XMLNode* annt_xmln;
  if (getSBMLDocument() != NULL)
  {
    const XMLNamespaces* xmlns = getSBMLDocument()->getNamespaces();
    annt_xmln = XMLNode::convertStringToXMLNode(annotation,xmlns);
  }
  else
  {
    annt_xmln = XMLNode::convertStringToXMLNode(annotation);
  }

  if(annt_xmln != NULL)
  {
    success = setAnnotation(annt_xmln);
    delete annt_xmln;
  }
  return success;
}


/*
 * Appends annotation to the existing annotations.
 * This allows other annotations to be preserved whilst
 * adding additional information.
 */
int
SpeciesReference::appendAnnotation (const XMLNode* annotation)
{
  int success = LIBSBML_OPERATION_FAILED;
  if(!annotation) return LIBSBML_OPERATION_SUCCESS;

  XMLNode* new_annotation = annotation->clone();

  success = SBase::appendAnnotation(new_annotation);

  delete new_annotation;

  return success;
}

/*
 * Appends annotation (by string) to the existing annotations.
 * This allows other annotations to be preserved whilst
 * adding additional information.
 */
int
SpeciesReference::appendAnnotation (const std::string& annotation)
{
  int success = LIBSBML_OPERATION_FAILED;
  XMLNode* annt_xmln;
  if (getSBMLDocument() != NULL)
  {
    const XMLNamespaces* xmlns = getSBMLDocument()->getNamespaces();
    annt_xmln = XMLNode::convertStringToXMLNode(annotation,xmlns);
  }
  else
  {
    annt_xmln = XMLNode::convertStringToXMLNode(annotation);
  }

  if(annt_xmln != NULL)
  {
    success = appendAnnotation(annt_xmln);
    delete annt_xmln;
  }

  return success;
}


/*
 * @return the name of this element ie "speciesReference".
 
 */
const string&
SpeciesReference::getElementName () const
{
  static const string specie  = "specieReference";
  static const string species = "speciesReference";

  return (getLevel() == 1 && getVersion() == 1) ? specie : species;
}


/** @cond doxygenLibsbmlInternal */
void
SpeciesReference::sortMath()
{
  if (mStoichiometryMath != NULL && 
    mStoichiometryMath->isSetMath() &&
    mStoichiometryMath->getMath()->isRational())
  {
    mStoichiometry = (double)mStoichiometryMath->getMath()->getNumerator();
    mDenominator   = (int)mStoichiometryMath->getMath()->getDenominator();

    delete mStoichiometryMath;
    mStoichiometryMath = NULL;
  }
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or @c NULL if the token was not recognized.
 */
SBase*
SpeciesReference::createObject (XMLInputStream& stream)
{
  SBase *object = NULL;
  const string& name = stream.peek().getName();
  
  if (name == "stoichiometryMath")
  {
    if (getLevel() != 2)
    {
      return NULL;
    }
    delete mStoichiometryMath;

    try
    {
      mStoichiometryMath = new StoichiometryMath(getSBMLNamespaces());
    }
    catch (SBMLConstructorException)
    {
      mStoichiometryMath = new StoichiometryMath(
                                           SBMLDocument::getDefaultLevel(),
                                           SBMLDocument::getDefaultVersion());
    }
    return mStoichiometryMath;
  }

  return object;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Subclasses should override this method to read (and store) XHTML,
 * MathML, etc. directly from the XMLInputStream.
 *
 * @return @c true if the subclass read from the stream, false otherwise.
 */
bool
SpeciesReference::readOtherXML (XMLInputStream& stream)
{
  bool          read = false;
  const string& name = stream.peek().getName();

  // This has to do additional work for reading annotations, so the code
  // here is copied and expanded from SBase::readNotes().

  if (name == "annotation")
  {
    // if annotation already exists then it is an error 
    if (mAnnotation != NULL)
    {
      if (getLevel() < 3) 
      {
        logError(NotSchemaConformant, getLevel(), getVersion(),
          "Only one <annotation> element is permitted inside a "
          "particular containing element.");
      }
      else
      {
        string msg = "An SBML <speciesReference> element ";
        if (isSetId()) {
          msg += "with the id '" + getId() + "' ";
        }
        msg += "has multiple <annotation> children.";
        logError(MultipleAnnotations, getLevel(), getVersion(), msg);
      }
    }
    delete mAnnotation;
    mAnnotation = new XMLNode(stream);
    checkAnnotation();
    if (mCVTerms != NULL)
    {
      unsigned int size = mCVTerms->getSize();
      while (size--) delete static_cast<CVTerm*>( mCVTerms->remove(0) );
      delete mCVTerms;
    }
    mCVTerms = new List();
    delete mHistory;
    if (RDFAnnotationParser::hasHistoryRDFAnnotation(mAnnotation))
    {
      mHistory = RDFAnnotationParser::parseRDFAnnotation(mAnnotation, 
                                            getMetaId().c_str(), &(stream), this);

      if (mHistory != NULL && mHistory->hasRequiredAttributes() == false)
      {
        logError(RDFNotCompleteModelHistory, getLevel(), getVersion(),
          "An invalid ModelHistory element has been stored.");
      }
      setModelHistory(mHistory);
    }
    else
      mHistory = NULL;
    if (RDFAnnotationParser::hasCVTermRDFAnnotation(mAnnotation))
      RDFAnnotationParser::parseRDFAnnotation(mAnnotation, mCVTerms, 
                                               getMetaId().c_str(), &(stream));

    read = true;
  }

  /* ------------------------------
   *
   *   (EXTENSION)
   *
   * ------------------------------ */
  if ( SBase::readOtherXML(stream) )
    read = true;

  return read;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/**
 * Subclasses should override this method to get the list of
 * expected attributes.
 * This function is invoked from corresponding readAttributes()
 * function.
 */
void
SpeciesReference::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SimpleSpeciesReference::addExpectedAttributes(attributes);

  const unsigned int level   = getLevel  ();

  attributes.add("stoichiometry");
  if (level == 1)
  {
    attributes.add("denominator");
  }
  if (level > 2)
  {
    attributes.add("constant");
  }
}


/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parent's implementation of this method as well.
 */
void
SpeciesReference::readAttributes (const XMLAttributes& attributes,
                                  const ExpectedAttributes& expectedAttributes)
{
  SimpleSpeciesReference::readAttributes(attributes,expectedAttributes);

  const unsigned int level   = getLevel  ();
  switch (level)
  {
  case 1:
    readL1Attributes(attributes);
    break;
  case 2:
    readL2Attributes(attributes);
    break;
  default: //case 3:
    readL3Attributes(attributes);
    break;
  }
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parent's implementation of this method as well.
 */
void
SpeciesReference::readL1Attributes (const XMLAttributes& attributes)
{
  //
  // stoichiometry: integer  { use="optional" default="1" }  (L1v1, L1v2)
  // stoichiometry: double   { use="optional" default="1" }  (L2v1->)
  //
  mIsSetStoichiometry = attributes.readInto("stoichiometry", mStoichiometry, getErrorLog(), false, getLine(), getColumn());
  if (!mIsSetStoichiometry)
  {
    //  
    // setting default value
    //
    mStoichiometry = 1;
    mIsSetStoichiometry = true;
  }
  else
  {
    mExplicitlySetStoichiometry = true;
  }

  //
  // denominator: integer  { use="optional" default="1" }  (L1v1, L1v2)
  //
  mExplicitlySetDenominator = attributes.readInto("denominator", mDenominator,getErrorLog(), false, getLine(), getColumn());

}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parent's implementation of this method as well.
 */
void
SpeciesReference::readL2Attributes (const XMLAttributes& attributes)
{
  // stoichiometry: double   { use="optional" default="1" }  (L2v1->)
  //
  mIsSetStoichiometry = attributes.readInto("stoichiometry", mStoichiometry, getErrorLog(), false, getLine(), getColumn());
  mExplicitlySetStoichiometry = mIsSetStoichiometry;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parent's implementation of this method as well.
 */
void
SpeciesReference::readL3Attributes (const XMLAttributes& attributes)
{
  const unsigned int level = 3;
  const unsigned int version = getVersion();
  //
  // stoichiometry: double   { use="optional" default="1" }  (L2v1->)
  //
  mIsSetStoichiometry = attributes.readInto("stoichiometry", mStoichiometry, getErrorLog(), false, getLine(), getColumn());
  mExplicitlySetStoichiometry = mIsSetStoichiometry;

  string elplusid = "<" + getElementName() + ">";
  if (!mId.empty()) {
    elplusid += " with the id '" + mId + "'";
  }
  const SBase* rxn = getAncestorOfType(SBML_REACTION);
  if (rxn && rxn->isSetId()) 
  {
    elplusid += " from the <reaction> with the id '" + rxn->getId() + "'";
  }
  //
  // constant: bool { use="required" } (L3v1 -> )
  //
  mIsSetConstant = attributes.readInto("constant", mConstant, getErrorLog(), false, getLine(), getColumn());
  
  if (!mIsSetConstant && !isModifier())
  {
    logError(AllowedAttributesOnSpeciesReference, level, version, 
             "The required attribute 'constant' is missing from the "
             + elplusid + ".");
  }

}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parent's implementation
 * of this method as well.
 */
void
SpeciesReference::writeAttributes (XMLOutputStream& stream) const
{
  SimpleSpeciesReference::writeAttributes(stream);

  if (getLevel() == 1)
  {
    //
    // stoichiometry: integer  { use="optional" default="1" }  (L1v1, L1v2)
    //
    int s = static_cast<int>( mStoichiometry );
    if (isExplicitlySetStoichiometry() || s != 1) stream.writeAttribute("stoichiometry", s);

    //
    // denominator  { use="optional" default="1" }  (L1v1, L1v2)
    //
    if (isExplicitlySetDenominator() || mDenominator != 1) stream.writeAttribute("denominator", mDenominator);
  }
  else if (getLevel() == 2)
  {
    //
    // stoichiometry: double   { use="optional" default="1" }  (L2v1, L2v2)
    //
    if ((mDenominator == 1) && 
      (mStoichiometry != 1 || isExplicitlySetStoichiometry()))
    {
      stream.writeAttribute("stoichiometry", mStoichiometry);
    }
  }
  else
  {
    if (isSetStoichiometry())
      stream.writeAttribute("stoichiometry", mStoichiometry);
  }
  //
  // constant: bool { use="required" } (L3v1 -> )
  //
  if (getLevel() > 2 && isSetConstant())
  {
    // in L3 only write it out if it has been set
    stream.writeAttribute("constant", mConstant);
  }
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Subclasses should override this method to write out their contained
 * SBML objects as XML elements.  Be sure to call your parent's
 * implementation of this method as well.
 */
void
SpeciesReference::writeElements (XMLOutputStream& stream) const
{
  if (mNotes != NULL)
  {
    mNotes->writeToStream(stream);
  }
  SpeciesReference * sr = const_cast <SpeciesReference *> (this);
  sr->syncAnnotation();
  if ( mAnnotation != NULL ) stream << *mAnnotation;

  if (
      getLevel() == 2
      && (mStoichiometryMath || mDenominator != 1)
     )
  {
    if (mStoichiometryMath != NULL) 
    {
      mStoichiometryMath->write(stream);
    }
    else
    {
      ASTNode node;
      node.setValue(static_cast<long>(mStoichiometry), mDenominator);

      stream.startElement("stoichiometryMath");
      writeMathML(&node, stream);
      stream.endElement("stoichiometryMath");
    }
  }

  //
  // (EXTENSION)
  //
  SBase::writeExtensionElements(stream);

}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
bool 
SpeciesReference::isExplicitlySetStoichiometry() const 
{
  return mExplicitlySetStoichiometry;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
bool 
SpeciesReference::isExplicitlySetDenominator() const 
{
  return mExplicitlySetDenominator;
}
/** @endcond */


/*
 * Creates a new ListOfSpeciesReferences items.
 */
ListOfSpeciesReferences::ListOfSpeciesReferences (unsigned int level, unsigned int version)
  : ListOf(level,version)
 , mType(Unknown)
{
}


/*
 * Creates a new ListOfSpeciesReferences items.
 */
ListOfSpeciesReferences::ListOfSpeciesReferences (SBMLNamespaces* sbmlns)
  : ListOf(sbmlns)
 , mType(Unknown)
{
  loadPlugins(sbmlns);
}


/*
 * @return a (deep) copy of this ListOfSpeciesReferences.
 */
ListOfSpeciesReferences*
ListOfSpeciesReferences::clone () const
{
  return new ListOfSpeciesReferences(*this);
}


/*
 * @return the typecode (int) of SBML objects contained in this ListOf or
 * SBML_UNKNOWN (default).
 */
int
ListOfSpeciesReferences::getItemTypeCode () const
{
  switch(mType)
  {
  case Reactant:
  case Product:
    return SBML_SPECIES_REFERENCE;
  case Modifier:
    return SBML_MODIFIER_SPECIES_REFERENCE;
  case Unknown:
    return SBML_UNKNOWN;
  default:
    return SBML_UNKNOWN;
  }
}


/*
 * @return the name of this element ie "listOfReactants" or "listOfProducts" etc.
 */
const string&
ListOfSpeciesReferences::getElementName () const
{
  static const string unknown   = "listOfUnknowns";
  static const string reactants = "listOfReactants";
  static const string products  = "listOfProducts";
  static const string modifiers = "listOfModifiers";

       if (mType == Reactant) return reactants;
  else if (mType == Product ) return products;
  else if (mType == Modifier) return modifiers;
  else return unknown;
}


/**
 * Used by ListOfSpeciesReferences::get() to lookup an SBase based by its id.
 */
struct IdEqSSR
{
  const string& mId;

  explicit IdEqSSR (const string& id) : mId(id) { }
  bool operator() (SBase* sb) const
       { return (static_cast <SimpleSpeciesReference *> (sb)->getId()  == mId)
         || (static_cast <SimpleSpeciesReference *> (sb)->getSpecies() == mId); } 
};


/* return nth item in list */
SimpleSpeciesReference *
ListOfSpeciesReferences::get(unsigned int n)
{
  return static_cast<SimpleSpeciesReference*>(ListOf::get(n));
}


/* return nth item in list */
const SimpleSpeciesReference *
ListOfSpeciesReferences::get(unsigned int n) const
{
  return static_cast<const SimpleSpeciesReference*>(ListOf::get(n));
}


/* return item by id */
SimpleSpeciesReference*
ListOfSpeciesReferences::get (const std::string& sid)
{
  return const_cast<SimpleSpeciesReference*>( 
    static_cast<const ListOfSpeciesReferences&>(*this).get(sid) );
}


/* return item by id */
const SimpleSpeciesReference*
ListOfSpeciesReferences::get (const std::string& sid) const
{
  vector<SBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqSSR(sid) );
  return (result == mItems.end()) ? NULL : 
                           static_cast <SimpleSpeciesReference*> (*result);
}


/* Removes the nth item from this list */
SimpleSpeciesReference*
ListOfSpeciesReferences::remove (unsigned int n)
{
   return static_cast<SimpleSpeciesReference*>(ListOf::remove(n));
}


/* Removes item in this list by id */
SimpleSpeciesReference*
ListOfSpeciesReferences::remove (const std::string& sid)
{
  SBase* item = NULL;
  vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqSSR(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <SimpleSpeciesReference*> (item);
}


/** @cond doxygenLibsbmlInternal */
/*
 * @return the ordinal position of the element with respect to its siblings
 * or -1 (default) to indicate the position is not significant.
 */
int
ListOfSpeciesReferences::getElementPosition () const
{
  int position;

  switch (mType)
  {
    case Reactant: position =  1; break;
    case Product:  position =  2; break;
    case Modifier: position =  3; break;
    default:       position = -1; break;
  }

  return position;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Sets type of this ListOfSpeciesReferences.
 */
void
ListOfSpeciesReferences::setType (SpeciesType type)
{
  mType = type;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * gets type of this ListOfSpeciesReferences.
 */
unsigned int
ListOfSpeciesReferences::getType() const
{
  switch (mType)
  {
  case Unknown:
    return 0;

  case Reactant:
    return 1;

  case Product:
    return 2; 

  case Modifier:
    return 3;

  default:
    return 0;

  }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or @c NULL if the token was not recognized.
 */
SBase*
ListOfSpeciesReferences::createObject (XMLInputStream& stream)
{
  const string& name   = stream.peek().getName();
  SBase*        object = NULL;


  if (mType == Reactant || mType == Product)
  {
    if (name == "speciesReference" || name == "specieReference")
    {
      try
      {
        object = new SpeciesReference(getSBMLNamespaces());
      }
      catch (SBMLConstructorException)
      {
        object = new SpeciesReference(SBMLDocument::getDefaultLevel(),
        SBMLDocument::getDefaultVersion());
      }
    }
    else if (name == "annotation" || name == "notes")
    {
      // do nothing
    }
    else
    {
      /* create the object anyway - or will also get unrecognized element message 
       * which is confusion if user has merely reversed modifierSpeciesReference
       * and speciesReference */
      try
      {
        object = new SpeciesReference(getSBMLNamespaces());
      }
      catch (SBMLConstructorException)
      {
        object = new SpeciesReference(SBMLDocument::getDefaultLevel(),
        SBMLDocument::getDefaultVersion());
      }
      logError(InvalidReactantsProductsList);
    }
  }
  else if (mType == Modifier)
  {
    if (name == "modifierSpeciesReference")
    {
      try
      {
        object = new ModifierSpeciesReference(getSBMLNamespaces());
      }
      catch (SBMLConstructorException)
      {
        object = new ModifierSpeciesReference(SBMLDocument::getDefaultLevel(),
        SBMLDocument::getDefaultVersion());
      }
    }
    else if (name == "annotation" || name == "notes")
    {
      // do nothing
    }
    else
    {
      try
      {
        object = new ModifierSpeciesReference(getSBMLNamespaces());
      }
      catch (SBMLConstructorException)
      {
        object = new ModifierSpeciesReference(SBMLDocument::getDefaultLevel(),
        SBMLDocument::getDefaultVersion());
      }
      logError(InvalidModifiersList);
    }
  }

  if (object != NULL) mItems.push_back(object);

  return object;
}
/** @endcond */


#endif /* __cplusplus */
/** @cond doxygenIgnored */
LIBSBML_EXTERN
SpeciesReference_t *
SpeciesReference_create (unsigned int level, unsigned int version)
{
  try
  {
    SpeciesReference* obj = new SpeciesReference(level,version);
    return obj;
  }
  catch (SBMLConstructorException)
  {
    return NULL;
  }
}


LIBSBML_EXTERN
SpeciesReference_t *
SpeciesReference_createWithNS (SBMLNamespaces_t* sbmlns)
{
  try
  {
    SpeciesReference* obj = new SpeciesReference(sbmlns);
    return obj;
  }
  catch (SBMLConstructorException)
  {
    return NULL;
  }
}


LIBSBML_EXTERN
SpeciesReference_t *
SpeciesReference_createModifier (unsigned int level, unsigned int version)
{
  try
  {
    ModifierSpeciesReference* obj = new ModifierSpeciesReference(level,version);
    return obj;
  }
  catch (SBMLConstructorException)
  {
    return NULL;
  }
}


LIBSBML_EXTERN
SpeciesReference_t *
SpeciesReference_createModifierWithNS (SBMLNamespaces_t* sbmlns)
{
  try
  {
    ModifierSpeciesReference* obj = new ModifierSpeciesReference(sbmlns);
    return obj;
  }
  catch (SBMLConstructorException)
  {
    return NULL;
  }
}


LIBSBML_EXTERN
void
SpeciesReference_free (SpeciesReference_t *sr)
{
  if (sr != NULL)
    delete sr;
}


LIBSBML_EXTERN
SpeciesReference_t *
SpeciesReference_clone (const SpeciesReference_t *sr)
{
  return (sr != NULL) ? static_cast<SpeciesReference_t*>( sr->clone() ) : NULL;
}


LIBSBML_EXTERN
void
SpeciesReference_initDefaults (SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return;
    static_cast<SpeciesReference*>(sr)->initDefaults();
  }
}


LIBSBML_EXTERN
const XMLNamespaces_t *
SpeciesReference_getNamespaces(const SpeciesReference_t *sr)
{
  return (sr != NULL) ? sr->getNamespaces() : NULL;
}

LIBSBML_EXTERN
int
SpeciesReference_isModifier (const SpeciesReference_t *sr)
{
  return (sr != NULL) ? static_cast<int>( sr->isModifier() ) : 0;
}


LIBSBML_EXTERN
const char *
SpeciesReference_getId (const SpeciesReference_t *sr)
{
  return (sr != NULL && sr->isSetId()) ? sr->getId().c_str() : NULL;
}


LIBSBML_EXTERN
const char *
SpeciesReference_getName (const SpeciesReference_t *sr)
{
  return (sr != NULL && sr->isSetName()) ? sr->getName().c_str() : NULL;
}


LIBSBML_EXTERN
const char *
SpeciesReference_getSpecies (const SpeciesReference_t *sr)
{
  return (sr != NULL && sr->isSetSpecies()) ? sr->getSpecies().c_str() : NULL;
}


LIBSBML_EXTERN
double
SpeciesReference_getStoichiometry (const SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return 0.0;
    return static_cast<const SpeciesReference*>(sr)->getStoichiometry();
  }
  else
    return numeric_limits<double>::quiet_NaN();
}


LIBSBML_EXTERN
StoichiometryMath_t *
SpeciesReference_getStoichiometryMath (SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return NULL;
    return static_cast<SpeciesReference*>(sr)->getStoichiometryMath();
  }
  else
    return NULL;
}


LIBSBML_EXTERN
int
SpeciesReference_getDenominator (const SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return 0;
    return static_cast<const SpeciesReference*>(sr)->getDenominator();
  }
  else
    return SBML_INT_MAX;
}


LIBSBML_EXTERN
int
SpeciesReference_getConstant (const SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return 0;
    return static_cast<const SpeciesReference*>(sr)->getConstant();
  }
  else
    return 0;
}


LIBSBML_EXTERN
int
SpeciesReference_isSetId (const SpeciesReference_t *sr)
{
  return (sr != NULL) ? static_cast<int>( sr->isSetId() ) : 0;
}


LIBSBML_EXTERN
int
SpeciesReference_isSetName (const SpeciesReference_t *sr)
{
  return (sr != NULL) ? static_cast<int>( sr->isSetName() ) : 0;
}


LIBSBML_EXTERN
int
SpeciesReference_isSetSpecies (const SpeciesReference_t *sr)
{
  return (sr != NULL) ? static_cast<int>( sr->isSetSpecies() ) : 0;
}


LIBSBML_EXTERN
int
SpeciesReference_isSetStoichiometryMath (const SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return 0;

    return static_cast<int>
    (
      static_cast<const SpeciesReference*>(sr)->isSetStoichiometryMath()
    );
  }
  else
    return 0;
}


LIBSBML_EXTERN
int
SpeciesReference_isSetStoichiometry (const SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return 0;

    return static_cast<int>( 
      static_cast<const SpeciesReference*>(sr)->isSetStoichiometry() );
  }
  else
    return 0;
}


LIBSBML_EXTERN
int
SpeciesReference_isSetConstant (const SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return 0;

    return static_cast<int>
      (static_cast<const SpeciesReference*>(sr)->isSetConstant() );
  }
  else
    return 0;
}


LIBSBML_EXTERN
int
SpeciesReference_setId (SpeciesReference_t *sr, const char *sid)
{
  if (sr != NULL)
    return (sid == NULL) ? sr->unsetId() : sr->setId(sid);
  else
    return LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_setName (SpeciesReference_t *sr, const char *name)
{
  if (sr != NULL)
    return (name == NULL) ? sr->unsetName() : sr->setName(name);
  else
    return LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_setSpecies (SpeciesReference_t *sr, const char *sid)
{
  if (sr != NULL)
    return sr->setSpecies(sid != NULL ? sid : "");
  else
    return LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_setStoichiometry (SpeciesReference_t *sr, double value)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return LIBSBML_UNEXPECTED_ATTRIBUTE;
    return static_cast<SpeciesReference*>(sr)->setStoichiometry(value);
  }
  else
    return LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
StoichiometryMath_t *
SpeciesReference_createStoichiometryMath (SpeciesReference_t *sr)
{
  return (sr != NULL) ? 
    static_cast<SpeciesReference*> (sr)->createStoichiometryMath() : NULL;
}

LIBSBML_EXTERN
int
SpeciesReference_setStoichiometryMath (  SpeciesReference_t *sr
                                       , const StoichiometryMath_t    *math )
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return LIBSBML_UNEXPECTED_ATTRIBUTE;
    return static_cast<SpeciesReference*>(sr)->setStoichiometryMath(math);
  }
  else
    return LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_setDenominator (SpeciesReference_t *sr, int value)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return LIBSBML_UNEXPECTED_ATTRIBUTE;
    return static_cast<SpeciesReference*>(sr)->setDenominator(value);
  }
  else
    return LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_setConstant (SpeciesReference_t *sr, int value)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return LIBSBML_UNEXPECTED_ATTRIBUTE;
    return static_cast<SpeciesReference*>(sr)->setConstant(value);
  }
  else
    return LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_unsetId (SpeciesReference_t *sr)
{
  return (sr != NULL) ? sr->unsetId() : LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_unsetName (SpeciesReference_t *sr)
{
  return (sr != NULL) ? sr->unsetName() : LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_unsetSpecies (SpeciesReference_t *sr)
{
  return (sr != NULL) ? sr->unsetSpecies() : LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_unsetConstant (SpeciesReference_t *sr)
{
  return (sr != NULL) ? static_cast<SpeciesReference*>(sr)->unsetConstant() 
                      : LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_unsetStoichiometryMath (SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return LIBSBML_UNEXPECTED_ATTRIBUTE;
    return static_cast<SpeciesReference*>(sr)->unsetStoichiometryMath();
  }
  else
    return LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_unsetStoichiometry (SpeciesReference_t *sr)
{
  if (sr != NULL)
  {
    if (sr->isModifier()) return LIBSBML_UNEXPECTED_ATTRIBUTE;
    return static_cast<SpeciesReference*>(sr)->unsetStoichiometry();
  }
  else
    return LIBSBML_INVALID_OBJECT;
}


LIBSBML_EXTERN
int
SpeciesReference_hasRequiredAttributes(SpeciesReference_t *sr)
{
  return (sr != NULL) ? static_cast<int>(
    static_cast<SpeciesReference*>(sr)->hasRequiredAttributes()) : 0;
}


LIBSBML_EXTERN
SpeciesReference_t *
ListOfSpeciesReferences_getById (ListOf_t *lo, const char *sid)
{
  if (lo != NULL)
    return (sid != NULL) ? 
      static_cast <ListOfSpeciesReferences *> (lo)->get(sid) : NULL;
  else
    return NULL;
}


LIBSBML_EXTERN
SpeciesReference_t *
ListOfSpeciesReferences_removeById (ListOf_t *lo, const char *sid)
{
  if (lo != NULL)
    return (sid != NULL) ? 
      static_cast <ListOfSpeciesReferences *> (lo)->remove(sid) : NULL;
  else
    return NULL;
}
/** @endcond */

LIBSBML_CPP_NAMESPACE_END

