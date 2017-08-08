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
*
*/

/**
* @class NUMLWriter
* @brief Methods for writing NUML to files and text strings.
*
* The NUMLWriter class is the converse of NUMLReader, and provides the
* main interface for serializing NUML models into XML and writing the
* result to files and text strings.  The methods for writing NUML all take
* an NUMLDocument object and a destination.  They return a boolean value
* to indicate success or failure.
*/

#ifndef NUMLWriter_h
#define NUMLWriter_h


#include <numl/common/extern.h>
#include <numl/common/numlfwd.h>





#include <iosfwd>
#include <string>

#ifdef __cplusplus
LIBNUML_CPP_NAMESPACE_BEGIN

class NUMLDocument;


class LIBNUML_EXTERN NUMLWriter
{
public:

  /**
   * Creates a new NUMLWriter.
   */
  NUMLWriter  ();


  /**
   * Destroys this NUMLWriter.
   */
  ~NUMLWriter ();


  /**
   * Sets the name of this program, i.e., the program that is about to
   * write out the NUMLDocument.
   *
   * If the program name and version are set (setProgramVersion()), the
   * following XML comment, intended for human consumption, will be written
   * at the beginning of the document:
   * @verbatim
   <!-- Created by <program name> version <program version>
   on yyyy-MM-dd HH:mm with libnuml version <libnuml version>. -->
@endverbatim
   *
   * @param name the name of this program (where "this program" refers to
   * program in which libNUML is embedded, not libNUML itself!)
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBNUML_OPERATION_SUCCESS
   * 
   * @see setProgramVersion(const std::string& version)
   */
  int setProgramName (const std::string& name);


  /**
   * Sets the version of this program, i.e., the program that is about to
   * write out the NUMLDocument.
   *
   * If the program version and name are set (setProgramName()), the
   * following XML comment, intended for human consumption, will be written
   * at the beginning of the document:
   * @verbatim
   <!-- Created by <program name> version <program version>
   on yyyy-MM-dd HH:mm with libnuml version <libnuml version>. -->
@endverbatim
   *
   * @param version the version of this program (where "this program"
   * refers to program in which libNUML is embedded, not libNUML itself!)
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBNUML_OPERATION_SUCCESS
   *
   * @see setProgramName(const std::string& name)
   */
  int setProgramVersion (const std::string& version);


  /**
   * Writes the given NUML document to filename.
   *
   * If the given filename ends with the suffix @c ".gz" (for example, @c
   * "myfile.xml.gz"), libNUML assumes the caller wants the file to be
   * written compressed in @em gzip.  Similarly, if the given filename ends
   * with @c ".zip" or @c ".bz2", libNUML assumes the caller wants the file
   * to be compressed in @em zip or @em bzip2 format (respectively).  Files
   * whose names lack these suffixes will be written uncompressed.
   * <em>Special considerations for the zip format</em>: If the given
   * filename ends with @c ".zip", the file placed in the zip archive will
   * have the suffix @c ".xml" or @c ".numl".  For example, the file in
   * the zip archive will be named @c "test.xml" if the given filename is
   * @c "test.xml.zip" or @c "test.zip".  Similarly, the filename in the
   * archive will be @c "test.numl" if the given filename is @c
   * "test.numl.zip".
   *
   * @note To write a gzip/zip file, libNUML needs to be configured and
   * linked with the <a href="http://www.zlib.net/">zlib</a> library at
   * compile time.  It also needs to be linked with the <a
   * href="">bzip2</a> library to write files in @em bzip2 format.  (Both
   * of these are the default configurations for libNUML.)  Errors about
   * unreadable files will be logged and this method will return @c false
   * if a compressed filename is given and libNUML was @em not linked with
   * the corresponding required library.
   *
   * @note NUMLReader::hasZlib() and NUMLReader::hasBzip2() can be used to
   * check whether libNUML has been linked with each library.
   *
   * @param d the NUML document to be written
   *
   * @param filename the name or full pathname of the file where the NUML
   * is to be written. 
   *
   * @return @c true on success and @c false if the filename could not be
   * opened for writing.
   */
  bool writeNUML (const NUMLDocument* d, const std::string& filename);


  /**
   * Writes the given NUML document to the output stream.
   *
   * @param d the NUML document to be written
   *
   * @param stream the stream object where the NUML is to be written.
   *
   * @return @c true on success and @c false if one of the underlying
   * parser components fail (rare).
   */
  bool writeNUML (const NUMLDocument* d, std::ostream& stream);


  /**
   * Writes the given NUML document to an in-memory string and returns a
   * pointer to it.
   *
   * The string is owned by the caller and should be freed (with @c free())
   * when no longer needed.
   *
   * @param d the NUML document to be written
   *
   * @return the string on success and @c 0 if one of the underlying parser
   * components fail.
   */
  char* writeToString (const NUMLDocument* d);


  /**
   * Predicate returning @c true or @c false depending on whether
   * underlying libNUML is linked with zlib.
   *
   * LibNUML supports reading and writing files compressed with either
   * bzip2 or zip/gzip compression.  The facility depends on libNUML having
   * been compiled with the necessary support libraries.  This method
   * allows a calling program to inquire whether that is the case for the
   * copy of libNUML it is running.
   *
   * @return @c true if libNUML is linked with zlib, @c false otherwise.
   */
  static bool hasZlib();


  /**
   * Predicate returning @c true or @c false depending on whether
   * underlying libNUML is linked with bzip2.
   *
   * LibNUML supports reading and writing files compressed with either
   * bzip2 or zip/gzip compression.  The facility depends on libNUML having
   * been compiled with the necessary support libraries.  This method
   * allows a calling program to inquire whether that is the case for the
   * copy of libNUML it is running.
   *
   * @return @c true if libNUML is linked with bzip2, @c false otherwise.
   */
  static bool hasBzip2();


 protected:

  std::string mProgramName;
  std::string mProgramVersion;

};

LIBNUML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


LIBNUML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


#ifndef SWIG


/**
 * Creates a new NUMLWriter and returns a pointer to it.
 */
LIBNUML_EXTERN
NUMLWriter_t *
NUMLWriter_create (void);

/**
 * Frees the given NUMLWriter.
 */
LIBNUML_EXTERN
void
NUMLWriter_free (NUMLWriter_t *sw);

/**
 * Sets the name of this program. i.\ e.\ the one about to write out the
 * NUMLDocument.  If the program name and version are set
 * (setProgramVersion()), the following XML comment, intended for human
 * consumption, will be written at the beginning of the document:
 *
 *   <!-- Created by <program name> version <program version>
 *   on yyyy-MM-dd HH:mm with libnuml version <libnuml version>. -->
 */
LIBNUML_EXTERN
int
NUMLWriter_setProgramName (NUMLWriter_t *sw, const char *name);

/**
 * Sets the version of this program. i.\ e.\ the one about to write out the
 * NUMLDocument.  If the program version and name are set
 * (setProgramName()), the following XML comment, intended for human
 * consumption, will be written at the beginning of the document:
 *
 *   <!-- Created by <program name> version <program version>
 *   on yyyy-MM-dd HH:mm with libnuml version <libnuml version>. -->
 */
LIBNUML_EXTERN
int
NUMLWriter_setProgramVersion (NUMLWriter_t *sw, const char *version);

/**
 * Writes the given NUML document to filename.
 *
 * If the given filename ends with the suffix @c ".gz" (for example, @c
 * "myfile.xml.gz"), libNUML assumes the caller wants the file to be
 * written compressed in @em gzip.  Similarly, if the given filename ends
 * with @c ".zip" or @c ".bz2", libNUML assumes the caller wants the file
 * to be compressed in @em zip or @em bzip2 format (respectively).  Files
 * whose names lack these suffixes will be written uncompressed.
 * <em>Special considerations for the zip format</em>: If the given
 * filename ends with @c ".zip", the file placed in the zip archive will
 * have the suffix @c ".xml" or @c ".numl".  For example, the file in
 * the zip archive will be named @c "test.xml" if the given filename is
 * @c "test.xml.zip" or @c "test.zip".  Similarly, the filename in the
 * archive will be @c "test.numl" if the given filename is @c
 * "test.numl.zip".
 *
 * @note To write a gzip/zip file, libNUML needs to be configured and
 * linked with the <a href="http://www.zlib.net/">zlib</a> library at
 * compile time.  It also needs to be linked with the <a
 * href="">bzip2</a> library to write files in @em bzip2 format.  (Both
 * of these are the default configurations for libNUML.)  Errors about
 * unreadable files will be logged and this method will return @c false
 * if a compressed filename is given and libNUML was @em not linked with
 * the corresponding required library.
 *
 * @note NUMLReader::hasZlib() and NUMLReader::hasBzip2() can be used to
 * check whether libNUML has been linked with each library.
 *
 * @return non-zero on success and zero if the filename could not be opened
 * for writing.
 */
LIBNUML_EXTERN
int
NUMLWriter_writeNUML ( NUMLWriter_t         *sw,
                       const NUMLDocument_t *d,
                       const char           *filename );

/**
 * Writes the given NUML document to an in-memory string and returns a
 * pointer to it.  The string is owned by the caller and should be freed
 * (with free()) when no longer needed.
 *
 * @return the string on success and NULL if one of the underlying parser
 * components fail (rare).
 */
LIBNUML_EXTERN
char *
NUMLWriter_writeNUMLToString (NUMLWriter_t *sw, const NUMLDocument_t *d);


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * libNUML is linked with zlib at compile time.
 *
 * @return @c non-zero if zlib is linked, @c zero otherwise.
 */
LIBNUML_EXTERN
int
NUMLWriter_hasZlib ();


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * libNUML is linked with bzip2 at compile time.
 *
 * @return @c non-zero if bzip2 is linked, @c zero otherwise.
 */
LIBNUML_EXTERN
int
NUMLWriter_hasBzip2 ();

#endif  /* !SWIG */


/**
 * Writes the given NUML document to filename.  This convenience function
 * is functionally equivalent to:
 *
 *   NUMLWriter_writeNUML(NUMLWriter_create(), d, filename);
 *
 * @return non-zero on success and zero if the filename could not be opened
 * for writing.
 */
LIBNUML_EXTERN
int
writeNUML (const NUMLDocument_t *d, const char *filename);


/**
 * Writes the given NUML document to an in-memory string and returns a
 * pointer to it.  The string is owned by the caller and should be freed
 * (with free()) when no longer needed.  This convenience function is
 * functionally equivalent to:
 *
 *   NUMLWriter_writeNUMLToString(NUMLWriter_create(), d);
 *
 * @return the string on success and NULL if one of the underlying parser
 * components fail (rare).
 */
LIBNUML_EXTERN
char *
writeNUMLToString (const NUMLDocument_t *d);


END_C_DECLS
LIBNUML_CPP_NAMESPACE_END

#endif  /* NUMLWriter_h */
