#ifdef USE_SPATIAL

SBMLCONSTRUCTOR_EXCEPTION(SpatialPkgNamespaces)
SBMLCONSTRUCTOR_EXCEPTION(DomainType)
SBMLCONSTRUCTOR_EXCEPTION(Domain)
SBMLCONSTRUCTOR_EXCEPTION(InteriorPoint)
SBMLCONSTRUCTOR_EXCEPTION(Boundary)
SBMLCONSTRUCTOR_EXCEPTION(AdjacentDomains)
SBMLCONSTRUCTOR_EXCEPTION(GeometryDefinition)
SBMLCONSTRUCTOR_EXCEPTION(CompartmentMapping)
SBMLCONSTRUCTOR_EXCEPTION(CoordinateComponent)
SBMLCONSTRUCTOR_EXCEPTION(SampledFieldGeometry)
SBMLCONSTRUCTOR_EXCEPTION(SampledField)
SBMLCONSTRUCTOR_EXCEPTION(SampledVolume)
SBMLCONSTRUCTOR_EXCEPTION(AnalyticGeometry)
SBMLCONSTRUCTOR_EXCEPTION(AnalyticVolume)
SBMLCONSTRUCTOR_EXCEPTION(ParametricGeometry)
SBMLCONSTRUCTOR_EXCEPTION(ParametricObject)
SBMLCONSTRUCTOR_EXCEPTION(CSGeometry)
SBMLCONSTRUCTOR_EXCEPTION(CSGObject)
SBMLCONSTRUCTOR_EXCEPTION(CSGNode)
SBMLCONSTRUCTOR_EXCEPTION(CSGTransformation)
SBMLCONSTRUCTOR_EXCEPTION(CSGTranslation)
SBMLCONSTRUCTOR_EXCEPTION(CSGRotation)
SBMLCONSTRUCTOR_EXCEPTION(CSGScale)
SBMLCONSTRUCTOR_EXCEPTION(CSGHomogeneousTransformation)
SBMLCONSTRUCTOR_EXCEPTION(TransformationComponent)
SBMLCONSTRUCTOR_EXCEPTION(CSGPrimitive)
SBMLCONSTRUCTOR_EXCEPTION(CSGSetOperator)
SBMLCONSTRUCTOR_EXCEPTION(SpatialSymbolReference)
SBMLCONSTRUCTOR_EXCEPTION(DiffusionCoefficient)
SBMLCONSTRUCTOR_EXCEPTION(AdvectionCoefficient)
SBMLCONSTRUCTOR_EXCEPTION(BoundaryCondition)
SBMLCONSTRUCTOR_EXCEPTION(Geometry)
SBMLCONSTRUCTOR_EXCEPTION(MixedGeometry)
SBMLCONSTRUCTOR_EXCEPTION(OrdinalMapping)
SBMLCONSTRUCTOR_EXCEPTION(SpatialPoints)
SBMLCONSTRUCTOR_EXCEPTION(ListOfDomainTypes)
SBMLCONSTRUCTOR_EXCEPTION(ListOfDomains)
SBMLCONSTRUCTOR_EXCEPTION(ListOfInteriorPoints)
SBMLCONSTRUCTOR_EXCEPTION(ListOfAdjacentDomains)
SBMLCONSTRUCTOR_EXCEPTION(ListOfGeometryDefinitions)
SBMLCONSTRUCTOR_EXCEPTION(ListOfCoordinateComponents)
SBMLCONSTRUCTOR_EXCEPTION(ListOfSampledFields)
SBMLCONSTRUCTOR_EXCEPTION(ListOfSampledVolumes)
SBMLCONSTRUCTOR_EXCEPTION(ListOfAnalyticVolumes)
SBMLCONSTRUCTOR_EXCEPTION(ListOfParametricObjects)
SBMLCONSTRUCTOR_EXCEPTION(ListOfCSGObjects)
SBMLCONSTRUCTOR_EXCEPTION(ListOfCSGNodes)
SBMLCONSTRUCTOR_EXCEPTION(ListOfOrdinalMappings)

/**
 * Convert GeometryDefinition objects into the most specific object possible.
 */
%typemap(out) GeometryDefinition*
{
	$result = SWIG_NewPointerObj($1, GetDowncastSwigTypeForPackage($1, "spatial"), $owner | %newpointer_flags);
}

/**
 * Convert CSGNode objects into the most specific object possible.
 */
%typemap(out) CSGNode*
{
	$result = SWIG_NewPointerObj($1, GetDowncastSwigTypeForPackage($1, "spatial"), $owner | %newpointer_flags);
}

/**
 * Convert CSGTransformation objects into the most specific object possible.
 */
%typemap(out) CSGTransformation*
{
	$result = SWIG_NewPointerObj($1, GetDowncastSwigTypeForPackage($1, "spatial"), $owner | %newpointer_flags);
}


#endif // USE_SPATIAL 

