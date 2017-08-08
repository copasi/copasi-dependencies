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
 * @class DimensionDescription
 * @brief This class stores the dimension description
 */


#ifndef DIMENSIONDESCRIPTION_H_
#define DIMENSIONDESCRIPTION_H_

#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>

#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>


//#include <numl/AtomicDescription.h>
//#include <numl/CompositeDescription.h>
//#include <numl/TupleDescription.h>

#include <numl/NUMLVisitor.h>
#include <numl/NUMLError.h>
#include <numl/NMBase.h>
#include <numl/NUMLList.h>

#include <string>

#ifdef __cplusplus

LIBNUML_CPP_NAMESPACE_BEGIN

class NUMLDocument;
class AtomicDescription;
class TupleDescription;
class CompositeDescription;
/*
 *
 */
class LIBNUML_EXTERN DimensionDescription : public NUMLList
{
public:
  DimensionDescription();
  /**
  * Creates a new DimensionDescription using the given NUML @p level and @p version
  * values.
  *
  * @param level an unsigned int, the NUML Level to assign to this DimensionDescription
  *
  * @param version an unsigned int, the NUML Version to assign to this
  * DimensionDescription
  *
  * @note Once a DimensionDescription has been added to an NUMLDocument, the @p level,
  * @p version for the document @em override those used
  * to create the DimensionDescription.  Despite this, the ability to supply the values
  * at creation time is an important aid to creating valid NUML.  Knowledge of
  * the intented NUML Level and Version determine whether it is valid to
  * assign a particular value to an attribute, or whether it is valid to add
  * an object to an existing NUMLDocument.
  */
  DimensionDescription (unsigned int level, unsigned int version);

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
  DimensionDescription (NUMLNamespaces* numlns);


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
  virtual DimensionDescription* clone () const;


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
//		virtual void setNUMLDocument (NUMLDocument* d);



  /**
   * Sets the parent NUML object of this NUML object.
   *
   * @param sb the NUML object to use
   */
  //	virtual void setParentNUMLObject (NMBase* sb);

  /**
   * Creates a new CompositeDescription and add it to DimensionDescription's list inside this ResultComponent and return it.
   *
   * @return the CompositeDescription object created
   *
   * @see addCompositeDescription(const CompositeDescription *compDesc)
   */
  CompositeDescription* createCompositeDescription();

  /**
   * creates a new tupledescription and adds it to the dimensiondescription
   * @return the created tuple description
   */
  TupleDescription* createTupleDescription();

  /**
   * creates a new atomic description and adds it to the dimensiondescription
   * @return the created atomic description
   */
  AtomicDescription* createAtomicDescription();


  /**
  * Returns the XML element name of this object, which for DimensionDescription, is
  * always @c "model".
  *
  * @return the name of this element, i.e., @c "model".
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
  virtual NUMLTypeCode_t getTypeCode () const { return NUML_DIMENSIONDESCRIPTION; };


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
  * Returns the XML element name of this object.
  *
  * For DimensionDescription, the XML element name is @c "DimensionDescription".
  *
  * @return the name of this element, i.e., @c "DimensionDescription".
  */
//	virtual const std::string& getElementName () const;


  /**
  * Get a CompositeDescription from the DimensionDescription.
  *
  * @param n the index number of the CompositeDescription to get.
  *
  * @return the nth CompositeDescription in this DimensionDescription.
  *
  * @see size()
  */
  virtual DimensionDescription * get(unsigned int n);


  /**
  * Get a CompositeDescription from the DimensionDescription.
  *
  * @param n the index number of the CompositeDescription to get.
  *
  * @return the nth CompositeDescription in this DimensionDescription.
  *
  * @see size()
  */
  virtual const DimensionDescription * get(unsigned int n) const;

  /**
  * Get a CompositeDescription from the DimensionDescription
  * based on its identifier.
  *
  * @param sid a string representing the identifier
  * of the CompositeDescription to get.
  *
  * @return CompositeDescription in this DimensionDescription
  * with the given id or NULL if no such
  * CompositeDescription exists.
  *
  * @see get(unsigned int n)
  * @see size()
  */
  virtual DimensionDescription* get (const std::string& sid);


  /**
  * Get a CompositeDescription from the DimensionDescription
  * based on its identifier.
  *
  * @param sid a string representing the identifier
  * of the CompositeDescription to get.
  *
  * @return CompositeDescription in this DimensionDescription
  * with the given id or NULL if no such
  * CompositeDescription exists.
  *
  * @see get(unsigned int n)
  * @see size()
  */
  virtual const DimensionDescription* get (const std::string& sid) const;

  virtual const std::string& getId() const;
  virtual int setId(const std::string& id);

  virtual const std::string& getName() const;
  virtual int setName(const std::string& name);

  /**
  * Removes the nth item from this DimensionDescription items and returns a pointer to
  * it.
  *
  * The caller owns the returned item and is responsible for deleting it.
  *
  * @param n the index of the item to remove
  *
  * @see size()
  */
  virtual DimensionDescription* remove (unsigned int n);


  /**
  * Removes item in this DimensionDescription items with the given identifier.
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
  virtual DimensionDescription* remove (const std::string& sid);


  /**
  * Get the ordinal position of this element in the containing object
  * (which in this case is the Model object).
  *
  * The ordering of elements in the XML form of %NUML is generally fixed
  * for most components in %NUML.  So, for example, the DimensionDescription in
  * a model is (in %NUML Level&nbsp;2 Version&nbsp;4) the sixth
  * NUMLList.  (However, it differs for different Levels and Versions of
  * NUML.)
  *
  * @return the ordinal position of the element with respect to its
  * siblings, or @c -1 (default) to indicate the position is not significant.
  */
  virtual int getElementPosition () const;


  virtual ~DimensionDescription();


protected:
  
  /**
  * @return the SBML object corresponding to next XMLToken in the
  * XMLInputStream or NULL if the token was not recognized.
  */
  virtual NMBase* createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);

  /**
   *
   * Subclasses should override this method to write their xmlns attriubutes
   * (if any) to the XMLOutputStream.
   *
   */
  virtual void writeXMLNS(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;

  std::string  mId;
  std::string  mName;

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

#endif /* DIMENSIONDESCRIPTION_H_ */
