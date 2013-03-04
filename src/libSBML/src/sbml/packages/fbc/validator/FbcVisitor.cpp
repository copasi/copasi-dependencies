/**
 * @file   FbcVisitor.cpp
 * @brief  Visitor Design Pattern for the SBML object tree  
 * @author Sarah Keating
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EBML-EBI), Hinxton, UK
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

#include <sbml/packages/fbc/validator/FbcVisitor.h>

#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>

LIBSBML_CPP_NAMESPACE_BEGIN

FbcVisitor::~FbcVisitor ()
{
}


void
FbcVisitor::visit (const SBMLDocument& x)
{
  visit( static_cast<const SBase&>(x) );
}


void
FbcVisitor::visit (const Model& x)
{
  visit( static_cast<const SBase&>(x) );
}


void
FbcVisitor::visit (const ListOf& x, int type)
{
  visit( static_cast<const SBase&>(x) );
}


bool
FbcVisitor::visit (const SBase& sb)
{
  return false;
}


bool
FbcVisitor::visit (const FluxBound& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
FbcVisitor::visit (const FluxObjective& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
FbcVisitor::visit (const Objective& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
FbcVisitor::visit (const Species& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
FbcVisitor::visit (const ListOfObjectives& x)
{
  return visit( static_cast<const SBase&>(x) );
}


void
FbcVisitor::leave (const SBMLDocument& x)
{
}


void
FbcVisitor::leave (const Model& x)
{
}


void
FbcVisitor::leave (const SBase& x)
{
}


void
FbcVisitor::leave (const ListOf& x, int type)
{
}

void
FbcVisitor::leave (const Objective& x)
{
}




LIBSBML_CPP_NAMESPACE_END
