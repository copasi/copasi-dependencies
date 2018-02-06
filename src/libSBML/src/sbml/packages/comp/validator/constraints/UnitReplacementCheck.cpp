/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    UnitReplacementCheck.cpp
 * @brief   Ensures the appropriate ids within a Model are unique
 * @author  Sarah Keating
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#include <sbml/Model.h>
#include <sbml/util/ElementFilter.h>
#include "UnitReplacementCheck.h"


/** @cond doxygenIgnored */
using namespace std;
/** @endcond */

LIBSBML_CPP_NAMESPACE_BEGIN
#ifdef __cplusplus
class ReplacedFilter : public ElementFilter
{
public:
  ReplacedFilter() : ElementFilter()
  {
  }

  virtual bool filter(const SBase* element)
  {
    // return in case we don't have a valid element with a comp plugin
    if (element == NULL)
    {
      return false;
    }
    const CompSBasePlugin * plug = static_cast<const CompSBasePlugin*>
                                   (element->getPlugin("comp"));
    if (plug == NULL)
    {
      return false;
    }
    // otherwise we have a comp plugin
    // we want to keep the object if it has ReplacedElements
    if (plug->getNumReplacedElements() == 0)
    {
      return false;
    }


    return true;
  }

};


class ReplacedByFilter : public ElementFilter
{
public:
  ReplacedByFilter() : ElementFilter()
  {
  }

  virtual bool filter(const SBase* element)
  {
    // return in case we don't have a valid element with a comp plugin
    if (element == NULL)
    {
      return false;
    }
    const CompSBasePlugin * plug = static_cast<const CompSBasePlugin*>
                                   (element->getPlugin("comp"));
    if (plug == NULL)
    {
      return false;
    }
    // otherwise we have a comp plugin
    // we want to keep the object if it has ReplacedElements
    if (plug->getReplacedBy() == NULL)
    {
      return false;
    }


    return true;
  }

};


/*
 * Creates a new Constraint with the given constraint id.
 */
 UnitReplacementCheck::UnitReplacementCheck (unsigned int id, CompValidator& v):
  TConstraint<Model>(id, v)
{
}


/*
 * Destroys this Constraint.
 */
UnitReplacementCheck::~UnitReplacementCheck ()
{
}


/*
 * Checks that all ids on the following Model objects are unique:
 * FunctionDefinitions, Species, Compartments, global Parameters,
 * Reactions, and Events.
 */
void
UnitReplacementCheck::check_ (const Model& m, const Model&)
{
  unsigned int n, size;
  const CompSBasePlugin * plug;
  ReplacedFilter filter;
  ReplacedByFilter repByFilter;

  /* get all elements that have replaced elements */
  List* allElements = const_cast<Model *>(&m)->getAllElements(&filter);

  size = allElements->getSize();


  for (n = 0; n < size; ++n) 
  {
    SBase *sb = static_cast<SBase*>(allElements->get(n));
    plug = static_cast<const CompSBasePlugin*>(sb->getPlugin("comp"));

    for (unsigned int i = 0; i < plug->getNumReplacedElements(); i++)
    {
      checkReferencedElement(*(const_cast<ReplacedElement*>
                                        (plug->getReplacedElement(i))),
                                        m);
    }
  }

  delete allElements;

  /* get all elements that have replaced elements */
  allElements = const_cast<Model *>(&m)->getAllElements(&repByFilter);

  size = allElements->getSize();


  for (n = 0; n < size; ++n) 
  {
    SBase *sb = static_cast<SBase*>(allElements->get(n));
    plug = static_cast<const CompSBasePlugin*>(sb->getPlugin("comp"));

    checkReferencedElement(*(const_cast<ReplacedBy*>
                                        (plug->getReplacedBy())));
  }

  delete allElements;
}

void 
UnitReplacementCheck::checkReferencedElement(ReplacedBy& repBy)
{
  unsigned int numErrsB4 = repBy.getSBMLDocument()->getNumErrors();
  
  SBase* refElem = repBy.getReferencedElement();
  
  // if there is an issue with references the getReferencedElement
  // will log errors and possibly fail
  // we dont want to try any further
  unsigned int numErrsAfter = repBy.getSBMLDocument()->getNumErrors();

  if (numErrsB4 != numErrsAfter || refElem == NULL)
  {
    return;
  }

  SBase * parent = repBy.getParentSBMLObject();
  UnitDefinition *parentUnits = parent->getDerivedUnitDefinition();
  //bool delparunit = parent->getTypeCode()==SBML_PARAMETER;
  
  UnitDefinition *refElemUnits = refElem->getDerivedUnitDefinition();
  //bool delrefunit = refElem->getTypeCode()==SBML_PARAMETER;

  if (parentUnits == NULL || refElemUnits == NULL)
  {
    //if (delparunit)
    //{
    //  delete parentUnits;
    //}
    //if (delrefunit)
    //{
    //  delete refElemUnits;
    //}
    return;
  }

  if (parent->containsUndeclaredUnits() == true ||
    refElem->containsUndeclaredUnits() == true)
  {
    //if (delparunit)
    //{
    //  delete parentUnits;
    //}
    //if (delrefunit)
    //{
    //  delete refElemUnits;
    //}
    return;
  }

  if (UnitDefinition::areIdentical(parentUnits, refElemUnits) == false)
  {
    logMismatchUnits(repBy, refElem, parent);
  }
  else
  {
    // if we have Compartments that have spatialDimensions but no units
    // we can check this 
    if (parent->getTypeCode() == SBML_COMPARTMENT 
      && parentUnits->getNumUnits() == 0
      && refElem->getTypeCode() == SBML_COMPARTMENT
      && refElemUnits->getNumUnits() == 0)
    {
      if (static_cast<Compartment *>(parent)->isSetSpatialDimensions() == true
      && static_cast<Compartment *>(refElem)->isSetSpatialDimensions() == true)
      {
        if (util_isEqual(
          static_cast<Compartment *>(parent)->getSpatialDimensionsAsDouble(),
          static_cast<Compartment *>(refElem)->getSpatialDimensionsAsDouble())
                         == 0)
        {
          logMismatchSpatialDimensions(repBy, refElem, parent);
        }
      }
    }
  }
  //if (delparunit)
  //{
  //  delete parentUnits;
  //}
  //if (delrefunit)
  //{
  //  delete refElemUnits;
  //}

}


void 
UnitReplacementCheck::checkReferencedElement(ReplacedElement& repE, 
                                             const Model& m)
{
  // if the deletion attribute is set then it is not a true replacement
  if (repE.isSetDeletion() == true)
  {
    return;
  }

  unsigned int numErrsB4 = repE.getSBMLDocument()->getNumErrors();
  
  SBase* refElem = repE.getReferencedElement();
  
  // if there is an issue with references the getReferencedElement
  // will log errors and possibly fail
  // we dont want to try any further
  unsigned int numErrsAfter = repE.getSBMLDocument()->getNumErrors();

  if (numErrsB4 != numErrsAfter || refElem == NULL)
  {
    return;
  }

  SBase * parent = repE.getParentSBMLObject()->getParentSBMLObject();
  UnitDefinition *parentUnits = parent->getDerivedUnitDefinition();
  
  UnitDefinition *refElemUnits = refElem->getDerivedUnitDefinition();
  bool delrefelem = false;

  bool cfPresent = false;
  /* adjust the refElement units for conversion factor */
  if (repE.isSetConversionFactor() == true && refElemUnits && refElemUnits->getNumUnits() > 0)
  {
    Parameter * p = const_cast<Model *>(&m)
                                   ->getParameter(repE.getConversionFactor());
    UnitDefinition *ud = p->getDerivedUnitDefinition();
    UnitDefinition *newRefElemUnits = UnitDefinition::combine(refElemUnits, ud);
    refElemUnits = newRefElemUnits;
    delrefelem = true;
    cfPresent = true;
  }

  if (parentUnits == NULL)
  {
    if (delrefelem)
    {
      delete refElemUnits;
    }
    return;
  }

  if (refElemUnits == NULL)
  {
    return;
  }


  if (parent->containsUndeclaredUnits() == true ||
    refElem->containsUndeclaredUnits() == true)
  {
    if (delrefelem)
    {
      delete refElemUnits;
    }
    return;
  }

  if (UnitDefinition::areIdentical(parentUnits, refElemUnits) == false)
  {
    logMismatchUnits(repE, refElem, parent, cfPresent);
  }
  else
  {
    // if we have Compartments that have spatialDimensions but no units
    // we can check this 
    if (parent->getTypeCode() == SBML_COMPARTMENT 
      && parentUnits->getNumUnits() == 0
      && refElem->getTypeCode() == SBML_COMPARTMENT
      && refElemUnits->getNumUnits() == 0)
    {
      if (static_cast<Compartment *>(parent)->isSetSpatialDimensions() == true
      && static_cast<Compartment *>(refElem)->isSetSpatialDimensions() == true)
      {
        if (util_isEqual(
          static_cast<Compartment *>(parent)->getSpatialDimensionsAsDouble(),
          static_cast<Compartment *>(refElem)->getSpatialDimensionsAsDouble())
                         == 0)
        {
          logMismatchSpatialDimensions(repE, refElem, parent);
        }
      }
    }
  }
  if (delrefelem)
  {
    delete refElemUnits;
  }
}


void 
UnitReplacementCheck::logMismatchUnits (ReplacedBy& repBy, 
                                           SBase* refElem, SBase* parent)
{
  UnitDefinition * ud = parent->getDerivedUnitDefinition();
  msg = "The ";
  msg += SBMLTypeCode_toString(parent->getTypeCode(), 
                               parent->getPackageName().c_str());
  msg += " object with units ";
  msg += UnitDefinition::printUnits(ud, true);
  msg += " is replaced by the ";
  msg += SBMLTypeCode_toString(refElem->getTypeCode(), 
                               refElem->getPackageName().c_str());
  msg += " object with units ";

  ud = refElem->getDerivedUnitDefinition();

  msg += UnitDefinition::printUnits(ud, true);
  msg += ".";

  logFailure(repBy);
}

void 
UnitReplacementCheck::logMismatchUnits (ReplacedElement& repE, 
                                           SBase* refElem, SBase* parent,
                                           bool cfPresent)
{
  UnitDefinition* parentud = parent->getDerivedUnitDefinition();
  UnitDefinition* refElemud  = refElem->getDerivedUnitDefinition();
  msg = "The ";
  msg += SBMLTypeCode_toString(parent->getTypeCode(), 
                               parent->getPackageName().c_str());
  msg += " object with units ";
  msg += UnitDefinition::printUnits(parentud, true);
  msg += " attempts to replace the ";
  msg += SBMLTypeCode_toString(refElem->getTypeCode(), 
                               refElem->getPackageName().c_str());
  msg += " object with units ";
  msg += UnitDefinition::printUnits(refElemud, true);
  if (cfPresent == false)
  {
    msg += " with no appropriate conversionFactor declared.";
  }
  else
  {
    msg += " with an inaccurate conversionFactor declared.";
  }

  logFailure(repE);
}


void 
UnitReplacementCheck::logMismatchSpatialDimensions (ReplacedElement& repE, 
                                           SBase* refElem, SBase* parent)
{
  std::ostringstream spatial1, spatial2;
  spatial1 << static_cast<Compartment*>(parent)->getSpatialDimensionsAsDouble();
  spatial2 << static_cast<Compartment*>(refElem)
                                       ->getSpatialDimensionsAsDouble();
  msg = "Although the Compartment with id '";
  msg += parent->getId();
  msg += "' has no units declared it has spatialDimensions of '";
  msg += spatial1.str();
  msg +="' which is inconsistent with the Compartment it attempts to replace ";
  msg += "that has spatialDimensions '";
  msg += spatial2.str();
  msg += "'.";


  logFailure(repE);
}


void 
UnitReplacementCheck::logMismatchSpatialDimensions (ReplacedBy& repBy, 
                                           SBase* refElem, SBase* parent)
{
  std::ostringstream spatial1, spatial2;
  spatial1 << static_cast<Compartment*>(parent)->getSpatialDimensionsAsDouble();
  spatial2 << static_cast<Compartment*>(refElem)
                                       ->getSpatialDimensionsAsDouble();
  msg = "Although the Compartment with id '";
  msg += parent->getId();
  msg += "' has no units declared it has spatialDimensions of '";
  msg += spatial1.str();
  msg +="' which is inconsistent with the Compartment it is replaced by ";
  msg += "that has spatialDimensions '";
  msg += spatial2.str();
  msg += "'.";


  logFailure(repBy);
}



#endif /* __cplusplus */

LIBSBML_CPP_NAMESPACE_END
/** @endcond */
