/**
 * @file CaOmexManifest.h
 * @brief Definition of the CaOmexManifest class.
 * @author DEVISER
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML. Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 * 3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 * Pasadena, CA, USA
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation. A copy of the license agreement is provided in the
 * file named "LICENSE.txt" included with this software distribution and also
 * available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class CaOmexManifest
 * @sbmlbrief{combine} TODO:Definition of the CaOmexManifest class.
 */


#ifndef CaOmexManifest_H__
#define CaOmexManifest_H__


#include <omex/common/extern.h>
#include <omex/common/combinefwd.h>


#ifdef __cplusplus


#include <string>


#include <omex/CaBase.h>
#include <omex/CaListOfContents.h>
#include <omex/CaErrorLog.h>
#include <sbml/common/libsbml-namespace.h>


LIBCOMBINE_CPP_NAMESPACE_BEGIN


class LIBCOMBINE_EXTERN CaOmexManifest : public CaBase
{
protected:

  /** @cond doxygenlibCombineInternal */

  CaListOfContents mContents;
  CaErrorLog mErrorLog;

  /** @endcond */

public:

  /**
   * Creates a new CaOmexManifest instance.
   */
  CaOmexManifest();


  /**
   * Creates a new CaOmexManifest using the given CaNamespaces object @p
   * omexns.
   *
   * @param omexns the CaNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  CaOmexManifest(CaNamespaces *omexns);


  /**
   * Copy constructor for CaOmexManifest.
   *
   * @param orig the CaOmexManifest instance to copy.
   */
  CaOmexManifest(const CaOmexManifest& orig);


  /**
   * Assignment operator for CaOmexManifest.
   *
   * @param rhs the CaOmexManifest object whose values are to be used as the
   * basis of the assignment.
   */
  CaOmexManifest& operator=(const CaOmexManifest& rhs);


  /**
   * Creates and returns a deep copy of this CaOmexManifest object.
   *
   * @return a (deep) copy of this CaOmexManifest object.
   */
  virtual CaOmexManifest* clone() const;


  /**
   * Destructor for CaOmexManifest.
   */
  virtual ~CaOmexManifest();


  /**
   * Returns the CaListOfContents from this CaOmexManifest.
   *
   * @return the CaListOfContents from this CaOmexManifest.
   *
   * @copydetails doc_returned_unowned_pointer
   *
   * @see addContent(const CaContent* object)
   * @see createContent()
   * @see getContent(const std::string& sid)
   * @see getContent(unsigned int n)
   * @see getNumContents()
   * @see removeContent(const std::string& sid)
   * @see removeContent(unsigned int n)
   */
  const CaListOfContents* getListOfContents() const;


  /**
   * Returns the CaListOfContents from this CaOmexManifest.
   *
   * @return the CaListOfContents from this CaOmexManifest.
   *
   * @copydetails doc_returned_unowned_pointer
   *
   * @see addContent(const CaContent* object)
   * @see createContent()
   * @see getContent(const std::string& sid)
   * @see getContent(unsigned int n)
   * @see getNumContents()
   * @see removeContent(const std::string& sid)
   * @see removeContent(unsigned int n)
   */
  CaListOfContents* getListOfContents();


  /**
   * Get a CaContent from the CaOmexManifest.
   *
   * @param n an unsigned int representing the index of the CaContent to
   * retrieve.
   *
   * @return the nth CaContent in the CaListOfContents within this
   * CaOmexManifest.
   *
   * @copydetails doc_returned_unowned_pointer
   *
   * @see addContent(const CaContent* object)
   * @see createContent()
   * @see getContent(const std::string& sid)
   * @see getNumContents()
   * @see removeContent(const std::string& sid)
   * @see removeContent(unsigned int n)
   */
  CaContent* getContent(unsigned int n);


  /**
   * Get a CaContent from the CaOmexManifest.
   *
   * @param n an unsigned int representing the index of the CaContent to
   * retrieve.
   *
   * @return the nth CaContent in the CaListOfContents within this
   * CaOmexManifest.
   *
   * @copydetails doc_returned_unowned_pointer
   *
   * @see addContent(const CaContent* object)
   * @see createContent()
   * @see getContent(const std::string& sid)
   * @see getNumContents()
   * @see removeContent(const std::string& sid)
   * @see removeContent(unsigned int n)
   */
  const CaContent* getContent(unsigned int n) const;


  /**
   * Adds a copy of the given CaContent to this CaOmexManifest.
   *
   * @param cc the CaContent object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_INVALID_OBJECT, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_PKG_VERSION_MISMATCH,
   * OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied
   *
   * @see createContent()
   * @see getContent(const std::string& sid)
   * @see getContent(unsigned int n)
   * @see getNumContents()
   * @see removeContent(const std::string& sid)
   * @see removeContent(unsigned int n)
   */
  int addContent(const CaContent* cc);


  /**
   * Get the number of CaContent objects in this CaOmexManifest.
   *
   * @return the number of CaContent objects in this CaOmexManifest.
   *
   * @see addContent(const CaContent* object)
   * @see createContent()
   * @see getContent(const std::string& sid)
   * @see getContent(unsigned int n)
   * @see removeContent(const std::string& sid)
   * @see removeContent(unsigned int n)
   */
  unsigned int getNumContents() const;


  /**
   * Creates a new CaContent object, adds it to this CaOmexManifest object and
   * returns the CaContent object created.
   *
   * @return a new CaContent object instance.
   *
   * @copydetails doc_returned_unowned_pointer
   *
   * @see addContent(const CaContent* object)
   * @see getContent(const std::string& sid)
   * @see getContent(unsigned int n)
   * @see getNumContents()
   * @see removeContent(const std::string& sid)
   * @see removeContent(unsigned int n)
   */
  CaContent* createContent();


  /**
   * Removes the nth CaContent from this CaOmexManifest and returns a pointer
   * to it.
   *
   * @param n an unsigned int representing the index of the CaContent to
   * remove.
   *
   * @return a pointer to the nth CaContent in this CaOmexManifest.
   *
   * @copydetails doc_returned_owned_pointer
   *
   * @see addContent(const CaContent* object)
   * @see createContent()
   * @see getContent(const std::string& sid)
   * @see getContent(unsigned int n)
   * @see getNumContents()
   * @see removeContent(const std::string& sid)
   */
  CaContent* removeContent(unsigned int n);


  /**
   * Returns the XML element name of this CaOmexManifest object.
   *
   * For CaOmexManifest, the XML element name is always @c "omexManifest".
   *
   * @return the name of this element, i.e. @c "omexManifest".
   */
  virtual const std::string& getElementName() const;


  /**
   * Returns the libCombine type code for this CaOmexManifest object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the OMEX type code for this object:
   * @omexconstant{LIB_COMBINE_OMEXMANIFEST, CaTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   */
  virtual int getTypeCode() const;



  /** @cond doxygenlibCombineInternal */

  /**
   * Write any contained elements
   */
  virtual void writeElements(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream&
    stream) const;

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Accepts the given CaVisitor
   */
  virtual bool accept(CaVisitor& v) const;

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Sets the parent CaOmexManifest
   */
  virtual void setCaOmexManifest(CaOmexManifest* d);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Connects to child elements
   */
  virtual void connectToChild();

  /** @endcond */




  #ifndef SWIG



  /** @cond doxygenlibCombineInternal */

  /**
   * Gets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName, bool& value)
    const;

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Gets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName, int& value) const;

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Gets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName,
                           double& value) const;

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Gets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName,
                           unsigned int& value) const;

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Gets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName,
                           std::string& value) const;

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Predicate returning @c true if this CaOmexManifest's attribute
   * "attributeName" is set.
   *
   * @param attributeName, the name of the attribute to query.
   *
   * @return @c true if this CaOmexManifest's attribute "attributeName" has
   * been set, otherwise @c false is returned.
   */
  virtual bool isSetAttribute(const std::string& attributeName) const;

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Sets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName, bool value);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Sets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName, int value);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Sets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName, double value);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Sets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName,
                           unsigned int value);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Sets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName,
                           const std::string& value);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Unsets the value of the "attributeName" attribute of this CaOmexManifest.
   *
   * @param attributeName, the name of the attribute to query.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetAttribute(const std::string& attributeName);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Creates and returns an new "elementName" object in this CaOmexManifest.
   *
   * @param elementName, the name of the element to create.
   *
   * @return pointer to the element created.
   */
  virtual CaBase* createChildObject(const std::string& elementName);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Adds a new "elementName" object to this CaOmexManifest.
   *
   * @param elementName, the name of the element to create.
   *
   * @param element, pointer to the element to be added.
   *
   * @copydetails doc_returns_success_code
   * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int addChildObject(const std::string& elementName,
                             const CaBase* element);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Removes and returns the new "elementName" object with the given id in this
   * CaOmexManifest.
   *
   * @param elementName, the name of the element to remove.
   *
   * @param id, the id of the element to remove.
   *
   * @return pointer to the element removed.
   */
  virtual CaBase* removeChildObject(const std::string& elementName,
                                    const std::string& id);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Returns the number of "elementName" in this CaOmexManifest.
   *
   * @param elementName, the name of the element to get number of.
   *
   * @return unsigned int number of elements.
   */
  virtual unsigned int getNumObjects(const std::string& elementName);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Returns the nth object of "objectName" in this CaOmexManifest.
   *
   * @param elementName, the name of the element to get number of.
   *
   * @param index, unsigned int the index of the object to retrieve.
   *
   * @return pointer to the object.
   */
  virtual CaBase* getObject(const std::string& elementName, unsigned int
    index);

  /** @endcond */




  #endif /* !SWIG */


  /**
   * Returns the first child element that has the given @p id in the model-wide
   * SId namespace, or @c NULL if no such object is found.
   *
   * @param id a string representing the id attribute of the object to
   * retrieve.
   *
   * @return a pointer to the CaBase element with the given @p id. If no such
   * object is found, this method returns @c NULL.
   */
  virtual CaBase* getElementBySId(const std::string& id);


  /**
   * Returns the value of the "Namespaces" element of this CaOmexManifest.
   *
   * @return the value of the "Namespaces" element of this CaOmexManifest as a
   * LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces*.
   */
  virtual const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* getNamespaces()
    const;


  /**
   * Returns the value of the "Namespaces" element of this CaOmexManifest.
   *
   * @return the value of the "Namespaces" element of this CaOmexManifest as a
   * LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces*.
   */
  virtual LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* getNamespaces();


  /**
   * Returns the value of the "CaErrorLog" element of this CaOmexManifest.
   *
   * @return the value of the "CaErrorLog" element of this CaOmexManifest as a
   * CaErrorLog*.
   */
  const CaErrorLog* getErrorLog() const;


  /**
   * Returns the value of the "CaErrorLog" element of this CaOmexManifest.
   *
   * @return the value of the "CaErrorLog" element of this CaOmexManifest as a
   * CaErrorLog*.
   */
  CaErrorLog* getErrorLog();


  /**
   * Get a CaError from the CaDocument.
   *
   * @param n an unsigned int representing the index of the CaError to
   * retrieve.
   *
   * @return the nth CaError within this CaDocument.
   *
   * @copydetails doc_returned_unowned_pointer
   *
   * @see getNumErrors()
   */
  CaError* getError(unsigned int n);


  /**
   * Get a CaError from the CaDocument.
   *
   * @param n an unsigned int representing the index of the CaError to
   * retrieve.
   *
   * @return the nth CaError within this CaDocument.
   *
   * @copydetails doc_returned_unowned_pointer
   *
   * @see getNumErrors()
   */
  const CaError* getError(unsigned int n) const;


  /**
   * Get the number of CaError objects in this CaDocument.
   *
   * @return the number of CaError objects in this CaDocument.
   *
   * @see getError(unsigned int n)
   */
  unsigned int getNumErrors() const;


  /**
   * Get the number of CaError objects in this CaDocument with the given
   * severity.
   *
   * @param severity the severity of the CaError to return.
   *
   * @return the number of CaError objects in this CaDocument.
   *
   * @see getError(unsigned int n)
   */
  unsigned int getNumErrors(unsigned int severity) const;


protected:


  /** @cond doxygenlibCombineInternal */

  /**
   * Creates a new object from the next XMLToken on the XMLInputStream
   */
  virtual CaBase* createObject(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream&
    stream);

  /** @endcond */



  /** @cond doxygenlibCombineInternal */

  /**
   * Writes the namespace for the Combine package
   */
  virtual void writeXMLNS(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream&
    stream) const;

  /** @endcond */


};



LIBCOMBINE_CPP_NAMESPACE_END




#endif /* __cplusplus */




#ifndef SWIG




LIBCOMBINE_CPP_NAMESPACE_BEGIN




BEGIN_C_DECLS


/**
 * Creates a new CaOmexManifest_t instance.
 *
 * @memberof CaOmexManifest_t
 */
LIBCOMBINE_EXTERN
CaOmexManifest_t *
CaOmexManifest_create();


/**
 * Creates and returns a deep copy of this CaOmexManifest_t object.
 *
 * @param com the CaOmexManifest_t structure.
 *
 * @return a (deep) copy of this CaOmexManifest_t object.
 *
 * @copydetails doc_returned_owned_pointer
 *
 * @memberof CaOmexManifest_t
 */
LIBCOMBINE_EXTERN
CaOmexManifest_t*
CaOmexManifest_clone(const CaOmexManifest_t* com);


/**
 * Frees this CaOmexManifest_t object.
 *
 * @param com the CaOmexManifest_t structure.
 *
 * @memberof CaOmexManifest_t
 */
LIBCOMBINE_EXTERN
void
CaOmexManifest_free(CaOmexManifest_t* com);


/**
 * Returns a ListOf_t * containing CaContent_t objects from this
 * CaOmexManifest_t.
 *
 * @param com the CaOmexManifest_t structure whose CaListOfContents is sought.
 *
 * @return the CaListOfContents from this CaOmexManifest_t as a ListOf_t *.
 *
 * @copydetails doc_returned_unowned_pointer
 *
 * @see CaOmexManifest_addContent()
 * @see CaOmexManifest_createContent()
 * @see CaOmexManifest_getContentById()
 * @see CaOmexManifest_getContent()
 * @see CaOmexManifest_getNumContents()
 * @see CaOmexManifest_removeContentById()
 * @see CaOmexManifest_removeContent()
 *
 * @memberof CaOmexManifest_t
 */
LIBCOMBINE_EXTERN
CaListOf_t*
CaOmexManifest_getListOfContents(CaOmexManifest_t* com);


/**
 * Get a CaContent_t from the CaOmexManifest_t.
 *
 * @param com the CaOmexManifest_t structure to search.
 *
 * @param n an unsigned int representing the index of the CaContent_t to
 * retrieve.
 *
 * @return the nth CaContent_t in the CaListOfContents within this
 * CaOmexManifest.
 *
 * @copydetails doc_returned_unowned_pointer
 *
 * @memberof CaOmexManifest_t
 */
LIBCOMBINE_EXTERN
CaContent_t*
CaOmexManifest_getContent(CaOmexManifest_t* com, unsigned int n);


/**
 * Adds a copy of the given CaContent_t to this CaOmexManifest_t.
 *
 * @param com the CaOmexManifest_t structure to which the CaContent_t should be
 * added.
 *
 * @param cc the CaContent_t object to add.
 *
 * @copydetails doc_returns_success_code
 * @li @omexconstant{LIBCOMBINE_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @omexconstant{LIBCOMBINE_OPERATION_FAILED, OperationReturnValues_t}
 * @li @omexconstant{LIBCOMBINE_INVALID_OBJECT, OperationReturnValues_t}
 * @li @omexconstant{LIBCOMBINE_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @omexconstant{LIBCOMBINE_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @omexconstant{LIBCOMBINE_PKG_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @omexconstant{LIBCOMBINE_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
 *
 * @memberof CaOmexManifest_t
 */
LIBCOMBINE_EXTERN
int
CaOmexManifest_addContent(CaOmexManifest_t* com, const CaContent_t* cc);


/**
 * Get the number of CaContent_t objects in this CaOmexManifest_t.
 *
 * @param com the CaOmexManifest_t structure to query.
 *
 * @return the number of CaContent_t objects in this CaOmexManifest_t.
 *
 * @memberof CaOmexManifest_t
 */
LIBCOMBINE_EXTERN
unsigned int
CaOmexManifest_getNumContents(CaOmexManifest_t* com);


/**
 * Creates a new CaContent_t object, adds it to this CaOmexManifest_t object
 * and returns the CaContent_t object created.
 *
 * @param com the CaOmexManifest_t structure to which the CaContent_t should be
 * added.
 *
 * @return a new CaContent_t object instance.
 *
 * @copydetails doc_returned_unowned_pointer
 *
 * @memberof CaOmexManifest_t
 */
LIBCOMBINE_EXTERN
CaContent_t*
CaOmexManifest_createContent(CaOmexManifest_t* com);


/**
 * Removes the nth CaContent_t from this CaOmexManifest_t and returns a pointer
 * to it.
 *
 * @param com the CaOmexManifest_t structure to search.
 *
 * @param n an unsigned int representing the index of the CaContent_t to
 * remove.
 *
 * @return a pointer to the nth CaContent_t in this CaOmexManifest_t.
 *
 * @copydetails doc_returned_owned_pointer
 *
 * @memberof CaOmexManifest_t
 */
LIBCOMBINE_EXTERN
CaContent_t*
CaOmexManifest_removeContent(CaOmexManifest_t* com, unsigned int n);




END_C_DECLS




LIBCOMBINE_CPP_NAMESPACE_END




#endif /* !SWIG */




#endif /* !CaOmexManifest_H__ */


