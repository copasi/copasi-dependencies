
#ifdef USE_SPATIAL
if (pkgName == "spatial")
{
	switch( sb->getTypeCode() )
    {
      case SBML_DOCUMENT:
        return SWIGTYPE_p_SBMLDocumentPlugin;
      case SBML_MODEL:
        return SWIGTYPE_p_SpatialModelPlugin;
      case SBML_COMPARTMENT:
        return SWIGTYPE_p_SpatialCompartmentPlugin;
      case SBML_PARAMETER:
        return SWIGTYPE_p_SpatialParameterPlugin;
      case SBML_SPECIES:
      case SBML_REACTION:
        return SWIGTYPE_p_SpatialSpeciesRxnPlugin;
    }
}
#endif // USE_SPATIAL

