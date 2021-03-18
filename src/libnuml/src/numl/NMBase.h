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
 * @class NMBase
 * @brief This is the base class for all numl objects
 */


#ifndef NMBase_h
#define NMBase_h


#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>
#include <sbml/util/List.h>
#include <numl/NUMLTypeCodes.h>
#include <numl/NUMLNamespaces.h>
#include <sbml/SyntaxChecker.h> //change from numl
#include <numl/NUMLVisitor.h>


#include <string>
#include <stdexcept>
#include <algorithm>

#include <numl/NUMLErrorLog.h>


#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLOutputStream.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLNamespaces.h>
#include <sbml/xml/XMLToken.h>



#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

class NUMLErrorLog;
//class NUMLVisitor;
class NUMLDocument;

class NUMLConstructorException : public std::invalid_argument
{
public:

  /* constructor */
  NUMLConstructorException ();

};


class LIBNUML_EXTERN NMBase
{
public:

  /**
   * Destroy this object.
   */
  virtual ~NMBase ();


  /**
   * Assignment operator for NMBase.
   */
  NMBase& operator=(const NMBase& orig);


  /**
   * Accepts the given NUMLVisitor for this instance of NMBase.
   *
   * @param v the NUMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept (NUMLVisitor& v) const = 0;


  /**
   * Creates and returns a deep copy of this NMBase object.
   * 
   * @return a (deep) copy of this NMBase object
   */
  virtual NMBase* clone () const = 0;


  /**
   * Returns the value of the "metaid" attribute of this object.
   * 
   * The optional attribute named "metaid", present on every major NUML
   * component type, is for supporting metadata annotations using RDF
   * (Resource Description Format). The attribute value has the data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, the XML
   * identifier type, which means each "metaid" value must be globally
   * unique within an NUML file.  (Importantly, this uniqueness criterion
   * applies across any attribute with type <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, not just the
   * "metaid" attribute used by NUML&mdash;something to be aware of if your
   * application-specific XML content inside the "annotation" subelement
   * happens to use <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>.)
   * The "metaid" value serves to identify a model
   * component for purposes such as referencing that component from
   * metadata placed within "annotation" subelements.
   *  
   * @return the metaid of this NUML object.
   *
   * @see isSetMetaId()
   * @see setMetaId(const std::string& metaid)
   */
  const std::string& getMetaId () const;


  /**
   * Returns the value of the "metaid" attribute of this object.
   * 
   * The optional attribute named "metaid", present on every major NUML
   * component type, is for supporting metadata annotations using RDF
   * (Resource Description Format). The attribute value has the data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, the XML
   * identifier type, which means each "metaid" value must be globally
   * unique within an NUML file.  (Importantly, this uniqueness criterion
   * applies across any attribute with type <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, not just the
   * "metaid" attribute used by NUML&mdash;something to be aware of if your
   * application-specific XML content inside the "annotation" subelement
   * happens to use <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>.)
   * The "metaid" value serves to identify a model
   * component for purposes such as referencing that component from
   * metadata placed within "annotation" subelements.
   *  
   * @return the metaid of this NUML object.
   *
   * @see isSetMetaId()
   * @see setMetaId(const std::string& metaid)
   */
  std::string& getMetaId ();


  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
   *
   * Returns the value of the "id" attribute of this object.
   * 
   * @return the id of this NUML object.
   */
  virtual const std::string& getId () const;


  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
   *
   * Returns the value of the "name" attribute of this object.
   * 
   * @return the name of this NUML object.
   */
  virtual const std::string& getName () const;
  /**
   * Returns the content of the "annotation" subelement of this object as a
   * character string.
   *
   * Whereas the NMBase "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from NMBase can have its own value for
   * "annotation".  The element's content type is XML type
   * <code>any</code>, allowing essentially arbitrary well-formed XML data
   * content.
   *
   * NUML places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the NUML specifications for more details.
   *
   * The annotations returned by this method will be in string form.
   *
   * @return the annotation string of this NUML object.
   *
   * @see getAnnotation()
   * @see isSetAnnotation()
   * @see setAnnotation(const XMLNode* annotation)
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const std::string& annotation)
   * @see unsetAnnotation()
   */
//TODO  std::string getAnnotationString ();


  /**
   * Returns a list of the XML Namespaces declared on this NUML document.
   * 
   * @return the XML Namespaces associated with this NUML object
   */
  virtual LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* getNamespaces() const ;


  /**
   * Returns the parent NUMLDocument object.
   *
   * LibNUML uses the class NUMLDocument as a top-level container for
   * storing NUML content and data associated with it (such as warnings and
   * error messages).  An NUML model in libNUML is contained inside an
   * NUMLDocument object.  NUMLDocument corresponds roughly to the class
   * <i>Sbrml</i> defined in the NUML Level&nbsp;2 specification, but it
   * does not have a direct correspondence in NUML Level&nbsp;1.  (But, it
   * is created by libNUML no matter whether the model is Level&nbsp;1 or
   * Level&nbsp;2.)
   *
   * This method allows the NUMLDocument for the current object to be
   * retrieved.
   * 
   * @return the parent NUMLDocument object of this NUML object.
   */
  const NUMLDocument* getNUMLDocument () const;


  /**
   * Returns the parent NUMLDocument object.
   *
   * LibNUML uses the class NUMLDocument as a top-level container for
   * storing NUML content and data associated with it (such as warnings and
   * error messages).  An NUML model in libNUML is contained inside an
   * NUMLDocument object.  NUMLDocument corresponds roughly to the class
   * <i>Sbrml</i> defined in the NUML Level&nbsp;2 specification, but it
   * does not have a direct correspondence in NUML Level&nbsp;1.  (But, it
   * is created by libNUML no matter whether the model is Level&nbsp;1 or
   * Level&nbsp;2.)
   *
   * This method allows the NUMLDocument for the current object to be
   * retrieved.
   * 
   * @return the parent NUMLDocument object of this NUML object.
   */
  NUMLDocument* getNUMLDocument ();

  /**
   * Returns the parent NUML object.
   *
   * This method is convenient when holding an object nested inside
   * other objects in an NUML model.  It allows direct access to the
   * <code>&lt;model&gt;</code> element containing it.
   * 
   * @return the parent NUML object of this NUML object.
   */
  NMBase* getParentNUMLObject();

  /**
     * Returns the ancestor NUML object that corresponds to the
     * given NUMLTypeCode_t.
     *
     * This function allows any object to determine its exact
     * location/function within a model. For example a StoichiometryMath
     * object has ancestors of type SpeciesReference,
     * ListOfProducts/ListOfReactants/ListOfModifiers, Reaction,
     * ListOfReactions and Model; any of which can be accessed via this
     * function.
     *
     * @param type the NUMLTypeCode_t of the ancestor to be returned.
     *
     * @return the ancestor NUML object of this NUML object, that
     * corresponds to the given NUMLTypeCode_t, NULL if none exists.
     */
   NMBase* getAncestorOfType(NUMLTypeCode_t type);


  /**
   * Returns the line number on which this object first appears in the XML
   * representation of the NUML document.
   * 
   * @return the line number of this NUML object.
   *
   * @see getColumn()
   */
  unsigned int getLine () const;


  /**
   * Returns the column number on which this object first appears in the XML
   * representation of the NUML document.
   * 
   * @return the column number of this NUML object.
   * 
   * @see getLine()
   */
  unsigned int getColumn () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * object's "metaid" attribute has been set.
   *
   * The optional attribute named "metaid", present on every major NUML
   * component type, is for supporting metadata annotations using RDF
   * (Resource Description Format). The attribute value has the data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, the XML
   * identifier type, which means each "metaid" value must be globally
   * unique within an NUML file.  (Importantly, this uniqueness criterion
   * applies across any attribute with type <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, not just the
   * "metaid" attribute used by NUML&mdash;something to be aware of if your
   * application-specific XML content inside the "annotation" subelement
   * happens to use <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>.)
   * The "metaid" value serves to identify a model
   * component for purposes such as referencing that component from
   * metadata placed within "annotation" subelements.
   *  
   * @return @c true if the "metaid" attribute of this NUML object has been
   * set, @c false otherwise.
   *
   * @see getMetaId()
   * @see setMetaId(const std::string& metaid)
   */
  bool isSetMetaId () const;



  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
   * Predicate returning @c true or @c false depending on whether this
   * object's "name" attribute has been set.
   *
   * @return @c true if the "name" attribute of this NUML object has been
   * set, @c false otherwise.
   */
  bool isSetName () const;

  /**
   * Sets the value of the "metaid" attribute of this object.
   *
   * The string @p metaid is copied.  The value of @p metaid must be an
   * identifier conforming to the syntax defined by the XML 1.0 data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">ID</a>.  Among other
   * things, this type requires that a value is unique among all the values
   * of type XML ID in an NUMLDocument.  Although NUML only uses <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a> for the "metaid"
   * attribute, callers should be careful if they use
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>'s in XML
   * portions of a model that are not defined by NUML, such as in the
   * application-specific content of the "annotation" subelement.
   *
   * @param metaid the identifier string to use as the value of the
   * "metaid" attribute
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBNUML_OPERATION_SUCCESS
   * @li LIBNUML_INVALID_ATTRIBUTE_VALUE
   * @li LIBNUML_UNEXPECTED_ATTRIBUTE
   * 
   * @see getMetaId()
   * @see isSetMetaId()
   */
  int setMetaId (const std::string& metaid);

    /*
     * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
     *
     * Sets the value of the "id" attribute of this NUML object to a copy
     * of @p id.
     *
     * The string @p sid is copied.  Note that NUML has strict requirements
     * for the syntax of identifiers.
     *
     * @param sid the string to use as the identifier of this object
     *
     * @return integer value indicating success/failure of the
     * function.  The possible values returned by this function are:
     * @li @link OperationReturnValues_t#LIBNUML_OPERATION_SUCCESS LIBNUML_OPERATION_SUCCESS @endlink
     * @li @link OperationReturnValues_t#LIBNUML_INVALID_ATTRIBUTE_VALUE LIBNUML_INVALID_ATTRIBUTE_VALUE @endlink
     */
     virtual int setId (const std::string& sid);



  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
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
 // TODO int setName (const std::string& name);

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
   * Sets the namespaces relevant of this NUML object.
   *
   * @param xmlns the namespaces to set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBNUML_OPERATION_SUCCESS
   */
  int setNamespaces(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* xmlns);


  /**
   * Unsets the value of the "metaid" attribute of this NUML object.
   *
   * The optional attribute named "metaid", present on every major NUML
   * component type, is for supporting metadata annotations using RDF
   * (Resource Description Format). The attribute value has the data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, the XML
   * identifier type, which means each "metaid" value must be globally
   * unique within an NUML file.  (Importantly, this uniqueness criterion
   * applies across any attribute with type <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, not just the
   * "metaid" attribute used by NUML&mdash;something to be aware of if your
   * application-specific XML content inside the "annotation" subelement
   * happens to use <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>.)
   * The "metaid" value serves to identify a model
   * component for purposes such as referencing that component from
   * metadata placed within "annotation" subelements.
   *  
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBNUML_OPERATION_SUCCESS
   * @li LIBNUML_OPERATION_FAILED
   */
  int unsetMetaId ();


#if 0
  /**
   * Unsets the value of the "id" attribute of this NUML object.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   *
   * @li LIBNUML_OPERATION_SUCCESS
   * @li LIBNUML_OPERATION_FAILED
   */
  int unsetId ();


  /**
   * Unsets the value of the "name" attribute of this NUML object.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   *
   * @li LIBNUML_OPERATION_SUCCESS
   * @li LIBNUML_OPERATION_FAILED
   */
  int unsetName ();
#endif

  /**
   * Returns the NUML Level of the overall NUML document.
   * 
   * @return the NUML level of this NUML object.
   * 
   * @see getVersion()
   */
  unsigned int getLevel () const;


  /**
   * Returns the Version within the NUML Level of the overall NUML document.
   * 
   * @return the NUML version of this NUML object.
   *
   * @see getLevel()
   */
  unsigned int getVersion () const;

  /**
     * Returns the libnuml type code for this object.
     *
     * This method may return the type code of this NUML object, or it may
     * return @link NUMLTypeCode_t#NUML_UNKNOWN NUML_UNKNOWN@endlink.  This
     * is because subclasses of NMBase are not required to implement this
     * method to return a type code.  This method is meant primarily for the
     * LibNUML C interface, in which class and subclass information is not
     * readily available.
     *
     * @return the @if clike #NUMLTypeCode_t value@else NUML object type code@endif@~
     * of this NUML object or
     * @link NUMLTypeCode_t#NUML_UNKNOWNNUML_UNKNOWN@endlink (the default).
     *
     * @see getElementName()
     * @see getPackageName()
     */
 //   virtual int getTypeCode () const;

  /**
   * Returns the libNUML type code for this object.
   * 
   * This method MAY return the typecode of this NUML object or it MAY
   * return NUML_UNKNOWN.  That is, subclasses of NMBase are not required to
   * implement this method to return a typecode.  This method is meant
   * primarily for the LibNUML C interface where class and subclass
   * information is not readily available.
   *
   * @return the #NUMLTypeCode_t value of this NUML object or NUML_UNKNOWN
   * (default).
   *
   * @see getElementName()
   */
  virtual NUMLTypeCode_t getTypeCode () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * object's level/version and namespace values correspond to a valid
   * NUML specification.
   *
   * The valid combinations of NUML Level, Version and Namespace as of this release
   * of libNUML are the following:
   * <ul>
   * <li> Level&nbsp;1 Version&nbsp;1 "http://www.numl.org/numl/level1"
   * </ul>
   *
   * @return @c true if the level, version and namespace values of this 
   * NUML object correspond to a valid set of values, @c false otherwise.
   */
  bool hasValidLevelVersionNamespaceCombination();

  
  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * NUML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
   */
  virtual const std::string& getElementName () const = 0;


  /**
   * Returns a string that consists of the partial NUML describing this
   * object.  This is primarily provided for testing and debugging
   * purposes.  It may be removed in a future version of libNUML.
   * 
   * @return the partial NUML that describes this NUML object.
   */
  char* toNUML ();


  /**
   * Reads (initializes) this NUML object by reading from XMLInputStream.
   */
  void read (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);


  /**
   * Writes (serializes) this NUML object by writing it to XMLOutputStream.
   */
  void write (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;


  /**
   * Subclasses should override this method to write out their contained
   * NUML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.  For example:
   *
   *   NMBase::writeElements(stream);
   *   mTuples.write(stream);
   *   mResultComponents.write(stream);
   *   ...
   */
  virtual void writeElements (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;


  /* function returns true if component has all the required
   * attributes
   * needs to be overloaded for each component
   */
  virtual bool hasRequiredAttributes() const ;


  /* function returns true if component has all the required
   * elements
   * needs to be overloaded for each component
   */
  virtual bool hasRequiredElements() const ;


  /* sets the NUMLnamespaces - internal use only*/
  void setNUMLNamespaces(NUMLNamespaces * numlns);


  /* gets the NUMLnamespaces - internal use only*/
  NUMLNamespaces * getNUMLNamespaces() const;

  void syncAnnotation();
  bool isSetNotes() const;
  bool isSetAnnotation() const;
  virtual int setAnnotation(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode* annotation);
  virtual int setAnnotation(const std::string& annotation);
  virtual int appendAnnotation(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode* annotation);
  virtual int appendAnnotation(const std::string& annotation);
  int removeTopLevelAnnotationElement(const std::string elementName,
                                      const std::string elementURI = "");
  int replaceTopLevelAnnotationElement(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode* annotation);
  int replaceTopLevelAnnotationElement(const std::string& annotation);
  int setNotes(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode* notes);
  int setNotes(const std::string& notes, bool addXHTMLMarkup = false);
  int appendNotes(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode* notes);
  int appendNotes(const std::string& notes);
  int unsetNotes();
  int unsetAnnotation();

  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode* getNotes() const;
  std::string getNotesString() const;
  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode* getAnnotation() const;
  std::string getAnnotationString() const;



protected:


  /**
   *
   * Subclasses should override this method to write their xmlns attriubutes
   * (if any) to the XMLOutputStream.
   *
   */
  virtual void writeXMLNS(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;


  /**
   * Only subclasses may create NMBase objects.
   *
   * @docnote The native C++ implementation of this method defines a
   * default argument value.  In the documentation generated for different
   * libNUML language bindings, you may or may not see corresponding
   * arguments in the method declarations.  For example, in Java, a default
   * argument is handled by declaring two separate methods, with one of
   * them having the argument and the other one lacking the argument.
   * However, the libNUML documentation will be @em identical for both
   * methods.  Consequently, if you are reading this and do not see an
   * argument even though one is described, please look for descriptions of
   * other variants of this method near where this one appears in the
   * documentation.
   */
  NMBase (const std::string& id = "", const std::string& name = "");

  
  /**
   * Creates a new NMBase object with the given sboTerm.
   * Only subclasses may create NMBase objects.
   */
  NMBase (unsigned int level, unsigned int version);


  /**
   * Creates a new NMBase object with the given NUMLNamespaces.
   * Only subclasses may create NMBase objects.
   */
  NMBase (NUMLNamespaces* numlns);


  /**
  * Copy constructor. Creates a copy of this NMBase object.
  */
  NMBase(const NMBase& orig);


  /**
   * Subclasses should override this method to create, store, and then
   * return an NUML object corresponding to the next XMLToken in the
   * XMLInputStream.
   *
   * @return the NUML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual NMBase* createObject (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);


  /**
   * Subclasses should override this method to read (and store) XHTML,
   * MathML, etc. directly from the XMLInputStream.
   *
   * @return true if the subclass read from the stream, false otherwise.
   */
  virtual bool readOtherXML (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);


  /**
   * The NUML XML Schema is written such that the order of child elements
   * is significant.  LibNUML can read elements out of order.  If you
   * override this method to indicate the ordinal position of element with
   * respect to its siblings, libNUML will log an error if the element is
   * read out of order.
   *
   * @return the ordinal position of the element with respect to its
   * siblings or -1 (default) to indicate the position is not significant.
   */
  virtual int getElementPosition () const;


  /**
   * @return the NUMLErrorLog used to log errors during while reading and
   * validating NUML.
   */
  NUMLErrorLog* getErrorLog ();


  /**
   * Convenience method for easily logging problems from within method
   * implementations.
   *
   * This is essentially a short form of getErrorLog()->logError(...)
   *
   * @docnote The native C++ implementation of this method defines a
   * default argument value.  In the documentation generated for different
   * libNUML language bindings, you may or may not see corresponding
   * arguments in the method declarations.  For example, in Java, a default
   * argument is handled by declaring two separate methods, with one of
   * them having the argument and the other one lacking the argument.
   * However, the libNUML documentation will be @em identical for both
   * methods.  Consequently, if you are reading this and do not see an
   * argument even though one is described, please look for descriptions of
   * other variants of this method near where this one appears in the
   * documentation.
   */
  void logError (  unsigned int       id
                 , const unsigned int level   = 2
                 , const unsigned int version = 3
                 , const std::string& details = "" );


  /**
   * Helper to log a common type of error.
   */
  void logUnknownAttribute( std::string attribute,
          const unsigned int level,
          const unsigned int version,
          const std::string element );


  /**
   * Helper to log a common type of error.
   */
  void logUnknownElement( const std::string element,
        const unsigned int level,
        const unsigned int version );

 
  /**
   * Helper to log a common type of error.
   */
  void logEmptyString( std::string attribute,
          const unsigned int level,
          const unsigned int version,
          std::string element);


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


  /**
   * Checks that NUML element has been read in the proper order.  If object
   * is not in the expected position, an error is logged.
   */
  void checkOrderAndLogError (NMBase* object, int expected);


  /**
   * Checks that an NUML NUMLList element has been populated.
   * If a NUMLList element has been declared with no elements,
   * an error is logged.
   */
  void checkNUMLListPopulated(NMBase* object);


#if 0
  /**
   * Checks the syntax of the unit attribute.
   * The syntax of an unit is of type UnitSId which is defined as:
   *
   *  - letter ::= 'a'..'z','A'..'Z'
   *  - digit  ::= '0'..'9'
   *  - idChar ::= letter | digit | '_'
   *  - UnitSId    ::= ( letter | '_' ) idChar*
   *
   * If the syntax of the unit attribute of this object is incorrect, 
   * an error is logged
   *
   * @docnote The native C++ implementation of this method defines a
   * default argument value.  In the documentation generated for different
   * libNUML language bindings, you may or may not see corresponding
   * arguments in the method declarations.  For example, in Java, a default
   * argument is handled by declaring two separate methods, with one of
   * them having the argument and the other one lacking the argument.
   * However, the libNUML documentation will be @em identical for both
   * methods.  Consequently, if you are reading this and do not see an
   * argument even though one is described, please look for descriptions of
   * other variants of this method near where this one appears in the
   * documentation.
   */
  void checkUnitSyntax(unsigned int flag = 0);
#endif

  /**
   * Checks that the given default namespace in the given element is valid.
   * If the given default namespace is not valid, an error is logged.
   */
  void checkDefaultNamespace(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* xmlns, const std::string& elementName);


  /**
   * Checks that the XHTML is valid.
   * If the xhtml does not conform to the specification of valid xhtml within
   * an numl document, an error is logged.
  */
  void checkXHTML(const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode *);

  /**
  * Checks the annotation does not declare an numl namespace.
  * If the annotation declares an numl namespace an error is logged.
  */
  void checkAnnotation();

  std::string mMetaId;
  std::string mId;
  std::string mName;

  NUMLDocument* mNUML;

  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode* mNotes;
  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode* mAnnotation;

  //int mObjectLevel;
  //int mObjectVersion;

  NUMLNamespaces *mNUMLNamespaces;


  unsigned int mLine;
  unsigned int mColumn;

  int mDept;

  /* store the parent NUML object */
  NMBase* mParentNUMLObject;

  /* flag that allows object to know its been deleted
   * for OS where the memory is still readable after a delete
   */
  bool mHasBeenDeleted;

  std::string mEmptyString;

  

private:
  
  /**
   * Stores the location (line and column) and any XML namespaces (for
   * roundtripping) declared on this NUML (XML) element.
   */
  void setNMBaseFields (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLToken& element);


  /**
   * @return true if read an <annotation> element from the stream
   */
  bool readAnnotation (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);


  /**
   * @return true if read a <notes> element from the stream
   */
  bool readNotes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream& stream);

  bool getHasBeenDeleted();


};

LIBNUML_CPP_NAMESPACE_END

#endif  /* __cplusplus */



#ifndef SWIG

LIBNUML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/*-----------------------------------------------------------------------------
 * See the .cpp file for the documentation of the following functions.
 *---------------------------------------------------------------------------*/


LIBNUML_EXTERN
const char *
NMBase_getMetaId (NMBase_t *sb);


LIBNUML_EXTERN
const NUMLDocument_t *
NMBase_getNUMLDocument (NMBase_t *sb);


LIBNUML_EXTERN
const NMBase_t *
NMBase_getParentNUMLObject (NMBase_t *sb);


LIBNUML_EXTERN
const NMBase_t *
NMBase_getAncestorOfType (NMBase_t *sb, NUMLTypeCode_t type);


LIBNUML_EXTERN
unsigned int
NMBase_getLine (const NMBase_t *sb);


LIBNUML_EXTERN
unsigned int
NMBase_getColumn (const NMBase_t *sb);


LIBNUML_EXTERN
LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode_t *
NMBase_getNotes (NMBase_t *sb);


LIBNUML_EXTERN
char*
NMBase_getNotesString (NMBase_t *sb);


LIBNUML_EXTERN
LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode_t *
NMBase_getAnnotation (NMBase_t *sb);


LIBNUML_EXTERN
char*
NMBase_getAnnotationString (NMBase_t *sb);


LIBNUML_EXTERN
int
NMBase_isSetMetaId (const NMBase_t *sb);


LIBNUML_EXTERN
int
NMBase_isSetNotes (const NMBase_t *sb);


LIBNUML_EXTERN
int
NMBase_isSetAnnotation (const NMBase_t *sb);


LIBNUML_EXTERN
int
NMBase_setMetaId (NMBase_t *sb, const char *metaid);


LIBNUML_EXTERN
int
NMBase_setNamespaces (NMBase_t *sb, LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces_t *xmlns);


LIBNUML_EXTERN
int
NMBase_setNotes (NMBase_t *sb, LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode_t *notes);


LIBNUML_EXTERN
int
NMBase_setNotesString (NMBase_t *sb, char *notes);


LIBNUML_EXTERN
int
NMBase_appendNotes (NMBase_t *sb, LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode_t *notes);


LIBNUML_EXTERN
int
NMBase_appendNotesString (NMBase_t *sb, char *notes);


LIBNUML_EXTERN
int
NMBase_setAnnotation (NMBase_t *sb, LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode_t *annotation);


LIBNUML_EXTERN
int
NMBase_setAnnotationString (NMBase_t *sb, char *annotation);


LIBNUML_EXTERN
int
NMBase_appendAnnotation (NMBase_t *sb, LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNode_t *annotation);


LIBNUML_EXTERN
int
NMBase_appendAnnotationString (NMBase_t *sb, char *annotation);


LIBNUML_EXTERN
int
NMBase_unsetMetaId (NMBase_t *sb);

LIBNUML_EXTERN
unsigned int
NMBase_getLevel (const NMBase_t *sb);


LIBNUML_EXTERN
unsigned int
NMBase_getVersion (const NMBase_t *sb);


LIBNUML_EXTERN
NUMLTypeCode_t
NMBase_getTypeCode (const NMBase_t *sb);


LIBNUML_EXTERN
const char *
NMBase_getElementName (const NMBase_t *sb);


LIBNUML_EXTERN
int
NMBase_hasValidLevelVersionNamespaceCombination(NMBase_t *sb);


END_C_DECLS
LIBNUML_CPP_NAMESPACE_END

#endif  /* !SWIG   */
#endif  /* NMBase_h */
