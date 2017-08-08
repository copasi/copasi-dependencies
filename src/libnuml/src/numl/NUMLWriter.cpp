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

#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>

#include <numl/common/common.h>
#include <sbml/xml/XMLOutputStream.h>

#include <numl/NUMLError.h>
#include <numl/NUMLDocument.h>
#include <numl/NUMLWriter.h>
#include <numl/common/operationReturnValues.h>

#include <sbml/compress/CompressCommon.h>
#include <sbml/compress/OutputCompressor.h>
#include <sbml/SBMLWriter.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

/*
 * Creates a new NUMLWriter.
 */
NUMLWriter::NUMLWriter ()
{
}


/*
 * Destroys this NUMLWriter.
 */
NUMLWriter::~NUMLWriter ()
{
}


/*
 * Sets the name of this program. i.\ e.\ the one about to write out the
 * NUMLDocument.  If the program name and version are set
 * (setProgramVersion()), the following XML comment, intended for human
 * consumption, will be written at the beginning of the document:
 *
 *   <!-- Created by <program name> version <program version>
 *   on yyyy-MM-dd HH:mm with libnuml version <libnuml version>. -->
 */
int
NUMLWriter::setProgramName (const std::string& name)
{
  mProgramName = name;
  return LIBNUML_OPERATION_SUCCESS;
}


/*
 * Sets the version of this program. i.\ e.\ the one about to write out the
 * NUMLDocument.  If the program version and name are set
 * (setProgramName()), the following XML comment, intended for human
 * consumption, will be written at the beginning of the document:
 *
 *   <!-- Created by <program name> version <program version>
 *   on yyyy-MM-dd HH:mm with libnuml version <libnuml version>. -->
 */
int
NUMLWriter::setProgramVersion (const std::string& version)
{
  mProgramVersion = version;
  return LIBNUML_OPERATION_SUCCESS;
}


/*
 * Writes the given NUML document to filename.
 *
 * If the filename ends with @em .gz, the file will be compressed by @em gzip.
 * Similary, if the filename ends with @em .zip or @em .bz2, the file will be
 * compressed by @em zip or @em bzip2, respectively. Otherwise, the fill will be
 * uncompressed.
 * If the filename ends with @em .zip, a filename that will be added to the
 * zip archive file will end with @em .xml or @em .numl. For example, the filename
 * in the zip archive will be @em test.xml if the given filename is @em test.xml.zip
 * or @em test.zip. Also, the filename in the archive will be @em test.numl if the
 * given filename is @em test.numl.zip.
 *
 * @note To create a gzip/zip file, underlying libNUML needs to be linked with zlib at
 * compile time. Also, underlying libNUML needs to be linked with bzip2 to create a
 * bzip2 file.
 * File unwritable error will be logged and @c false will be returned if a compressed 
 * file name is given and underlying libNUML is not linked with the corresponding
 * required library.
 * NUMLWriter::hasZlib() and NUMLWriter::hasBzip2() can be used to check whether
 * underlying libNUML is linked with the library.
 *
 * @return true on success and false if the filename could not be opened
 * for writing.
 */
bool
NUMLWriter::writeNUML (const NUMLDocument* d, const std::string& filename)
{
  std::ostream* stream = NULL;

  try
  {
    // open an uncompressed XML file.
    if ( string::npos != filename.find(".xml", filename.length() - 4) )
    {
      stream = new(std::nothrow) std::ofstream(filename.c_str());
    }
    // open a gzip file
    else if ( string::npos != filename.find(".gz", filename.length() - 3) )
    {
     stream = LIBSBML_CPP_NAMESPACE_QUALIFIER OutputCompressor::openGzipOStream(filename);
    }
    // open a bz2 file
    else if ( string::npos != filename.find(".bz2", filename.length() - 4) )
    {
      stream = LIBSBML_CPP_NAMESPACE_QUALIFIER OutputCompressor::openBzip2OStream(filename);
    }
    // open a zip file
    else if ( string::npos != filename.find(".zip", filename.length() - 4) )
    {
      std::string filenameinzip = filename.substr(0, filename.length() - 4);
  
      if ( ( string::npos == filenameinzip.find(".xml",  filenameinzip.length() - 4) ) &&
           ( string::npos == filenameinzip.find(".numl", filenameinzip.length() - 5) )
         )
      {
        filenameinzip += ".xml";
      }


#if defined(WIN32) && !defined(CYGWIN)
      char sepr = '\\';
#else
      char sepr = '/';
#endif
      size_t spos = filenameinzip.rfind(sepr, filenameinzip.length() - 1);
      if( spos != string::npos )
      {
        filenameinzip = filenameinzip.substr(spos + 1, filenameinzip.length() - 1);
      }

      
      stream = LIBSBML_CPP_NAMESPACE_QUALIFIER OutputCompressor::openZipOStream(filename, filenameinzip);
    }
    else
    {
      stream = new(std::nothrow) std::ofstream(filename.c_str());
    }
  }
  catch ( LIBSBML_CPP_NAMESPACE_QUALIFIER ZlibNotLinked& /*zlib*/)
  {
    // libNUML is not linked with zlib.
    LIBSBML_CPP_NAMESPACE_QUALIFIER XMLErrorLog *log = (const_cast<NUMLDocument *>(d))->getErrorLog();
    std::ostringstream oss;
    oss << "Tried to write " << filename << ". Writing a gzip/zip file is not enabled because "
        << "underlying libNUML is not linked with zlib.";
    log->add(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLError( LIBSBML_CPP_NAMESPACE_QUALIFIER XMLFileUnwritable, oss.str(), 0, 0) );
    return false;
  } 
  catch ( LIBSBML_CPP_NAMESPACE_QUALIFIER Bzip2NotLinked& /*bz2*/)
  {
    // libNUML is not linked with bzip2.
    LIBSBML_CPP_NAMESPACE_QUALIFIER XMLErrorLog *log = (const_cast<NUMLDocument *>(d))->getErrorLog();
    std::ostringstream oss;
    oss << "Tried to write " << filename << ". Writing a bzip2 file is not enabled because "
        << "underlying libNUML is not linked with bzip2.";
    log->add(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLError( LIBSBML_CPP_NAMESPACE_QUALIFIER XMLFileUnwritable, oss.str(), 0, 0) );
    return false;
  } 


  if ( stream == NULL || stream->fail() || stream->bad())
  {
    NUMLErrorLog *log = (const_cast<NUMLDocument *>(d))->getErrorLog();
    log->logError(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLFileUnwritable);
    return false;
  }

   bool result = writeNUML(d, *stream);
   delete stream;

   return result;

}


/**
 * Writes the given NUML document to the output stream.
 *
 * @return true on success and false if one of the underlying parser
 * components fail (rare).
 */
bool
NUMLWriter::writeNUML (const NUMLDocument* d, std::ostream& stream)
{
  bool result = false;

  try
  {
    stream.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
    LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream xos(stream, "UTF-8", true, mProgramName,
                                               mProgramVersion);
    d->write(xos);
    stream << endl;

    result = true;
  }
  catch (ios_base::failure&)
  {
    NUMLErrorLog *log = (const_cast<NUMLDocument *>(d))->getErrorLog();
    log->logError(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLFileOperationError);
  }

  return result;
}


/*
 * Writes the given NUML document to an in-memory string and returns a
 * pointer to it.  The string is owned by the caller and should be freed
 * (with free()) when no longer needed.
 *
 * @return the string on success and 0 if one of the underlying parser
 * components fail (rare).
 */
LIBNUML_EXTERN
char*
NUMLWriter::writeToString (const NUMLDocument* d)
{
  ostringstream stream;
  writeNUML(d, stream);

  return LIBSBML_CPP_NAMESPACE_QUALIFIER safe_strdup( stream.str().c_str() );
}


/**
 * Predicate returning @c true or @c false depending on whether
 * underlying libNUML is linked with zlib.
 *
 * @return @c true if libNUML is linked with zlib, @c false otherwise.
 */
bool 
NUMLWriter::hasZlib()
{
  return LIBSBML_CPP_NAMESPACE_QUALIFIER SBMLWriter::hasZlib();
}


/**
 * Predicate returning @c true or @c false depending on whether
 * underlying libNUML is linked with bzip2.
 *
 * @return @c true if libNUML is linked with bzip2, @c false otherwise.
 */
bool 
NUMLWriter::hasBzip2()
{
  return LIBSBML_CPP_NAMESPACE_QUALIFIER SBMLWriter::hasBzip2();
}



/**
 * Creates a new NUMLWriter and returns a pointer to it.
 */
LIBNUML_EXTERN
NUMLWriter_t *
NUMLWriter_create ()
{
  return new(nothrow) NUMLWriter;
}


/**
 * Frees the given NUMLWriter.
 */
LIBNUML_EXTERN
void
NUMLWriter_free (NUMLWriter_t *sw)
{
  delete sw;
}


/**
 * Sets the name of this program. i.\ e.\  the one about to write out the
 * NUMLDocument.  If the program name and version are set
 * (setProgramVersion()), the following XML comment, intended for human
 * consumption, will be written at the beginning of the document:
 *
 *   <!-- Created by <program name> version <program version>
 *   on yyyy-MM-dd HH:mm with libnuml version <libnuml version>. -->
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif The possible values
 * returned by this function are:
 * @li LIBNUML_OPERATION_SUCCESS
 */
LIBNUML_EXTERN
int
NUMLWriter_setProgramName (NUMLWriter_t *sw, const char *name)
{
  return (name == NULL) ? sw->setProgramName("") : sw->setProgramName(name);
}


/**
 * Sets the version of this program. i.\ e.\ the one about to write out the
 * NUMLDocument.  If the program version and name are set
 * (setProgramName()), the following XML comment, intended for human
 * consumption, will be written at the beginning of the document:
 *
 *   <!-- Created by <program name> version <program version>
 *   on yyyy-MM-dd HH:mm with libnuml version <libnuml version>. -->
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif The possible values
 * returned by this function are:
 * @li LIBNUML_OPERATION_SUCCESS
 */
LIBNUML_EXTERN
int
NUMLWriter_setProgramVersion (NUMLWriter_t *sw, const char *version)
{
  return (version == NULL) ? sw->setProgramVersion("") :
                             sw->setProgramVersion(version);
}


/**
 * Writes the given NUML document to filename.
 *
 * If the filename ends with @em .gz, the file will be compressed by @em gzip.
 * Similary, if the filename ends with @em .zip or @em .bz2, the file will be
 * compressed by @em zip or @em bzip2, respectively. Otherwise, the fill will be
 * uncompressed.
 * If the filename ends with @em .zip, a filename that will be added to the
 * zip archive file will end with @em .xml or @em .numl. For example, the filename
 * in the zip archive will be @em test.xml if the given filename is @em test.xml.zip
 * or @em test.zip. Also, the filename in the archive will be @em test.numl if the
 * given filename is @em test.numl.zip.
 *
 * @note To create a gzip/zip file, libNUML needs to be linked with zlib at
 * compile time. Also, libNUML needs to be linked with bzip2 to create a bzip2 file.
 * File unwritable error will be logged and @c zero will be returned if a compressed 
 * file name is given and libNUML is not linked with the required library.
 * NUMLWriter_hasZlib() and NUMLWriter_hasBzip2() can be used to check whether
 * libNUML was linked with the library at compile time.
 *
 * @return non-zero on success and zero if the filename could not be opened
 * for writing.
 */
LIBNUML_EXTERN
int
NUMLWriter_writeNUML ( NUMLWriter_t         *sw,
                       const NUMLDocument_t *d,
                       const char           *filename )
{
  return static_cast<int>( sw->writeNUML(d, filename) );
}


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
NUMLWriter_writeNUMLToString (NUMLWriter_t *sw, const NUMLDocument_t *d)
{
  return sw->writeToString(d);
}


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * libNUML is linked with zlib at compile time.
 *
 * @return @c non-zero if zlib is linked, @c zero otherwise.
 */
LIBNUML_EXTERN
int
NUMLWriter_hasZlib ()
{
  return static_cast<int>( NUMLWriter::hasZlib() );
}


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * libNUML is linked with bzip2 at compile time.
 *
 * @return @c non-zero if bzip2 is linked, @c zero otherwise.
 */
LIBNUML_EXTERN
int
NUMLWriter_hasBzip2 ()
{
   return static_cast<int>( NUMLWriter::hasBzip2() );
}


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
writeNUML (const NUMLDocument_t *d, const char *filename)
{
  NUMLWriter sw;
  return static_cast<int>( sw.writeNUML(d, filename) );
}


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
writeNUMLToString (const NUMLDocument_t *d)
{
  NUMLWriter sw;
  return sw.writeToString(d);
}

LIBNUML_CPP_NAMESPACE_END
