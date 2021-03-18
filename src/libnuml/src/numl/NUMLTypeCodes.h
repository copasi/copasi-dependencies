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

#ifndef NUMLTypeCodes_h
#define NUMLTypeCodes_h


#include <numl/common/libnuml-config.h>
#include <numl/common/extern.h>

LIBNUML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/**
 * An enumeration of NUML types to help identify NUML objects at runtime.
 * Abstract types do not have a typecode since they cannot be instantiated.
 */
typedef enum
{
    NUML_UNKNOWN
  , NUML_DOCUMENT
  , NUML_ONTOLOGYTERMS
  , NUML_ONTOLOGYTERM
  , NUML_RESULTCOMPONENT
  , NUML_RESULTCOMPONENTS
  , NUML_DIMENSION
  , NUML_DIMENSIONDESCRIPTION
  , NUML_COMPOSITEVALUE
  , NUML_COMPOSITEVALUES
  , NUML_TUPLE
  , NUML_TUPLES
  , NUML_ATOMICVALUE
  , NUML_ATOMICVALUES
  , NUML_COMPOSITEDESCRIPTION
  , NUML_TUPLEDESCRIPTION
  , NUML_ATOMICDESCRIPTION
  , NUML_NUMLLIST

} NUMLTypeCode_t;

/**
 * This method takes an NUML type code and returns a string representing
 * the code.
 *
 * @if clike LibNUML attaches an identifying code to every
 * kind of NUML object.  These are known as <em>NUML type codes</em>.
 * The set of possible type codes is defined in the enumeration
 * #NUMLTypeCode_t.  The names of the type codes all begin with the
 * characters @c NUML_. @endif@if java LibNUML attaches an
 * identifying code to every kind of NUML object.  These are known as
 * <em>NUML type codes</em>.  In other languages, the set of type codes
 * is stored in an enumeration; in the Java language interface for
 * libNUML, the type codes are defined as static integer constants in
 * interface class {@link libnumlConstants}.  The names of the type codes
 * all begin with the characters @c NUML_. @endif
 * This method takes a type code as argument, and returns a string name
 * corresponding to that code.  For example, passing it the type code
 * <code>NUML_COMPARTMENT</code> will return the string
 * "<code>Compartment</code>". 
 *
 * @return a human readable name for the given NUMLTypeCode_t.
 *
 * @note The caller does not own the returned string and is therefore not
 * allowed to modify it.
 */
LIBNUML_EXTERN
const char *
NUMLTypeCode_toString (NUMLTypeCode_t tc);


END_C_DECLS
LIBNUML_CPP_NAMESPACE_END

#endif  /* NUMLTypeCodes_h */
