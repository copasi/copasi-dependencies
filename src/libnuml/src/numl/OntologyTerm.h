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
 * @class OntologyTerm
 * @brief ontology terms store information about what individual elements represents
 */


#ifndef ONTOLOGYTERM_H_
#define ONTOLOGYTERM_H_

#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>



//#include <numl/NUMLVisitor.h>
#include <numl/NMBase.h>
#include <string>

#include <numl/NUMLList.h>

#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

/*
 *
 */
class LIBNUML_EXTERN OntologyTerm : public NMBase  {

public:
	/**
	* Creates an new OntologyTerm using the given NUML @p level and @p version
	* values.
	*
	* @param level an unsigned int, the NUML Level to assign to thisOntologyTerm
	*
	* @param version an unsigned int, the NUML Version to assign to this
	*OntologyTerm
	*
	* @note Once an OntologyTerm has been added to an NUMLDocument, the @p level,
	* @p version for the document @em override those used
	* to create the OntologyTerm.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	OntologyTerm (unsigned int level, unsigned int version);


	/**
	* Creates a new OntologyTerm using the given NUMLNamespaces object
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
	* (identifier) attribute of an OntologyTerm is required to have a value.
	* Thus, callers are cautioned to assign a value after calling this
	* constructor.  Setting the identifier can be accomplished using the
	* method @if clike NMBase::setId() @endif@if java NMBase::setId(String id) @endif.
	*
	* @param numlns an NUMLNamespaces object.
	*
	* @note Once an OntologyTerm has been added to an NUMLDocument, the @p level,
	* @p version and @p xmlns namespaces for the document @em override those used
	* to create the OntologyTerm.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	OntologyTerm (NUMLNamespaces* numlns);

	/**
	* Accepts the given NUMLVisitor for this instance of OntologyTerm.
	*
	* @param v the NUMLVisitor instance to be used.
	*
	* @return the result of calling <code>v.visit()</code>.
	*/
	virtual bool accept (NUMLVisitor& v) const;


	/**
	* Creates and returns a deep copy of this OntologyTerm.
	*
	* @return a (deep) copy of this OntologyTerm.
	*/
	virtual OntologyTerm* clone () const;

	/**
	* Returns the value of the "id" attribute of this OntologyTerm.
	*
	* @return the id of this OntologyTerm.
	*/
	const std::string& getId () const;


	/**
	* Returns the value of the "term" attribute of this OntologyTerm.
	*
	* @return the term of this OntologyTerm.
	*/
	const std::string& getTerm () const;

	/**
	* Returns the value of the "sourceTermId" attribute of this OntologyTerm.
	*
	* @return the sourceTermId of this OntologyTerm.
	*/
	const std::string& getSourceTermId () const;

	/**
	* Returns the value of the "ontologyURI" attribute of this OntologyTerm.
	*
	* @return the ontologyURI of this OntologyTerm.
	*/
	const std::string& getOntologyURI () const;


	virtual ~OntologyTerm();

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
	 * @param id the string to use as the identifier of this object
	 *
	 * @return integer value indicating success/failure of the
	 * function.  @if clike The value is drawn from the
	 * enumeration #OperationReturnValues_t. @endif The possible values
	 * returned by this function are:
	 * @li LIBNUML_OPERATION_SUCCESS
	 * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
	 */
	int setId (const std::string& sid);


	/**
	* Sets the value of the "term" attribute of this NUML object.
	*
	* The string in @p term is copied.
	*
	* @param term the new term for the object
	*
	* @return integer value indicating success/failure of the
	* function.  @if clike The value is drawn from the
	* enumeration #OperationReturnValues_t. @endif The possible values
	* returned by this function are:
	* @li LIBNUML_OPERATION_SUCCESS
	* @li LIBNUML_INVALID_ATTRIBUTE_VALUE
	*/
	int setTerm (const std::string& term);

	/**
	* Sets the value of the "sourceTermId" attribute of this NUML object.
	*
	* The string in @p sourceTermId is copied.
	*
	* @param sourceTermId the new sourceTermId for the object
	*
	* @return integer value indicating success/failure of the
	* function.  @if clike The value is drawn from the
	* enumeration #OperationReturnValues_t. @endif The possible values
	* returned by this function are:
	 * @li LIBNUML_OPERATION_SUCCESS
	 * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
	 */
	int setSourceTermId(const std::string& sourceTermId);

	/**
	 * Sets the value of the "ontologyURI" attribute of this NUML object.
	 *
	 * The string in @p ontologyURI is copied.
	 *
	 * @param ontologyURI the new ontologyURI for the object
	 *
	 * @return integer value indicating success/failure of the
	 * function.  @if clike The value is drawn from the
	 * enumeration #OperationReturnValues_t. @endif The possible values
	 * returned by this function are:
	 * @li LIBNUML_OPERATION_SUCCESS
	 * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
	 */
	int setOntologyURI(const std::string& ontologyURI);

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
	 * Returns the XML element name of this object, which for OntologyTerm, is
	 * always @c "ontologyTerm".
	 *
	 * @return the name of this element, i.e., @c "ontologyTerm".
	 */
	virtual const std::string& getElementName() const;

	/**
	 * Subclasses should override this method to read values from the given
	 * XMLAttributes set into their specific fields.  Be sure to call your
	 * parents implementation of this method as well.
	 */
  virtual void readAttributes(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes);


	/**
	* Subclasses should override this method to write their XML attributes
	* to the XMLOutputStream.  Be sure to call your parents implementation
	* of this method as well.
	*/
  virtual void writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;

protected:


	OntologyTerm();

	std::string mId;
	std::string mTerm;
	std::string mSourceTermId;
	std::string mOntologyURI;

	//std::vector<ONTOLOGY> mOntologies;



	/* the validator classes need to be friends to access the
	 * protected constructor that takes no arguments
	 */
	friend class Validator;
	friend class ConsistencyValidator;
	friend class IdentifierConsistencyValidator;
	friend class InternalConsistencyValidator;

};

//structure for recognised ontologies
struct ONTOLOGY {
	std::string term;
	std::string sourceTermId;
	std::string ontologyURI;
};
class LIBNUML_EXTERN OntologyTerms : public NUMLList
{
public:

	/**
	* Creates and returns a deep copy of this OntologyTerms instance.
	*
	* @return a (deep) copy of this OntologyTerms.
	*/
	virtual OntologyTerms* clone () const;


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
	virtual NUMLTypeCode_t getTypeCode () const { return NUML_NUMLLIST; };


	/**
	* Returns the libNUML type code for the objects contained in this NUMLList
	* (i.e., OntologyTerm objects, if the list is non-empty).
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
	* For OntologyTerms, the XML element name is @c "OntologyTerms".
	*
	* @return the name of this element, i.e., @c "OntologyTerms".
	*/
	virtual const std::string& getElementName () const;

	/**
	* Get a OntologyTerm from the OntologyTerms.
	*
	* @param n the index number of the OntologyTerm to get.
	*
	* @return the nth OntologyTerm in this OntologyTerms.
	*
	* @see size()
	*/
	virtual OntologyTerm * get(unsigned int n);


	/**
	* Get a OntologyTerm from the OntologyTerms.
	*
	* @param n the index number of the OntologyTerm to get.
	*
	* @return the nth OntologyTerm in this OntologyTerms.
	*
	* @see size()
	*/
	virtual const OntologyTerm * get(unsigned int n) const;


	/**
	* Get a OntologyTerm from the OntologyTerms
	* based on its identifier.
	*
	* @param sid a string representing the identifier
	* of the OntologyTerm to get.
	*
	* @return OntologyTerm in this OntologyTerms
	* with the given id or NULL if no such
	* OntologyTerm exists.
	*
	* @see get(unsigned int n)
	* @see size()
	*/
	virtual OntologyTerm* get (const std::string& sid);


	/**
	* Get a OntologyTerm from the OntologyTerms
	* based on its identifier.
	*
	* @param sid a string representing the identifier
	* of the OntologyTerm to get.
	*
	* @return OntologyTerm in this OntologyTerms
	* with the given id or NULL if no such
	* OntologyTerm exists.
	*
	* @see get(unsigned int n)
	* @see size()
	*/
	virtual const OntologyTerm* get (const std::string& sid) const;


	/**
	* Removes the nth item from this OntologyTerms items and returns a pointer to
	* it.
	*
	* The caller owns the returned item and is responsible for deleting it.
	*
	* @param n the index of the item to remove
	*
	* @see size()
	*/
	virtual OntologyTerm* remove (unsigned int n);


	/**
	* Removes item in this OntologyTerms items with the given identifier.
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
	virtual OntologyTerm* remove (const std::string& sid);



	/**
	* Get the ordinal position of this element in the containing object
	* (which in this case is the Model object).
	*
	* The ordering of elements in the XML form of %NUML is generally fixed
	* for most components in %NUML.  So, for example, the OntologyTerms in
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
	* XMLInputStream or NULL if the token was not recognized.
	*/
  virtual NMBase* createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);


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

#endif /* ONTOLOGYTERM_H_ */
