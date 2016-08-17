/**
 * \file    local.i
 * \brief   Java-specific SWIG directives for wrapping libNUML API
*
* Begin svn Header
* $Rev$:	Revision of last commit
* $Author$:	Author of last commit
* $Date$:	Date of last commit
* $HeadURL$
* $Id$
* End svn Header
* ***************************************************************************
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
* The original code was initially developed by:
*
* 	SBML Team
* 	Control and Dynamical Systems, MC 107-81
* 	California Institute of Technology
* 	Pasadena, CA, 91125, USA
*
*  	http://www.sbml.org
* 	mailto:sbml-team@caltech.edu
*
* Adapted for NUML by:
*
* 	NUML Team
* 	Computational Systems Biology Group
* 	School of Computer Science
* 	The University of Manchester
* 	Manchester, M1 7DN UK
* 	mailto:josephodada@gmail.com
*
*   Contributor(s):
*   Joseph O. Dada, The University of Manchester - initial LIBNUML API and implementation
* ***************************************************************************
**/



//%include "javadoc.i"
%include "std_string.i"

%{
#include <iostream>
#include <fstream>
#include <sstream>
%}


/**
 * Renames *::clone() to *::cloneObject().
 * In JDK 1.4.2, libnuml's *::clone() methods can't override 
 * "Object Java.lang.Object.clone()" because JDK 1.4.2 doesn't
 * allow override with different return type. 
 *
 * (2008-12-07)
 * Currently, JDK 1.5 or later, which allows a covariant return type,
 * is required for libNUML Java binding, and thus we don't have to use
 * this rename directive. However, this directive is still enabled for 
 * compatiblity.
 */

%rename(cloneObject) *::clone;

/**
 * Wraps covariant return types of ::clone functions.
 *
 * Currently, SWIG doesn't allow a covariant return type although
 * JDK 1.5 or later supports it.
 * Thus, the following directives are required to enable the 
 * covariant return type. 
 */

#pragma SWIG nowarn=822

%define COVARIANT_RTYPE_CLONE(_CNAME_)
%typemap(jstype) _CNAME_* _CNAME_::clone  "_CNAME_"
%enddef

COVARIANT_RTYPE_CLONE(OntologyTerms)
COVARIANT_RTYPE_CLONE(OntologyTerm)
COVARIANT_RTYPE_CLONE(ResultComponents)
COVARIANT_RTYPE_CLONE(ResultComponent)
COVARIANT_RTYPE_CLONE(DimensionDescription)
COVARIANT_RTYPE_CLONE(Dimension)
COVARIANT_RTYPE_CLONE(CompositeDescription)
COVARIANT_RTYPE_CLONE(CompositeValue)
COVARIANT_RTYPE_CLONE(TupleDescription)
COVARIANT_RTYPE_CLONE(Tuple)
COVARIANT_RTYPE_CLONE(AtomicDescription)
COVARIANT_RTYPE_CLONE(AtomicValue)
COVARIANT_RTYPE_CLONE(NUMLList)
COVARIANT_RTYPE_CLONE(NUMLDocument)

/**
 *
 * Wraps covariant return types of NUMLList::get functions.
 *
 */

%typemap(jstype) OntologyTerm* OntologyTerms::get    "OntologyTerm"
%typemap(jstype) OntologyTerm* OntologyTerms::remove "OntologyTerm"
%typemap(jstype) ResultComponent* ResultComponents::get    "ResultComponent"
%typemap(jstype) ResultComponent* ResultComponents::remove "ResultComponent"
%typemap(jstype) DimensionDescription* ResultComponent::get    "DimensionDescription"
%typemap(jstype) DimensionDescription* ResultComponent::remove "DimensionDescription"
%typemap(jstype) Dimension* ResultComponent::get    "Dimension"
%typemap(jstype) Dimension* ResultComponent::remove "Dimension"

%typemap(jstype) CompositeDescription* DimensionDescritpion::get    "CompositeDescription"
%typemap(jstype) CompositeDescription* DimensionDescription::remove "CompositeDescription"
%typemap(jstype) CompositeValue* Dimension::get    "CompositeValue"
%typemap(jstype) CompositeValue* Dimension::remove "CompositeValue"

%typemap(jstype) CompositeValue* CompositeValue::get    "CompositeValue"
%typemap(jstype) CompositeValue* CompositeValue::remove "CompositeValue"

%typemap(jstype) AtomicDescription* TupleDescription::get    "AtomicDescription"
%typemap(jstype) AtomicDescription* TupleDescription::remove "AtomicDescription"
%typemap(jstype) AtomicValue* Tuple::get    "AtomicValue"
%typemap(jstype) AtomicValue* Tuple::remove "AtomicValue"


/**
 * Wraps the NUMLConstructorException
 *
 * The NUMLConstructorException (C++ class) is wrapped as the 
 * NUMLConsturctorException (Java class) which is derived from
 * the built-in IllegalArgumentException class which is a subclass
 * of RunTimeException.
 *
 * For example, the exception can be catched in Java code as follows:
 *
 * ---------------------------------------------
 *  try
 *  {
 *    Model s = new Model(level,version);
 *  }
 *  catch (NUMLConstructorException e)
 *  {
 *     String errmsg = e.getMessage();
 *  }
 * ---------------------------------------------
 */

%typemap(javabase) NUMLConstructorException "java.lang.IllegalArgumentException";
%typemap(javacode) NUMLConstructorException 
%{
  protected NUMLConstructorException(long cPtr, boolean cMemoryOwn, String v)
  {
    super(v);
    swigCMemOwn = cMemoryOwn;
    swigCPtr    = cPtr;
  }

  public NUMLConstructorException(String v)
  {
    this(libnumlJNI.new_NUMLConstructorException(), true, v);
  }
%}

%define NUMLCONSTRUCTOR_EXCEPTION(NUMBASE_CLASS_NAME)
%javaexception("org.numl.libnuml.NUMLConstructorException") NUMBASE_CLASS_NAME
%{
  try {
    $action
  }
  catch (const NUMLConstructorException &e) {
    jenv->ExceptionClear();
    jclass clazz = jenv->FindClass("org/numl/libnuml/NUMLConstructorException");
    if (clazz)
      jenv->ThrowNew(clazz, e.what());
    return $null;
  }
%}
%enddef


NUMLCONSTRUCTOR_EXCEPTION(OntologyTerms)
NUMLCONSTRUCTOR_EXCEPTION(ResultComponents)
NUMLCONSTRUCTOR_EXCEPTION(ResultComponent)
NUMLCONSTRUCTOR_EXCEPTION(DimensionDescription)
NUMLCONSTRUCTOR_EXCEPTION(Dimension)
NUMLCONSTRUCTOR_EXCEPTION(CompositeDescription)
NUMLCONSTRUCTOR_EXCEPTION(CompositeValue)
NUMLCONSTRUCTOR_EXCEPTION(TupleDescription)
NUMLCONSTRUCTOR_EXCEPTION(Tuple)
NUMLCONSTRUCTOR_EXCEPTION(AtomicDescription)
NUMLCONSTRUCTOR_EXCEPTION(AtomicValue)

/**
 * Ignores XMLToken::clone() in order to use XMLNode::clone().
 * (XMLNode is a derived class of XMLToken)
 * In JDK 1.4.2, "XMLNode XMLNode::clone()" can't override 
 * "XMLToken XMLToken::clone()" because JDK 1.4.2 doesn't
 * allow override with different return type.
 */

//%javamethodmodifiers       XMLToken::clone "private"

/**
 * Ignores XMLErrorLog::getError(unsigned int) in order to use
 * NUMLErrorLog::getError(unsigned int).
 * (XMLErrorLog is a derived class of NUMLErrorLog)
 * In JDK 1.4.2, "NUMLError* NUMLErrorLog::getError(unsigned int)"
 * can't override "XMLError* XMLErrorLog::getError(unsigned int)"
 * due to the above mentioned reason.
 */

//%javamethodmodifiers       XMLErrorLog::getError "private"

/**
 * Turns off object destruction.  For testing purposes only.
 *
 * FIXME: Disable for the final 3.0 release.
 */
/*
%typemap (javafinalize) SWIGTYPE %{ %}
*/


/**
 * Make most libNUML constants (e.g. NUMLTypecodes) Java compile-time
 * constants so they may be used in switch statements.
 */
%include "enumsimple.swg"
%javaconst(1);


/**
 * A bug in Swig prevents these four ASTNode constants being generated
 * as Java compile-time constants.  Swig does not parse the following
 * enum correctly:
 *
 *   typedef enum
 *   {
 *       AST_PLUS    = '+'
 *     , AST_MINUS   = '-'
 *     , AST_TIMES   = '*'
 *     , AST_DIVIDE  = '/'
 *     , AST_POWER   = '^'
 *
 *
 * The generated Java code does not like the tick marks (').  To fix
 * this, we need to be explicit about (and duplicate) the value of
 * the constants here.
 */

%javaconstvalue("'+'") AST_PLUS;
%javaconstvalue("'-'") AST_MINUS;
%javaconstvalue("'*'") AST_TIMES;
%javaconstvalue("'/'") AST_DIVIDE;
%javaconstvalue("'^'") AST_POWER;


/**
 * @return the most specific Java object possible for the given NMBase
 * object.
 */
%pragma(java) modulecode =
%{
  /**
   * Internal method.  This is not actually deprecated, but we
   * cheat and use @deprecated to hide this in the documentation
   * produced by Javadoc:
   *
   * @deprecated
   */
  public static NMBase DowncastNMBase(long cPtr, boolean owner)
  {
    if (cPtr == 0) return null;

    NMBase sb = new NMBase(cPtr,false);
    switch( sb.getTypeCode() )
    {
      case libnumlConstants.NUML_ONTOLOGYTERMS:
        return new OntologyTerms(cPtr, owner);

      case libnumlConstants.NUML_ONTOLOGYTERM:
        return new OntologyTerm(cPtr, owner);    

      case libnumlConstants.NUML_DOCUMENT:
        return new NUMLDocument(cPtr, owner);      

      case libnumlConstants.NUML_RESULTCOMPONENT:
        return new ResultComponent(cPtr, owner);
        
      case libnumlConstants.NUML_RESULTCOMPONENTS:
        return new ResultComponents(cPtr, owner);
   
      case libnumlConstants.NUML_ATOMICVALUE:
        return new AtomicValue(cPtr, owner);

      case libnumlConstants.NUML_NUMLLIST:
	String name = sb.getElementName();
        if(name.equals("NUMLList")){
          return new NUMLList(cPtr, owner);
        }
	else if(name.equals("resultComponent")){
          return new ResultComponent(cPtr, owner);
	}	
	else if(name.equals("dimension")){
          return new Dimension(cPtr, owner);
	}
        else if(name.equals("dimensionDescription")){
          return new DimensionDescription(cPtr, owner);
        }        
        else if(name.equals("compositeDescription")){
          return new CompositeDescription(cPtr, owner);
        }
	else if(name.equals("compositeValue")){
          return new CompositeValue(cPtr, owner);
	}
        else if(name.equals("tupleDescription")){
          return new TupleDescription(cPtr, owner);
        }
        else if(name.equals("tuple")){
          return new Tuple(cPtr, owner);
        }        
        else if(name.equals("atomicDescription")){
          return new AtomicDescription(cPtr, owner);
        }
        else if(name.equals("atomicValue")){
          return new AtomicValue(cPtr, owner);
        }
        return new NUMLList(cPtr, owner);       

      default:
        return new NMBase(cPtr, owner);
    }
  }
%}


/**
 * Convert NMBase objects into the most specific object possible.
 */
%typemap("javaout") NMBase*
{
  return libnuml.DowncastNMBase($jnicall, $owner);
}

/**
 * getCPtrAndDisown() is like getCPtr() but it also sets the SWIG memory
 * ownsership flag to false.
 *
 * We used to use %typemap(javagetcptr), but this has been deprecated
 * in SWIG 1.3.24.  Instead we add getCPtrAndDisown() from the incantation
 * below (taken from the SWIG 1.3.24 CHANGES file).
 */

/* Utility macro for manipulating the Java body code method attributes */
%define SWIGJAVA_ATTRIBS(TYPENAME, CTOR_ATTRIB, GETCPTR_ATTRIB)

%typemap(javabody) TYPENAME
%{
   private long swigCPtr;
   protected boolean swigCMemOwn;

   CTOR_ATTRIB $javaclassname(long cPtr, boolean cMemoryOwn)
   {
     swigCMemOwn = cMemoryOwn;
     swigCPtr    = cPtr;
   }

   GETCPTR_ATTRIB static long getCPtr($javaclassname obj)
   {
     return (obj == null) ? 0 : obj.swigCPtr;
   }

   GETCPTR_ATTRIB static long getCPtrAndDisown ($javaclassname obj)
   {
     long ptr = 0;

     if (obj != null)
     {
       ptr             = obj.swigCPtr;
       obj.swigCMemOwn = false;
     }

     return ptr;
   }
%}


%typemap(javabody_derived) TYPENAME
%{
   private long swigCPtr;

   CTOR_ATTRIB $javaclassname(long cPtr, boolean cMemoryOwn)
   {
   	super($moduleJNI.$javaclassname_SWIGUpcast(cPtr), cMemoryOwn);
 //    super($moduleJNI.SWIG$javaclassnameUpcast(cPtr), cMemoryOwn);
     swigCPtr = cPtr;
   }

   GETCPTR_ATTRIB static long getCPtr($javaclassname obj)
   {
     return (obj == null) ? 0 : obj.swigCPtr;
   }

   GETCPTR_ATTRIB static long getCPtrAndDisown ($javaclassname obj)
   {
     long ptr = 0;

     if (obj != null)
     {
       ptr             = obj.swigCPtr;
       obj.swigCMemOwn = false;
     }

     return ptr;
   }
%}

%enddef

/* The default is protected getCPtr, protected constructor */
SWIGJAVA_ATTRIBS(SWIGTYPE, protected, protected)

/* Public getCPtr method, protected constructor */
%define PUBLIC_GETCPTR(TYPENAME)
SWIGJAVA_ATTRIBS(TYPENAME, protected, public)
%enddef

/* Public getCPtr method, public constructor */
%define PUBLIC_BODYMETHODS(TYPENAME)
SWIGJAVA_ATTRIBS(TYPENAME, public, public)
%enddef

/**
 *
 * Overrides the 'equals' method for NMBase subclasses and other classes
 * in libnuml.
 *
 * By default, 'equals' method ( and '==' operator) for each wrapped class
 * object returns 'true' if the given two objects refer to the same 
 * *Java proxy object* (not the underlying C++ object). 
 * For example, the following code returns 'true'.
 *
 *   Model m = new Model();
 *   m.createReaction();
 *   Reaction r1  = m.getReaction(0);
 *   Reaction r2 = r1;
 *   return r1.equals(r2);  <---- this returns 'true'
 *
 * On the other hand, the following code returns 'false' in spite of
 * the same underlying C++ objects.
 *
 *   Model m = new Model();
 *   m.createReaction();
 *   Reaction r1 = m.getReaction(0);
 *   Reaction r2 = m.getReaction(0);
 *   return r1.equals(r2);  <---- this returns 'false'
 *
 * The following override changes the behaviour of 'equals' method such that
 * returns 'true' if the given two objects refer to the same underlying C++ 
 * object (i.e. 'true' is returned in the both above examples).
 *
 * (Unfortunately, '==' operator can't be overidden in Java.
 *  Thus, the underlying C++ objects can't be compared by the '==' operator.)
 * 
 */

%define SWIGJAVA_EQUALS(CLASS)
%typemap("javacode") CLASS
%{
  /**
   * Equality comparison method for CLASS.
   * <p>
   * Because the Java methods for libNUML are actually wrappers around code
   * implemented in C++ and C, certain operations will not behave as
   * expected.  Equality comparison is one such case.  An instance of a
   * libNUML object class is actually a <em>proxy object</em>
   * wrapping the real underlying C/C++ object.  The normal <code>==</code>
   * equality operator in Java will <em>only compare the Java proxy objects</em>,
   * not the underlying native object.  The result is almost never what you
   * want in practical situations.  Unfortunately, Java does not provide a
   * way to override <code>==</code>.
   *  <p>
   * The alternative that must be followed is to use the
   * <code>equals()</code> method.  The <code>equals</code> method on this
   * class overrides the default java.lang.Object one, and performs an
   * intelligent comparison of instances of objects of this class.  The
   * result is an assessment of whether two libNUML Java objects are truly 
   * the same underlying native-code objects.
   *  <p>
   * The use of this method in practice is the same as the use of any other
   * Java <code>equals</code> method.  For example,
   * <em>a</em><code>.equals(</code><em>b</em><code>)</code> returns
   * <code>true</code> if <em>a</em> and <em>b</em> are references to the
   * same underlying object.
   *
   * @param sb a reference to an object to which the current object
   * instance will be compared
   *
   * @return <code>true</code> if <code>sb</code> refers to the same underlying 
   * native object as this one, <code>false</code> otherwise
   */
  public boolean equals(Object sb)
  {
    if ( this == sb ) 
    {
      return true;
    }
    return swigCPtr == getCPtr((CLASS)(sb));
  }

  /**
   * Returns a hashcode for this CLASS object.
   *
   * @return a hash code usable by Java methods that need them.
   */
  public int hashCode()
  {
    return (int)(swigCPtr^(swigCPtr>>>32));
  }
%}
%enddef

SWIGJAVA_EQUALS(NMBase)
SWIGJAVA_EQUALS(NUMLReader)
SWIGJAVA_EQUALS(NUMLWriter)
SWIGJAVA_EQUALS(ASTNode)
SWIGJAVA_EQUALS(CVTerm)
SWIGJAVA_EQUALS(Date)
SWIGJAVA_EQUALS(ModelHistory)
SWIGJAVA_EQUALS(ModelCreator)
SWIGJAVA_EQUALS(XMLNamespaces)
SWIGJAVA_EQUALS(NUMLNamespaces)
SWIGJAVA_EQUALS(XMLAttributes)
SWIGJAVA_EQUALS(XMLToken)
SWIGJAVA_EQUALS(XMLNode)
SWIGJAVA_EQUALS(XMLTriple)
SWIGJAVA_EQUALS(XMLError)
SWIGJAVA_EQUALS(XMLErrorLog)
SWIGJAVA_EQUALS(XMLOutputStream)

/**
 * Part of libNUML methods takeover ownership of passed-in objects, so we need
 * to make sure SWIG disowns the object.
 * (Most libNUML methods don't takeover ownership since 3.0.0)
 *
 */

/**
 * takeover ownership
 *
 * - void ListOf::appendAndOwn(NMBase* item)
 */
%typemap(javain) NMBase*       item "NMBase.getCPtrAndDisown($javainput)";
%typemap(javain) const NMBase* item "NMBase.getCPtr($javainput)";

/**
 * takeover ownership
 *
 * - void ASTNode::addChild (ASTNode* child)
 * - void ASTNode::prependChild (ASTNode* child)
 */
%typemap(javain) ASTNode*       child "ASTNode.getCPtrAndDisown($javainput)";
%typemap(javain) const ASTNode* child "ASTNode.getCPtr($javainput)";

/**
 * takeover ownership
 *
 * - void ASTNode::insertChild (unsigned int n, ASTNode *newChild)
 * - void ASTNode::replaceChild(unsigned int n, ASTNode *newChild)
 */
%typemap(javain) ASTNode*       newChild "ASTNode.getCPtrAndDisown($javainput)";
%typemap(javain) const ASTNode* newChild "ASTNode.getCPtr($javainput)";

/**
 * takeover ownership
 *
 * - void ASTNode::addSemanticsAnnotation (XMLNode* sAnnotation);
 */
%typemap(javain) XMLNode*       sAnnotation "XMLNode.getCPtrAndDisown($javainput)";
%typemap(javain) const XMLNode* sAnnotation "XMLNode.getCPtr($javainput)";


/**
 * On Windows, a string for filename should be encoded by ANSI CP 
 * instead of UTF-8 because file I/O APIs internally used in libNUML 
 * requires an ANSI CP encoded string for the given filename.
 *  
 *  1) NUMLReader::readNUML(const std::string& filename)
 *  2) readNUML(const char* filename)
 *  3) NUMLWriter::writeNUML(NUMLDocument*, const std::string& filename)
 *  4) writeNUML(NUMLDocument*, const char* filename)
 */

//
// UTF8 -> ANSI CP (for const std::string& filename)
//
%typemap("in") const std::string& filename (const char* arg_pstr, std::string arg_str) 
%{
  if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null std::string");
    return $null;
  }
  arg_pstr = (char const *)jenv->GetStringUTFChars($input, 0);
  if (!arg_pstr) return $null;
#ifdef WIN32
    char*  mbstr = convertUTF8ToACP(arg_pstr);
    if (!mbstr)
    {
      jenv->ReleaseStringUTFChars($input, arg_pstr);
      return $null;
    }

    arg_str.assign(mbstr);
    delete[] mbstr;
#else
  arg_str.assign(arg_pstr);
#endif
  $1 = &arg_str;
  jenv->ReleaseStringUTFChars($input, arg_pstr);

%}



//
// Unicode -> ANSI CP (for const char* filename)
//
%typemap("in")  const char* filename (const char* arg_pstr)
%{
  if ($input) {
    arg_pstr = (char const *)jenv->GetStringUTFChars($input, 0);
    if (!arg_pstr) return $null;
  }
  else
  {
    arg_pstr = 0;
  }
#ifdef WIN32
  $1 = convertUTF8ToACP(arg_pstr);
  if (!$1)
  {
    jenv->ReleaseStringUTFChars($input, (const char*)arg_pstr);
    return $null;
  }
  jenv->ReleaseStringUTFChars($input, (const char*)arg_pstr);
#else
  $1 = (char*)arg_pstr;
#endif
%}

%typemap("freearg")  const char* filename 
%{
#ifdef WIN32
  delete[] $1;
#else
  if ($1) jenv->ReleaseStringUTFChars($input, (const char*)$1);
#endif
%}


/**
 * Some combinations of platforms and underlying XML parsers *require*
 * an absolute path to a filename while others do not.  It's best to
 * hide this from the end-user by making NUMLReader.readNUML() and
 * readNUML() always compute an absolute path and filename.
 */

%pragma(java) modulecode =
%{
   static String getAbsolutePath(String filename)
   {
     java.io.File file = new java.io.File(filename);
     return file.getAbsolutePath();
   }
%}

%typemap("javain") const std::string& filename "libnuml.getAbsolutePath($javainput)";
%typemap("javain") const char* filename        "libnuml.getAbsolutePath($javainput)";

/**
 * Wraps std::ostream by implementing three simple wrapper classes.
 *
 * 1) OStream wraps std::cout, std::cerr, and std::clog.
 *    The following public final static variables are provied in
 *    libnuml class like in C++.
 *
 *    1. public final static OStream cout;
 *    2. public final static OStream cerr;
 *    3. public final static OStream clog;
 *
 * 2) OFStream (derived class of OStream) wraps std::ofstream 
 *    with ios_base::cout (default) or ios_base::app flag. 
 *
 * 3) OStringStream (derived class of OStream) wraps std::ostringstream.
 *
 * These wrapper classes provide only the minimum functions.
 *
 * (sample code) -----------------------------------------------------
 *
 * 1. wraps std::cout
 *
 *    XMLOutputStream xos = new XMLOutputStream(libnuml.cout);
 *
 * 2. wraps std::cerr
 *
 *    NUMLDocument d = libnuml.readNUML("foo.xml");
 *    if ( d.getNumErrors() > 0) {
 *       d.printErrors(libnuml.cerr);
 *    }
 *
 * 3. wraps std::ofstream (write to file "foo.xml")
 *
 *    OFStream   ofs = new OFStream("foo.xml");
 *    NUMLDocument d = libnuml.readNUML("foo.xml");
 *    NUMLWriter   w = new NUMLWriter();
 *    w.writeNUML(d,ofs);
 *
 * 4. wraps std::ofstream (write to file "foo.xml" with append mode)
 *
 *    OFStream ofs = new OFStream("foo.xml",true);
 *    XMLOutputStream xos = new XMLOutputStream(ofs);
 *
 * 5. wraps std::ostringstream 
 *
 *    OStringStream   oss = new OStringStream();
 *    XMLOutputStream xos = new XMLOutputStream(oss);
 *    ...
 *    oss.endl();
 *    String s = oss.str();
 *
 */

%typemap(jstype) std::ostream& "OStream"
%typemap(javain) std::ostream& "SWIGTYPE_p_std__ostream.getCPtr($javainput.get_ostream())";

%include "OStream.h"

%{
#include "OStream.cpp"
%}

%pragma(java) modulecode =
%{
  /**
    * Stream handle for low-level C++ standard output stream.
    * <p>
    * A few libNUML methods accept an argument for indicating where to send
    * text string output.  An example is the {@link
    * NUMLDocument#printErrors} method.  However, the methods use C++ style
    * streams and not Java stream objects.  The OStream object class in the
    * libNUML Java interface provides a wrapper for the underlying C++
    * streams.  The present object (cout) is a static final variable that
    * can be used directly from your code.  An example use might be
    * something like this:
    * <p>
    * <div class="fragment"><pre class="fragment">
    * NUMLDocument document = libnuml.readNUML("somefile.xml");
    * if (document.getNumErrors() > 0)
    * {
    *     document.printErrors(libnuml.cout);
    *     println("Please correct the above problems first.");
    *     System.exit(1);
    * }</div>
    *
    * @see #cerr
    * @see #clog
    */
  public final static OStream cout;


  /**
    * Stream handle for low-level C++ standard error stream.
    * <p>
    * A few libNUML methods accept an argument for indicating where to send
    * text string output.  An example is the {@link
    * NUMLDocument#printErrors} method.  However, the methods use C++ style
    * streams and not Java stream objects.  The OStream object class in the
    * libNUML Java interface provides a wrapper for the underlying C++
    * streams.  The present object (cerr) is a static final variable that
    * can be used directly from your code.  An example use might be
    * something like this:
    * <p>
    * <div class="fragment"><pre class="fragment">
    * NUMLDocument document = libnuml.readNUML("somefile.xml");
    * if (document.getNumErrors() > 0)
    * {
    *     document.printErrors(libnuml.cerr);
    *     println("Please correct the above problems first.");
    *     System.exit(1);
    * }</div>
    * <p>
    * By default, most operating systems have have their standard error and
    * logging output streams directed to the console/terminal, and this is
    * where text messages will be shown.  This can usually be redirected
    * elsewhere, although how to do this depends on the specific environment
    * where the program is running.
    *
    * @see #cout
    * @see #clog
    */
  public final static OStream cerr;


  /**
    * Stream handle for low-level C++ standard logging stream.
    * <p>
    * A few libNUML methods accept an argument for indicating where to send
    * text string output.  An example is the {@link
    * NUMLDocument#printErrors} method.  However, the methods use C++ style
    * streams and not Java stream objects.  The OStream object class in the
    * libNUML Java interface provides a wrapper for the underlying C++
    * streams.  The present object (clog) is a static final variable that
    * can be used directly from your code.  An example use might be
    * something like this:
    * <p>
    * <div class="fragment"><pre class="fragment">
    * NUMLDocument document = libnuml.readNUML("somefile.xml");
    * if (document.getNumErrors() > 0)
    * {
    *     document.printErrors(libnuml.clog);
    *     println("Please correct the above problems first.");
    *     System.exit(1);
    * }</div>
    * <p>
    * By default, most operating systems have have their standard error and
    * logging output streams directed to the console/terminal, and this is
    * where text messages will be shown.  This can usually be redirected
    * elsewhere, although how to do this depends on the specific environment
    * where the program is running.
    *
    * @see #cout
    * @see #cerr
    */
  public final static OStream clog;

  static {
    cout = new OStream(OStream.COUT); 
    cerr = new OStream(OStream.CERR); 
    clog = new OStream(OStream.CLOG); 
  }

  /**
   * This private constructor does nothing and never invoked.
   * The purpose of this constuctor is to hide a default constructor of this 
   * class in javadoc documentation.
   */
  private libnuml() {}

%}



/**
 *  Wraps the following functions by using the corresponding
 *  ListWrapper<TYPENAME> class.
 *
 *  - List* ModelHistory::getListCreators()
 *  - List* ModelHistory::getListModifiedDates()
 *  - List* NMBase::getCVTerms()
 *
 *  ListWrapper<TYPENAME> class is wrapped as TYPENAMEListclass.
 *  So, the above functions are wrapped as follows:
 *
 *  - ModelCreatorList ModelHistory.getListCreators()
 *  - DateList         ModelHistory.getListModifiedDates()
 *  - CVTermList       NMBase.getCVTerms()
 *
 */

%define LIST_WRAPPER(_FNAME_,_TYPENAME_)
%typemap(jstype)  List* _FNAME_ %{ _TYPENAME_ ## List %}

%typemap(javaout) List* _FNAME_ 
{ 
  long cPtr = $jnicall;
  return (cPtr == 0) ? null : new  _TYPENAME_ ## List(cPtr, true);
}

%typemap(out) List* _FNAME_ 
{
   ListWrapper<_TYPENAME_> *listw = ($1 != 0) ? new ListWrapper<_TYPENAME_>($1) : 0;  
   *( ListWrapper<_TYPENAME_>   **)&$result = listw;
}
%enddef

LIST_WRAPPER(ModelHistory::getListCreators,ModelCreator)
LIST_WRAPPER(ModelHistory::getListModifiedDates,Date)
LIST_WRAPPER(NMBase::getCVTerms,CVTerm)

