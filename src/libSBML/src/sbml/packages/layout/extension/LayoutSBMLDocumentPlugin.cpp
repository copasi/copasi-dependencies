/**
 * @file   LayoutSBMLDocumentPlugin.cpp
 * @brief  Implementation of the LayoutSBMLDocumentPlugin class
 * @author Generated by autocreate code
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
 * ------------------------------------------------------------------------ -->
 */
#include <sbml/packages/layout/extension/LayoutSBMLDocumentPlugin.h>
#include <sbml/packages/layout/validator/LayoutConsistencyValidator.h>
#include <sbml/packages/layout/validator/LayoutIdentifierConsistencyValidator.h>
#include <sbml/packages/layout/validator/LayoutSBMLError.h>

#include <sbml/util/MetaIdFilter.h>
#include <sbml/util/IdFilter.h>
#include <sbml/Model.h>


#ifdef __cplusplus

/** @cond doxygenLibsbmlInternal */
using namespace std;
/** @endcond */

LIBSBML_CPP_NAMESPACE_BEGIN

LayoutSBMLDocumentPlugin::LayoutSBMLDocumentPlugin (const std::string& uri, 
                                                    const std::string &prefix, LayoutPkgNamespaces *layoutns)
  : SBMLDocumentPlugin(uri,prefix, layoutns)
  , mElementsWithId(NULL)
  , mValidationListsPopulated ( false )
{
}


LayoutSBMLDocumentPlugin::LayoutSBMLDocumentPlugin(const LayoutSBMLDocumentPlugin& orig)
  : SBMLDocumentPlugin(orig)
  , mElementsWithId(NULL)
  , mValidationListsPopulated ( false )
{
}


LayoutSBMLDocumentPlugin& 
LayoutSBMLDocumentPlugin::operator=(const LayoutSBMLDocumentPlugin& orig)
{
  if(&orig!=this)
  {
    SBMLDocumentPlugin::operator =(orig);
    if (mElementsWithId != NULL) 
    delete mElementsWithId;
    mValidationListsPopulated = false;
    mElementsWithId = NULL;
    
  }    
  return *this;
}


/*
 * Creates and returns a deep copy of this LayoutSBMLDocumentPlugin object.
 */
LayoutSBMLDocumentPlugin* 
LayoutSBMLDocumentPlugin::clone () const
{
  return new LayoutSBMLDocumentPlugin(*this);  
}

/*
 * Destroy this object.
 */
LayoutSBMLDocumentPlugin::~LayoutSBMLDocumentPlugin () 
{
  if (mElementsWithId != NULL) delete mElementsWithId;
}


/** @cond doxygenLibsbmlInternal */
void 
LayoutSBMLDocumentPlugin::readAttributes (const XMLAttributes& attributes,
                            const ExpectedAttributes&)
{
  // for now don't read the required flag for L2 models 
  if (getSBMLDocument() != NULL && getSBMLDocument()->getLevel() < 3) return;

  // skip reading the required flag for L2 layout on l3 models where we would remove it before writing
  if (mURI == LayoutExtension::getXmlnsL2() && getSBMLDocument() != NULL && getSBMLDocument()->getLevel() > 2) 
    return;
  
  unsigned int numErrs = getErrorLog()->getNumErrors();
  XMLTriple tripleRequired("required", mURI, getPrefix());
  bool assigned = attributes.readInto(tripleRequired, mRequired);
  if (assigned == false)
  {
    if (getErrorLog()->getNumErrors() == numErrs + 1 && 
        getErrorLog()->contains(XMLAttributeTypeMismatch))
    {
      getErrorLog()->remove(XMLAttributeTypeMismatch);
      getErrorLog()->logPackageError("layout", LayoutAttributeRequiredMustBeBoolean,
        getPackageVersion(), getLevel(), getVersion(), "", getLine(), getColumn());
    }
    else
    {
      getErrorLog()->logPackageError("layout", LayoutAttributeRequiredMissing,
        getPackageVersion(), getLevel(), getVersion(), "", getLine(), getColumn());
    }
  }
  else
  {
    mIsSetRequired = true;
    /* LOG ERROR RELATING TO EXPECTED VALUE */
    if (mRequired == true)
    {
      getErrorLog()->logPackageError("layout", LayoutRequiredFalse,
        getPackageVersion(), getLevel(), getVersion(), "", getLine(), getColumn());
    }
  }
}
/** @endcond*/


/** @cond doxygenLibsbmlInternal */
bool
LayoutSBMLDocumentPlugin::isCompFlatteningImplemented() const
{
  return true;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
unsigned int 
LayoutSBMLDocumentPlugin::checkConsistency()
{
  unsigned int nerrors = 0;
  unsigned int total_errors = 0;

  SBMLDocument* doc = static_cast<SBMLDocument *>(this->getParentSBMLObject());

  /* populate the listofMetaids in the model*/
  if (haveValidationListsBeenPopulated() == false)
  {
    populateValidationLists();
  }

  SBMLErrorLog *log = doc->getErrorLog();

  unsigned char applicableValidators = doc->getApplicableValidators();

  /* determine which validators to run */
  bool id    = ((applicableValidators & 0x01) == 0x01);
  bool sbml  = ((applicableValidators & 0x02) == 0x02);
  /* LIST OTHERS HERE */

  LayoutIdentifierConsistencyValidator id_validator;
  LayoutConsistencyValidator validator;
  /* LIST OTHERS HERE */

  if (id)
  {
    id_validator.init();
    nerrors = id_validator.validate(*doc);
    total_errors += nerrors;
    if (nerrors > 0) 
    {
      log->add(id_validator.getFailures() );
      /* only want to bail if errors not warnings */
      if (log->getNumFailsWithSeverity(LIBSBML_SEV_ERROR) > 0)
      {
        return total_errors;
      }
    }
  }

  if (sbml)
  {
    validator.init();
    nerrors = validator.validate(*doc);
    total_errors += nerrors;
    if (nerrors > 0) 
    {
      log->add(validator.getFailures() );
      // DO NOT NEED THIS IN LAST CALL
      // /* only want to bail if errors not warnings */
      // if (log->getNumFailsWithSeverity(LIBSBML_SEV_ERROR) > 0)
      // {
      //   return total_errors;
      // }
    }
  }

  /* ADD OTHERS HERE */

  return total_errors;  
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
IdList
LayoutSBMLDocumentPlugin::getMetaidList() const
{
  return mMetaIdList;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
IdList
LayoutSBMLDocumentPlugin::getIdList() const
{
  return mIdList;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
List *
LayoutSBMLDocumentPlugin::getListElementsWithId() const
{
  return mElementsWithId;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
bool
LayoutSBMLDocumentPlugin::haveValidationListsBeenPopulated() const
{
  return mValidationListsPopulated;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void
LayoutSBMLDocumentPlugin::populateValidationLists()
{
  
  delete mElementsWithId;
  mElementsWithId = NULL;
  
  mMetaIdList.clear();
  mIdList.clear();

  SBMLDocument* doc = static_cast<SBMLDocument *>(this->getParentSBMLObject());

  MetaIdFilter metaidFilter;

  Model* mod = doc->getModel();
  List* allElementsWithMetaid;
  if (mod==NULL) {
    allElementsWithMetaid = new List();
  }
  else {
    allElementsWithMetaid = doc->getModel()->getAllElements(&metaidFilter);
  }  

  for (ListIterator iter = allElementsWithMetaid->begin(); iter != allElementsWithMetaid->end(); ++iter)
  {
    mMetaIdList.append(static_cast<SBase*>(*iter)->getMetaId());
  }
  
  delete allElementsWithMetaid;
  
  IdFilter idFilter;

  if (mod==NULL) {
    mElementsWithId = new List();
  }
  else {
    mElementsWithId = doc->getModel()->getAllElements(&idFilter);
  }  
  
  for (ListIterator iter = mElementsWithId->begin(); iter != mElementsWithId->end(); ++iter)
  {
    mIdList.append(static_cast<SBase*>(*iter)->getId());
  }

  mValidationListsPopulated = true;
}
/** @endcond */

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
