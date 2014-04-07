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

#ifdef USE_REQUIREDELEMENTS

/**
 * Adds DownCastBase(long cPtr, boolean owner) method for the layout package extension
 */
%typemap(cscode) RequiredElementsExtension
%{
	public override SBasePlugin DowncastSBasePlugin(IntPtr cPtr, bool owner)
	{
		if (cPtr.Equals(IntPtr.Zero)) return null;
		
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

	public override SBase DowncastSBase(IntPtr cPtr, bool owner)
	{
		if (cPtr.Equals(IntPtr.Zero)) return null;		
		return new SBase(cPtr, owner);
	}
%}

SBMLCONSTRUCTOR_EXCEPTION(RequiredElementsPkgNamespaces)

#endif


