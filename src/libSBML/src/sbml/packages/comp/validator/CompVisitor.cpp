/**
 * @file   CompVisitor.cpp
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

#include <sbml/packages/comp/validator/CompVisitor.h>

#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>

LIBSBML_CPP_NAMESPACE_BEGIN

CompVisitor::~CompVisitor ()
{
}


void
CompVisitor::visit (const SBMLDocument& x)
{
  visit( static_cast<const SBase&>(x) );
}


void
CompVisitor::visit (const Model& x)
{
  visit( static_cast<const SBase&>(x) );
}


void
CompVisitor::visit (const ListOf& x, int type)
{
  visit( static_cast<const SBase&>(x) );
}


bool
CompVisitor::visit (const SBase& sb)
{
  return false;
}


bool
CompVisitor::visit (const Port& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
CompVisitor::visit (const Deletion& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
CompVisitor::visit (const ReplacedElement& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
CompVisitor::visit (const ReplacedBy& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
CompVisitor::visit (const ModelDefinition& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
CompVisitor::visit (const ExternalModelDefinition& x)
{
  return visit( static_cast<const SBase&>(x) );
}


bool
CompVisitor::visit (const Submodel& x)
{
  return visit( static_cast<const SBase&>(x) );
}


void
CompVisitor::leave (const SBMLDocument& x)
{
}


void
CompVisitor::leave (const Model& x)
{
}


void
CompVisitor::leave (const SBase& x)
{
}


void
CompVisitor::leave (const Submodel& x)
{
}


void
CompVisitor::leave (const ListOf& x, int type)
{
}

LIBSBML_CPP_NAMESPACE_END
