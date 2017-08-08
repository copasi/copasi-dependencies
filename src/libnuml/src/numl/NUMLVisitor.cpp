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

#include <numl/NUMLTypes.h>
#include <numl/NUMLVisitor.h>

LIBNUML_CPP_NAMESPACE_BEGIN

NUMLVisitor::~NUMLVisitor ()
{
}


void
NUMLVisitor::visit (const NUMLDocument& x)
{
  visit( static_cast<const NMBase&>(x) );
}

void
NUMLVisitor::visit (const NUMLList& x, NUMLTypeCode_t type)
{
  visit( static_cast<const NMBase&>(x) );
}

bool
NUMLVisitor::visit (const NMBase& sb)
{
  return false;
}

bool
NUMLVisitor::visit (const OntologyTerm& x)
{
  return visit( static_cast<const NMBase&>(x));
}

bool
NUMLVisitor::visit (const ResultComponent& x)
{
  return visit( static_cast<const NMBase&>(x));
}

bool
NUMLVisitor::visit (const Dimension& x)
{
  return visit( static_cast<const NMBase&>(x));
}

bool
NUMLVisitor::visit (const Tuple& x)
{
  return visit( static_cast<const NMBase&>(x));
}

bool
NUMLVisitor::visit (const CompositeValue& x)
{
  return visit( static_cast<const NMBase&>(x));
}
bool
NUMLVisitor::visit (const AtomicValue& x)
{
  return visit( static_cast<const NMBase&>(x));
}

bool
NUMLVisitor::visit (const DimensionDescription& x)
{
  return visit( static_cast<const NMBase&>(x));
}

bool
NUMLVisitor::visit (const CompositeDescription& x)
{
  return visit( static_cast<const NMBase&>(x));
}

bool
NUMLVisitor::visit (const TupleDescription& x)
{
  return visit( static_cast<const NMBase&>(x));
}

bool
NUMLVisitor::visit (const AtomicDescription& x)
{
  return visit( static_cast<const NMBase&>(x));
}

void
NUMLVisitor::leave (const NUMLDocument& x)
{
}

void
NUMLVisitor::leave (const OntologyTerm& x)
{
}


/*void
NUMLVisitor::leave (const Result& x)
{
}*/


void
NUMLVisitor::leave (const ResultComponent& x)
{
}

void
NUMLVisitor::leave (const Dimension& x)
{
}

void
NUMLVisitor::leave (const CompositeValue& x)
{
}

void
NUMLVisitor::leave (const Tuple& x)
{
}

void
NUMLVisitor::leave (const AtomicValue& x)
{
}

void
NUMLVisitor::leave (const DimensionDescription& x)
{
}

void
NUMLVisitor::leave (const CompositeDescription& x)
{
}

void
NUMLVisitor::leave (const TupleDescription& x)
{
}

void
NUMLVisitor::leave (const AtomicDescription& x)
{
}


void
NUMLVisitor::leave (const NUMLList& x, NUMLTypeCode_t type)
{
}

LIBNUML_CPP_NAMESPACE_END
