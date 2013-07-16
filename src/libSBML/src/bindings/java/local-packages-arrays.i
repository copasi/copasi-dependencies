#ifdef USE_ARRAYS

/**
 * Adds DownCastBase(long cPtr, boolean owner) method for the layout package extension
 */
%typemap(javacode) ArraysExtension
%{
	public SBasePlugin DowncastSBasePlugin(long cPtr, boolean owner)
	{
		if (cPtr == 0) return null;
		
		SBasePlugin sbp = new SBasePlugin(cPtr, false);
		SBase sb = sbp.getParentSBMLObject();
		
		switch( sb.getTypeCode() )
		{
			case (int) libsbml.SBML_PARAMETER:
				return new ArraysParameterPlugin(cPtr,owner);
			case (int) libsbml.SBML_COMPARTMENT:
				return new ArraysCompartmentPlugin(cPtr,owner);
			case (int) libsbml.SBML_SPECIES:
				return new ArraysSpeciesPlugin(cPtr,owner);
			case (int) libsbml.SBML_REACTION:
				return new ArraysReactionPlugin(cPtr,owner);
			case (int) libsbml.SBML_EVENT:
				return new ArraysEventPlugin(cPtr,owner);
			case (int) libsbml.SBML_CONSTRAINT:
				return new ArraysConstraintPlugin(cPtr,owner);
			case (int) libsbml.SBML_EVENT_ASSIGNMENT:
				return new ArraysEventAssignmentPlugin(cPtr,owner);
			case (int) libsbml.SBML_SPECIES_REFERENCE:
				return new ArraysSpeciesReferencePlugin(cPtr,owner);
			case (int) libsbml.SBML_RULE:
				return new ArraysRulePlugin(cPtr,owner);
			case (int) libsbml.SBML_INITIAL_ASSIGNMENT:
				return new ArraysInitialAssignmentPlugin(cPtr,owner);
			default:
				return new SBasePlugin(cPtr,owner);
		}
	}
	
	public SBase DowncastSBase(long cPtr, boolean owner)
	{
		if (cPtr == 0) return null;
		
		SBase sb = new SBase(cPtr, false);
		switch( sb.getTypeCode() )
		{
			case (int) libsbml.SBML_LIST_OF:
				String name = sb.getElementName();
				if(name.equals("listOfDimensions"))
				{
					return new ListOfDimensions(cPtr, owner);
				}
				else if(name.equals("listOfIndices"))
				{
					return new ListOfIndices(cPtr, owner);
				}
				return new ListOf(cPtr, owner);
				
			case (int) libsbml.SBML_ARRAYS_DIMENSION:
				return new Dimension(cPtr, owner);
				
			case (int) libsbml.SBML_ARRAYS_INDEX:
				return new Index(cPtr, owner);

			default:
				return new SBase(cPtr, owner);
		}
	}
	
	%}


COVARIANT_RTYPE_CLONE(ArraysExtension)
COVARIANT_RTYPE_CLONE(Dimension)
COVARIANT_RTYPE_CLONE(Index)
COVARIANT_RTYPE_CLONE(ListOfDimensions)
COVARIANT_RTYPE_CLONE(ListOfIndices)

COVARIANT_RTYPE_LISTOF_GET_REMOVE(Dimension)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(Index)


SBMLCONSTRUCTOR_EXCEPTION(ArraysPkgNamespaces)
SBMLCONSTRUCTOR_EXCEPTION(Dimension)
SBMLCONSTRUCTOR_EXCEPTION(Index)
SBMLCONSTRUCTOR_EXCEPTION(ListOfDimensions)
SBMLCONSTRUCTOR_EXCEPTION(ListOfIndices)


#endif  /* USE_ARRAYS */
