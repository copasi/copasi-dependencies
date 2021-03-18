/**
 * @file    local-normal.i
 * @brief   Python-specific SWIG directives for wrapping libNUML API
 * @author  Ben Bornstein
 * @author  Ben Kovitz
 * @author  Akiya Jouraku
 * 
 *<!---------------------------------------------------------------------------
 * This file is part of libNUML.  Please visit http://numl.org for more
 * information about NUML, and the latest version of libNUML.
 *
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EBML-EBI), Hinxton, UK
 *  
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://numl.org/software/libnuml/license.html
 *----------------------------------------------------------------------- -->*/


/**
 * Turn on (minimal) Python docstrings and then append our own.
 */
%feature("autodoc", "1");

%{
#include <sbml/common/libsbml-namespace.h>
LIBSBML_CPP_NAMESPACE_USE
%}

%include "local-contrib.i"


/**
 *  Wraps std::cout, std::cerr, std::clog, std::ostream, and std::ostringstream, 
 *
 * (sample code) -----------------------------------------------------
 *
 * 1. wraps std::cout
 *
 *    xos = libnuml.XMLOutputStream(libnuml.cout)
 *
 * 2. wraps std::cerr
 *
 *    d = libnuml.readNUML("foo.xml")
 *    if d.getNumErrors() > 0 :
 *       d.printErrors(libnuml.cerr)
 *    
 *
 * 3. wraps std::ostringstream
 *
 *    oss = libnuml.ostringstream()
 *    xos = libnuml.XMLOutputStream(oss)
 *    ...
 *    libnuml.endl(oss)
 *    s = oss.str()
 * 
 */

// ignores C++ specific methods in std::string.
%ignore std::basic_string<char>::begin;
%ignore std::basic_string<char>::end;
%ignore std::basic_string<char>::rbegin;
%ignore std::basic_string<char>::rend;
%ignore std::basic_string<char>::get_allocator;
%ignore std::basic_string<char>::capacity;
%ignore std::basic_string<char>::reserve;

%include <std_alloc.i>
%include <std_basic_string.i>
%include <std_string.i>

#pragma SWIG nowarn=509
%warnfilter(401) basic_ios<char>;

namespace std
{
  // Template class basic ios
  template<typename _CharT, typename _Traits = char_traits<_CharT> >
  class basic_ios : public ios_base {};

  // Template class basic_ostream
  template<typename _CharT, typename _Traits = char_traits<_CharT> >
  class basic_ostream : virtual public basic_ios<_CharT, _Traits> 
  {
    public:
      explicit
      basic_ostream(std::basic_streambuf<_CharT, _Traits>* __sb);
      virtual 
      ~basic_ostream();
  };

  // Template class basic_ostringstream
  template<typename _CharT, typename _Traits = char_traits<_CharT>,
           typename _Alloc = allocator<_CharT> >
  class basic_ostringstream : public basic_ostream<_CharT, _Traits>
  {
    public:
      explicit
      basic_ostringstream(std::ios_base::openmode __mode = std::ios_base::out);
      ~basic_ostringstream();

      basic_string<_CharT, _Traits, _Alloc> 
      str() const;

      void
      str(const basic_string<_CharT, _Traits, _Alloc>& __s);
  };


  /**
   * Insert a newline character into the given C++ stream.
   *
   * This is a wrapper around the underlying C++ OStream method
   * <code>endl</code>.  It inserts a newline into the stream
   * passed as argument.  Additionally, it flushes buffered
   * streams.
   */
  template<typename _CharT, typename _Traits = char_traits<_CharT> >
  basic_ostream<_CharT, _Traits>& 
  endl(basic_ostream<_CharT, _Traits>&);


  /**
   * Flush the given C++ stream.
   *
   * This is a wrapper around the underlying C++ OStream method
   * <code>flush</code>.  It flush any pending output in the stream 
   * passed as argument.  
   */
  template<typename _CharT, typename _Traits = char_traits<_CharT> >
  basic_ostream<_CharT, _Traits>& 
  flush(basic_ostream<_CharT, _Traits>&);
}

namespace std
{
  /**
   *  std::ostream and std::ostringstream 
   *  (std::ios is not wrapped)
   */
  typedef basic_ios<char>           ios;
  typedef basic_ostream<char>       ostream ;
  typedef basic_ostringstream<char> ostringstream ;

  %template()              basic_ios<char>;
  %template(ostream)       basic_ostream<char>;
  %template(ostringstream) basic_ostringstream<char>;

  /**
   *  output manipulators
   */
  %template(endl)  endl<char, char_traits<char> >;
  %template(flush) flush<char, char_traits<char> >;

  /**
   *  std::cout, std::cerr, and std::clog.
   */
  %immutable;
  extern std::ostream cout;
  extern std::ostream cerr;
  extern std::ostream clog;
  %mutable;
}


/**
 * Convert an NMBase object to a string.
 *
%extend NMBase
{
  %pythoncode
  {
    def __str__(self):
      """
      Return a string representation of this object.

      Note that the string will not be a complete NUML document.
      """

      return self.toNUML()
  }
}*/


/**
 * Allows NUMLList objects:
 *
 *   - To be indexed and sliced, e.g. lst[0].
 */
%extend NUMLList
{
  int __len__()
  {
    return self->size();
  }

  %pythoncode
  {
    def __getitem__(self, key):

      try:
         keyIsSlice = isinstance(key, slice)
      except:
         keyIsSlice = 0

      if keyIsSlice:
        start = key.start
        if start is None:
          start = 0
        stop = key.stop
        if stop is None:
          stop = self.size()
        return [self[i] for i in range(
          self._fixNegativeIndex(start), self._fixNegativeIndex(stop)
        )]

      key = self._fixNegativeIndex(key)
      if key < 0 or key >= self.size():
        raise IndexError(key)
      return self.get(key)


    def _fixNegativeIndex(self, index):
      if index < 0:
        return index + self.size()
      else:
        return index


    def __iter__(self):
      for i in range(self.size()):
        yield self[i]


    def __repr__(self):
      return "[" + ", ".join([repr(self[i]) for i in range(len(self))]) + "]"


    def __str__(self):
      return repr(self)
  }
}



/**
 * Convert objects into the most specific type possible.
 */
%typemap(out) NMBase*, NUMLList*, ResultComponent*, Dimension*, DimensionDescription*, CompositeDescription*, CompositeValue*, TupleDescription*, Tuple*, AtomicDescription*, AtomicValue*, OntologyTerm*
{
  $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), GetDowncastSwigType($1),
                               $owner | %newpointer_flags);
}


/*
 * SWIG-generated wrapper code wrongly invokes 
 * XMLOutputStream::writeAttribute(.., const unsigned int& value) instead of
 * XMLOutputStream::writeAttribute(.., const bool& value) even if the writeAttribute 
 * function invoked with a bool value (True or False) in Python code.
 * It seems that a bool value could be casted to unsigned int, int, or long value 
 * in SWIG-generated internal type check code when these types are overloaded in the
 * wrapped function.
 *
 * To avoid this problem, XMLOutputStream::writeAttribute(.., const bool& value)
 * is internally wrapped as XMLOutputStream::writeAttributeBool(.., const bool&) 
 * and this function is properly invoked when the writeAttribute function is invoked
 * with a bool value in Python code.
 */

%extend XMLOutputStream
{
  void writeAttributeBool(const std::string& name, const bool& value)
  {
    $self->writeAttribute(name, value);
  }

  void writeAttributeBool(const XMLTriple& name, const bool& value)
  {
    $self->writeAttribute(name, value);
  }
}

#if SWIG_VERSION > 0x010336
%feature("pythonprepend")
XMLOutputStream::writeAttribute
%{
        if type(args[1]) == type(True): return _libnuml.XMLOutputStream_writeAttributeBool(self, *args)
%}
#else 
%feature("pythonprepend")
XMLOutputStream::writeAttribute
%{
        if type(args[2]) == type(True): return _libnuml.XMLOutputStream_writeAttributeBool(*args)
%}
#endif

/**
 * Add an equality operator to NMBase.  All subclasses of NMBase
 * will inherit this method.
 *
 * The %extend rewrites __cmp__ such that two objects of
 * disimilar type can be compared without throwing a TypeError.  For
 * example: the following will return false and not throw an exception:
 *
 *   c = libnuml.Compartment()
 *   n = 5
 *   c == n
 *
 * For more information, see testEquality() in test/TestPython.py
 */

%define SWIGPYTHON__CMP__(CLASS)
%extend CLASS
{
  %pythoncode
  {
    def __eq__(self, rhs):
      if ((self is None) and (rhs is None)): return True
      if ((self is None) or  (rhs is None)): return False
      if (hasattr(self, 'this') and hasattr(rhs, 'this')):
        if (self.this == rhs.this): return True
      return False

    def __ne__(self, rhs):
      if ((self is None) and (rhs is None)): return False
      if ((self is None) or  (rhs is None)): return True
      if (hasattr(self, 'this') and hasattr(rhs, 'this')):
        if (self.this == rhs.this): return False
      return True
  }
}
%enddef

SWIGPYTHON__CMP__(NMBase)
SWIGPYTHON__CMP__(NUMLWriter)
SWIGPYTHON__CMP__(NUMLReader)
SWIGPYTHON__CMP__(ResultComponent)
SWIGPYTHON__CMP__(ASTNode)
SWIGPYTHON__CMP__(XMLNamespaces)
SWIGPYTHON__CMP__(NUMLNamespaces)
SWIGPYTHON__CMP__(XMLAttributes)
SWIGPYTHON__CMP__(XMLToken)
SWIGPYTHON__CMP__(XMLTriple)
SWIGPYTHON__CMP__(XMLError)
SWIGPYTHON__CMP__(XMLErrorLog)
SWIGPYTHON__CMP__(XMLOutputStream)

/**
 * The features directives below override the default SWIG generated
 * code for certain methods.  The idea is to tell SWIG to disown the
 * passed-in object.  The containing object will takeover ownership
 * and delete the object as appropriate.  This avoids a deadly
 * double-delete which can result in a segmentation fault.  For
 * example, each NMBase that is appended to a NUMLList is subsequently
 * owned by that NUMLList.
 */

%define TAKEOVER_OWNERSHIP(METHOD_NAME,ARG_INDEX)
%feature("pythonprepend")
METHOD_NAME
%{
        if args[ARG_INDEX] is not None: args[ARG_INDEX].thisown = 0
%}
%enddef

// ----------------------------------------------------------------------
// NUMLList
// ----------------------------------------------------------------------

#if SWIG_VERSION > 0x010336
TAKEOVER_OWNERSHIP(NUMLList::appendAndOwn(NMBase*),0)
#else
TAKEOVER_OWNERSHIP(NUMLList::appendAndOwn(NMBase*),1)
#endif

// ----------------------------------------------------------------------
// ASTNode
// ----------------------------------------------------------------------

#if SWIG_VERSION > 0x010336
TAKEOVER_OWNERSHIP(ASTNode::addChild(ASTNode*),0)
TAKEOVER_OWNERSHIP(ASTNode::prependChild(ASTNode*),0)
TAKEOVER_OWNERSHIP(ASTNode::insertChild(unsigned int, ASTNode*),1)
TAKEOVER_OWNERSHIP(ASTNode::replaceChild(unsigned int, ASTNode*),1)
TAKEOVER_OWNERSHIP(ASTNode::addSemanticsAnnotation(XMLNode*),0)
#else
TAKEOVER_OWNERSHIP(ASTNode::addChild(ASTNode*),1)
TAKEOVER_OWNERSHIP(ASTNode::prependChild(ASTNode*),1)
TAKEOVER_OWNERSHIP(ASTNode::insertChild(unsigned int, ASTNode*),2)
TAKEOVER_OWNERSHIP(ASTNode::replaceChild(unsigned int, ASTNode*),2)
TAKEOVER_OWNERSHIP(ASTNode::addSemanticsAnnotation(XMLNode*),1)
#endif

/**
 *
 * Wraps the NUMLConstructorException class (C++ exception defined by libNUML) 
 * as the ValueError class (Python built-in exception).
 *
 * For example, the exception can be catched in Python code as follows:
 *
 * --------------------------------------
 *  try:
 *    s = libnuml.CompartmentType(level,version)
 *  except ValueError, inst:
 *    errmsg = inst.args[0]
 * --------------------------------------
 */

%ignore NUMLConstructorException;

%define NUMLCONSTRUCTOR_EXCEPTION(SBASE_CLASS_NAME)
%exception SBASE_CLASS_NAME {
  try {
    $action
  }
  catch (NUMLConstructorException &e) {
    PyErr_SetString(PyExc_ValueError, const_cast<char*>(e.what()));
    return NULL;
  }
}
%enddef

NUMLCONSTRUCTOR_EXCEPTION(Compartment)
NUMLCONSTRUCTOR_EXCEPTION(CompartmentType)
NUMLCONSTRUCTOR_EXCEPTION(Delay)
NUMLCONSTRUCTOR_EXCEPTION(Event)
NUMLCONSTRUCTOR_EXCEPTION(EventAssignment)
NUMLCONSTRUCTOR_EXCEPTION(FunctionDefinition)
NUMLCONSTRUCTOR_EXCEPTION(InitialAssignment)
NUMLCONSTRUCTOR_EXCEPTION(KineticLaw)
NUMLCONSTRUCTOR_EXCEPTION(Model)
NUMLCONSTRUCTOR_EXCEPTION(Parameter)
NUMLCONSTRUCTOR_EXCEPTION(Priority)
NUMLCONSTRUCTOR_EXCEPTION(LocalParameter)
NUMLCONSTRUCTOR_EXCEPTION(Reaction)
NUMLCONSTRUCTOR_EXCEPTION(AssignmentRule)
NUMLCONSTRUCTOR_EXCEPTION(AlgebraicRule)
NUMLCONSTRUCTOR_EXCEPTION(RateRule)
NUMLCONSTRUCTOR_EXCEPTION(Species)
NUMLCONSTRUCTOR_EXCEPTION(SpeciesReference)
NUMLCONSTRUCTOR_EXCEPTION(ModifierSpeciesReference)
NUMLCONSTRUCTOR_EXCEPTION(SpeciesType)
NUMLCONSTRUCTOR_EXCEPTION(StoichiometryMath)
NUMLCONSTRUCTOR_EXCEPTION(Trigger)
NUMLCONSTRUCTOR_EXCEPTION(Unit)
NUMLCONSTRUCTOR_EXCEPTION(UnitDefinition)
NUMLCONSTRUCTOR_EXCEPTION(NUMLDocument)
NUMLCONSTRUCTOR_EXCEPTION(NUMLNamespaces)
NUMLCONSTRUCTOR_EXCEPTION(NUMLExtensionNamespaces)

NUMLCONSTRUCTOR_EXCEPTION(ListOf)
NUMLCONSTRUCTOR_EXCEPTION(ListOfCompartments)
NUMLCONSTRUCTOR_EXCEPTION(ListOfCompartmentTypes)
NUMLCONSTRUCTOR_EXCEPTION(ListOfEventAssignments)
NUMLCONSTRUCTOR_EXCEPTION(ListOfEvents)
NUMLCONSTRUCTOR_EXCEPTION(ListOfFunctionDefinitions)
NUMLCONSTRUCTOR_EXCEPTION(ListOfInitialAssignments)
NUMLCONSTRUCTOR_EXCEPTION(ListOfParameters)
NUMLCONSTRUCTOR_EXCEPTION(ListOfLocalParameters)
NUMLCONSTRUCTOR_EXCEPTION(ListOfReactions)
NUMLCONSTRUCTOR_EXCEPTION(ListOfRules)
NUMLCONSTRUCTOR_EXCEPTION(ListOfSpecies)
NUMLCONSTRUCTOR_EXCEPTION(ListOfSpeciesReferences)
NUMLCONSTRUCTOR_EXCEPTION(ListOfSpeciesTypes)
NUMLCONSTRUCTOR_EXCEPTION(ListOfUnitDefinitions)
NUMLCONSTRUCTOR_EXCEPTION(ListOfUnits)

/**
 *
 * Wraps the XMLConstructorException class (C++ exception defined by libNUML) 
 * as the VaueError class (Python built-in exception).
 *
 * For example, the exception can be catched in Python code as follows:
 *
 * --------------------------------------
 *  try:
 *    s = libnuml.XMLAttributes(invalid arguments)
 *  except ValueError, inst:
 *    errmsg = inst.args[0]
 * --------------------------------------
 */

%ignore XMLConstructorException;

%define XMLCONSTRUCTOR_EXCEPTION(SBASE_CLASS_NAME)
%exception SBASE_CLASS_NAME {
  try {
    $action
  }
  catch (XMLConstructorException &e) {
    PyErr_SetString(PyExc_ValueError, const_cast<char*>(e.what()));
    return NULL;
  }
}
%enddef

XMLCONSTRUCTOR_EXCEPTION(XMLAttributes)
XMLCONSTRUCTOR_EXCEPTION(XMLError)
XMLCONSTRUCTOR_EXCEPTION(XMLNamespaces)
XMLCONSTRUCTOR_EXCEPTION(XMLNode)
XMLCONSTRUCTOR_EXCEPTION(XMLOutputStream)
XMLCONSTRUCTOR_EXCEPTION(XMLToken)
XMLCONSTRUCTOR_EXCEPTION(XMLTripple)


// ----------------------------------------------------------------------
// NUMLReader
// ----------------------------------------------------------------------


%pythoncode
%{
import sys
import os.path

# @cond doxygen-libnuml-internal

def conditional_abspath (filename):
  """conditional_abspath (filename) -> filename

  Returns filename with an absolute path prepended, if necessary.
  Some combinations of platforms and underlying XML parsers *require*
  an absolute path to a filename while others do not.  This function
  encapsulates the appropriate logic.  It is used by readNUML() and
  NUMLReader.readNUML().
  """
  if sys.platform.find('cygwin') != -1:
    return filename
  else:
    return os.path.abspath(filename)

# @endcond
%}


/*
 * I haven't been able to attach the original docstring without
 * copying it here.  A %feature("docstring") directive placed
 * after here won't do it, and neither will moving the pydoc.i
 * file inclusion from the top of this file to the end of this
 * file.  I'm giving up and just copy-pasting the doc string
 * from NUMLReader.h.  Definitely non-ideal, but I'm at the end
 * of ideas.
 */
%feature("shadow")
NUMLReader::readNUML(const std::string&)
%{
  def readNUML(*args):
    """
    readNUML(self, string filename) -> NUMLDocument

    Reads an NUML document from a file.

    This method is identical to readNUMLFromFile().

    If the file named 'filename' does not exist or its content is not
    valid NUML, one or more errors will be logged with the NUMLDocument
    object returned by this method.  Callers can use the methods on
    NUMLDocument such as NUMLDocument.getNumErrors() and
    NUMLDocument.getError() to get the errors.  The object returned by
    NUMLDocument.getError() is an NUMLError object, and it has methods to
    get the error code, category, and severity level of the problem, as
    well as a textual description of the problem.  The possible severity
    levels range from informational messages to fatal errors; see the
    documentation for NUMLError for more information.

    If the file 'filename' could not be read, the file-reading error will
    appear first.  The error code can provide a clue about what happened.
    For example, a file might be unreadable (either because it does not
    actually exist or because the user does not have the necessary access
    priviledges to read it) or some sort of file operation error may have
    been reported by the underlying operating system.  Callers can check
    for these situations using a program fragment such as the following:

     reader = NUMLReader()
     doc    = reader.readNUML(filename)

     if doc.getNumErrors() > 0:
       if doc.getError(0).getErrorId() == libnuml.XMLFileUnreadable:
         # Handle case of unreadable file here.
       elif doc.getError(0).getErrorId() == libnuml.XMLFileOperationError:
         # Handle case of other file error here.
       else:
         # Handle other error cases here.

    If the given filename ends with the suffix \".gz\" (for example,
    \"myfile.xml.gz\"), the file is assumed to be compressed in gzip format
    and will be automatically decompressed upon reading.  Similarly, if the
    given filename ends with \".zip\" or \".bz2\", the file is assumed to be
    compressed in zip or bzip2 format (respectively).  Files whose names
    lack these suffixes will be read uncompressed.  Note that if the file
    is in zip format but the archive contains more than one file, only the
    first file in the archive will be read and the rest ignored.

    To read a gzip/zip file, libNUML needs to be configured and linked with
    the zlib library at compile time.  It also needs to be linked with the
    bzip2 library to read files in bzip2 format.  (Both of these are the
    default configurations for libNUML.)  Errors about unreadable files
    will be logged if a compressed filename is given and libNUML was not
    linked with the corresponding required library.

    Parameter 'filename is the name or full pathname of the file to be
    read.

    Returns a pointer to the NUMLDocument created from the NUML content.

    See also NUMLError.

    Note:

    LibNUML versions 2.x and later versions behave differently in
    error handling in several respects.  One difference is how early some
    errors are caught and whether libNUML continues processing a file in
    the face of some early errors.  In general, libNUML versions after 2.x
    stop parsing NUML inputs sooner than libNUML version 2.x in the face
    of XML errors, because the errors may invalidate any further NUML
    content.  For example, a missing XML declaration at the beginning of
    the file was ignored by libNUML 2.x but in version 3.x and later, it
    will cause libNUML to stop parsing the rest of the input altogether.
    While this behavior may seem more severe and intolerant, it was
    necessary in order to provide uniform behavior regardless of which
    underlying XML parser (Expat, Xerces, libxml2) is being used by
    libNUML.  The XML parsers themselves behave differently in their error
    reporting, and sometimes libNUML has to resort to the lowest common
    denominator.
    """
    args_copy    = list(args)
    args_copy[1] = conditional_abspath(args[1])
    return _libnuml.NUMLReader_readNUML(*args_copy)
%}

%feature("shadow")
NUMLReader::readNUMLFromFile(const std::string&)
%{
  def readNUMLFromFile(*args):
    """
    readNUMLFromFile(self, string filename) -> NUMLDocument

    Reads an NUML document from a file.

    This method is identical to readNUMLFromFile().

    If the file named 'filename' does not exist or its content is not
    valid NUML, one or more errors will be logged with the NUMLDocument
    object returned by this method.  Callers can use the methods on
    NUMLDocument such as NUMLDocument.getNumErrors() and
    NUMLDocument.getError() to get the errors.  The object returned by
    NUMLDocument.getError() is an NUMLError object, and it has methods to
    get the error code, category, and severity level of the problem, as
    well as a textual description of the problem.  The possible severity
    levels range from informational messages to fatal errors; see the
    documentation for NUMLError for more information.

    If the file 'filename' could not be read, the file-reading error will
    appear first.  The error code can provide a clue about what happened.
    For example, a file might be unreadable (either because it does not
    actually exist or because the user does not have the necessary access
    priviledges to read it) or some sort of file operation error may have
    been reported by the underlying operating system.  Callers can check
    for these situations using a program fragment such as the following:

     reader = NUMLReader()
     doc    = reader.readNUML(filename)

     if doc.getNumErrors() > 0:
       if doc.getError(0).getErrorId() == libnuml.XMLFileUnreadable:
         # Handle case of unreadable file here.
       elif doc.getError(0).getErrorId() == libnuml.XMLFileOperationError:
         # Handle case of other file error here.
       else:
         # Handle other error cases here.

    If the given filename ends with the suffix \".gz\" (for example,
    \"myfile.xml.gz\"), the file is assumed to be compressed in gzip format
    and will be automatically decompressed upon reading.  Similarly, if the
    given filename ends with \".zip\" or \".bz2\", the file is assumed to be
    compressed in zip or bzip2 format (respectively).  Files whose names
    lack these suffixes will be read uncompressed.  Note that if the file
    is in zip format but the archive contains more than one file, only the
    first file in the archive will be read and the rest ignored.

    To read a gzip/zip file, libNUML needs to be configured and linked with
    the zlib library at compile time.  It also needs to be linked with the
    bzip2 library to read files in bzip2 format.  (Both of these are the
    default configurations for libNUML.)  Errors about unreadable files
    will be logged if a compressed filename is given and libNUML was not
    linked with the corresponding required library.

    Parameter 'filename is the name or full pathname of the file to be
    read.
    getElementName()
    Returns a pointer to the NUMLDocument created from the NUML content.

    See also NUMLError.

    Note:

    LibNUML versions 2.x and later versions behave differently in
    error handling in several respects.  One difference is how early some
    errors are caught and whether libNUML continues processing a file in
    the face of some early errors.  In general, libNUML versions after 2.x
    stop parsing NUML inputs sooner than libNUML version 2.x in the face
    of XML errors, because the errors may invalidate any further NUML
    content.  For example, a missing XML declaration at the beginning of
    the file was ignored by libNUML 2.x but in version 3.x and later, it
    will cause libNUML to stop parsing the rest of the input altogether.
    While this behavior may seem more severe and intolerant, it was
    necessary in order to provide uniform behavior regardless of which
    underlying XML parser (Expat, Xerces, libxml2) is being used by
    libNUML.  The XML parsers themselves behave differently in their error
    reporting, and sometimes libNUML has to resort to the lowest common
    denominator.
    """
    args_copy    = list(args)
    args_copy[1] = conditional_abspath(args[1])
    return _libnuml.NUMLReader_readNUML(*args_copy)
%}


/**
 * Since we cannot seem to "shadow" readNUML() (maybe because it's
 * not a method of some object, but rather a top-level function, we
 * employ the following HACK: Tell SWIG to ignore readNUML and just
 * define it in terms of NUMLReader.readNUML().  This is less than
 * ideal, because the libNUML C/C++ core does essentially the same
 * thing, so now we're repeating ourselves.
 */

%ignore readNUML(const char*);

%pythoncode
%{
def readNUML(*args):
  """
  readNUML(self, string filename) -> NUMLDocument

  Reads an NUML document from a file.

  This method is identical to readNUMLFromFile().

  If the file named 'filename' does not exist or its content is not
  valid NUML, one or more errors will be logged with the NUMLDocument
  object returned by this method.  Callers can use the methods on
  NUMLDocument such as NUMLDocument.getNumErrors() and
  NUMLDocument.getError() to get the errors.  The object returned by
  NUMLDocument.getError() is an NUMLError object, and it has methods to
  get the error code, category, and severity level of the problem, as
  well as a textual description of the problem.  The possible severity
  levels range from informational messages to fatal errors; see the
  documentation for NUMLError for more information.

  If the file 'filename' could not be read, the file-reading error will
  appear first.  The error code can provide a clue about what happened.
  For example, a file might be unreadable (either because it does not
  actually exist or because the user does not have the necessary access
  priviledges to read it) or some sort of file operation error may have
  been reported by the underlying operating system.  Callers can check
  for these situations using a program fragment such as the following:

   reader = NUMLReader()
   doc    = reader.readNUML(filename)

   if doc.getNumErrors() > 0:
     if doc.getError(0).getErrorId() == libnuml.XMLFileUnreadable:
       # Handle case of unreadable file here.
     elif doc.getError(0).getErrorId() == libnuml.XMLFileOperationError:
       # Handle case of other file error here.
     else:
       # Handle other error cases here.

  If the given filename ends with the suffix \".gz\" (for example,
  \"myfile.xml.gz\"), the file is assumed to be compressed in gzip format
  and will be automatically decompressed upon reading.  Similarly, if the
  given filename ends with \".zip\" or \".bz2\", the file is assumed to be
  compressed in zip or bzip2 format (respectively).  Files whose names
  lack these suffixes will be read uncompressed.  Note that if the file
  is in zip format but the archive contains more than one file, only the
  first file in the archive will be read and the rest ignored.

  To read a gzip/zip file, libNUML needs to be configured and linked with
  the zlib library at compile time.  It also needs to be linked with the
  bzip2 library to read files in bzip2 format.  (Both of these are the
  default configurations for libNUML.)  Errors about unreadable files
  will be logged if a compressed filename is given and libNUML was not
  linked with the corresponding required library.

  Parameter 'filename is the name or full pathname of the file to be
  read.

  Returns a pointer to the NUMLDocument created from the NUML content.

  See also NUMLError.

  Note:

  LibNUML versions 2.x and later versions behave differently in
  error handling in several respects.  One difference is how early some
  errors are caught and whether libNUML continues processing a file in
  the face of some early errors.  In general, libNUML versions after 2.x
  stop parsing NUML inputs sooner than libNUML version 2.x in the face
  of XML errors, because the errors may invalidate any further NUML
  content.  For example, a missing XML declaration at the beginning of
  the file was ignored by libNUML 2.x but in version 3.x and later, it
  will cause libNUML to stop parsing the rest of the input altogether.
  While this behavior may seem more severe and intolerant, it was
  necessary in order to provide uniform behavior regardless of which
  underlying XML parser (Expat, Xerces, libxml2) is being used by
  libNUML.  The XML parsers themselves behave differently in their error
  reporting, and sometimes libNUML has to resort to the lowest common
  denominator.
  """
  reader = NUMLReader()
  return reader.readNUML(args[0])
%}


/**
 *  Wraps the following functions by using the corresponding 
 *  ListWrapper<TYPENAME> class.
 *
 *  - List* ModelHistory::getListCreators()
 *  - List* ModelHistory::getListModifiedDates()
 *  - List* NMBase::getCVTerms()
 *  - List* NUMLNamespaces::getSupportedNamespaces()
 *
 *  ListWrapper<TYPENAME> class is wrapped as TYPENAMEList class.
 *  So, the above functions are wrapped as follows:
 *
 *  - ModelCreatorList ModelHistory.getListCreators()
 *  - DateList         ModelHistory.getListModifiedDates()
 *  - CVTermList       NMBase.getCVTerms()
 *  - NUMLNamespacesList NUMLNamespaces::getSupportedNamespaces()
 *
 */

%feature("shadow")
NUMLNamespaces::getSupportedNamespaces
%{
  def getSupportedNamespaces(self):
    """
    getSupportedNamespaces(self) -> NUMLNamespaceList

    Get the List of supported NUMLNamespaces for this 
    version of LibNUML.

    Returns the supported list of NUMLNamespaces.
          

    """
    return _libnuml.NUMLNamespaces_getSupportedNamespaces(self)
%}

%typemap(out) List* NUMLNamespaces::getSupportedNamespaces
{
  ListWrapper<NUMLNamespaces> *listw = ($1 != 0) ? new ListWrapper<NUMLNamespaces>($1) : 0;
  $result = SWIG_NewPointerObj(SWIG_as_voidptr(listw), 
#if SWIG_VERSION > 0x010333
                               SWIGTYPE_p_ListWrapperT_NUMLNamespaces_t, 
#else
                               SWIGTYPE_p_ListWrapperTNUMLNamespaces_t, 
#endif
                               SWIG_POINTER_OWN |  0 );
}
  
