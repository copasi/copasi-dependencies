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
 * @class NUMLDocument
 * @brief This represents the numl document that contains all information
 */


#ifndef NUMLDocument_h
#define NUMLDocument_h


#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>

#include <numl/NUMLError.h>
#include <numl/NUMLErrorLog.h>
#include <numl/NMBase.h>

#include <numl/OntologyTerm.h>
#include <numl/ResultComponent.h>




#include <string>
#include <algorithm>
#include <iosfwd>
#include <time.h>

#ifdef __cplusplus


LIBNUML_CPP_NAMESPACE_BEGIN

class OntologyTerm;
class ResultComponent;
class NUMLVisitor;


/* Internal constants for setting/unsetting particular consistency checks. */

#ifndef IdCheckON
#define IdCheckON         0x01;
#endif
#ifndef IdCheckOFF
#define IdCheckOFF        0xfe;
#endif
#ifndef NUMLCheckON
#define NUMLCheckON       0x02;
#endif
#ifndef NUMLCheckOFF
#define NUMLCheckOFF      0xfd;
#endif
#ifndef SBOCheckON
#define SBOCheckON        0x04;
#endif
#ifndef SBOCheckOFF
#define SBOCheckOFF       0xfb;
#endif
#ifndef MathCheckON
#define MathCheckON       0x08;
#endif
#ifndef MathCheckOFF
#define MathCheckOFF      0xf7;
#endif
#ifndef UnitsCheckON
#define UnitsCheckON      0x10;
#endif
#ifndef UnitsCheckOFF
#define UnitsCheckOFF     0xef;
#endif
#ifndef OverdeterCheckON
#define OverdeterCheckON  0x20;
#endif
#ifndef OverdeterCheckOFF
#define OverdeterCheckOFF 0xdf;
#endif
#ifndef PracticeCheckON
#define PracticeCheckON   0x40;
#endif
#ifndef PracticeCheckOFF
#define PracticeCheckOFF  0xbf;
#endif
#ifndef AllChecksON
#define AllChecksON       0x7f;
#endif



class LIBNUML_EXTERN NUMLDocument: public NMBase
{
public:

    /**
   * The default NUML Level of new NUMLDocument objects.
   *
   * This "default level" corresponds to the most recent NUML specification
   * Level available at the time this version of libNUML was released.  For
   * this copy of libNUML, the value is <code>2</code>.  The default Level
   * is used by NUMLDocument if no Level is explicitly specified at the
   * time of the construction of an NUMLDocument instance.
   *
   * @return an integer indicating the most recent NUML specification Level
   *
   * @see getDefaultVersion()
   */
    static unsigned int getDefaultLevel ();


    /**
   * The default Version of new NUMLDocument objects.
   *
   * This "default version" corresponds to the most recent NUML Version
   * within the most recent Level of NUML available at the time this
   * version of libNUML was released.  For this copy of libNUML, the value
   * is <code>4</code>.  The default Version is used by NUMLDocument if no
   * Version is explicitly specified at the time of the construction of an
   * NUMLDocument instance.
   *
   * @return an integer indicating the most recent NUML specification
   * Version
   *
   * @see getDefaultLevel()
   */
    static unsigned int getDefaultVersion ();


    /**
   * Creates a new NUMLDocument, optionally with given values for the NUML
   * Level and Version.
   *
   * If <em>both</em> the NUML Level and Version attributes are not
   * specified, the NUML document is treated as having the latest Level and
   * Version of NUML as determined by NUMLDocument::getDefaultLevel() and
   * NUMLDocument::getDefaultVersion(); <em>however</em>, the NUMLDocument
   * object is otherwise left blank.  In particular, the blank NUMLDocument
   * object has no associated XML attributes, including (but not limited
   * to) an XML namespace declaration.  The XML namespace declaration is
   * not added until the model is written out, <em>or</em> the method
   * @if clike NUMLDocument::setLevelAndVersion() @endif@if java NUMLDocument::setLevelAndVersion(long lev, long ver) @endif is called.  This may be important to keep in mind
   * if an application needs to add additional XML namespace declarations
   * on the <code>&lt;numl&gt;</code> element.  Application writers should
   * either provide values for @p level and @p version on the call to this
   * constructor, or else call @if clike NUMLDocument::setLevelAndVersion() @endif@if java NUMLDocument::setLevelAndVersion(long lev, long ver) @endif shortly after creating
   * the NUMLDocument object.
   *
   * @param level an integer for the NUML Level
   *
   * @param version an integer for the Version within the NUML Level
   *
   * @if clike @see NUMLDocument::setLevelAndVersion() @endif@if java @see NUMLDocument::setLevelAndVersion(long lev, long ver) @endif
   * @see getDefaultLevel()
   * @see getDefaultVersion()
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
    NUMLDocument (unsigned int level = 0, unsigned int version = 0);


    /**
   * Destroys this NUMLDocument.
   */
    virtual ~NUMLDocument ();


    /**
   * Copy constructor; creates a copy of this NUMLDocument.
   */
    NUMLDocument (const NUMLDocument& rhs);


    /**
   * Accepts the given NUMLVisitor for this instance of NUMLDocument.
   *
   * @param v the NUMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
    virtual bool accept (NUMLVisitor& v) const;


    /**
   * Creates and returns a deep copy of this NUMLDocument.
   *
   * @return a (deep) copy of this NUMLDocument.
   */
    virtual NUMLDocument* clone () const;

    /**
     * Get a the number of ontologyTerm objects in this NUMLDocument.
     *
     * @return the number of ontologyTerms in the NUMLDocument.
     *
     */
    unsigned int getNumOntologyTerms () const;

    /**
         * Returns the OntologyTerms object stored in this NUMLDocument.
         *
         * It is important to note that this method <em>does not create</em> a
         * OntologyTerm instance.  The model in the NUMLDocument must have been created
         * at some prior time, for example using NUMLDocument::createOntologyTerm()
         * or @if clike NUMLDocument::setOntologyTerm() @endif@if java NUMLDocument::setOntologyTerm(OntologyTerm m) @endif.
         * This method returns NULL if a model does not yet exist.
         *
         * @return the OntologyTerm contained in this NUMLDocument.
         *
         * @see createOntologyTerm()
         */
    OntologyTerms* getOntologyTerms ();


    /**
     * Returns the OntologyTerms object stored in this NUMLDocument.
     *
     * It is important to note that this method <em>does not create</em> a
     * OntologyTerm instance.  The model in the NUMLDocument must have been created
     * at some prior time, for example using NUMLDocument::createOntologyTerm()
     * or @if clike NUMLDocument::setOntologyTerm() @endif@if java NUMLDocument::setOntologyTerm(OntologyTerm m) @endif.
     * This method returns NULL if a model does not yet exist.
     *
     * @return the OntologyTerm contained in this NUMLDocument.
     *
     * @see createOntologyTerm()
     */
    const OntologyTerms* getOntologyTerms () const;

    /**
     * Returns the ResultComponents object stored in this NUMLDocument.
     *
     * It is important to note that this method <em>does not create</em> a
     * ResultComponent instance.  The model in the NUMLDocument must have been created
     * at some prior time, for example using NUMLDocument::createResultComponent()
     * or @if clike NUMLDocument::setResultComponent() @endif@if java NUMLDocument::setResultComponent(ResultComponent m) @endif.
     * This method returns NULL if a model does not yet exist.
     *
     * @return the ResultComponent contained in this NUMLDocument.
     *
     * @see createResultComponent()
     */

    ResultComponents* getResultComponents ();

    /**
        * Returns the ResultComponents object stored in this NUMLDocument.
        *
        * It is important to note that this method <em>does not create</em> a
        * ResultComponent instance.  The model in the NUMLDocument must have been created
        * at some prior time, for example using NUMLDocument::createResultComponent()
        * or @if clike NUMLDocument::setResultComponent() @endif@if java NUMLDocument::setResultComponent(ResultComponent m) @endif.
        * This method returns NULL if a model does not yet exist.
        *
        * @return the ResultComponent contained in this NUMLDocument.
        *
        * @see createResultComponent()
        */
    const ResultComponents* getResultComponents () const;

    /**
   * Get a the number of resultComponent objects in this NUMLDocument.
   *
   * @return the number of resultComponents in the NUMLDocument.
   *
   */
    unsigned int getNumResultComponents() const;

    /**
     * @return the reult component with given index
     */
    ResultComponent* getResultComponent (unsigned int index);

    /**
   * Sets the NUML Level and Version of this NUMLDocument instance,
   * attempting to convert the model as needed.
   *
   */
    bool setLevelAndVersion (unsigned int level, unsigned int version,
                             bool strict = true);

    OntologyTerm* createOntologyTerm ();

    ResultComponent* createResultComponent ();

    // OntologyTerm* createOntologyTerm (const std::string& sid = ""); // will be removed later


    /**
  * Returns the nth error or warning encountered during parsing,
  * consistency checking, or attempted translation of this model.
  *
  * Callers can use method XMLError::getSeverity() on the result to assess
   * the severity of the problem.  The possible severity levels range from
   * informational messages to fatal errors.
   *
   * @return the error or warning indexed by integer @p n, or return @c
   * NULL if <code>n &gt; (getNumErrors() - 1)</code>.
   *
   * @param n the integer index of the error sought.
   *
   * @see NUMLDocument::getNumErrors()
   */
    const NUMLError* getError (unsigned int n) const;


    /**
   * Returns the number of errors or warnings mencountered during parsing,
   * consistency checking, or attempted translation of this model.
   *
   * @return the number of errors or warnings encountered
   */
    unsigned int getNumErrors () const;


    /**
   * Prints to the given output stream all the errors or warnings
   * encountered during parsing, consistency checking, or attempted
   * translation of this model.
   *
   * If no errors have occurred, i.e., <code>getNumErrors() == 0</code>, no
   * output will be sent to the stream.
   *
   * The format of the output is:
   * @verbatim
   N error(s):
     line NNN: (id) message
@endverbatim
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
    void printErrors (std::ostream& stream = std::cerr) const;





    /*
     * Sets the parent NUML object of this NUML object.
     *
     * @param sb the NUML object to use
     */
    virtual void setParentNUMLObject (NMBase* sb);


    /**
   * No-op; it is provided for consistency with the method available on
   * other libNUML object classes but has no effect on NUMLDocument.
   */
    virtual void setNUMLDocument (NUMLDocument* d);


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
   * @see NUMLDocument::getElementName()
   */
    virtual NUMLTypeCode_t getTypeCode () const;


    /**
   * Returns the XML element name of this object, which for NUMLDocument,
   * is always @c "numl".
   *
   * @return the name of this element, i.e., @c "numl".
   */
    virtual const std::string& getElementName () const;


    /**
   * Returns the list of errors or warnings logged during parsing,
   * consistency checking, or attempted translation of this model.
   *
   * @return the NUMLErrorLog used for this NUMLDocument
   *
   * @see NUMLDocument::getNumErrors()
   */
    NUMLErrorLog* getErrorLog ();


    /**
   * Returns a list of XML Namespaces associated with the XML content
   * of this NUML document.
   *
   * @return the XML Namespaces associated with this NUML object
   */
    virtual LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces* getNamespaces() const;



    /**
   * @return the ordinal position of the element with respect to its
   * siblings or -1 (default) to indicate the position is not significant.
   */
    int getElementPosition () const;


    /**
   * Subclasses should override this method to write out their contained
   * NUML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.
   */
    virtual void writeElements (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;


protected:

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
    virtual
    void readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLAttributes& attributes);


    /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parents implementation
   * of this method as well.
   */
    virtual void writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream) const;

    /*
   * Predicate returning true if the errors encountered are not ignorable.
   */
    bool conversion_errors(unsigned int errors);

    /*
   * Predicate returning true if model has strict unit consistency.
   */
    bool hasStrictUnits();

    /*
   * Predicate returning true if model has strict sbo consistency.
   */
    bool hasStrictSBO();


    int mLevel;
    int mVersion;

    OntologyTerms mOntologyTerms;
    ResultComponents mResultComponents;


    NUMLErrorLog mErrorLog;

    unsigned char mApplicableValidators;
    unsigned char mApplicableValidatorsForConversion;


    friend class NMBase;
    friend class NUMLReader;

};

LIBNUML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


#ifndef SWIG

#include <stdio.h>

LIBNUML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/*-----------------------------------------------------------------------------
 * See the .cpp file for the documentation of the following functions.
 *---------------------------------------------------------------------------*/


LIBNUML_EXTERN
NUMLDocument_t *
NUMLDocument_create (void);


LIBNUML_EXTERN
NUMLDocument_t *
NUMLDocument_createWithLevelAndVersion (unsigned int level, unsigned int version);


LIBNUML_EXTERN
void
NUMLDocument_free (NUMLDocument_t *d);


LIBNUML_EXTERN
NUMLDocument_t *
NUMLDocument_clone (const NUMLDocument_t *d);


LIBNUML_EXTERN
unsigned int
NUMLDocument_getLevel (const NUMLDocument_t *d);


LIBNUML_EXTERN
unsigned int
NUMLDocument_getVersion (const NUMLDocument_t *d);


LIBNUML_EXTERN
int
NUMLDocument_setLevelAndVersion (  NUMLDocument_t *d
                                   , unsigned int    level
                                   , unsigned int    version );


LIBNUML_EXTERN
int
NUMLDocument_setLevelAndVersionStrict (  NUMLDocument_t *d
                                         , unsigned int    level
                                         , unsigned int    version );

LIBNUML_EXTERN
void
NUMLDocument_setConsistencyChecks(NUMLDocument_t *d,
                                  NUMLErrorCategory_t category,
                                  int apply);

LIBNUML_EXTERN
void
NUMLDocument_setConsistencyChecksForConversion(NUMLDocument_t *d,
                                               NUMLErrorCategory_t category,
                                               int apply);

LIBNUML_EXTERN
unsigned int
NUMLDocument_checkConsistency (NUMLDocument_t *d);


LIBNUML_EXTERN
unsigned int
NUMLDocument_checkInternalConsistency (NUMLDocument_t *d);


LIBNUML_EXTERN
unsigned int
NUMLDocument_checkL1Compatibility (NUMLDocument_t *d);


LIBNUML_EXTERN
unsigned int
NUMLDocument_checkL2v1Compatibility (NUMLDocument_t *d);


LIBNUML_EXTERN
unsigned int
NUMLDocument_checkL2v2Compatibility (NUMLDocument_t *d);


LIBNUML_EXTERN
unsigned int
NUMLDocument_checkL2v3Compatibility (NUMLDocument_t *d);


LIBNUML_EXTERN
unsigned int
NUMLDocument_checkL2v4Compatibility (NUMLDocument_t *d);


LIBNUML_EXTERN
const NUMLError_t *
NUMLDocument_getError (NUMLDocument_t *d, unsigned int n);


LIBNUML_EXTERN
unsigned int
NUMLDocument_getNumErrors (const NUMLDocument_t *d);


LIBNUML_EXTERN
void
NUMLDocument_printErrors (NUMLDocument_t *d, FILE *stream);


LIBNUML_EXTERN
unsigned int
NUMLDocument_getDefaultLevel (void);


LIBNUML_EXTERN
unsigned int
NUMLDocument_getDefaultVersion (void);

LIBNUML_EXTERN
const LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces_t *
NUMLDocument_getNamespaces(NUMLDocument_t *d);

END_C_DECLS
LIBNUML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* NUMLDocument_h */
