/**
 * @file ListOfCoordinateComponents.cpp
 * @brief Implementation of the ListOfCoordinateComponents class.
 * @author SBMLTeam
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML. Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2020 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *     3. University College London, London, UK
 *
 * Copyright (C) 2019 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 * 3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 * Pasadena, CA, USA
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation. A copy of the license agreement is provided in the
 * file named "LICENSE.txt" included with this software distribution and also
 * available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 */
#include <sbml/packages/spatial/sbml/ListOfCoordinateComponents.h>
#include <sbml/packages/spatial/validator/SpatialSBMLError.h>


using namespace std;



LIBSBML_CPP_NAMESPACE_BEGIN




#ifdef __cplusplus


/*
 * Creates a new ListOfCoordinateComponents using the given SBML Level, Version
 * and &ldquo;spatial&rdquo; package version.
 */
ListOfCoordinateComponents::ListOfCoordinateComponents(unsigned int level,
                                                       unsigned int version,
                                                       unsigned int pkgVersion)
  : ListOf(level, version)
{
  setSBMLNamespacesAndOwn(new SpatialPkgNamespaces(level, version,
    pkgVersion));
}


/*
 * Creates a new ListOfCoordinateComponents using the given
 * SpatialPkgNamespaces object.
 */
ListOfCoordinateComponents::ListOfCoordinateComponents(SpatialPkgNamespaces
  *spatialns)
  : ListOf(spatialns)
{
  setElementNamespace(spatialns->getURI());
}


/*
 * Copy constructor for ListOfCoordinateComponents.
 */
ListOfCoordinateComponents::ListOfCoordinateComponents(const
  ListOfCoordinateComponents& orig)
  : ListOf( orig )
{
}


/*
 * Assignment operator for ListOfCoordinateComponents.
 */
ListOfCoordinateComponents&
ListOfCoordinateComponents::operator=(const ListOfCoordinateComponents& rhs)
{
  if (&rhs != this)
  {
    ListOf::operator=(rhs);
  }

  return *this;
}


/*
 * Creates and returns a deep copy of this ListOfCoordinateComponents object.
 */
ListOfCoordinateComponents*
ListOfCoordinateComponents::clone() const
{
  return new ListOfCoordinateComponents(*this);
}


/*
 * Destructor for ListOfCoordinateComponents.
 */
ListOfCoordinateComponents::~ListOfCoordinateComponents()
{
}


/*
 * Get a CoordinateComponent from the ListOfCoordinateComponents.
 */
CoordinateComponent*
ListOfCoordinateComponents::get(unsigned int n)
{
  return static_cast<CoordinateComponent*>(ListOf::get(n));
}


/*
 * Get a CoordinateComponent from the ListOfCoordinateComponents.
 */
const CoordinateComponent*
ListOfCoordinateComponents::get(unsigned int n) const
{
  return static_cast<const CoordinateComponent*>(ListOf::get(n));
}


/*
 * Get a CoordinateComponent from the ListOfCoordinateComponents based on its
 * identifier.
 */
CoordinateComponent*
ListOfCoordinateComponents::get(const std::string& sid)
{
  return const_cast<CoordinateComponent*>(static_cast<const
    ListOfCoordinateComponents&>(*this).get(sid));
}


/*
 * Get a CoordinateComponent from the ListOfCoordinateComponents based on its
 * identifier.
 */
const CoordinateComponent*
ListOfCoordinateComponents::get(const std::string& sid) const
{
  vector<SBase*>::const_iterator result;
  result = find_if(mItems.begin(), mItems.end(),
    IdEq<CoordinateComponent>(sid));
  return (result == mItems.end()) ? 0 : static_cast <const
    CoordinateComponent*> (*result);
}


/*
 * Removes the nth CoordinateComponent from this ListOfCoordinateComponents and
 * returns a pointer to it.
 */
CoordinateComponent*
ListOfCoordinateComponents::remove(unsigned int n)
{
  return static_cast<CoordinateComponent*>(ListOf::remove(n));
}


/*
 * Removes the CoordinateComponent from this ListOfCoordinateComponents based
 * on its identifier and returns a pointer to it.
 */
CoordinateComponent*
ListOfCoordinateComponents::remove(const std::string& sid)
{
  SBase* item = NULL;
  vector<SBase*>::iterator result;

  result = find_if(mItems.begin(), mItems.end(),
    IdEq<CoordinateComponent>(sid));

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <CoordinateComponent*> (item);
}


/*
 * Adds a copy of the given CoordinateComponent to this
 * ListOfCoordinateComponents.
 */
int
ListOfCoordinateComponents::addCoordinateComponent(const CoordinateComponent*
  cc)
{
  if (cc == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (cc->hasRequiredAttributes() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (cc->hasRequiredElements() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != cc->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (matchesRequiredSBMLNamespacesForAddition(static_cast<const
    SBase*>(cc)) == false)
  {
    return LIBSBML_NAMESPACES_MISMATCH;
  }
  else
  {
    return append(cc);
  }
}


/*
 * Get the number of CoordinateComponent objects in this
 * ListOfCoordinateComponents.
 */
unsigned int
ListOfCoordinateComponents::getNumCoordinateComponents() const
{
  return size();
}


/*
 * Creates a new CoordinateComponent object, adds it to this
 * ListOfCoordinateComponents object and returns the CoordinateComponent object
 * created.
 */
CoordinateComponent*
ListOfCoordinateComponents::createCoordinateComponent()
{
  CoordinateComponent* cc = NULL;

  try
  {
    SPATIAL_CREATE_NS(spatialns, getSBMLNamespaces());
    cc = new CoordinateComponent(spatialns);
    delete spatialns;
  }
  catch (...)
  {
  }

  if (cc != NULL)
  {
    appendAndOwn(cc);
  }

  return cc;
}


/*
 * Returns the XML element name of this ListOfCoordinateComponents object.
 */
const std::string&
ListOfCoordinateComponents::getElementName() const
{
  static const string name = "listOfCoordinateComponents";
  return name;
}


/*
 * Returns the libSBML type code for this ListOfCoordinateComponents object.
 */
int
ListOfCoordinateComponents::getTypeCode() const
{
  return SBML_LIST_OF;
}


/*
 * Returns the libSBML type code for the SBML objects contained in this
 * ListOfCoordinateComponents object.
 */
int
ListOfCoordinateComponents::getItemTypeCode() const
{
  return SBML_SPATIAL_COORDINATECOMPONENT;
}



/** @cond doxygenLibsbmlInternal */

/*
 * Creates a new CoordinateComponent in this ListOfCoordinateComponents
 */
SBase*
ListOfCoordinateComponents::createObject(XMLInputStream& stream)
{
  const std::string& name = stream.peek().getName();
  SBase* object = NULL;
  SPATIAL_CREATE_NS(spatialns, getSBMLNamespaces());

  if (name == "coordinateComponent")
  {
    object = new CoordinateComponent(spatialns);
    appendAndOwn(object);
  }

  delete spatialns;
  return object;
}

/** @endcond */



/** @cond doxygenLibsbmlInternal */

/*
 * Writes the namespace for the Spatial package
 */
void
ListOfCoordinateComponents::writeXMLNS(XMLOutputStream& stream) const
{
  XMLNamespaces xmlns;
  std::string prefix = getPrefix();

  if (prefix.empty())
  {
    const XMLNamespaces* thisxmlns = getNamespaces();
    if (thisxmlns && thisxmlns->hasURI(SpatialExtension::getXmlnsL3V1V1()))
    {
      xmlns.add(SpatialExtension::getXmlnsL3V1V1(), prefix);
    }
  }

  stream << xmlns;
}

/** @endcond */




#endif /* __cplusplus */


/*
 * Get a CoordinateComponent_t from the ListOf_t.
 */
LIBSBML_EXTERN
CoordinateComponent_t*
ListOfCoordinateComponents_getCoordinateComponent(ListOf_t* lo,
                                                  unsigned int n)
{
  if (lo == NULL)
  {
    return NULL;
  }

  return static_cast <ListOfCoordinateComponents*>(lo)->get(n);
}


/*
 * Get a CoordinateComponent_t from the ListOf_t based on its identifier.
 */
LIBSBML_EXTERN
CoordinateComponent_t*
ListOfCoordinateComponents_getById(ListOf_t* lo, const char *sid)
{
  if (lo == NULL)
  {
    return NULL;
  }

  return (sid != NULL) ? static_cast
    <ListOfCoordinateComponents*>(lo)->get(sid) : NULL;
}


/*
 * Removes the nth CoordinateComponent_t from this ListOf_t and returns a
 * pointer to it.
 */
LIBSBML_EXTERN
CoordinateComponent_t*
ListOfCoordinateComponents_remove(ListOf_t* lo, unsigned int n)
{
  if (lo == NULL)
  {
    return NULL;
  }

  return static_cast <ListOfCoordinateComponents*>(lo)->remove(n);
}


/*
 * Removes the CoordinateComponent_t from this ListOf_t based on its identifier
 * and returns a pointer to it.
 */
LIBSBML_EXTERN
CoordinateComponent_t*
ListOfCoordinateComponents_removeById(ListOf_t* lo, const char* sid)
{
  if (lo == NULL)
  {
    return NULL;
  }

  return (sid != NULL) ? static_cast
    <ListOfCoordinateComponents*>(lo)->remove(sid) : NULL;
}




LIBSBML_CPP_NAMESPACE_END


