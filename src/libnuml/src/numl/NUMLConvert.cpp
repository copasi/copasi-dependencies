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

#include <sbml/util/List.h>
//#include <numl/math/ASTNode.h>
#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>

LIBNUML_CPP_NAMESPACE_BEGIN

//bool
//Model::remove(XMLNode& ann)
//{
//  bool resetNecessary = false;
//  //make a copy to work with
//  XMLNode * newAnnotation = ann->clone();
//
//  unsigned int numChildren = newAnnotation->getNumChildren();
//  if (numChildren == 1)
//    return resetNecessary;
//  for (unsigned int i = 0; i < numChildren; i++)
//  {
//    std::string name = newAnnotation->getChild(i).getName();
//    for (unsigned int j = i+1; j < numChildren; j++)
//    {
//      if (name == newAnnotation->getChild(j).getName())
//      {
//        resetNecessary = true;
//      }
//    }
//  }
//}
/*void
Model::removeDuplicateTopLevelAnnotations()
{
  unsigned int i, n;
  this->removeDuplicateAnnotations();

  if (getNumFunctionDefinitions() > 0)
  {
    getListOfFunctionDefinitions()->removeDuplicateAnnotations();
    for (i = 0; i < getNumFunctionDefinitions(); i++)
    {
      getFunctionDefinition(i)->removeDuplicateAnnotations();
    }
  }
  if (getNumUnitDefinitions() > 0)
  {
    getListOfUnitDefinitions()->removeDuplicateAnnotations();
    for (i = 0; i < getNumUnitDefinitions(); i++)
    {
      getUnitDefinition(i)->removeDuplicateAnnotations();
      getUnitDefinition(i)->getListOfUnits()->removeDuplicateAnnotations();
      for (n = 0; n < getUnitDefinition(i)->getNumUnits(); n++)
      {
        getUnitDefinition(i)->getUnit(n)->removeDuplicateAnnotations();
      }
    }
  }
  if (getNumCompartmentTypes() > 0)
  {
    getListOfCompartmentTypes()->removeDuplicateAnnotations();
    for (i = 0; i < getNumCompartmentTypes(); i++)
    {
      getCompartmentType(i)->removeDuplicateAnnotations();
    }
  }
  if (getNumSpeciesTypes() > 0)
  {
    getListOfSpeciesTypes()->removeDuplicateAnnotations();
    for (i = 0; i < getNumSpeciesTypes(); i++)
    {
      getSpeciesType(i)->removeDuplicateAnnotations();
    }
  }
  if (getNumCompartments() > 0)
  {
    getListOfCompartments()->removeDuplicateAnnotations();
    for (i = 0; i < getNumCompartments(); i++)
    {
      getCompartment(i)->removeDuplicateAnnotations();
    }
  }
  if (getNumSpecies() > 0)
  {
    getListOfSpecies()->removeDuplicateAnnotations();
    for (i = 0; i < getNumSpecies(); i++)
    {
      getSpecies(i)->removeDuplicateAnnotations();
    }
  }
  if (getNumParameters() > 0)
  {
    getListOfParameters()->removeDuplicateAnnotations();
    for (i = 0; i < getNumParameters(); i++)
    {
      getParameter(i)->removeDuplicateAnnotations();
    }
  }
  if (getNumInitialAssignments() > 0)
  {
    getListOfInitialAssignments()->removeDuplicateAnnotations();
    for (i = 0; i < getNumInitialAssignments(); i++)
    {
      getInitialAssignment(i)->removeDuplicateAnnotations();
    }
  }
  if (getNumRules() > 0)
  {
    getListOfRules()->removeDuplicateAnnotations();
    for (i = 0; i < getNumRules(); i++)
    {
      getRule(i)->removeDuplicateAnnotations();
    }
  }
  if (getNumReactions() > 0)
  {
    getListOfReactions()->removeDuplicateAnnotations();
    for (i = 0; i < getNumReactions(); i++)
    {
      Reaction * r = getReaction(i);
      r->removeDuplicateAnnotations();
      if (r->getNumReactants() > 0)
      {
        r->getListOfReactants()->removeDuplicateAnnotations();
        for (n = 0; n < r->getNumReactants(); n++)
        {
          r->getReactant(n)->removeDuplicateAnnotations();
        }
      }
      if (r->getNumProducts() > 0)
      {
        r->getListOfProducts()->removeDuplicateAnnotations();
        for (n = 0; n < r->getNumProducts(); n++)
        {
          r->getProduct(n)->removeDuplicateAnnotations();
        }
      }
      if (r->getNumModifiers() > 0)
      {
        r->getListOfModifiers()->removeDuplicateAnnotations();
        for (n = 0; n < r->getNumModifiers(); n++)
        {
          r->getModifier(n)->removeDuplicateAnnotations();
        }
      }
      if (r->isSetKineticLaw())
      {
        r->getKineticLaw()->removeDuplicateAnnotations();
        if (r->getKineticLaw()->getNumParameters() > 0)
        {
          r->getKineticLaw()->getListOfParameters()
                            ->removeDuplicateAnnotations();
          for (n = 0; n < r->getKineticLaw()->getNumParameters(); n++)
          {
            r->getKineticLaw()->getParameter(n)->removeDuplicateAnnotations();
          }
        }
      }
    }
  }
  if (getNumEvents() > 0)
  {
    getListOfEvents()->removeDuplicateAnnotations();
    for (i = 0; i < getNumEvents(); i++)
    {
      getEvent(i)->removeDuplicateAnnotations();
      if (getEvent(i)->getNumEventAssignments() > 0)
      {
        getEvent(i)->getListOfEventAssignments()->removeDuplicateAnnotations();
        for (n = 0; n < getEvent(i)->getNumEventAssignments(); n++)
        {
          getEvent(i)->getEventAssignment(n)->removeDuplicateAnnotations();
        }
      }
    }
  }
}*/


LIBNUML_CPP_NAMESPACE_END

