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

#include <numl/NUMLNamespaces.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

NUMLNamespaces::NUMLNamespaces(unsigned int level, unsigned int version)
{
  mLevel = level;
  mVersion = version;
  mNamespaces = new LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces();

  mNamespaces->add(NUML_XMLNS_L1);
}

NUMLNamespaces::~NUMLNamespaces()
{
  if (mNamespaces)
    delete mNamespaces;
}


/*
 * Copy constructor; creates a copy of a NUMLNamespaces.
 */
NUMLNamespaces::NUMLNamespaces(const NUMLNamespaces& orig) :
          mLevel    (orig.mLevel)
      ,   mVersion  (orig.mVersion) 
{
  if(orig.mNamespaces)
    this->mNamespaces = 
           new LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces(*const_cast<NUMLNamespaces&>(orig).mNamespaces);
  else
    this->mNamespaces = 0;
}



/*
 * Assignment operator for NUMLNamespaces.
 */
NUMLNamespaces&
NUMLNamespaces::operator=(const NUMLNamespaces& orig)
{
  if (&orig != this)
  {
    mLevel = orig.mLevel;
    mVersion = orig.mVersion;
    delete this->mNamespaces;
    if(orig.mNamespaces)
      this->mNamespaces = 
            new LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces(*const_cast<NUMLNamespaces&>(orig).mNamespaces);
    else
      this->mNamespaces = 0;
  }

  return *this;
}



/*
 * Creates and returns a deep copy of this NUMLNamespaces.
 */
NUMLNamespaces *
NUMLNamespaces::clone () const
{
  return new NUMLNamespaces(*this);
}


std::string 
NUMLNamespaces::getNUMLNamespaceURI(unsigned int level,
                                 unsigned int version)
{
	return NUML_XMLNS_L1;
}


unsigned int 
NUMLNamespaces::getLevel()
{
  return mLevel;
}


unsigned int 
NUMLNamespaces::getVersion()
{
  return mVersion;
}


LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces *
NUMLNamespaces::getNamespaces()
{
  return mNamespaces;
}

void
NUMLNamespaces::addNamespaces(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces * xmlns)
{
  if (xmlns == NULL)
    return;

  /* check whether the namespace already exists
   * add if it does not
   */
  for (int i = 0; i < xmlns->getLength(); i++)
  {
    if (!(mNamespaces->hasNS(xmlns->getURI(i), xmlns->getPrefix(i))))
    {
      mNamespaces->add(xmlns->getURI(i), xmlns->getPrefix(i));
    }
  }
}

void
NUMLNamespaces::setLevel(unsigned int level)
{
  mLevel = level;
}


void 
NUMLNamespaces::setVersion(unsigned int version)
{
  mVersion = version;
}


void 
NUMLNamespaces::setNamespaces(LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces * xmlns)
{
  delete mNamespaces;
  if (xmlns)
    mNamespaces = xmlns->clone();
  else
    mNamespaces = NULL;
}

/**
 * Creates a new NUMLNamespaces_t structure corresponding to the given NUML
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
 * @return NUMLNamespaces_t structure created
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

LIBNUML_EXTERN
NUMLNamespaces_t *
NUMLNamespaces_create(unsigned int level, unsigned int version)
{
  return new NUMLNamespaces(level, version);
}


/**
 * Get the NUML Level of this NUMLNamespaces_t structure.
 *
 * @param numlns the NUMLNamespaces_t structure to query
 *
 * @return the NUML Level of this NUMLNamespaces_t structure.
 */
LIBNUML_EXTERN
unsigned int
NUMLNamespaces_getLevel(NUMLNamespaces_t *numlns)
{
  return numlns->getLevel();
}


/**
 * Get the NUML Version of this NUMLNamespaces_t structure.
 *
 * @param numlns the NUMLNamespaces_t structure to query
 *
 * @return the NUML Version of this NUMLNamespaces_t structure.
 */
LIBNUML_EXTERN
unsigned int
NUMLNamespaces_getVersion(NUMLNamespaces_t *numlns)
{
  return numlns->getVersion();
}


/**
 * Get the NUML Version of this NUMLNamespaces_t structure.
 *
 * @param numlns the NUMLNamespaces_t structure to query
 *
 * @return the XMLNamespaces_t structure of this NUMLNamespaces_t structure.
 */
LIBNUML_EXTERN
LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces_t *
NUMLNamespaces_getNamespaces(NUMLNamespaces_t *numlns)
{
  return numlns->getNamespaces();
}


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
LIBNUML_EXTERN
const char *
NUMLNamespaces_getNUMLNamespaceURI(unsigned int level, unsigned int version)
{
  return NUMLNamespaces::getNUMLNamespaceURI(level, version).c_str();
}


/**
 * Add the XML namespaces list to the set of namespaces
 * within this NUMLNamespaces_t structure.
 * 
 * @param numlns the NUMLNamespaces_t structure to add to
 * @param xmlns the XML namespaces to be added.
 */
LIBNUML_EXTERN
void
NUMLNamespaces_addNamespaces(NUMLNamespaces_t *numlns,
                             LIBSBML_CPP_NAMESPACE_QUALIFIER XMLNamespaces_t * xmlns)
{
  numlns->addNamespaces(xmlns);
}


LIBNUML_CPP_NAMESPACE_END

