#ifdef USE_SPATIAL

/**
 * Handling array of ints for USE_SPATIAL
 */

 /** For ImageData::setSamples() **/
%include "arrays_java.i"
%apply int[] {int *};

/** For ImageData::getSamples() **/
%include "typemaps.i"
void ImageData::getSamples(int *outputSamples);
void ImageData::getUncompressed(int *outputSamples);

/** For PolygonObject::setPointIndices() **/
%include "arrays_java.i"
%apply int[] {int *};

%ignore ImageData::getUncompressedData;


/** For PolygonObject::getPointIndices() **/
%include "typemaps.i"
void PolygonObject::getPointIndices(int* outputPointIndices);

/** For TransformationComponents::setComponents() **/
%include "arrays_java.i"
%apply double[] {double *};

/** For TransformationComponents::getComponents() **/
%include "typemaps.i"
void TransformationComponents::getComponents(double* outputComponents);
/*** END handling arrays ***/

/**
 * Adds DownCastBase(long cPtr, boolean owner) method for the layout package extension
 */
%typemap(javacode) SpatialExtension
%{
	public SBasePlugin DowncastSBasePlugin(long cPtr, boolean owner)
	{
		if (cPtr == 0) return null;

		SBasePlugin sbp = new SBasePlugin(cPtr,false);
		SBase sb = sbp.getParentSBMLObject();

		if (sb == null)
		{
		return new SBasePlugin(cPtr,owner);
		}

		switch( sb.getTypeCode() )
		{
			case libsbmlConstants.SBML_DOCUMENT:
				return new SBMLDocumentPlugin(cPtr,owner);
			case libsbmlConstants.SBML_MODEL:
				return new SpatialModelPlugin(cPtr,owner);
			case libsbmlConstants.SBML_COMPARTMENT:
				return new SpatialCompartmentPlugin(cPtr,owner);
			case libsbmlConstants.SBML_PARAMETER:
				return new SpatialParameterPlugin(cPtr,owner);
			case libsbmlConstants.SBML_SPECIES:
			case libsbmlConstants.SBML_REACTION:
				return new SpatialSpeciesRxnPlugin(cPtr,owner);
			default:
				return new SBasePlugin(cPtr,owner);
		}
	}

	public SBase DowncastSBase(long cPtr, boolean owner)
	{
		if (cPtr == 0) return null;

		SBase sb = new SBase(cPtr,false);

		switch( sb.getTypeCode() )
		{
			case libsbmlConstants.SBML_LIST_OF:
				String name = sb.getElementName();
				if(name.equals("listOfCoordinateComponents")){
				return new ListOfCoordinateComponents(cPtr, owner);
				}
				else if(name.equals("listOfDomainTypes")){
				return new ListOfDomainTypes(cPtr, owner);
				}
				else if(name.equals("listOfDomains")){
				return new ListOfDomains(cPtr, owner);
				}
				else if(name.equals("listOfAdjacentDomains")){
				return new ListOfAdjacentDomains(cPtr, owner);
				}
				else if(name.equals("listOfGeometryDefinitions")){
				return new ListOfGeometryDefinitions(cPtr, owner);
				}
				else if(name.equals("listOfAnalyticVolumes")){
				return new ListOfAnalyticVolumes(cPtr, owner);
				}
				else if(name.equals("listOfInteriorPoints")){
				return new ListOfInteriorPoints(cPtr, owner);
				}
				else if(name.equals("listOfSpatialPoints")){
				return new ListOfSpatialPoints(cPtr, owner);
				}
				else if(name.equals("listOfSampledVolumes")){
				return new ListOfSampledVolumes(cPtr, owner);
				}
				else if(name.equals("listOfParametricObjects")){
				return new ListOfParametricObjects(cPtr, owner);
				}
				else if(name.equals("listOfCSGObjects")){
				return new ListOfCSGObjects(cPtr, owner);
				}
				else if(name.equals("listOfCSGNodes")){
				return new ListOfCSGNodes(cPtr, owner);
				}

				return new ListOf(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_GEOMETRY:
			return new Geometry(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_DOMAINTYPE:
			return new DomainType(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_COMPARTMENTMAPPING:
			return new CompartmentMapping(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_DOMAIN:
			return new Domain(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_COORDINATECOMPONENT:
			return new CoordinateComponent(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_BOUNDARYMIN:
			return new BoundaryCondition(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_BOUNDARYMAX:
			return new BoundaryCondition(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_ADJACENTDOMAINS:
			return new AdjacentDomains(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_ANALYTICGEOMETRY:
			return new AnalyticGeometry(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_SAMPLEDFIELDGEOMETRY:
			return new SampledFieldGeometry(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_ANALYTICVOLUME:
			return new AnalyticVolume(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_INTERIORPOINT:
			return new InteriorPoint(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_SPATIALPOINT:
			return new SpatialPoint(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_SAMPLEDFIELD:
			return new SampledField(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_SAMPLEDVOLUME:
			return new SampledVolume(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_PARAMETRICGEOMETRY:
			return new ParametricGeometry(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_PARAMETRICOBJECT:
			return new ParametricObject(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_CSGGEOMETRY:
			return new CSGeometry(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_CSGOBJECT:
			return new CSGObject(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_CSGTRANSLATION:
			return new CSGTranslation(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_CSGROTATION:
			return new CSGRotation(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_CSGSCALE:
			return new CSGScale(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_CSGHOMOGENEOUSTRANSFORMATION:
			return new CSGHomogeneousTransformation(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_CSGPRIMITIVE:
			return new CSGPrimitive(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_CSGPSEUDOPRIMITIVE:
			return new CSGPseudoPrimitive(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_CSGSETOPERATOR:
			return new CSGSetOperator(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_SPATIALSYMBOLREFERENCE:
			return new SpatialSymbolReference(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_DIFFUSIONCOEFFICIENT:
			return new DiffusionCoefficient(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_ADVECTIONCOEFFICIENT:
			return new AdvectionCoefficient(cPtr, owner);

		case libsbmlConstants.SBML_SPATIAL_BOUNDARYCONDITION:
			return new BoundaryCondition(cPtr, owner);

		default:
			return new SBase(cPtr, owner);
		}
	}

%}

//
// Convert GeometryDefinition objects into the most specific object possible.
//
%typemap("javaout") GeometryDefinition*
{
  return (GeometryDefinition) libsbml.DowncastSBase($jnicall, $owner);
}


//
// Convert Boundary objects into the most specific object possible.
//
%typemap("javaout") Boundary*
{
  return (Boundary) libsbml.DowncastSBase($jnicall, $owner);
}

//
// Convert CSGNode objects into the most specific object possible.
//
%typemap("javaout") CSGNode*
{
  return (CSGNode) libsbml.DowncastSBase($jnicall, $owner);
}

//
// Convert CSGTransformation objects into the most specific object possible.
//
%typemap("javaout") CSGTransformation*
{
  return (CSGTransformation) libsbml.DowncastSBase($jnicall, $owner);
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
COVARIANT_RTYPE_CLONE(ParametricGeometry)
COVARIANT_RTYPE_CLONE(ParametricObject)
COVARIANT_RTYPE_CLONE(PolygonObject)
COVARIANT_RTYPE_CLONE(CSGeometry)
COVARIANT_RTYPE_CLONE(CSGObject)
COVARIANT_RTYPE_CLONE(CSGNode)
COVARIANT_RTYPE_CLONE(CSGTransformation)
COVARIANT_RTYPE_CLONE(CSGTranslation)
COVARIANT_RTYPE_CLONE(CSGRotation)
COVARIANT_RTYPE_CLONE(CSGScale)
COVARIANT_RTYPE_CLONE(CSGHomogeneousTransformation)
COVARIANT_RTYPE_CLONE(TransformationComponents)
COVARIANT_RTYPE_CLONE(CSGPrimitive)
COVARIANT_RTYPE_CLONE(CSGPseudoPrimitive)
COVARIANT_RTYPE_CLONE(CSGSetOperator)
COVARIANT_RTYPE_CLONE(SpatialSymbolReference)
COVARIANT_RTYPE_CLONE(DiffusionCoefficient)
COVARIANT_RTYPE_CLONE(AdvectionCoefficient)
COVARIANT_RTYPE_CLONE(BoundaryCondition)
COVARIANT_RTYPE_CLONE(ListOfCompartmentMappings)
COVARIANT_RTYPE_CLONE(ListOfCoordinateComponents)
COVARIANT_RTYPE_CLONE(ListOfDomainTypes)
COVARIANT_RTYPE_CLONE(ListOfDomains)
COVARIANT_RTYPE_CLONE(ListOfAdjacentDomains)
COVARIANT_RTYPE_CLONE(ListOfGeometryDefinitions)
COVARIANT_RTYPE_CLONE(ListOfAnalyticVolumes)
COVARIANT_RTYPE_CLONE(ListOfInteriorPoints)
COVARIANT_RTYPE_CLONE(ListOfSpatialPoints)
COVARIANT_RTYPE_CLONE(ListOfSampledVolumes)
COVARIANT_RTYPE_CLONE(ListOfParametricObjects)
COVARIANT_RTYPE_CLONE(ListOfCSGObjects)
COVARIANT_RTYPE_CLONE(ListOfCSGNodes)


COVARIANT_RTYPE_LISTOF_GET_REMOVE(CoordinateComponent)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(DomainType)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(Domain)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(AdjacentDomains)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(GeometryDefinition)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(AnalyticVolume)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(InteriorPoint)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(SpatialPoint)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(SampledVolume)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(ParametricObject)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(CSGObject)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(CSGNode)

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



#endif  /* USE_SPATIAL */
