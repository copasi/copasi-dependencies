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
 * @class CompositeValue
 * @brief This class stores the composite values
 */


#ifndef COMPOSITEVALUES_H_
#define COMPOSITEVALUES_H_

#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>

//#include <numl/Tuple.h>
//#include <numl/AtomicValue.h>



#include <numl/NMBase.h>
#include <numl/NUMLList.h>
#include <numl/Dimension.h>

#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

//class CompositeValue;
class NUMLDocument;
class Tuple;
class AtomicValue;
/*
 *
 */
class LIBNUML_EXTERN CompositeValue : public Dimension
{
public:

	CompositeValue();
	~CompositeValue();
	/**
	* Creates a new CompositeValue using the given NUML @p level and @p version
	* values.
	*
	* @param level an unsigned int, the NUML Level to assign to this CompositeValue
	*
	* @param version an unsigned int, the NUML Version to assign to this
	* CompositeValue
	*
	* @note Once a CompositeValue has been added to an NUMLDocument, the @p level,
	* @p version for the document @em override those used
	* to create the CompositeValue.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	CompositeValue (unsigned int level, unsigned int version);

	/**
	* Creates a new CompositeValue using the given NUMLNamespaces object
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
	* @note Once a CompositeValue has been added to an NUMLDocument, the @p level,
	* @p version and @p xmlns namespaces for the document @em override those used
	* to create the CompositeValue.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	CompositeValue (NUMLNamespaces* numlns);

	/**
	* Creates and returns a deep copy of this CompositeValue instance.
	*
	* @return a (deep) copy of this CompositeValue.
	*/
	virtual CompositeValue* clone () const;

	/**
	* Accepts the given NUMLVisitor for this instance of Dimension.
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
	virtual NUMLTypeCode_t getTypeCode () const { return NUML_COMPOSITEVALUE; };


	/**
	* Returns the value of the "indexValue" attribute of this CompositeValue.
	*
	* @return the indexValue of this CompositeValue.
	*/
	const std::string& getIndexValue () const;

	/**
	* Returns the value of the "description" attribute of this CompositeValue.
	*
	* @return the description of this CompositeValue.
	*/
	const std::string& getDescription () const;

	/**
	* Sets the value of the "indexValue" attribute of this NUML object.
	*
	* The string in @p indexValue is copied.
	*
	* @param indexValue the new indexValue for the object
	*
	* @return integer value indicating success/failure of the
	* function.  @if clike The value is drawn from the
	* enumeration #OperationReturnValues_t. @endif The possible values
	* returned by this function are:
	* @li LIBNUML_OPERATION_SUCCESS
	* @li LIBNUML_INVALID_ATTRIBUTE_VALUE
	*/
	int setIndexValue (const std::string& indexValue);

	/**
	* Sets the value of the "description" attribute of this NUML object.
	*
	* The string in @p description is copied.
	*
	* @param description the new description for the object
	*
	* @return integer value indicating success/failure of the
	* function.  @if clike The value is drawn from the
	* enumeration #OperationReturnValues_t. @endif The possible values
	* returned by this function are:
	* @li LIBNUML_OPERATION_SUCCESS
	* @li LIBNUML_INVALID_ATTRIBUTE_VALUE
	*/
	int setDescription (const std::string& description);


	/**
	* Returns the XML element name of this object, which for CompositeValue, is
	* always @c "compositeValue".
	*
	* @return the name of this element, i.e., @c "compositeValue".
	*/
	virtual const std::string& getElementName () const;

	/**
	* Returns the libNUML type code for the objects contained in this NUMLList
	* (i.e., CompositeValue objects, if the list is non-empty).
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
	* Predicate returning @c true or @c false depending on whether this
	* CompositeValue object's content is another CompositeValue object.
	*
	* @return @c true if the content of this CompositeValue object is another CompositeValue object,
	* @c false otherwise.
	*
	* @see getCompositeValue(unsigned int n)
	*/
	bool isContentCompositeValue () const;

	/**
	* Predicate returning @c true or @c false depending on whether this
	* CompositeValue object's content is Tuple object.
	*
	* @return @c true if the content of this CompositeValue object is Tuple object,
	* @c false otherwise.
	*
	* @see getTuple()
	*/
	bool isContentTuple () const;

	/**
	* Predicate returning @c true or @c false depending on whether this
	* CompositeValue object's content is AtomicValue object.
	*
	* @return @c true if the content of this CompositeValue object is AtomicValue object,
	* @c false otherwise.
	*
	* @see getAtomicValue()
	*/
	bool isContentAtomicValue () const;

	/**
	 * Get a CompositeValue from the CompositeValue.
	 *
	 * @param n the index number of the CompositeValue to get.
	 *
	 * @return the nth CompositeValue in this CompositeValue.
	 *
	 * @see size()
	 */

	CompositeValue * getCompositeValue(unsigned int n);

	/**
	* Get a Tuple from the CompositeValue.
	*
	* @param n the index number of the Tuple to get is alway zero for Tuple.
	*
	* @return the 0th Tuple in this CompositeValue.
	*
	* @see size()
	*/

	Tuple * getTuple();

	/**
	* Get AtomicValue from the CompositeValue.
	*
	* @param n the index number of the AtomicValue to get is alway zero for AtomicValue.
	*
	* @return the 0th AtomicValue in this CompositeValue.
	*
	* @see size()
	*/

	AtomicValue * getAtomicValue();

	/**
	 * Get a CompositeValue from the CompositeValue.
	 *
	 * @param n the index number of the CompositeValue to get.
	 *
	 * @return the nth CompositeValue in this CompositeValue.
	 *
	 * @see size()
	 */
	virtual const CompositeValue * get(unsigned int n) const;

	/**
	 * Get a CompositeValue from the CompositeValue.
	 *
	 * @param n the index number of the CompositeValue to get.
	 *
	 * @return the nth CompositeValue in this CompositeValue.
	 *
	 * @see size()
	 */
	virtual CompositeValue * get(unsigned int n);

	/**
	 * Removes the nth item from this CompositeValue items and returns a pointer to
	 * it.
	 *
	 * The caller owns the returned item and is responsible for deleting it.
	 *
	 * @param n the index of the item to remove
	 *
	 * @see size()
	*/
	virtual CompositeValue* remove(unsigned int n);

	/**
	* Subclasses should override this method to read values from the given
	* XMLAttributes set into their specific fields.  Be sure to call your
	* parents implementation of this method as well.
	*/
  virtual void readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes);


	/**
	* Subclasses should override this method to write their XML attributes
	* to the XMLOutputStream.  Be sure to call your parents implementation
	* of this method as well.
	*/
  virtual void writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;

	/**
	* Adds a given CompositeValue object as a child in this CompositeVlaue.
	*
	* The CompositeValue instance in @p compValue is copied.
	*
	* @param compValue a CompositeValue object referring to a Child in the
	* CompositeValue
	*
	* @return integer value indicating success/failure of the
	* function.  @if clike The value is drawn from the
	* enumeration #OperationReturnValues_t. @endif The possible values
	* returned by this function are:
	* @li LIBNUML_OPERATION_SUCCESS
	* @li LIBNUML_LEVEL_MISMATCH
	* @li LIBNUML_VERSION_MISMATCH
	* @li LIBNUML_OPERATION_FAILED
	*
	* @note This method should be used with some caution.  The fact that
	* this method @em copies the object passed to it means that the caller
	* will be left holding a physically different object instance than the
	* one contained in this CompositeVlaue.  Changes made to the original object
	* instance (such as resetting attribute values) will <em>not affect the
	* instance in the CompositeVlaue</em>.  In addition, the caller should make
	* sure to free the original object if it is no longer being used, or
	* else a memory leak will result.  Please see CompositeVlaue::createReactant()
	* for a method that does not lead to these issues.
	*
	* @see createCompositeValue()
	*/
	int addCompositeValue(const CompositeValue* compValue);

	/**
	 * Creates a new CompositeValue object inside this CompositeValue add to it's List and return it.
	 *
	 * @return the CompositeValue object created
	 *
	 * @see addCompositeValue(const CompositeValue *compValue)
	 */
	CompositeValue* createCompositeValue();

	/**
	 * Creates a new Tuple inside this CompositeValue and return it.
	 *
	 * @return the Tuple object created
	 *
	 * @see addTuple(const Tuple *tuple)
	 */
	Tuple* createTuple();

	/**
	* Creates a new AtomicValue to this CompositeValue and returns it.
	*
	* @return the AtomicValue object created
	*
	* @see addAtomicValue(const AtomicValue *aValue)
	*/
	AtomicValue* createAtomicValue();

//	void setAtomicValue(AtomicValue* atomicValue) ;

//	void setTuple(Tuple* tuple) ;

	/**
	 * Subclasses should override this method to write out their contained
	 * NUML objects as XML elements.  Be sure to call your parents
	 * implementation of this method as well.
	 */
//	virtual void writeElements(XMLOutputStream& stream) const;


protected:
	
	/**
	* @return the NUML object corresponding to next XMLToken in the
	* XMLInputStream or NULL if the token was not recognized.
	*/
  virtual NMBase* createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);
	std::string mIndexValue;
	std::string mDescription;
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

#endif /* COMPOSITEVALUES_H_ */
