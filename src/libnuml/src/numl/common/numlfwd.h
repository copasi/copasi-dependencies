/**
* Begin svn Header
* $Rev: 29 $:	Revision of last commit
* $Author: josephodada@gmail.com $:	Author of last commit
* $Date: 2013-04-25 10:57:46 +0100 (Thu, 25 Apr 2013) $:	Date of last commit
* $HeadURL: https://numl.googlecode.com/svn/trunk/libnuml/src/common/numlfwd.h $
* $Id: numlfwd.h 29 2013-04-25 09:57:46Z josephodada@gmail.com $
* End svn Header
* ***************************************************************************
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
* The original code was initially developed by:
*
* 	SBML Team
* 	Control and Dynamical Systems, MC 107-81
* 	California Institute of Technology
* 	Pasadena, CA, 91125, USA
*
*  	http://www.sbml.org
* 	mailto:sbml-team@caltech.edu
*
* Adapted for NUML by:
*
* 	NUML Team
* 	Computational Systems Biology Group
* 	School of Computer Science
* 	The University of Manchester
* 	Manchester, M1 7DN UK
* 	mailto:josephodada@gmail.com
*
*   Contributor(s):
*   Joseph O. Dada, The University of Manchester - initial LIBNUML API and implementation
* ***************************************************************************
**/


/**
 *
 * Declaring all types up-front avoids compilation errors of the form
 * <em>"Redefinition of type 'Foo'"</em>, and allows our combined C/C++
 * headers to depend minimally upon each other.  Put another way, the type
 * definitions below serve the same purpose as <tt>class Foo;</tt> forward
 * declarations in C++ code.
 */

#ifndef numlfwd_h
#define numlfwd_h

#include <sbml/common/libsbml-namespace.h>
#include <numl/common/libnuml-namespace.h>
#include <numl/common/libnuml-config.h>


#ifdef __cplusplus
/**
 * Defined as a class when compiling for C++ and as a C struct when
 * compiling for C.
 */
#  define CLASS_OR_STRUCT class
#else
/**
 * Defined as a class when compiling for C++ and as a C struct when
 * compiling for C.
 */
#  define CLASS_OR_STRUCT struct
#endif  /* __cplusplus */

LIBNUML_CPP_NAMESPACE_BEGIN

/**
 * @var typedef class NMBase NMBase_t
 * @copydoc NMBase
 */
typedef CLASS_OR_STRUCT NMBase                     NMBase_t;

/**
 * @var typedef class NUMLDocument NUMLDocument_t
 * @copydoc NUMLDocument
 */
typedef CLASS_OR_STRUCT NUMLDocument           NUMLDocument_t;

/**
 * @var typedef class Model Model_t
 * @copydoc Model
 */
// TODO remove typedef CLASS_OR_STRUCT Model                     Model_t;

/**
 * @var typedef class OntologyTerm OntologyTerm_t
 * @copydoc OntologyTerm
 */
typedef CLASS_OR_STRUCT OntologyTerm        OntologyTerm_t;


/**
 * @var typedef class Dimension Dimension_t
 * @copydoc Dimension
 */
typedef CLASS_OR_STRUCT Dimension       Dimension_t;
/**
 * @var typedef class CompositeValue CompositeValue_t
 * @copydoc CompositeValue
 */
typedef CLASS_OR_STRUCT CompositeValue       CompositeValue_t;

/**
 * @var typedef class Tuple Tuple_t
 * @copydoc Tuple
 */
typedef CLASS_OR_STRUCT Tuple       Tuple_t;

/**
 * @var typedef class AtomicValue AtomicValue_t
 * @copydoc AtomicValue
 */
typedef CLASS_OR_STRUCT AtomicValue       AtomicValue_t;

/**
 * @var typedef class Method Method_t
 * @copydoc Method
 */
//TODO remove typedef CLASS_OR_STRUCT Method        Method_t;

/**
 * @var typedef class Software Software_t
 * @copydoc Software
 */
//TODO remove typedef CLASS_OR_STRUCT Software        Software_t;

/**
 * @var typedef class Result Result_t
 * @copydoc Result
 */
//typedef CLASS_OR_STRUCT Result        Result_t;

/**
 * @var typedef class ResultComponent ResultComponent_t
 * @copydoc ResultComponent
 */
typedef CLASS_OR_STRUCT ResultComponent       ResultComponent_t;

/**
 * @var typedef class DimensionDescription DimensionDescription_t
 * @copydoc DimensionDescription
 */
typedef CLASS_OR_STRUCT DimensionDescription       DimensionDescription_t;

/**
 * @var typedef class CompositeDescription CompositeDescription_t
 * @copydoc CompositeDescription
 */
typedef CLASS_OR_STRUCT CompositeDescription       CompositeDescription_t;

/**
 * @var typedef class TupleDescription TupleDescription_t
 * @copydoc TupleDescription
 */
typedef CLASS_OR_STRUCT TupleDescription       TupleDescription_t;

/**
 * @var typedef class AtomicDescription AtomicDescription_t
 * @copydoc AtomicDescription
 */
typedef CLASS_OR_STRUCT AtomicDescription       AtomicDescription_t;

/**
 * @var typedef class NUMLReader NUMLReader_t
 * @copydoc NUMLReader
 */
typedef CLASS_OR_STRUCT NUMLReader                NUMLReader_t;

/**
 * @var typedef class NUMLWriter NUMLWriter_t
 * @copydoc NUMLWriter
 */
typedef CLASS_OR_STRUCT NUMLWriter                NUMLWriter_t;

/**
 * @var typedef class NUMLError NUMLError_t
 * @copydoc NUMLError
 */
typedef CLASS_OR_STRUCT NUMLError                 NUMLError_t;

/**
 * @var typedef class NUMLNamespaces NUMLNamespaces_t
 * @copydoc NUMLNamespaces
 */
typedef CLASS_OR_STRUCT NUMLNamespaces             NUMLNamespaces_t;

/**
 * @var typedef class NUMLList NUMLList_t
 * @copydoc NUMLList
 */
typedef CLASS_OR_STRUCT NUMLList                      NUMLList_t;

LIBNUML_CPP_NAMESPACE_END

LIBSBML_CPP_NAMESPACE_BEGIN
/**
 * @var typedef class ASTNode ASTNode_t
 * @copydoc ASTNode
 */
typedef CLASS_OR_STRUCT ASTNode                   ASTNode_t;

/**
 * @var typedef class List List_t
 * @copydoc List
 */
typedef CLASS_OR_STRUCT List                      List_t;

/**
 * @var typedef class XMLError XMLError_t
 * @copydoc XMLError
 */
typedef CLASS_OR_STRUCT XMLError                  XMLError_t;

/**
 * @var typedef class XMLErrorLog XMLErrorLog_t
 * @copydoc XMLErrorLog
 */
typedef CLASS_OR_STRUCT XMLErrorLog               XMLErrorLog_t;

/**
 * @var typedef class XMLNode XMLNode_t
 * @copydoc XMLNode
 */
typedef CLASS_OR_STRUCT XMLNode                   XMLNode_t;

/**
 * @var typedef class XMLAttributes XMLAttributes_t
 * @copydoc XMLAttributes
 */
typedef CLASS_OR_STRUCT XMLAttributes             XMLAttributes_t;

/**
 * @var typedef class XMLNamespaces XMLNamespaces_t
 * @copydoc XMLNamespaces
 */
typedef CLASS_OR_STRUCT XMLNamespaces		  XMLNamespaces_t;

/**
 * @var typedef class XMLToken XMLToken_t
 * @copydoc XMLToken
 */
typedef CLASS_OR_STRUCT XMLToken                  XMLToken_t;

/**
 * @var typedef class XMLInputStream XMLInputStream_t
 * @copydoc XMLInputStream
 */
typedef CLASS_OR_STRUCT XMLInputStream            XMLInputStream_t;


/**
 * @var typedef class XMLOutputStream XMLOutputStream_t
 * @copydoc XMLOutputStream
 */
typedef CLASS_OR_STRUCT XMLOutputStream           XMLOutputStream_t;


/**
 * @var typedef class XMLTriple XMLTriple_t
 * @copydoc XMLTriple
 */
typedef CLASS_OR_STRUCT XMLTriple                 XMLTriple_t;

/**
 * @var typedef class Date Date_t
 * @copydoc Date
 */
typedef CLASS_OR_STRUCT Date                      Date_t;

/**
 * @var typedef class ModelCreator ModelCreator_t
 * @copydoc ModelCreator
 */
typedef CLASS_OR_STRUCT ModelCreator              ModelCreator_t;

/**
 * @var typedef class CVTerm CVTerm_t
 * @copydoc CVTerm
 */
typedef CLASS_OR_STRUCT CVTerm              CVTerm_t;

/**
 * @var typedef class ModelHistory ModelHistory_t
 * @copydoc ModelHistory
 */
typedef CLASS_OR_STRUCT ModelHistory              ModelHistory_t;

LIBSBML_CPP_NAMESPACE_END
#undef CLASS_OR_STRUCT

#endif  /* numlfwd_h  */
