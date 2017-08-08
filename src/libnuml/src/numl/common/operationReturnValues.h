/**
* Begin svn Header
* $Rev: 27 $:	Revision of last commit
* $Author: josephodada@gmail.com $:	Author of last commit
* $Date: 2013-04-25 10:07:15 +0100 (Thu, 25 Apr 2013) $:	Date of last commit
* $HeadURL: https://numl.googlecode.com/svn/trunk/libnuml/src/common/operationReturnValues.h $
* $Id: operationReturnValues.h 27 2013-04-25 09:07:15Z josephodada@gmail.com $
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

#ifndef LIBNUML_OPERATION_RETURN_VALUES_H
#define LIBNUML_OPERATION_RETURN_VALUES_H 1

LIBNUML_CPP_NAMESPACE_BEGIN

/**
 * Diagnostic return codes.
 *
 * Many methods in libNUML return a status code to indicate whether the
 * operation requested by the caller succeeded or failed.  This enumeration
 * lists all the possible return codes from any libNUML methods.
 */
typedef enum
{
    LIBNUML_OPERATION_SUCCESS       = 0
    /*!< The operation was successful. */

  , LIBNUML_INDEX_EXCEEDS_SIZE      = -1
    /*!< An index parameter exceeded the bounds of a data array or other
     * collection used in the operation.  This return value is typically
     * returned by methods that take index numbers to refer to lists
     * of objects, when the caller has provided an index that exceeds
     * the bounds of the list.  LibNUML provides methods for checking the
     * size of list/sequence/collection structures, and callers should
     * verify the sizes before calling methods that take index numbers. */

  , LIBNUML_UNEXPECTED_ATTRIBUTE    = -2
    /*!< The attribute that is the subject of this operation is not valid
     * for the combination of NUML Level and Version for the underlying
     * object.  This can happen because libNUML strives to offer a uniform
     * API for all NUML Levels and Versions, but some object attributes and
     * elements are not defined for all NUML Levels and Versions.  Calling
     * programs are expected to be aware of which object structures they
     * are working with, but when errors of this kind occur, they are
     * reported using this return value. */

  , LIBNUML_OPERATION_FAILED        = -3
    /*!< The requested action could not be performed.  This can occur in
     * a variety of contexts, such as passing a null object as a parameter
     * in a situation where it does not make sense to permit a null object.
     */

  , LIBNUML_INVALID_ATTRIBUTE_VALUE = -4
    /*!< A value passed as an argument to the method is not of a type that
     * is valid for the operation or kind of object involved.  For example,
     * this return code is used when a calling program attempts to set an
     * NUML object identifier to a string whose syntax does not conform to
     * the NUML identifier syntax. */

  , LIBNUML_INVALID_OBJECT          = -5
    /*!< The object passed as an argument to the method is not of a type
     * that is valid for the operation or kind of object involved.  For
     * example, handing an invalidly-constructed ASTNode to a method
     * expecting an ASTNode will result in this error. */

  , LIBNUML_DUPLICATE_OBJECT_ID     = -6
    /*!< There already exists an object with this identifier in the
     * context where this operation is being attempted.  This error is
     * typically returned in situations where NUML object identifiers must be
     * unique, such as attempting to add two species with the same identifier
     * to a model. */

  , LIBNUML_LEVEL_MISMATCH          = -7
    /*!< The NUML Level associated with the object does not match the Level
     * of the parent object.  This error can happen when an NUML component
     * such as a species or compartment object is created outside of a model
     * and a calling program then attempts to add the object to a model that
     * has a different NUML Level defined. */

  , LIBNUML_VERSION_MISMATCH        = -8
    /*!< The NUML Version within the NUML Level associated with the object
     * does not match the Version of the parent object.  This error can
     * happen when an NUML component such as a species or compartment object
     * is created outside of a model and a calling program then attempts to
     * add the object to a model that has a different NUML Level+Version
     * combination. */

  , LIBNUML_INVALID_XML_OPERATION   = -9
    /*!< The XML operation attempted is not valid for the object or context
     * involved.  This error is typically returned by the XML interface layer
     * of libNUML, when a calling program attempts to construct or manipulate
     * XML in an invalid way.  */
  , LIBNUML_DUPLICATE_ANNOTATION_NS = -11
  /*!< There already exists a top level annotation with the same namespace as
   * annoation being appended.  This error is
   * typically returned in situations where the appendAnnotation function
   * is being used to add an annotation that has a namespace that is already
   * present in the existing annotation. */
  , LIBNUML_ANNOTATION_NAME_NOT_FOUND = -12
  /*!< The existing annotation does not have a top-level element with
   * the given name. This error is
   * typically returned in situations where the
   * replaceTopLevelAnnotationElement function or
   * the removeTopLevelAnnotationElement function
   * is being used to replace or remove an annotation with a name that does
   * not match the name of any top-level element that is already
   * present in the existing annotation. */
  , LIBNUML_ANNOTATION_NS_NOT_FOUND = -13
  /*!< The existing annotation does not have a top-level element with
   * the given namespace. This error is
   * typically returned in situations where the
   * replaceTopLevelAnnotationElement function or
   * the removeTopLevelAnnotationElement function
   * is being used to replace or remove an annotation with a namespace that does
   * not match the namespace of any top-level element that is already
   * present in the existing annotation. */


} NUMLOperationReturnValues_t;

LIBNUML_CPP_NAMESPACE_END


#endif  /* LIBNUML_OPERATION_RETURN_VALUES_H */
