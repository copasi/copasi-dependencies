/**
 * @file    local-layout.i
 * @brief   cs-specific SWIG directives for wrapping libSBML layout API this file
 *          has been adapted from the SWIG java bindings written by
 * 	    Ben Bornstein and Akiya Jouraku
 * @author  Frank Bergmann (fbergman@u.washington.edu)
 * @author  Akiya Jouraku
 *
 * $Id: local.i 10190 2009-09-23 16:03:35Z ajouraku $
 * $URL: https://sbml.svn.sourceforge.net/svnroot/sbml/trunk/libsbml/src/bindings/csharp/local.i $
 */

/*
 *This file is part of libSBML.  Please visit http://sbml.org for more
 *information about SBML, and the latest version of libSBML.
 *
 *Copyright 2008 California Institute of Technology.
 *
 *This library is free software; you can redistribute it and/or modify it
 *under the terms of the GNU Lesser General Public License as published by
 *the Free Software Foundation.  A copy of the license agreement is provided
 *in the file named "LICENSE.txt" included with this software distribution
 *and also available online as http://sbml.org/software/libsbml/license.html
 */

#ifdef USE_SPATIAL


/**
 * Handling array of ints for USE_SPATIAL
 */

 /** For ImageData::setSamples() **/
%include "arrays_csharp.i"
%apply int OUTPUT[] {int* outputSamples};
%apply int OUTPUT[] {int* data};
%apply int INPUT[] {int* samples};


/** For ImageData::getSamples() **/
%include "typemaps.i"
void ImageData::getSamples(int *outputSamples);
void ImageData::getUncompressed(int *outputSamples);

/*** END handling arrays ***/
%ignore ImageData::getUncompressedData;


/**
 * Adds DownCastBase(long cPtr, boolean owner) method for the groups package extension
 */
%typemap(cscode) SpatialExtension
%{
	public override SBasePlugin DowncastSBasePlugin(IntPtr cPtr, bool owner)
	{
		if (cPtr == IntPtr.Zero) return null;

		SBasePlugin sbp = new SBasePlugin(cPtr,false);
		SBase sb = sbp.getParentSBMLObject();

		if (sb == null)
		{
		  return new SBasePlugin(cPtr,owner);
		}

		switch( sb.getTypeCode() )
		{
		  case (int) libsbml.SBML_DOCUMENT:
			return new SBMLDocumentPlugin(cPtr,owner);
		  case (int) libsbml.SBML_MODEL:
			return new SpatialModelPlugin(cPtr,owner);
		  case (int) libsbml.SBML_COMPARTMENT:
			return new SpatialCompartmentPlugin(cPtr,owner);
		  case (int) libsbml.SBML_PARAMETER:
			return new SpatialParameterPlugin(cPtr,owner);
		  case (int) libsbml.SBML_SPECIES:
		  case (int) libsbml.SBML_REACTION:
			return new SpatialSpeciesRxnPlugin(cPtr,owner);
		  default:
			return new SBasePlugin(cPtr,owner);
		}
	}

	public override SBase DowncastSBase(IntPtr cPtr, bool owner)
	{
		if (cPtr == IntPtr.Zero) return null;

		SBase sb = new SBase(cPtr,false);

		switch( sb.getTypeCode() )
		{
		  case (int) libsbml.SBML_LIST_OF:
			String name = sb.getElementName();
			if(name == "listOfCoordinateComponents"){
			  return new ListOfCoordinateComponents(cPtr, owner);
			}
			else if(name == "listOfDomainTypes"){
			  return new ListOfDomainTypes(cPtr, owner);
			}
			else if(name == "listOfDomains"){
			  return new ListOfDomains(cPtr, owner);
			}
			else if(name == "listOfAdjacentDomains"){
			  return new ListOfAdjacentDomains(cPtr, owner);
			}
			else if(name == "listOfGeometryDefinitions"){
			  return new ListOfGeometryDefinitions(cPtr, owner);
			}
			else if(name == "listOfAnalyticVolumes"){
			  return new ListOfAnalyticVolumes(cPtr, owner);
			}
			else if(name == "listOfInteriorPoints"){
			  return new ListOfInteriorPoints(cPtr, owner);
			}
			else if(name == "listOfSpatialPoints"){
			  return new ListOfSpatialPoints(cPtr, owner);
			}
			else if(name == "listOfSampledVolumes"){
			  return new ListOfSampledVolumes(cPtr, owner);
			}

			return new ListOf(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_GEOMETRY:
			return new Geometry(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_DOMAINTYPE:
			return new DomainType(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_COMPARTMENTMAPPING:
			return new CompartmentMapping(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_DOMAIN:
			return new Domain(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_COORDINATECOMPONENT:
			return new CoordinateComponent(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_BOUNDARYMIN:
		  	return new BoundaryCondition(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_BOUNDARYMAX:
		  	return new BoundaryCondition(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_ADJACENTDOMAINS:
			return new AdjacentDomains(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_ANALYTICGEOMETRY:
			return new AnalyticGeometry(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_SAMPLEDFIELDGEOMETRY:
			return new SampledFieldGeometry(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_ANALYTICVOLUME:
			return new AnalyticVolume(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_INTERIORPOINT:
			return new InteriorPoint(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_SPATIALPOINT:
			return new SpatialPoint(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_SAMPLEDFIELD:
			return new SampledField(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_SAMPLEDVOLUME:
			return new SampledVolume(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_SPATIALSYMBOLREFERENCE:
			return new SpatialSymbolReference(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_DIFFUSIONCOEFFICIENT:
			return new DiffusionCoefficient(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_ADVECTIONCOEFFICIENT:
			return new AdvectionCoefficient(cPtr, owner);

		  case (int) libsbml.SBML_SPATIAL_BOUNDARYCONDITION:
			return new BoundaryCondition(cPtr, owner);

		  default:
			return new SBase(cPtr, owner);
		}
	}
%}

//
// Convert GeometryDefinition objects into the most specific object possible.
//
%typemap("csout") GeometryDefinition*
{
  return (GeometryDefinition) libsbml.DowncastSBase($imcall, $owner);
}


//
// Convert Boundary objects into the most specific object possible.
//
%typemap("csout") Boundary*
{
  return (Boundary) libsbml.DowncastSBase($imcall, $owner);
}

COVARIANT_RTYPE_CLONE(Geometry)
COVARIANT_RTYPE_CLONE(CoordinateComponent)
COVARIANT_RTYPE_CLONE(Boundary)
COVARIANT_RTYPE_CLONE(BoundaryMin)
COVARIANT_RTYPE_CLONE(BoundaryMax)
COVARIANT_RTYPE_CLONE(DomainType)
COVARIANT_RTYPE_CLONE(CompartmentMapping)
COVARIANT_RTYPE_CLONE(Domain)
COVARIANT_RTYPE_CLONE(InteriorPoint)
COVARIANT_RTYPE_CLONE(AdjacentDomains)
COVARIANT_RTYPE_CLONE(GeometryDefinition)
COVARIANT_RTYPE_CLONE(AnalyticGeometry)
COVARIANT_RTYPE_CLONE(SampledFieldGeometry)
COVARIANT_RTYPE_CLONE(AnalyticVolume)
COVARIANT_RTYPE_CLONE(SpatialPoint)
COVARIANT_RTYPE_CLONE(SampledField)
COVARIANT_RTYPE_CLONE(ImageData)
COVARIANT_RTYPE_CLONE(SampledVolume)
COVARIANT_RTYPE_CLONE(SpatialSymbolReference)
COVARIANT_RTYPE_CLONE(DiffusionCoefficient)
COVARIANT_RTYPE_CLONE(AdvectionCoefficient)
COVARIANT_RTYPE_CLONE(BoundaryCondition)
COVARIANT_RTYPE_CLONE(ListOfCoordinateComponents)
COVARIANT_RTYPE_CLONE(ListOfDomainTypes)
COVARIANT_RTYPE_CLONE(ListOfDomains)
COVARIANT_RTYPE_CLONE(ListOfAdjacentDomains)
COVARIANT_RTYPE_CLONE(ListOfGeometryDefinitions)
COVARIANT_RTYPE_CLONE(ListOfAnalyticVolumes)
COVARIANT_RTYPE_CLONE(ListOfInteriorPoints)
COVARIANT_RTYPE_CLONE(ListOfSpatialPoints)
COVARIANT_RTYPE_CLONE(ListOfSampledVolumes)

COVARIANT_RTYPE_LISTOF_GET_REMOVE(CoordinateComponent)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(DomainType)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(Domain)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(AdjacentDomains)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(GeometryDefinition)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(AnalyticVolume)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(InteriorPoint)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(SpatialPoint)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(SampledVolume)

SBMLCONSTRUCTOR_EXCEPTION(AdjacentDomains)
SBMLCONSTRUCTOR_EXCEPTION(AdvectionCoefficient)
SBMLCONSTRUCTOR_EXCEPTION(AnalyticGeometry)
SBMLCONSTRUCTOR_EXCEPTION(AnalyticVolume)
SBMLCONSTRUCTOR_EXCEPTION(BoundaryCondition)
SBMLCONSTRUCTOR_EXCEPTION(CompartmentMapping)
SBMLCONSTRUCTOR_EXCEPTION(CoordinateComponent)
SBMLCONSTRUCTOR_EXCEPTION(CSGeometry)
SBMLCONSTRUCTOR_EXCEPTION(CSGHomogeneousTransformation)
SBMLCONSTRUCTOR_EXCEPTION(CSGObject)
SBMLCONSTRUCTOR_EXCEPTION(CSGPrimitive)
SBMLCONSTRUCTOR_EXCEPTION(CSGPseudoPrimitive)
SBMLCONSTRUCTOR_EXCEPTION(CSGRotation)
SBMLCONSTRUCTOR_EXCEPTION(CSGScale)
SBMLCONSTRUCTOR_EXCEPTION(CSGSetOperator)
SBMLCONSTRUCTOR_EXCEPTION(CSGTranslation)
SBMLCONSTRUCTOR_EXCEPTION(DiffusionCoefficient)
SBMLCONSTRUCTOR_EXCEPTION(Domain)
SBMLCONSTRUCTOR_EXCEPTION(DomainType)
SBMLCONSTRUCTOR_EXCEPTION(Geometry)
SBMLCONSTRUCTOR_EXCEPTION(ImageData)
SBMLCONSTRUCTOR_EXCEPTION(InteriorPoint)
SBMLCONSTRUCTOR_EXCEPTION(ListOfAdjacentDomains)
SBMLCONSTRUCTOR_EXCEPTION(ListOfAnalyticVolumes)
SBMLCONSTRUCTOR_EXCEPTION(ListOfCompartmentMappings)
SBMLCONSTRUCTOR_EXCEPTION(ListOfCoordinateComponents)
SBMLCONSTRUCTOR_EXCEPTION(ListOfCSGNodes)
SBMLCONSTRUCTOR_EXCEPTION(ListOfCSGObjects)
SBMLCONSTRUCTOR_EXCEPTION(ListOfDomains)
SBMLCONSTRUCTOR_EXCEPTION(ListOfDomainTypes)
SBMLCONSTRUCTOR_EXCEPTION(ListOfGeometryDefinitions)
SBMLCONSTRUCTOR_EXCEPTION(ListOfInteriorPoints)
SBMLCONSTRUCTOR_EXCEPTION(ListOfParametricObjects)
SBMLCONSTRUCTOR_EXCEPTION(ListOfSampledVolumes)
SBMLCONSTRUCTOR_EXCEPTION(ListOfSpatialPoints)
SBMLCONSTRUCTOR_EXCEPTION(ParametricGeometry)
SBMLCONSTRUCTOR_EXCEPTION(ParametricObject)
SBMLCONSTRUCTOR_EXCEPTION(SampledField)
SBMLCONSTRUCTOR_EXCEPTION(SampledFieldGeometry)
SBMLCONSTRUCTOR_EXCEPTION(SampledVolume)
SBMLCONSTRUCTOR_EXCEPTION(SpatialPkgNamespaces)
SBMLCONSTRUCTOR_EXCEPTION(SpatialPoint)
SBMLCONSTRUCTOR_EXCEPTION(SpatialSymbolReference)

%define COVARIANT_GETELEMENTNAME(_CNAME_)
%typemap(cstype) string   _CNAME_ ## ::getElementName  "_CNAME_"
%csmethodmodifiers  _CNAME_ ## ::getElementName  "public new"
%enddef

COVARIANT_GETELEMENTNAME(AdvectionCoefficient)
COVARIANT_GETELEMENTNAME(CoordinateComponent)
COVARIANT_GETELEMENTNAME(ListOfDomains)
COVARIANT_GETELEMENTNAME(Boundary)

#endif  /* USE_SPATIAL */

