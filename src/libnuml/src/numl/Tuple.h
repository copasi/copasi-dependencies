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
 * @class Tuple
 * @brief stores tuple information
 */


#ifndef TUPLE_H_
#define TUPLE_H_

#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>


#include <numl/NMBase.h>
#include <numl/NUMLList.h>
#include <numl/Dimension.h>
#include <vector>


#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

class AtomicValue;
class AtomicValues;
class NUMLVisitor;

class LIBNUML_EXTERN Tuple : public Dimension
{

public:

	Tuple();
	~Tuple();

	/**
	* Creates a new Tuple using the given NUML @p level and @p version
	* values.
	*
	* @param level an unsigned int, the NUML Level to assign to this Tuple
	*
	* @param version an unsigned int, the NUML Version to assign to this
	* Tuple
	*
	* @note Once a Tuple has been added to an NUMLDocument, the @p level,
	* @p version for the document @em override those used
	* to create the Tuple.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	Tuple (unsigned int level, unsigned int version);

	/**
	* Creates a new Tuple using the given NUMLNamespaces object
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
	* @note Once a Tuple has been added to an NUMLDocument, the @p level,
	* @p version and @p xmlns namespaces for the document @em override those used
	* to create the Tuple.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	Tuple (NUMLNamespaces* numlns);

	/**
	* Creates and returns a deep copy of this Tuple instance.
	*
	* @return a (deep) copy of this Tuple.
	*/
	virtual Tuple* clone () const;


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
	virtual NUMLTypeCode_t getTypeCode () const { return NUML_TUPLE; };


	/**
	* Returns the libNUML type code for the objects contained in this NUMLList
	* (i.e., AtomicValue objects, if the list is non-empty).
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
	* Get the ordinal position of this element in the containing object
	* (which in this case is the Model object).
	*
	* The ordering of elements in the XML form of %NUML is generally fixed
	* for most components in %NUML.  So, for example, the Tuple in
	* a model is (in %NUML Level&nbsp;2 Version&nbsp;4) the sixth
	* NUMLList.  (However, it differs for different Levels and Versions of
	* NUML.)
	*
	* @return the ordinal position of the element with respect to its
	* siblings, or @c -1 (default) to indicate the position is not significant.
	*/
	virtual int getElementPosition () const;

	/**
	* Returns the XML element name of this object.
	*
	* For Tuple, the XML element name is @c "Tuple".
	*
	* @return the name of this element, i.e., @c "Tuple".
	*/
	virtual const std::string& getElementName () const;





	/**
	* Get AtomicValue from the Tuple.
	*
	* @param n the index number of the AtomicValue to get.
	*
	* @return the nth AtomicValue in this Tuple.
	*
	* @see size()
	*/
	virtual AtomicValue * getAtomicValue(unsigned int n);


	/**
	* Get AtomicValue from the Tuple.
	*
	* @param n the index number of the AtomicValue to get.
	*
	* @return the nth AtomicValue in this Tuple.
	*
	* @see size()
	*/
	virtual const AtomicValue * getAtomicValue(unsigned int n) const;

	/**
	* Removes the nth AtomicValue from this Tuple items and returns a pointer to
	* it.
	*
	* The caller owns the returned AtomicValue and is responsible for deleting it.
	*
	* @param n the index of the AtomicValue to remove
	*
	* @see size()
	*/
	virtual AtomicValue* removeAtomicValue (unsigned int n);

	/**
	* Creates a new AtomicValue inside this Tuple and returns it.
	*
	* @return the AtomicValue object created
	*
	* @see addAtomicValue(const AtomicValue *aValue)
	*/
	AtomicValue* createAtomicValue();


  virtual NMBase* createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);

	/**
	 * Subclasses should override this method to write out their contained
	 * NUML objects as XML elements.  Be sure to call your parents
	 * implementation of this method as well.
	 */
	//virtual void writeElements(XMLOutputStream& stream) const;

  virtual void write(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;

protected:
	
	/**
	* @return the NUML object corresponding to next XMLToken in the
	* XMLInputStream or NULL if the token was not recognized.
	*/
	//virtual NMBase* createObject (XMLInputStream& stream);
	//Tuple *mTuple;

  virtual void readAttributes(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes & attributes);
  virtual void writeAttributes(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream & stream) const;

	//attributes
//	AtomicValue *mAtomicValue;


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
#endif /* TUPLE_H_ */
