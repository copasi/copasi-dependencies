
#ifdef USE_ARRAYS
if (pkgName == "arrays")
{
	if (sb->getTypeCode() == SBML_PARAMETER)
	{
		return SWIGTYPE_p_ArraysParameterPlugin;
	}
	else if (sb->getTypeCode() == SBML_COMPARTMENT)
	{
		return SWIGTYPE_p_ArraysCompartmentPlugin;
	}	
	else if (sb->getTypeCode() == SBML_SPECIES)
	{
		return SWIGTYPE_p_ArraysSpeciesPlugin;
	}	
	else if (sb->getTypeCode() == SBML_REACTION)
	{
		return SWIGTYPE_p_ArraysReactionPlugin;
	}	
	else if (sb->getTypeCode() == SBML_EVENT)
	{
		return SWIGTYPE_p_ArraysEventPlugin;
	}	
	else if (sb->getTypeCode() == SBML_CONSTRAINT)
	{
		return SWIGTYPE_p_ArraysConstraintPlugin;
	}
	else if (sb->getTypeCode() == SBML_EVENT_ASSIGNMENT)
	{
		return SWIGTYPE_p_ArraysEventAssignmentPlugin;
	}
	else if (sb->getTypeCode() == SBML_SPECIES_REFERENCE)
	{
		return SWIGTYPE_p_ArraysSpeciesReferencePlugin;
	}
	else if (sb->getTypeCode() == SBML_RULE)
	{
		return SWIGTYPE_p_ArraysRulePlugin;
	}	
	else if (sb->getTypeCode() == SBML_INITIAL_ASSIGNMENT)
	{
		return SWIGTYPE_p_ArraysInitialAssignmentPlugin;
	}	
}
#endif // USE_ARRAYS

