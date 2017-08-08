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
 * @class ResultComponent
 * @brief stores the result component
 */


#ifndef RESULTCOMPONENT_H_
#define RESULTCOMPONENT_H_

#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>


//#include <string>
//#include <algorithm>
//#include <vector>


#include <numl/Dimension.h>
#include <numl/DimensionDescription.h>
#include <numl/NMBase.h>
#include <string>

#include <numl/NUMLList.h>


#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

class ResultComponent;
class ResultComponents;
class Dimension;
class DimensionDescription;
class NUMLVisitor;

/*
 *
 */

class LIBNUML_EXTERN ResultComponent: public NMBase {
public:

//	ResultComponent();
	/**
	* Creates a new ResultComponent using the given NUML @p level and @p version
	* values.
	*
	* @param level an unsigned int, the NUML Level to assign to this ResultComponent
	*
	* @param version an unsigned int, the NUML Version to assign to this
	* ResultComponent
	*
	* @note Once a ResultComponent has been added to an NUMLDocument, the @p level,
	* @p version for the document @em override those used
	* to create the ResultComponent.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	ResultComponent (unsigned int level, unsigned int version);


	/**
	* Accepts the given NUMLVisitor for this instance of NMBase.
	*
	* @param v the NUMLVisitor instance to be used.
	*
	* @return the result of calling <code>v.visit()</code>.
	*/
	virtual bool accept (NUMLVisitor& v) const;


	/**
	* Creates and returns a deep copy of this NMBase object.
	*
	* @return a (deep) copy of this NMBase object
	*/
	virtual ResultComponent* clone () const;

	/**
	* Returns the value of the "id" attribute of this ResultComponent.
	*
	* @return the id of this ResultComponent.
	*/
	const std::string& getId () const;

	/*
	 *
	 * Sets the value of the "id" attribute of this NUML object.
	 *
	 * The string @p id is copied.  Note that NUML has strict requirements
	 * for the syntax of identifiers.  The following is summary of the
	 * definition of the NUML identifier type @c SId (here expressed in an
	 * extended form of BNF notation):
	 * @code
	 *   letter ::= 'a'..'z','A'..'Z'
	 *   digit  ::= '0'..'9'
	 *   idChar ::= letter | digit | '_'
	 *   SId    ::= ( letter | '_' ) idChar*
	 * @endcode
	 * The equality of NUML identifiers is determined by an exact character
	 * sequence match; i.e., comparisons must be performed in a
	 * case-sensitive manner.  In addition, there are a few conditions for
	 * the uniqueness of identifiers in an NUML model.  Please consult the
	 * NUML specifications for the exact formulations.
	 *
	 * @param id the striclass ResultComponents;ng to use as the identifier of this object
	 *
	 * @return integer value indicating success/failure of the
	 * function.  @if clike The value is drawn from the
	 * enumeration #ResultComponentReturnValues_t. @endif The possible values
	 * returned by this function are:
	 * @li LIBNUML_OPERATION_SUCCESS
	 * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
	 */
	int setId (const std::string& sid);

	//TODO remove the following

	/**
	* Returns a list of the XML Namespaces declared on this NUML document.
	*
	* @return the XML Namespaces associated with this NUML object
	*/
	//	virtual XMLNamespaces* getNamespaces() const ;

	/**
	* Sets the parent NUMLDocument of this NUML object.
	*
	* @param d the NUMLDocument object to use
	*/
	virtual void setNUMLDocument (NUMLDocument* d);



	/**
	* Sets the parent NUML object of this NUML object.
	*
	* @param sb the NUML object to use
	*/
	virtual void setParentNUMLObject (NMBase* sb);

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
	virtual NUMLTypeCode_t getTypeCode() const;

	/**
	* Returns the XML element name of this object, which for ResultComponent, is
	* always @c "model".
	*
	* @return the name of this element, i.e., @c "model".
	*/
	virtual const std::string& getElementName () const;

	/**
	* Creates a new ResultComponent using the given NUMLNamespaces object
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
	* @note Once a ResultComponent has been added to an NUMLDocument, the @p level,
	* @p version and @p xmlns namespaces for the document @em override those used
	* to create the ResultComponent.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	ResultComponent (NUMLNamespaces* numlns);

	/**
	* Creates a new DimensionDescription inside this ResultComponent and returns it.
	*
	* @return the DimensionDescription object created
	*
	* @see addDimensionDescription(const DimensionDescription *r)
	*/
	DimensionDescription* createDimensionDescription ();

	/**
	 * Creates a new CompositeDescription and add it to DimensionDescription's list inside this ResultComponent and return it.
	 *
	 * @return the CompositeDescription object created
	 *
	 * @see addCompositeDescription(const CompositeDescription *compDesc)
	 */
	CompositeDescription* createCompositeDescription();

	/**
	* Creates a new CompositeValue and add it to Dimension's list inside this ResultComponent and return it.
	*
	* @return the CompositeValue object created
	*
	* @see addCompositeValue(const CompositeValue *compValue)
	*/
	CompositeValue* createCompositeValue ();

  /**
   * creates a new tupledescription and adds it to the dimensiondescription
   * @return the created tuple description
   */
  TupleDescription* createTupleDescription();

  /**
   * creates a new tuple and adds it to the dimension
   * @return the created tuple
   */
  Tuple* createTuple();

  /**
   * creates a new atomic description and adds it to the dimensiondescription
   * @return the created atomic description
   */
  AtomicDescription* createAtomicDescription();

  /**
   * creates a new atomic value and adds it to the dimension
   * @return the created atomic value
   */
  AtomicValue* createAtomicValue();

	/**
	* Get the DimensionDescription object in this ResultComponent.
	*
	* @return the DimensionDescription of this ResultComponent.
	*/
	DimensionDescription* getDimensionDescription ();

	/**
	* Get the Dimension object in this ResultComponent.
	*
	* @return the Dimension of this ResultComponent.
	*/
	Dimension* getDimension ();

	/**
	 * Subclasses should override this method to write out their contained
	 * NUML objects as XML elements.  Be sure to call your parents
	 * implementation of this method as well.
	 */
  void writeElements(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;



	virtual ~ResultComponent();

//protected:
	/**
	* @return the NUML object corresponding to next XMLToken in the
	* XMLInputStream or NULL if the token was not recognized.
	*/
  virtual NMBase* createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);


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


protected:
	ResultComponent();

	std::string  mId;
	DimensionDescription mDimensionDescription;
	Dimension mDimension;

};

class LIBNUML_EXTERN ResultComponents : public NUMLList
{
public:

	/**
	* Creates and returns a deep copy of this ResultComponents instance.
	*
	* @return a (deep) copy of this ResultComponents.
	*/
	virtual ResultComponents* clone () const;


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
	virtual NUMLTypeCode_t getTypeCode () const { return NUML_RESULTCOMPONENTS; };


	/**
	* Returns the libNUML type code for the objects contained in this NUMLList
	* (i.e., ResultComponent objects, if the list is non-empty).
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
	* Returns the XML element name of this object.
	*
	* For ResultComponents, the XML element name is @c "ResultComponents".
	*
	* @return the name of this element, i.e., @c "ResultComponents".
	*/
	virtual const std::string& getElementName () const;


	/**
	* Get a ResultComponent from the ResultComponents.
	*
	* @param n the index number of the ResultComponent to get.
	*
	* @return the nth ResultComponent in this ResultComponents.
	*
	* @see size()
	*/
	virtual ResultComponent * get(unsigned int n);


	/**
	* Get a ResultComponent from the ResultComponents.
	*
	* @param n the index number of the ResultComponent to get.
	*
	* @return the nth ResultComponent in this ResultComponents.
	*
	* @see size()
	*/
	virtual const ResultComponent * get(unsigned int n) const;

	/**
	* Get all the ResultComponents.
	*
	*
	* @return this ResultComponents.
	*
	* @see size()
	*/
	virtual ResultComponents * getResultComponents();


	/**
	* Get a ResultComponent from the ResultComponents
	* based on its identifier.
	*
	* @param sid a string representing the identifier
	* of the ResultComponent to get.
	*
	* @return ResultComponent in this ResultComponents
	* with the given id or NULL if no such
	* ResultComponent exists.
	*
	* @see get(unsigned int n)
	* @see size()
	*/
	virtual ResultComponent* get (const std::string& sid);


	/**
	* Get a ResultComponent from the ResultComponents
	* based on its identifier.
	*
	* @param sid a string representing the identifier
	* of the ResultComponent to get.
	*
	* @return ResultComponent in this ResultComponents
	* with the given id or NULL if no such
	* ResultComponent exists.
	*
	* @see get(unsigned int n)
	* @see size()
	*/
	virtual const ResultComponent* get (const std::string& sid) const;


	/**
	* Removes the nth item from this ResultComponents items and returns a pointer to
	* it.
	*
	* The caller owns the returned item and is responsible for deleting it.
	*
	* @param n the index of the item to remove
	*
	* @see size()
	*/
	virtual ResultComponent* remove (unsigned int n);


	/**
	* Removes item in this ResultComponents items with the given identifier.
	*
	* The caller owns the returned item and is responsible for deleting it.
	* If none of the items in this list have the identifier @p sid, then @c
	* NULL is returned.
	*
	* @param sid the identifier of the item to remove
	*
	* @return the item removed.  As mentioned above, the caller owns the
	* returned item.
	*/
	virtual ResultComponent* remove (const std::string& sid);



	/**
	* Get the ordinal position of this element in the containing object
	* (which in this case is the Model object).
	*
	* The ordering of elements in the XML form of %NUML is generally fixed
	* for most components in %NUML.  So, for example, the ResultComponents in
	* a model is (in %NUML Level&nbsp;2 Version&nbsp;4) the sixth
	* NUMLList.  (However, it differs for different Levels and Versions of
	* NUML.)
	*
	* @return the ordinal position of the element with respect to its
	* siblings, or @c -1 (default) to indicate the position is not significant.
	*/
	virtual int getElementPosition () const;

	

protected:

	/**
	* @return the NUML object corresponding to next XMLToken in the
	* XMLInputStream or NULL if the token wascreateDimensionDescription not recognized.
	*/
  virtual NMBase* createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);


};

LIBNUML_CPP_NAMESPACE_END

#endif  /* __cplusplus */

//LIBNUML_EXTERN
//ResultComponent_t *
//ResultComponent_createDimensionDescription(void);

#ifndef SWIG

#include <stdio.h>

LIBNUML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

//TODO
LIBNUML_EXTERN
DimensionDescription_t *
ResultComponent_createDimensionDescription(ResultComponent_t *rComp);

LIBNUML_EXTERN
CompositeDescription_t *
ResultComponent_createCompositeDescription(ResultComponent_t *rComp);

END_C_DECLS
LIBNUML_CPP_NAMESPACE_END

#endif  /* !SWIG   */

#endif /* RESULTCOMPONENT_H_ */
