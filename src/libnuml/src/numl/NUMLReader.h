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
 * @class NUMLReader
 * @brief The reader class implementing reading NuML documents from file / string
 */


#ifndef NUMLReader_h
#define NUMLReader_h


#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>
#include <sbml/util/util.h>





#include <string>

#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

class NUMLDocument;


class LIBNUML_EXTERN NUMLReader
{
public:

  /**
   * Creates a new NUMLReader and returns it.
   */
  NUMLReader ();


  /**
   * Destroys this NUMLReader.
   */
  virtual ~NUMLReader ();


  /**
   * Reads an NUML document from a file.
   *
   * If the file named @p filename does not exist or its content is not
   * valid NUML, one or more errors will be logged with the NUMLDocument
   * object returned by this method.  Callers can use the methods on
   * NUMLDocument such as NUMLDocument::getNumErrors() and
   * @if clike NUMLDocument::getError() @endif@if java NUMLDocument::getError(long n) @endif to get the errors.  The object returned by
   * @if clike NUMLDocument::getError() @endif@if java NUMLDocument::getError(long n) @endif is an NUMLError object, and it has methods to
   * get the error code, category, and severity level of the problem, as
   * well as a textual description of the problem.  The possible severity
   * levels range from informational messages to fatal errors; see the
   * documentation for NUMLError for more information.
   *
   * If the file @p filename could not be read, the file-reading error will
   * appear first.  The error code can provide a clue about what happened.
   * For example, a file might be unreadable (either because it does not
   * actually exist or because the user does not have the necessary access
   * priviledges to read it) or some sort of file operation error may have
   * bee reported by the underlying operating system.  Callers can check
   * for these situations using code such as the following:
   * @code
   * NUMLReader* reader = new NUMLReader();
   * NUMLDocument* doc  = reader.readNUML(filename);
   *
   * if (doc->getNumErrors() > 0)
   * {
   *   if (doc->getError(0)->getId() == XMLError::FileUnreadable)
   *   {
   *     // Handle case of unreadable file here.
   *   } 
   *   else if (doc->getError(0)->getId() == XMLError::FileOperationError)
   *   {
   *     // Handle case of other file error here.
   *   }
   *   else
   *   {
   *     // Handle other cases -- see error codes defined in XMLErrorCode_t
   *     // for other possible cases to check.
   *   }
   * }
   * @endcode
   *
   * If the given filename ends with the suffix @c ".gz" (for example, @c
   * "myfile.xml.gz"), the file is assumed to be compressed in @em gzip
   * format and will be automatically decompressed upon reading.
   * Similarly, if the given filename ends with @c ".zip" or @c ".bz2", the
   * file is assumed to be compressed in @em zip or @em bzip2 format
   * (respectively).  Files whose names lack these suffixes will be read
   * uncompressed.  Note that if the file is in @em zip format but the
   * archive contains more than one file, only the first file in the
   * archive will be read and the rest ignored.
   *
   * @param filename the name or full pathname of the file to be read.
   *
   * @return a pointer to the NUMLDocument created from the NUML content.
   *
   * @see NUMLError
   *
   * @note LibNUML versions 2.x and 3.x behave differently in error
   * handling in several respects.  One difference is how early some errors
   * are caught and whether libNUML continues processing a file in the face
   * of some early errors.  In general, libNUML 3.x stops parsing NUML
   * inputs sooner than libNUML 2.x in the face of XML errors because the
   * errors may invalidate any further NUML content.  For example, a
   * missing XML declaration at the beginning of the file was ignored by
   * libNUML 2.x but in version 3.x, it will cause libNUML to stop parsing
   * the rest of the input altogether.  While this behavior may seem more
   * severe and intolerant, it was necessary in order to provide uniform
   * behavior regardless of which underlying XML parser (Expat, Xerces,
   * libxml2) is being used by libNUML.  The XML parsers themselves behave
   * differently in their error reporting, and sometimes libNUML has to
   * resort to the lowest common denominator.
   *
   * @note To read a gzip/zip file, libNUML needs to be configured and
   * linked with the <a href="http://www.zlib.net/">zlib</a> library at
   * compile time.  It also needs to be linked with the <a
   * href="">bzip2</a> library to read files in @em bzip2 format.  (Both of
   * these are the default configurations for libNUML.)  Errors about
   * unreadable files will be logged if a compressed filename is given and
   * libNUML was @em not linked with the corresponding required library.
   *
   * @note NUMLReader::hasZlib() and NUMLReader::hasBzip2() can be used to
   * check whether libNUML has been linked with each library.
   */
  NUMLDocument* readNUML (const std::string& filename);


  /**
   * Reads an NUML document from a file.
   *
   * If the file named @p filename does not exist or its content is not
   * valid NUML, one or more errors will be logged with the NUMLDocument
   * object returned by this method.  Callers can use the methods on
   * NUMLDocument such as NUMLDocument::getNumErrors() and
   * @if clike NUMLDocument::getError() @endif@if java NUMLDocument::getError(long n) @endif to get the errors.  The object returned by
   * @if clike NUMLDocument::getError() @endif@if java NUMLDocument::getError(long n) @endif is an NUMLError object, and it has methods to
   * get the error code, category, and severity level of the problem, as
   * well as a textual description of the problem.  The possible severity
   * levels range from informational messages to fatal errors; see the
   * documentation for NUMLError for more information.
   *
   * If the file @p filename could not be read, the file-reading error will
   * appear first.  The error code can provide a clue about what happened.
   * For example, a file might be unreadable (either because it does not
   * actually exist or because the user does not have the necessary access
   * priviledges to read it) or some sort of file operation error may have
   * bee reported by the underlying operating system.  Callers can check
   * for these situations using code such as the following:
   * @code
   * NUMLReader* reader = new NUMLReader();
   * NUMLDocument* doc  = reader.readNUML(filename);
   *
   * if (doc->getNumErrors() > 0)
   * {
   *   if (doc->getError(0)->getId() == XMLError::FileUnreadable)
   *   {
   *     // Handle case of unreadable file here.
   *   } 
   *   else if (doc->getError(0)->getId() == XMLError::FileOperationError)
   *   {
   *     // Handle case of other file error here.
   *   }
   *   else
   *   {
   *     // Handle other cases -- see error codes defined in XMLErrorCode_t
   *     // for other possible cases to check.
   *   }
   * }
   * @endcode
   *
   * If the given filename ends with the suffix @c ".gz" (for example, @c
   * "myfile.xml.gz"), the file is assumed to be compressed in @em gzip
   * format and will be automatically decompressed upon reading.
   * Similarly, if the given filename ends with @c ".zip" or @c ".bz2", the
   * file is assumed to be compressed in @em zip or @em bzip2 format
   * (respectively).  Files whose names lack these suffixes will be read
   * uncompressed.  Note that if the file is in @em zip format but the
   * archive contains more than one file, only the first file in the
   * archive will be read and the rest ignored.
   *
   * @param filename the name or full pathname of the file to be read.
   *
   * @return a pointer to the NUMLDocument created from the NUML content.
   *
   * @see NUMLError
   *
   * @note LibNUML versions 2.x and 3.x behave differently in error
   * handling in several respects.  One difference is how early some errors
   * are caught and whether libNUML continues processing a file in the face
   * of some early errors.  In general, libNUML 3.x stops parsing NUML
   * inputs sooner than libNUML 2.x in the face of XML errors because the
   * errors may invalidate any further NUML content.  For example, a
   * missing XML declaration at the beginning of the file was ignored by
   * libNUML 2.x but in version 3.x, it will cause libNUML to stop parsing
   * the rest of the input altogether.  While this behavior may seem more
   * severe and intolerant, it was necessary in order to provide uniform
   * behavior regardless of which underlying XML parser (Expat, Xerces,
   * libxml2) is being used by libNUML.  The XML parsers themselves behave
   * differently in their error reporting, and sometimes libNUML has to
   * resort to the lowest common denominator.
   *
   * @note To read a gzip/zip file, libNUML needs to be configured and
   * linked with the <a href="http://www.zlib.net/">zlib</a> library at
   * compile time.  It also needs to be linked with the <a
   * href="">bzip2</a> library to read files in @em bzip2 format.  (Both of
   * these are the default configurations for libNUML.)  Errors about
   * unreadable files will be logged if a compressed filename is given and
   * libNUML was @em not linked with the corresponding required library.
   *
   * @note NUMLReader::hasZlib() and NUMLReader::hasBzip2() can be used to
   * check whether libNUML has been linked with each library.
   */
  NUMLDocument* readNUMLFromFile (const std::string& filename);


  /**
   * Reads an NUML document from the given XML string.
   *
   * This method is flexible with respect to the presence of an XML
   * declaration at the beginning of the string.  In particular, if the
   * string in @p xml does not begin with the XML declaration
   * <code>&lt;?xml version='1.0' encoding='UTF-8'?&gt;</code>, then this
   * method will automatically prepend the declaration to @p xml.
   *
   * This method will log a fatal error if the content given in the
   * parameter @p xml is not NUML.  See the method documentation for
   * @if clike NUMLReader::readNUML() @endif@if java NUMLReader::readNUML(String filename) @endif for an example of code for testing the returned
   * error code.
   *
   * @param xml a string containing a full NUML model
   *
   * @return a pointer to the NUMLDocument created from the NUML content.
   *
   * @if clike @see NUMLReader::readNUML() @endif@if java @see NUMLReader::readNUML(String filename) @endif
   */
  NUMLDocument* readNUMLFromString (const std::string& xml);


  /**
   * Predicate returning @c true or @c false depending on whether
   * libNUML is linked with zlib at compile time.
   *
   * @return @c true if zlib is linked, @c false otherwise.
   */
  static bool hasZlib();


  /**
   * Predicate returning @c true or @c false depending on whether
   * libNUML is linked with bzip2 at compile time.
   *
   * @return @c true if bzip2 is linked, @c false otherwise.
   */
  static bool hasBzip2();


protected:
  /**
   * Used by readNUML() and readNUMLFromString().
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
  NUMLDocument* readInternal (const char* content, bool isFile = true);

};

LIBNUML_CPP_NAMESPACE_END

#endif /* __cplusplus */

LIBNUML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


#ifndef SWIG


/**
 * Creates a new NUMLReader and returns it.  By default XML Schema
 * validation is off.
 */
LIBNUML_EXTERN
NUMLReader_t *
NUMLReader_create (void);

/**
 * Frees the given NUMLReader.
 */
LIBNUML_EXTERN
void
NUMLReader_free (NUMLReader_t *sr);


/**
 * Reads an NUML document from the given file.  If filename does not exist
 * or is not an NUML file, an error will be logged.  Errors can be
 * identified by their unique ids, e.g.:
 *
 * <code>
 *   NUMLReader_t   *sr;\n
 *   NUMLDocument_t *d;
 *
 *   sr = NUMLReader_create();
 *
 *   d = NUMLReader_readNUML(reader, filename);
 *
 *   if (NUMLDocument_getNumErrors(d) > 0)\n
 *   {\n
 *     if (XMLError_getId(NUMLDocument_getError(d, 0))
 *                                           == NUML_READ_ERROR_FILE_NOT_FOUND)\n
 *     if (XMLError_getId(NUMLDocument_getError(d, 0))
 *                                           == NUML_READ_ERROR_NOT_NUML)\n
 *   }\n
 * </code>
 *
 * If the given filename ends with the suffix @c ".gz" (for example, @c
 * "myfile.xml.gz"), the file is assumed to be compressed in @em gzip
 * format and will be automatically decompressed upon reading.
 * Similarly, if the given filename ends with @c ".zip" or @c ".bz2", the
 * file is assumed to be compressed in @em zip or @em bzip2 format
 * (respectively).  Files whose names lack these suffixes will be read
 * uncompressed.  Note that if the file is in @em zip format but the
 * archive contains more than one file, only the first file in the
 * archive will be read and the rest ignored.
 *
 * @note LibNUML versions 2.x and 3.x behave differently in error
 * handling in several respects.  One difference is how early some errors
 * are caught and whether libNUML continues processing a file in the face
 * of some early errors.  In general, libNUML 3.x stops parsing NUML
 * inputs sooner than libNUML 2.x in the face of XML errors because the
 * errors may invalidate any further NUML content.  For example, a
 * missing XML declaration at the beginning of the file was ignored by
 * libNUML 2.x but in version 3.x, it will cause libNUML to stop parsing
 * the rest of the input altogether.  While this behavior may seem more
 * severe and intolerant, it was necessary in order to provide uniform
 * behavior regardless of which underlying XML parser (Expat, Xerces,
 * libxml2) is being used by libNUML.  The XML parsers themselves behave
 * differently in their error reporting, and sometimes libNUML has to
 * resort to the lowest common denominator.
 *
 * @note To read a gzip/zip file, libNUML needs to be configured and
 * linked with the <a href="http://www.zlib.net/">zlib</a> library at
 * compile time.  It also needs to be linked with the <a
 * href="">bzip2</a> library to read files in @em bzip2 format.  (Both of
 * these are the default configurations for libNUML.)  Errors about
 * unreadable files will be logged if a compressed filename is given and
 * libNUML was @em not linked with the corresponding required library.
 *
 * @note NUMLReader::hasZlib() and NUMLReader::hasBzip2() can be used to
 * check whether libNUML has been linked with each library.
 *
 * @return a pointer to the NUMLDocument read.
 */
LIBNUML_EXTERN
NUMLDocument_t *
NUMLReader_readNUML (NUMLReader_t *sr, const char *filename);

LIBNUML_EXTERN
NUMLDocument_t *
NUMLReader_readNUMLFromFile (NUMLReader_t *sr, const char *filename);

/**
 * Reads an NUML document from the given XML string.
 *
 * If the string does not begin with XML declaration:
 *
 *   <?xml version='1.0' encoding='UTF-8'?>
 *
 * it will be prepended.
 *
 * This method will log a fatal error if the XML string is not NUML.  See
 * the method documentation for readNUML(filename) for example error
 * checking code.
 *
 * @return a pointer to the NUMLDocument read.
 */
LIBNUML_EXTERN
NUMLDocument_t *
NUMLReader_readNUMLFromString (NUMLReader_t *sr, const char *xml);


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * underlying libNUML is linked with..
 *
 * @return @c non-zero if libNUML is linked with zlib, @c zero otherwise.
 */
LIBNUML_EXTERN
int
NUMLReader_hasZlib ();


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * libNUML is linked with bzip2.
 *
 * @return @c non-zero if libNUML is linked with bzip2, @c zero otherwise.
 */
LIBNUML_EXTERN
int
NUMLReader_hasBzip2 ();

#endif  /* !SWIG */


/**
 * Reads an NUML document from the given file @p filename.
 *
 * If @p filename does not exist, or it is not an NUML file, an error will
 * be logged in the error log of the NUMLDocument object returned by this
 * method.  Calling programs can inspect this error log to determine
 * the nature of the problem.  Please refer to the definition of
 * NUMLDocument for more information about the error reporting mechanism.
 *
 * @return a pointer to the NUMLDocument read.
 */
LIBNUML_EXTERN
NUMLDocument_t *
readNUML (const char *filename);


LIBNUML_EXTERN
NUMLDocument_t *
readNUMLFromFile (const char *filename);


/**
 * Reads an NUML document from a string assumed to be in XML format.
 *
 * If the string does not begin with XML declaration,
 *@verbatim
<?xml version='1.0' encoding='UTF-8'?>
@endverbatim
 *
 * an XML declaration string will be prepended.
 *
 * This method will report an error if the given string @p xml is not NUML.
 * The error will be logged in the error log of the NUMLDocument object
 * returned by this method.  Calling programs can inspect this error log to
 * determine the nature of the problem.  Please refer to the definition of
 * NUMLDocument for more information about the error reporting mechanism.
 *
 * @return a pointer to the NUMLDocument read.
 */
LIBNUML_EXTERN
NUMLDocument_t *
readNUMLFromString (const char *xml);


END_C_DECLS
LIBNUML_CPP_NAMESPACE_END

#endif  /* NUMLReader_h */
