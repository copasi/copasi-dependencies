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
*/

/**
* @class AtomicDescription
* @brief This class stores the atomic description
*/

#ifndef ATOMICDESCRIPTION_H_
#define ATOMICDESCRIPTION_H_

#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>


#include <numl/NMBase.h>
#include <numl/NUMLList.h>
#include <numl/DimensionDescription.h>
#include <string>
#include <sstream>

#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLOutputStream.h>

#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

class LIBNUML_EXTERN AtomicDescription : public DimensionDescription {

public:

  /**
  * Creates an new AtomicDescription using the given NUML @p level and @p version
  * values.
  *
  * @param level an unsigned int, the NUML Level to assign to this AtomicDescription
  *
  * @param version an unsigned int, the NUML Version to assign to this
  *AtomicDescription
  *
  * @note Once an AtomicDescription has been added to an NUMLDocument, the @p level,
  * @p version for the document @em override those used
  * to create the AtomicDescription.  Despite this, the ability to supply the values
  * at creation time is an important aid to creating valid NUML.  Knowledge of
  * the intented NUML Level and Version determine whether it is valid to
  * assign a particular value to an attribute, or whether it is valid to add
  * an object to an existing NUMLDocument.
  */
  AtomicDescription (unsigned int level, unsigned int version);


  /**
  * Creates a new AtomicDescription using the given NUMLNamespaces object
  * @p numlns.
  *
  * The NUMLNamespaces object encapsulates NUML Level/Version/namespaces
  * information.  It is used to communicate the NUML Level, Version, and
  * (in Level&nbsp;3) packages used in addition to NUML Level&nbsp; Core.
  * A common approach to using this class constructor is to create an
  * NUMLNamespaces object somewhere in a program, once, then pass it to
  * object constructors such as this one when needed.
  *
  * It is worth emphasizing that although this constructor does not take
  * an identifier argument, in NUML Level&nbsp;2 and beyond, the "id"
  * (identifier) attribute of an AtomicDescription is required to have a value.
  * Thus, callers are cautioned to assign a value after calling this
  * constructor.  Setting the identifier can be accomplished using the
  * method @if clike SBase::setId() @endif@if java SBase::setId(String id) @endif.
  *
  * @param numlns an NUMLNamespaces object.
  *
  * @note Once an AtomicDescription has been added to an NUMLDocument, the @p level,
  * @p version and @p xmlns namespaces for the document @em override those used
  * to create the AtomicDescription.  Despite this, the ability to supply the values
  * at creation time is an important aid to creating valid NUML.  Knowledge of
  * the intented NUML Level and Version determine whether it is valid to
  * assign a particular value to an attribute, or whether it is valid to add
  * an object to an existing NUMLDocument.
  */
  AtomicDescription (NUMLNamespaces* numlns);

  /**
  * Accepts the given NUMLVisitor for this instance of AtomicDescription.
  *
  * @param v the NUMLVisitor instance to be used.
  *
  * @return the result of calling <code>v.visit()</code>.
  */
  virtual bool accept (NUMLVisitor& v) const;


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
  virtual NUMLTypeCode_t getTypeCode () const;

  /**
  * Returns the XML element name of this object, which for AtomicDescription, is
  * always @c "atomicValue".
  *
  * @return the name of this element, i.e., @c "atomicValue".
  */
  virtual const std::string& getElementName () const;

  /**
  * Returns the value of the "valueType" attribute of this AtomicDescription.
  *
  * @return the valueType of this AtomicDescription.
  */
  const std::string& getValueType () const;

  /**
  * Returns the value of the "ontologyTerm" attribute of this AtomicDescription.
  *
  * @return the ontologyTerm of this AtomicDescription.
  */
  const std::string& getOntologyTerm () const;


  /**
  * Returns the character name in this object.
  *
  * @return the string name".
  */
  virtual const std::string& getName () const;


  /**
  * Sets the ontologyTerm of the "ontologyTerm" attribute of this NUML object.
  *
  * The string in @p ontologyTerm is copied.
  *
  * @param ontologyTerm the new ontologyTerm for the object
  *
  * @return integer value indicating success/failure of the
  * function.
  */
  int setOntologyTerm (const std::string& ontologyTerm);

  /**
  * Sets the valueType of the "valueType" attribute of this NUML object.
  *
  * The string in @p valueType is copied.
  *
  * @param valueType the new valueType for the object
  *
  * @return integer value indicating success/failure of the
  * function.
  */
  int setValueType (const std::string& valueType);

  /**
  * Sets the name of the "name" attribute of this NUML object.
  *
  * The string in @p name is copied.
  *
  * @param name the new name for the object
  *
  * @return integer value indicating success/failure of the
  * function.
  */
  int setName (const std::string& name);

  /**
   * @return the id of this element
   */
  virtual const std::string& getId() const;

  /**
   * set the id of this element
   * @param id the id to set
   * @return success code
   */
  virtual int setId(const std::string& id);

  /**
  * Creates and returns a deep copy of this AtomicDescription.
  *
  * @return a (deep) copy of this AtomicDescription.
  */
  virtual AtomicDescription* clone () const;
  AtomicDescription();
  virtual ~AtomicDescription();

protected:
  /* this is a constructor that takes no arguments and
   * only exists because the validator code needs it
   */
  // AtomicDescription ();

  /**
  * Subclasses should override this method to create, store, and then
  * return an NUML object corresponding to the next XMLToken in the
  * XMLInputStream.
  *
  * @return the NUML object corresponding to next XMLToken in the
  * XMLInputStream or NULL if the token was not recognized.
  */
  //virtual NMBase* createObject (XMLInputStream& stream);
  virtual void readAttributes(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes & attributes);
  virtual void writeAttributes(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream & stream) const;

  std::string  mId;
  std::string  mName;
  std::string  mOntologyTerm;
  std::string  mValueType;
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

#endif /* ATOMICDESCRIPTION_H_ */
