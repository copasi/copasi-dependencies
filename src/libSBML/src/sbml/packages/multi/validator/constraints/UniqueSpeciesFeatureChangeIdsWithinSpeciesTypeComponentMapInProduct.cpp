/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    UniqueSpeciesFeatureChangeIdsWithinSpeciesTypeComponentMapInProduct.cpp
 * @brief   Ensures the SpeciesFeatureChange ids within a SpeciesTypeComponentMapInProduct are unique
 * @author  Fengkai Zhang
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2016 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#include <sbml/Model.h>
#include "UniqueSpeciesFeatureChangeIdsWithinSpeciesTypeComponentMapInProduct.h"

/** @cond doxygenIgnored */

using namespace std;

/** @endcond */

LIBSBML_CPP_NAMESPACE_BEGIN
#ifdef __cplusplus

/*
 * Creates a new Constraint with the given constraint id.
 */
UniqueSpeciesFeatureChangeIdsWithinSpeciesTypeComponentMapInProduct::UniqueSpeciesFeatureChangeIdsWithinSpeciesTypeComponentMapInProduct (unsigned int id, MultiValidator& v) :
  UniqueMultiIdBase(id, v)
{
}


/*
 * Destroys this Constraint.
 */
UniqueSpeciesFeatureChangeIdsWithinSpeciesTypeComponentMapInProduct::~UniqueSpeciesFeatureChangeIdsWithinSpeciesTypeComponentMapInProduct ()
{
}


/*
 * Checks that all the SpeciesFeatureChange ids within a SpeciesTypeComponentMapInProduct are unique.
 */
void
UniqueSpeciesFeatureChangeIdsWithinSpeciesTypeComponentMapInProduct::doCheck (const Model& m)
{
  for (unsigned int n = 0; n < m.getNumReactions(); n++)
  {
    const Reaction* r = m.getReaction(n);
    if (r == NULL) continue;

    for (unsigned int i = 0; i < r->getNumProducts(); i++)
    {
        const SpeciesReference * sr = r->getProduct(i);
        if (sr == NULL) continue;

        const MultiSpeciesReferencePlugin * plug =
          dynamic_cast <const MultiSpeciesReferencePlugin*>(sr->getPlugin("multi"));
        if (plug == NULL)
        {
          continue;
        }

        for (unsigned int j = 0; j < plug->getNumSpeciesTypeComponentMapInProducts(); j++) {
            const SpeciesTypeComponentMapInProduct * stcmip = plug->getSpeciesTypeComponentMapInProduct(j);
            if (stcmip == NULL) {
                continue;
            }

            for (unsigned int m = 0; m < stcmip->getNumSpeciesFeatureChanges(); m++) {
               const SpeciesFeatureChange * sfc = stcmip->getSpeciesFeatureChange(m);
               checkId( *sfc );
            } // m
            reset();
        }// j
    } // i
  } // n
}

#endif /* __cplusplus */

LIBSBML_CPP_NAMESPACE_END

/** @endcond */
