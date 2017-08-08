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
 * @class NUMLNamespaces
 * @brief namespace class
 */


#ifndef NUMLNamespaces_h
#define NUMLNamespaces_h

#include <numl/common/extern.h>
#include <sbml/xml/XMLNamespaces.h>

#include <numl/common/common.h>
#include <numl/common/numlfwd.h>



#ifdef __cplusplus



LIBNUML_CPP_NAMESPACE_BEGIN

const unsigned int NUML_DEFAULT_LEVEL = 1;
const unsigned int NUML_DEFAULT_VERSION = 1;
const char* const  NUML_XMLNS_L1 = "http://www.numl.org/numl/level1/version1";
const char* const  NUML_XMLNS_L1V1 = "http://www.numl.org/numl/level1/version1";

class LIBNUML_EXTERN NUMLNamespaces
{
public:

  /**
   * Creates a new NUMLNamespaces object corresponding to the given NUML
   * @p level and @p version.
   *
   * NUMLNamespaces objects are used in libNUML to communicate NUML Level
   * and Version data between constructors and other methods.  The
   * NUMLNamespaces object class tracks 3-tuples (triples) consisting of
   * NUML Level, Version, and the corresponding NUML XML namespace.  Most
   * constructors for NUML objects in libNUML take a NUMLNamespaces object
   * as an argument, thereby allowing the constructor to produce the proper
   * combination of attributes and other internal data structures for the
   * given NUML Level and Version.
   *
   * The plural name "NUMLNamespaces" is not a mistake, because in NUML
   * Level&nbsp;3, objects may have extensions added by Level&nbsp;3
   * packages used by a given model; however, until the introduction of
   * NUML Level&nbsp;3, the NUMLNamespaces object only records one NUML
   * Level/Version/namespace combination at a time.
   *
   * @param level the NUML level
   * @param version the NUML version
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
  NUMLNamespaces(unsigned int level = NUML_DEFAULT_LEVEL,
                 unsigned int version = NUML_DEFAULT_VERSION);

  
  /**
   * Destroys this NUMLNamespaces object.
   */
  ~NUMLNamespaces();

  
  /**
   * Copy constructor; creates a copy of a NUMLNamespaces.
   * 
   * @param orig the NUMLNamespaces instance to copy.
   */
  NUMLNamespaces(const NUMLNamespaces& orig);


  /**
   * Assignment operator for NUMLNamespaces.
   */
  NUMLNamespaces& operator=(const NUMLNamespaces& orig);


  /**
   * Creates and returns a deep copy of this NUMLNamespaces.
   * 
   * @return a (deep) copy of this NUMLNamespaces.
   */
  NUMLNamespaces* clone () const;


  /**
   * Returns a string representing the NUML XML namespace for the
   * given @p level and @p version of NUML.
   *
   * @param level the NUML level
   * @param version the NUML version
   *
   * @return a string representing the NUML namespace that reflects the
   * NUML Level and Version specified.
   */
  static std::string getNUMLNamespaceURI(unsigned int level,
                                            unsigned int version);
  
  
  /**
   * Get the NUML Level of this NUMLNamespaces object.
   *
   * @return the NUML Level of this NUMLNamespaces object.
   */
  unsigned int getLevel();



  /**
   * Get the NUML Version of this NUMLNamespaces object.
   *
   * @return the NUML Version of this NUMLNamespaces object.
   */
  unsigned int getVersion();



  /**
   * Get the XML namespaces list for this NUMLNamespaces object.
   * 
   * The plural is not a mistake, because in NUML Level&nbsp;3, objects may
   * have extensions added by Level&nbsp;3 packages used by a given model,
   * and therefore there may be multiple XML namespaces involved too.
   * However, until the introduction of NUML Level&nbsp;3, the
   * NUMLNamespaces object only records one NUML Level/Version/namespace
   * combination at a time, and so this method will also only return
   * a list of one item.
   *
   * @return the XML namespaces of this NUMLNamespaces object.
   */
  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces * getNamespaces();


  /**
   * Add the XML namespaces list to the set of namespaces
   * within this NUMLNamespaces object.
   * 
   * @param xmlns the XML namespaces to be added.
   */
  void addNamespaces(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces * xmlns);


  void setLevel(unsigned int level);


  void setVersion(unsigned int version);


  void setNamespaces(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces * xmlns);

protected:  

  unsigned int    mLevel;
  unsigned int    mVersion;
  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces * mNamespaces;

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
NUMLNamespaces_t *
NUMLNamespaces_create(unsigned int level, unsigned int version);


LIBNUML_EXTERN
unsigned int
NUMLNamespaces_getLevel(NUMLNamespaces_t *numlns);


LIBNUML_EXTERN
unsigned int
NUMLNamespaces_getVersion(NUMLNamespaces_t *numlns);


LIBNUML_EXTERN
LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces_t *
NUMLNamespaces_getNamespaces(NUMLNamespaces_t *numlns);


LIBNUML_EXTERN
const char *
NUMLNamespaces_getNUMLNamespaceURI(unsigned int level, unsigned int version);


LIBNUML_EXTERN
void
NUMLNamespaces_addNamespaces(NUMLNamespaces_t *numlns,
                             LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces_t * xmlns);


END_C_DECLS
LIBNUML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* NUMLNamespaces_h */
