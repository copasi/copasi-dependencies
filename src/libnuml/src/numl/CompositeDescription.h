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

/**
 * @class CompositeDescription
 * @brief this class stores the component descriptions
 */


#ifndef COMPOSITEDESCRIPTION_H_
#define COMPOSITEDESCRIPTION_H_

#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>

#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>

//#include <numl/TupleDescription.h>
//#include <numl/AtomicDescription.h>
#include <numl/DimensionDescription.h>


#include <numl/NMBase.h>
#include <numl/NUMLList.h>

#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

//class XMLOutputStream;
class CompositeDescription;
class TupleDescription;
class AtomicDescription;

class NUMLVisitor;

class NUMLDocument;
/*
 *
 */
class LIBNUML_EXTERN CompositeDescription : public DimensionDescription
{

public:

  CompositeDescription();

  ~CompositeDescription();
  /**
  * Creates a new CompositeDescription using the given NUML @p level and @p version
  * values.
  *
  * @param level an unsigned int, the NUML Level to assign to this CompositeDescription
  *
  * @param version an unsigned int, the NUML Version to assign to this
  * CompositeDescription
  *
  * @note Once a CompositeDescription has been added to an NUMLDocument, the @p level,
  * @p version for the document @em override those used
  * to create the CompositeDescription.  Despite this, the ability to supply the values
  * at creation time is an important aid to creating valid NUML.  Knowledge of
  * the intented NUML Level and Version determine whether it is valid to
  * assign a particular value to an attribute, or whether it is valid to add
  * an object to an existing NUMLDocument.
  */
  CompositeDescription (unsigned int level, unsigned int version);

  /**
  * Creates a new CompositeDescription using the given NUMLNamespaces object
  * @p numlns.
  *
  * The NUMLNamespaces object encapsulates NUML Level/Version/namespaces
  * information.  It is used to communicate the NUML Level, Version, and
  * (in Level&nbsp;3) packages used in addition to NUML Level&nbsp; Core.
  * A common approach to using this class constructor is to create an
  * NUMLNamespaces object somewhere in a program, once, then pass it to
  * object constructors such as this one when needed.
  *
  * @param numlns an NUMLNamespaces object.
  *
  * @note Once a CompositeDescription has been added to an NUMLDocument, the @p level,
  * @p version and @p xmlns namespaces for the document @em override those used
  * to create the CompositeDescription.  Despite this, the ability to supply the values
  * at creation time is an important aid to creating valid NUML.  Knowledge of
  * the intented NUML Level and Version determine whether it is valid to
  * assign a particular value to an attribute, or whether it is valid to add
  * an object to an existing NUMLDocument.
  */
  CompositeDescription (NUMLNamespaces* numlns);

  /**
  * Creates and returns a deep copy of this CompositeDescription instance.
  *
  * @return a (deep) copy of this CompositeDescription.
  */
  virtual CompositeDescription* clone () const;

  /**
  * Accepts the given NUMLVisitor for this instance of Dimension.
  *
  * @param v the NUMLVisitor instance to be used.
  *
  * @return the result of calling <code>v.visit()</code>.
  */
  virtual bool accept (NUMLVisitor& v) const;

  /**
  * Returns the value of the "name" attribute of this CompositeDescription.
  *
  * @return the name of this CompositeDescription.
  */
  virtual const std::string& getName () const;

  virtual const std::string& getId() const;
  virtual int setId(const std::string& id);


  /**
  * Returns the value of the "indexType" attribute of this CompositeDescription.
  *
  * @return the indexType of this CompositeDescription.
  */
  const std::string& getIndexType () const;

  /**
    * Predicate returning @c true or @c false depending on whether this
    * CompositeDescription object's content is another CompositeDescription object.
    *
    * @return @c true if the content of this CompositeDescription object is another CompositeDescription object,
    * @c false otherwise.
    *
    * @see getCompositeDescription(unsigned int n)
    */
    bool isContentCompositeDescription () const;

    /**
    * Predicate returning @c true or @c false depending on whether this
    * CompositeDescription object's content is TupleDescription object.
    *
    * @return @c true if the content of this CompositeDescription object is TupleDescription object,
    * @c false otherwise.
    *
    * @see getTupleDescription()
    */
    bool isContentTupleDescription () const;

    /**
    * Predicate returning @c true or @c false depending on whether this
    * CompositeDescription object's content is AtomicDescription object.
    *
    * @return @c true if the content of this CompositeDescription object is AtomicDescription object,
    * @c false otherwise.
    *
    * @see getAtomicDescription()
    */
    bool isContentAtomicDescription () const;


  /**
  * Returns the value of the "ontologyTerm" attribute of this CompositeDescription.
  *
  * @return the ontologyTerm of this CompositeDescription.
  */
  const std::string& getOntologyTerm () const;

  /**
  * Sets the value of the "name" attribute of this NUML object.
  *
  * The string in @p name is copied.
  *
  * @param name the new name for the object
  *
  * @return integer value indicating success/failure of the
  * function.  @if clike The value is drawn from the
  * enumeration #OperationReturnValues_t. @endif The possible values
  * returned by this function are:
  * @li LIBNUML_OPERATION_SUCCESS
  * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
  */
  virtual int setName (const std::string& name);

  /**
  * Sets the value of the "indexType" attribute of this NUML object.
  *
  * The string in @p indexType is copied.
  *
  * @param indexType the new indexType for the object
  *
  * @return integer value indicating success/failure of the
  * function.  @if clike The value is drawn from the
  * enumeration #OperationReturnValues_t. @endif The possible values
  * returned by this function are:
  * @li LIBNUML_OPERATION_SUCCESS
  * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
  */
  int setIndexType (const std::string& indexType);


  /**
  * Sets the value of the "ontologyTerm" attribute of this NUML object.
  *
  * The string in @p ontologyTerm is copied.
  *
  * @param ontologyTerm the new ontologyTerm for the object
  *
  * @return integer value indicating success/failure of the
  * function.  @if clike The value is drawn from the
  * enumeration #OperationReturnValues_t. @endif The possible values
  * returned by this function are:
  * @li LIBNUML_OPERATION_SUCCESS
  * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
  */
  int setOntologyTerm (const std::string& ontologyTerm);

  /**
  * Returns the XML element name of this object, which for CompositeDescription, is
  * always @c "CompositeDescription".
  *
  * @return the name of this element, i.e., @c "compositeDescription".
  */
  virtual const std::string& getElementName () const;

  /**
  * Returns the libNUML type code for this %NUML object.
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
  *
  * @return the NUML type code for this object, or @c NUML_UNKNOWN (default).
  *
  * @see getElementName()
  */
  virtual NUMLTypeCode_t getTypeCode () const { return NUML_COMPOSITEDESCRIPTION; };


  /**
  * Returns the libNUML type code for the objects contained in this NUMLList
  * (i.e., CompositeDescription objects, if the list is non-empty).
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
  *
  * @return the NUML type code for the objects contained in this NUMLList
  * instance, or @c NUML_UNKNOWN (default).
  *
  * @see getElementName()
  */
  virtual NUMLTypeCode_t getItemTypeCode () const;


  /**
  * Get a CompositeDescription from the CompositeDescription.
  *
  * @param n the index number of the CompositeDescription to get.
  *
  * @return the nth CompositeDescription in this CompositeDescription.
  *
  * @see size()
  */

  CompositeDescription * getCompositeDescription(unsigned int n);

  /**
  * Get a TupleDescription from the CompositeDescription.
  *
  *
  * @return the only 0th TupleDescription in this CompositeDescription.
  *
  * @see size()
  */

  TupleDescription * getTupleDescription();

  /**
  * Get AtomicDescription from the CompositeDescription.
  *
  *
  * @return the only (0th) AtomicDescription in this CompositeDescription.
  *
  * @see size()
  */

  AtomicDescription * getAtomicDescription();


  /**
  * Get a CompositeDescription from the CompositeDescription.
  *
  * @param n the index number of the CompositeDescription to get.
  *
  * @return the nth CompositeDescription in this CompositeDescription.
  *
  * @see size()
  */
  virtual CompositeDescription * get(unsigned int n);


  /**
  * Get a CompositeDescription from the CompositeDescription.
  *
  * @param n the index number of the CompositeDescription to get.
  *
  * @return the nth CompositeDescription in this CompositeDescription.
  *
  * @see size()
  */
  virtual const CompositeDescription * get(unsigned int n) const;

  /**
  * Removes the nth item from this CompositeDescription items and returns a pointer to
  * it.
  *
  * The caller owns the returned item and is responsible for deleting it.
  *
  * @param n the index of the item to remove
  *
  * @see size()
  */
  virtual CompositeDescription* remove (unsigned int n);

  /**
  * Creates a new CompositeDescription object inside this CompositeDescription add to it's List and return it.
  *
  * @return the CompositeDescription object created
  *
  * @see addCompositeDescription(const CompositeDescription *compDescription)
  */
  CompositeDescription* createCompositeDescription();

  /**
  * Creates a new TupleDescription inside this CompositeDescription and return it.
  *
  * @return the TupleDescription object created
  *
  * @see addTupleDescription(const TupleDescription *tDescription)
  */
  TupleDescription* createTupleDescription();

  /**
  * Creates a new AtomicDescription to this CompositeDescription and returns it.
  *
  * @return the AtomicDescription object created
  *
  * @see addAtomicDescription(const AtomicValue *aDescription)
  */
  AtomicDescription* createAtomicDescription();
  /**
  * Subclasses should override this method to write out their contained
   * NUML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.
   */
  //virtual void writeElements(XMLOutputStream& stream) const;


protected:

  /**
  * @return the NUML object corresponding to next XMLToken in the
  * XMLInputStream or NULL if the token was not recognized.
  */
  virtual NMBase* createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);

  virtual void readAttributes(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes & attributes);
  virtual void writeAttributes(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream & stream) const;

//	TupleDescription mTupleDescription;
//	AtomicDescription *mAtomicDescription;

  std::string  mId;
  std::string  mName;
  std::string  mOntologyTerm;
  std::string  mIndexType;
  NUMLTypeCode_t mContentType;



  
};
LIBNUML_CPP_NAMESPACE_END

#endif  /* __cplusplus */

#ifndef SWIG

LIBNUML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

//TODO

END_C_DECLS
LIBNUML_CPP_NAMESPACE_END

#endif  /* !SWIG   */

#endif /* COMPOSITEDESCRIPTION_H_ */
