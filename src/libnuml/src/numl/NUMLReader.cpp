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

#include <sbml/xml/XMLError.h>
#include <sbml/xml/XMLErrorLog.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/SBMLReader.h>

#include <numl/NUMLErrorLog.h>
#include <numl/NUMLVisitor.h>
#include <numl/NUMLDocument.h>
#include <numl/NUMLError.h>
#include <numl/NUMLReader.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

/*
 * Creates a new NUMLReader and returns it.
 */
NUMLReader::NUMLReader ()
{
}


/*
 * Destorys this NUMLReader.
 */
NUMLReader::~NUMLReader ()
{
}


/*
 * Reads an NUML document from the given file.  If filename does not exist
 * or is not an NUML file, an error will be logged.  Errors can be
 * identified by their unique ids, e.g.:
 *
 * <code>
 *   NUMLDocument* d = reader.readNUML(filename);
 *
 *   if (d->getNumErrors() > 0)\n
 *   {\n
 *     if (d->getError(0)->getId() == NUML_READ_ERROR_FILE_NOT_FOUND)\n
 *     if (d->getError(0)->getId() == NUML_READ_ERROR_NOT_NUML)\n
 *   }\n
 * </code>
 *
 * If the filename ends with @em .gz, the file will be read as a @em gzip file.
 * Similary, if the filename ends with @em .zip or @em .bz2, the file will be
 * read as a @em zip or @em bzip2 file, respectively. Otherwise, the fill will be
 * read as an uncompressed file.
 * If the filename ends with @em .zip, only the first file in the archive will
 * be read if the zip archive contains two or more files.
 *
 * To read a gzip/zip file, underlying libNUML needs to be linked with zlib
 * at compile time. Also, underlying libNUML needs to be linked with bzip2
 * to read a bzip2 file. File unreadable error will be logged if a compressed 
 * file name is given and underlying libNUML is not linked with the corresponding
 * required library.
 * NUMLReader::hasZlib() and NUMLReader::hasBzip2() can be used to check
 * whether libNUML is linked with each library.
 * 
 * @return a pointer to the NUMLDocument read.
 */
NUMLDocument*
NUMLReader::readNUML (const std::string& filename)
{
  return readInternal(filename.c_str(), true);
}


/*
 * Reads an NUML document from the given XML string.
 */
NUMLDocument*
NUMLReader::readNUMLFromFile (const std::string& filename)
{

  return readInternal(filename.c_str(), true);
}


/*
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
NUMLDocument*
NUMLReader::readNUMLFromString (const std::string& xml)
{
  return readInternal(xml.c_str(), false);
}


/**
 * Predicate returning @c true or @c false depending on whether
 * libNUML is linked with zlib.
 *
 * @return @c true if libNUML is linked with zlib, @c false otherwise.
 */
bool 
NUMLReader::hasZlib()
{
  return LIBSBML_CPP_NAMESPACE_QUALIFIER SBMLReader::hasZlib();
}


/**
 * Predicate returning @c true or @c false depending on whether
 * libNUML is linked with bzip2.
 *
 * @return @c true if libNUML is linked with bzip2, @c false otherwise.
 */
bool 
NUMLReader::hasBzip2()
{
  return LIBSBML_CPP_NAMESPACE_QUALIFIER SBMLReader::hasBzip2();
}


static bool
isCriticalError(const unsigned int errorId)
{
  switch (errorId)
  {
  case LIBSBML_CPP_NAMESPACE_QUALIFIER  InternalXMLParserError:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER  UnrecognizedXMLParserCode:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER  XMLTranscoderError:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER BadlyFormedXML:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER InvalidXMLConstruct:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER UnclosedXMLToken:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER XMLTagMismatch:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER BadXMLPrefix:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER MissingXMLAttributeValue:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER BadXMLComment:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER XMLUnexpectedEOF:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER UninterpretableXMLContent:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER BadXMLDocumentStructure:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER InvalidAfterXMLContent:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER XMLExpectedQuotedString:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER XMLEmptyValueNotPermitted:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER MissingXMLElements:
  case LIBSBML_CPP_NAMESPACE_QUALIFIER BadXMLDeclLocation:
    return true;

  default:
    return false;
  }
}



/*
 * Used by readNUML() and readNUMLFromString().
 */
NUMLDocument*
NUMLReader::readInternal (const char* content, bool isFile)
{
  NUMLDocument* d = new NUMLDocument();

  if (isFile && content && (LIBSBML_CPP_NAMESPACE_QUALIFIER  util_file_exists(content) == false))
  {
    if(d->getErrorLog() != NULL)
    d->getErrorLog()->logError(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLFileUnreadable);
  }
  else
  {
    LIBSBML_CPP_NAMESPACE_QUALIFIER XMLInputStream stream(content, isFile, "", d->getErrorLog());

    d->read(stream);
    
    if (stream.isError())
    {
      // If we encountered an error, some parsers will report it sooner
      // than others.  Unfortunately, those that fail sooner do it in an
      // opaque call, so we can't change the behavior.  Since we don't want
      // different parsers to report different validation errors, we bring
      // all parsers back to the same point.

  ///TODO    d->setModel(NULL);

      if(d->getErrorLog() != NULL)
      for (unsigned int i = 0; i < d->getNumErrors(); ++i)
      {
        if (isCriticalError(d->getError(i)->getErrorId()))
        {
          // If we find even one critical error, all other errors are
          // suspect and may be bogus.  Remove them.

          for (int n = d->getNumErrors()-1; n >= 0; n--)      
            if (!isCriticalError(d->getError(n)->getErrorId()))
            {
              d->getErrorLog()->remove(d->getError(n)->getErrorId());
            }

          break;
        }
      }
    }
    else if(d->getErrorLog() != NULL)
    {
      // Low-level XML errors will have been caught in the first read,
      // before we even attempt to interpret the content as NUML.  Here
      // we want to start checking some basic NUML-level errors.

      if (stream.getEncoding() == "")
      {
        d->getErrorLog()->logError(LIBSBML_CPP_NAMESPACE_QUALIFIER MissingXMLEncoding);
      }
      else if (LIBSBML_CPP_NAMESPACE_QUALIFIER strcmp_insensitive(stream.getEncoding().c_str(), "UTF-8") != 0)
      {
        d->getErrorLog()->logError(NUMLNotUTF8);
      }

      if (stream.getVersion() == "")
      {
        d->getErrorLog()->logError(LIBSBML_CPP_NAMESPACE_QUALIFIER BadXMLDecl);
      }
      else if (LIBSBML_CPP_NAMESPACE_QUALIFIER strcmp_insensitive(stream.getVersion().c_str(), "1.0") != 0)
      {
        d->getErrorLog()->logError(LIBSBML_CPP_NAMESPACE_QUALIFIER BadXMLDecl);
      }

      if(d->getOntologyTerms() ==0)
      {
        d->getErrorLog()->logError(NUMLMissingOntologyTerms);
      }
      else if (d->getResultComponents() == 0)
      {
        d->getErrorLog()->logError(NUMLMissingResultComponents);
      }
      else if (d->getLevel() == 1)
      {

	// In Level 1, some elements were required, check if they are available.

      }
    }
  }
  return d;
}




/**
 * Creates a new NUMLReader and returns it.
 */
LIBNUML_EXTERN
NUMLReader_t *
NUMLReader_create ()
{
  return new (nothrow) NUMLReader;
}


/**
 * Frees the given NUMLReader.
 */
LIBNUML_EXTERN
void
NUMLReader_free (NUMLReader_t *sr)
{
  delete sr;
}


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
 * @return a pointer to the NUMLDocument read.
 */
LIBNUML_EXTERN
NUMLDocument_t *
NUMLReader_readNUML (NUMLReader_t *sr, const char *filename)
{
  return (filename != NULL) ? sr->readNUML(filename) : sr->readNUML("");
}


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
 * @return a pointer to the NUMLDocument read.
 */
LIBNUML_EXTERN
NUMLDocument_t *
NUMLReader_readNUMLFromFile (NUMLReader_t *sr, const char *filename)
{
  return (filename != NULL) ? sr->readNUML(filename) : sr->readNUML("");
}


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
NUMLReader_readNUMLFromString (NUMLReader_t *sr, const char *xml)
{
  return (xml != NULL) ? sr->readNUMLFromString(xml) :
                         sr->readNUMLFromString("");
}


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * underlying libNUML is linked with zlib at compile time.
 *
 * @return @c non-zero if libNUML is linked with zlib, @c zero otherwise.
 */
LIBNUML_EXTERN
int
NUMLReader_hasZlib ()
{
  return static_cast<int>( NUMLReader::hasZlib() );
}


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * underlying libNUML is linked with bzip2 at compile time.
 *
 * @return @c non-zero if libNUML is linked with bzip2, @c zero otherwise.
 */
LIBNUML_EXTERN
int
NUMLReader_hasBzip2 ()
{
  return static_cast<int>( NUMLReader::hasBzip2() );
}


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
 * If the filename ends with @em .gz, the file will be read as a @em gzip file.
 * Similary, if the filename ends with @em .zip or @em .bz2, the file will be
 * read as a @em zip or @em bzip2 file, respectively. Otherwise, the fill will be
 * read as an uncompressed file.
 * If the filename ends with @em .zip, only the first file in the archive will
 * be read if the zip archive contains two or more files.
 *
 * To read a gzip/zip file, underlying libNUML needs to be linked with zlib
 * at compile time. Also, underlying libNUML needs to be linked with bzip2
 * to read a bzip2 file. File unreadable error will be logged if a compressed 
 * file name is given and underlying libNUML is not linked with the corresponding
 * required library.
 * NUMLReader_hasZlib() and NUMLReader_hasBzip2() can be used to check
 * whether libNUML is linked with each library.
 *
 * @return a pointer to the NUMLDocument read.
 */
LIBNUML_EXTERN
NUMLDocument_t *
readNUML (const char *filename)
{
  NUMLReader sr;
  return sr.readNUML(filename);
}


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
 * If the filename ends with @em .gz, the file will be read as a @em gzip file.
 * Similary, if the filename ends with @em .zip or @em .bz2, the file will be
 * read as a @em zip or @em bzip2 file, respectively. Otherwise, the fill will be
 * read as an uncompressed file.
 * If the filename ends with @em .zip, only the first file in the archive will
 * be read if the zip archive contains two or more files.
 *
 * To read a gzip/zip file, underlying libNUML needs to be linked with zlib
 * at compile time. Also, underlying libNUML needs to be linked with bzip2
 * to read a bzip2 file. File unreadable error will be logged if a compressed 
 * file name is given and underlying libNUML is not linked with the corresponding
 * required library.
 * NUMLReader_hasZlib() and NUMLReader_hasBzip2() can be used to check
 * whether libNUML is linked with each library.
 *
 * @return a pointer to the NUMLDocument read.
 */
LIBNUML_EXTERN
NUMLDocument_t *
readNUMLFromFile (const char *filename)
{
  NUMLReader sr;
  return sr.readNUML(filename);
}


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
readNUMLFromString (const char *xml)
{
  NUMLReader sr;
  return sr.readNUMLFromString(xml);
}

LIBNUML_CPP_NAMESPACE_END


