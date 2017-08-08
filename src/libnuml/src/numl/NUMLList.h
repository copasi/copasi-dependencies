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
 * @class NUMLList
 * @brief Parent class for the various NUML list classes e.g. Operations, Result, ResultComponent etc.
 *
 */

#ifndef NUMLLIST_H_
#define NUMLLIST_H_

#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>
#include <numl/NUMLTypeCodes.h>





#include <vector>

#include <numl/NMBase.h>

#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

/*
 *
 */
class LIBNUML_EXTERN NUMLList : public NMBase{
public:

	/**
	* Creates a new NUMLList using the given NUML @p level and @p version
	* values.
	*
	* @param level an unsigned int, the NUML Level to assign to this NUMLList
	*
	* @param version an unsigned int, the NUML Version to assign to this
	* NUMLList
	*
	* @note Once a NUMLList has been added to an NUMLDocument, the @p level,
	* @p version for the document @em override those used
	* to create the NUMLList.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	NUMLList (unsigned int level, unsigned int version);

	/**
	* Creates a new DimensionDescription using the given NUMLNamespaces object
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
	* @note Once a DimensionDescription has been added to an NUMLDocument, the @p level,
	* @p version and @p xmlns namespaces for the document @em override those used
	* to create the DimensionDescription.  Despite this, the ability to supply the values
	* at creation time is an important aid to creating valid NUML.  Knowledge of
	* the intented NUML Level and Version determine whether it is valid to
	* assign a particular value to an attribute, or whether it is valid to add
	* an object to an existing NUMLDocument.
	*/
	NUMLList (NUMLNamespaces* numlns);

	NUMLList();
	virtual ~NUMLList();

	/**
	* Copy constructor.  Creates a copy of this NUMLList.
	*/
	NUMLList (const NUMLList& orig);


	/**
	* Assignment operator for NUMLList.
	*/
	NUMLList& operator=(const NUMLList& rhs);


	/**
	* Accepts the given NUMLVisitor.
	*
	* @param v the NUMLVisitor instance to be used.
	*
	* @return the result of calling <code>v.visit()</code>, which indicates
	* whether the Visitor would like to visit the next item in the
	* list.
	*/
	virtual bool accept (NUMLVisitor& v) const;


	/**
	* Creates and returns a deep copy of this NUMLList.
	*
	* @return a (deep) copy of this NUMLList.
	*/
	virtual NMBase* clone () const;


	/**
	* Adds item to the end of this NUMLList.
	*
	* This variant of the method makes a clone of the @p item handed to it.
	* This means that when the NUMLList is destroyed, the original items will
	* not be destroyed.
	*
	* @param item the item to be added to the list.
	*
	* @see appendAndOwn(NMBase* item)
	*/
	void append (const NMBase* item);


	/**
	* Adds item to the end of this NUMLList.
	*
	* This variant of the method does not clone the @p item handed to it;
	* instead, it assumes ownership of it.  This means that when the NUMLList
	* is destroyed, the item will be destroyed along with it.
	*
	* @param item the item to be added to the list.
	*
	* @see append(const NMBase* item)
	*/
	void appendAndOwn (NMBase* item);


	/**
	* Get an item from the list.
	*
	* @param n the index number of the item to get.
	*
	* @return the nth item in this NUMLList items.
	*
	* @see size()
	*/
	virtual const NMBase* get (unsigned int n) const;


	/**
	* Get an item from the list.
	*
	* @param n the index number of the item to get.
	*
	* @return the nth item in this NUMLList items.
	*
	* @see size()
	*/
	virtual NMBase* get (unsigned int n);


#if 0
	/**
	* Get an item from the list based on its identifier.
	*
	* @param sid a string representing the the identifier of the item to get.
	*
	* @return item in this NUMLList items with the given id or NULL if no such
	* item exists.
	*
	* @see get(unsigned int n)
	* @see size()
	*/
	virtual const NMBase* get (const std::string& sid) const;
#endif


#if 0
	/**
	* Get an item from the list based on its identifier.
	*
	* @param sid a string representing the the identifier of the item to get.
	*
	* @return item in this NUMLList items with the given id or NULL if no such
	* item exists.
	*
	* @see get(unsigned int n)
	* @see size()
	*/
	virtual NMBase* get (const std::string& sid);
#endif


	/**
	* Removes all items in this NUMLList object.
	*
	* If doDelete is true (default), all items in this NUMLList object are deleted
	* and cleared, and thus the caller doesn't have to delete those items.
	* Otherwise, all items are just cleared from this NUMLList object and the caller
	* is responsible for deleting all items (In this case, pointers to all items
	* should be stored elsewhere before calling this function by the caller).
	*
	* @param doDelete if true (default), all items are deleted and cleared.
	* Otherwise, all items are just cleared and not deleted.
	*/
	void clear (bool doDelete = true);


	/**
	* Removes the nth item from this NUMLList items and returns a pointer to
	* it.
	*
	* The caller owns the returned item and is responsible for deleting it.
	*
	* @param n the index of the item to remove
	*
	* @see size()
	*/
	virtual NMBase* remove (unsigned int n);


#if 0
	/**
	* Removes item in this NUMLList items with the given identifier.
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
	virtual NMBase* remove (const std::string& sid);
#endif


	/**
	* Get the size of this NUMLList.
	*
	* @return the number of items in this NUMLList items.
	*/
	unsigned int size () const;


	/**
	* Sets the parent NUMLDocument of this NUML object.
	*
	* @param d the NUMLDocument that should become the parent of this
	* NUMLList.
	*/
	virtual void setNUMLDocument (NUMLDocument* d);


	/**
	* Sets the parent NUML object of this NUML object.
	*
	* @param sb the NUML object to use
	*/
	virtual void setParentNUMLObject (NMBase* sb);



	  /**
	   * Returns the libnuml type code for this object, namely, @c
	   * NUMLLIST.
	   *
	   * @if clike LibNUML attaches an identifying code to every kind of NUML
	   * object.  These are known as <em>NUML type codes</em>.  The set of
	   * possible type codes is defined in the enumeration #SedTypeCode_t.
	   * The names of the type codes all begin with the characters @c
	   * NUML_. @endif@if java LibNUML attaches an identifying code to every
	   * kind of NUML object.  These are known as <em>NUML type codes</em>.  In
	   * other languages, the set of type codes is stored in an enumeration; in
	   * the Java language interface for libnuml, the type codes are defined as
	   * static integer constants in the interface class {@link
	   * libsbmlConstants}.  The names of the type codes all begin with the
	   * characters @c NUML_. @endif@if python LibNUML attaches an identifying
	   * code to every kind of NUML object.  These are known as <em>NUML type
	 NUMLLISTem>.  In the Python language interface for libnuml, the type
	   * codes are defined as static integer constants in the interface class
	   * @link libnuml@endlink.  The names of the type codes all begin with the
	   * characters @c NUML_. @endif@if csharp LibNUML attaches an identifying
	   * code to every kind of NUML object.  These are known as <em>NUML type
	   * codes</em>.  In the C# language interface for libnuml, the type codes
	   * are defined as static integer constants in the interface class @link
	   * libsbmlcs.libnuml@endlink.  The names of the type codes all begin with
	   * the characters @c NUML_. @endif@~
	   *
	   * @return the NUML type code for this object, or @link SedTypeCode_t#Sed_UNKNOWN SEDML_UNKNOWN@endlink (default).
	   *
	   * @see getElementName()
	   */
//	  virtual int getTypeCode () const;


	  /**
	   * Get the type code of the objects contained in this SedListOf.
	   *
	   * @if clike LibNUML attaches an identifying code to every kind of NUML
	   * object.  These are known as <em>NUML type codes</em>.  The set of
	   * possible type codes is defined in the enumeration #SedTypeCode_t.
	   * The names of the type codes all begin with the characters @c
	   * NUML_. @endif@if java LibNUML attaches an identifying code to every
	   * kind of NUML object.  These are known as <em>NUML type codes</em>.  In
	   * other languages, the set of type codes is stored in an enumeration; in
	   * the Java language interface for libnuml, the type codes are defined as
	   * static integer constants in the interface class {@link
	   * libsbmlConstants}.  The names of the type codes all begin with the
	   * characters @c NUML_. @endif@if python LibNUML attaches an identifying
	   * code to every kind of NUML object.  These are known as <em>NUML type
	   * codes</em>.  In the Python language interface for libnuml, the type
	   * codes are defined as static integer constants in the interface class
	   * @link libnuml@endlink.  The names of the type codes all begin with the
	   * characters @c NUML_. @endif@if csharp LibNUML attaches an identifying
	   * code to every kind of NUML object.  These are known as <em>NUML type
	   * codes</em>.  In the C# language interface for libnuml, the type codes
	   * are defined as static integer constants in the interface class @link
	   * libsbmlcs.libnuml@endlink.  The names of the type codes all begin with
	   * the characters @c NUML_. @endif@~
	   *
	   * @return the NUML type code for the objects contained in this SedListOf
	   * instance, or @link SedTypeCode_t#Sed_UNKNOWN SEDML_UNKNOWN@endlink (default).
	   */
	//  virtual int getItemTypeCode () const;






	/**
	* Returns the libNUML type code for this object, namely, @c
	* NUMLLIST.
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
	* interface class {@link libsbmlConstants}.  The names of the type codes
	* all begin with the characters @c NUML_. @endif
	*
	* @return the NUML type code for this object, or @c NUML_UNKNOWN (default).
	*
	* @see getElementName()
	*/
	virtual NUMLTypeCode_t getTypeCode () const;


	/**
	* Get the type code of the objects contained in this NUMLList.
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
	* interface class {@link libsbmlConstants}.  The names of the type codes
	* all begin with the characters @c NUML_. @endif
	*
	* @return the NUML type code for the objects contained in this NUMLList
	* instance, or @c NUML_UNKNOWN (default).
	*/
	virtual NUMLTypeCode_t getItemTypeCode () const;


	/**
	* Returns the XML element name of this object, which for NUMLList, is
	* always @c "listOf".
	*
	* @return the XML name of this element.
	*/
	virtual const std::string& getElementName () const;


	/**
	* Subclasses should override this method to write out their contained
	* NUML objects as XML elements.  Be sure to call your parents
	* implementation of this method as well.
	*/
  virtual void writeElements (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;


protected:
	
	/**
	* Subclasses should override this method to read values from the given
	* XMLAttributes set into their specific fields.  Be sure to call your
	* parents implementation of this method as well.
	*/
  virtual void readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes);

	/**
	* Subclasses should override this method to write their XML attributes
	* to the XMLOutputStream.  Be sure to call your parents implementation
	* of this method as well.  For example:
	*
	*   NMBase::writeAttributes(stream);
	*   stream.writeAttribute( "id"  , mId   );
	*   stream.writeAttribute( "name", mName );
	*   ...
	*/
  virtual void writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;

	std::vector<NMBase*> mItems;

	
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

#endif /* NUMLLIST_H_ */
