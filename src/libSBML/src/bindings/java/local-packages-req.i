#ifdef USE_REQUIREDELEMENTS

/**
 * Adds DownCastBase(long cPtr, boolean owner) method for the layout package extension
 */
%typemap(javacode) RequiredElementsExtension
%{
	public SBasePlugin DowncastSBasePlugin(long cPtr, boolean owner)
	{
		if (cPtr == 0) return null;
		
		SBasePlugin sbp = new SBasePlugin(cPtr, false);
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
			case (int) libsbml.SBML_COMPARTMENT:
			case (int) libsbml.SBML_COMPARTMENT_TYPE:
			case (int) libsbml.SBML_CONSTRAINT:
			case (int) libsbml.SBML_EVENT:
			case (int) libsbml.SBML_EVENT_ASSIGNMENT:
			case (int) libsbml.SBML_FUNCTION_DEFINITION:
			case (int) libsbml.SBML_INITIAL_ASSIGNMENT:
			case (int) libsbml.SBML_KINETIC_LAW:
			case (int) libsbml.SBML_LIST_OF:
			case (int) libsbml.SBML_PARAMETER:
			case (int) libsbml.SBML_REACTION:
			case (int) libsbml.SBML_RULE:
			case (int) libsbml.SBML_SPECIES:
			case (int) libsbml.SBML_SPECIES_REFERENCE:
			case (int) libsbml.SBML_SPECIES_TYPE:
			case (int) libsbml.SBML_MODIFIER_SPECIES_REFERENCE:
			case (int) libsbml.SBML_UNIT_DEFINITION:
			case (int) libsbml.SBML_UNIT:
			case (int) libsbml.SBML_ALGEBRAIC_RULE:
			case (int) libsbml.SBML_ASSIGNMENT_RULE:
			case (int) libsbml.SBML_RATE_RULE:
			case (int) libsbml.SBML_SPECIES_CONCENTRATION_RULE:
			case (int) libsbml.SBML_COMPARTMENT_VOLUME_RULE:
			case (int) libsbml.SBML_PARAMETER_RULE:
			case (int) libsbml.SBML_TRIGGER:
			case (int) libsbml.SBML_DELAY:
			case (int) libsbml.SBML_STOICHIOMETRY_MATH:
			case (int) libsbml.SBML_LOCAL_PARAMETER:
				return new RequiredElementsSBasePlugin(cPtr,owner);
			default: 
				return new SBasePlugin(cPtr,owner);		
		}
	}
	
	public SBase DowncastSBase(long cPtr, boolean owner)
	{
		if (cPtr == 0) return null;
		
		return new SBase(cPtr, owner);
	}
	
%}

SBMLCONSTRUCTOR_EXCEPTION(RequiredElementsPkgNamespaces)

#endif  /* USE_REQUIREDELEMENTS */
