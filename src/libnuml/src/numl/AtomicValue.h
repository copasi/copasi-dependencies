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
 * @class AtomicValue
 * @brief This class stores the atomic values
 */


#ifndef ATOMICVALUE_H_
#define ATOMICVALUE_H_

#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>


#include <numl/Dimension.h>
#include <numl/NUMLList.h>
#include <numl/NUMLTypes.h>
#include <string>
#include <sstream>

#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

class LIBNUML_EXTERN AtomicValue : public Dimension {

public:

	/**
	* Creates an new AtomicValue using the given NUML @p level and @p version
	* values.
	*
	* @param level an unsigned int, the NUML Level to assign to this AtomicValue
	*
	* @param version an unsigned int, the NUML Version to assign to this
	*AtomicValue
	*
	* @note Once an AtomicValue has been added to an NUMLDocument, the @p level,
	* @p version for the document @em override those used
	* to create the AtomicValue.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	AtomicValue (unsigned int level, unsigned int version);


	/**
	* Creates a new AtomicValue using the given NUMLNamespaces object
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
	* (identifier) attribute of an AtomicValue is required to have a value.
	* Thus, callers are cautioned to assign a value after calling this
	* constructor.  Setting the identifier can be accomplished using the
	* method @if clike SBase::setId() @endif@if java SBase::setId(String id) @endif.
	*
	* @param numlns an NUMLNamespaces object.
	*
	* @note Once an AtomicValue has been added to an NUMLDocument, the @p level,
	* @p version and @p xmlns namespaces for the document @em override those used
	* to create the AtomicValue.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	AtomicValue (NUMLNamespaces* numlns);

	/**
	* Accepts the given NUMLVisitor for this instance of AtomicValue.
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
	* Returns the XML element name of this object, which for AtomicValue, is
	* always @c "atomicValue".
	*
	* @return the name of this element, i.e., @c "atomicValue".
	*/
	virtual const std::string& getElementName () const;

	/**
	* Returns the character value in this object.
	*
	* @return the string value".
	*/
	virtual const std::string& getValue () const;

	/**
	* Returns the double value of the character value in this object.
	*
	* @return the double value of the character value".
	*/
	virtual double getDoubleValue ();


	/**
	* Sets the value of the "value" attribute of this NUML object.
	*
	* The string in @p value is copied.
	*
	* @param value the new value for the object
	*
	* @return integer value indicating success/failure of the
	* function.
	*/
	int setValue (const std::string& value);

	/**
	* Creates and returns a deep copy of this AtomicValue.
	*
	* @return a (deep) copy of this AtomicValue.
	*/

	/**
	 * Writes (serializes) this NUML object character value by writing it to XMLOutputStream.
	 */
  void writeChars(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;

	/**
	 * Subclasses should override this method to write out their contained
	 * NUML objects as XML elements.  Be sure to call your parents
	 * implementation of this method as well.  For example:
	 *
	 */
	//virtual void write(XMLOutputStream& stream) const;

//	virtual void write(XMLOutputStream& stream) const;

	virtual AtomicValue* clone () const;
	AtomicValue();
	virtual ~AtomicValue();

protected:

	/* this is a constructor that takes no arguments and
	 * only exists because the validator code needs it
	 */
	// AtomicValue ();

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


	std::string  mValue;
	//double  mDoubleValue;
};

LIBNUML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif /* ATOMICVALUE_H_ */
