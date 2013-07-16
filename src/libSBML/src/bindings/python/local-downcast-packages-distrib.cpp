#ifdef USE_DISTRIB
else if (pkgName == "distrib")
{
	switch (sb->getTypeCode() )
	{
		case SBML_LIST_OF:
			name = sb->getElementName();
			else if (name == "listOfDistribInputs")
			{
				return SWIGTYPE_p_ListOfDistribInputs;
			}

			return SWIGTYPE_p_ListOf;

		case SBML_DISTRIB_DRAW_FROM_DISTRIBUTION:
			return SWIGTYPE_p_DrawFromDistribution;

		case SBML_DISTRIB_INPUT:
			return SWIGTYPE_p_DistribInput;

		case SBML_DISTRIB_PREDEFINED_PDF:
			return SWIGTYPE_p_PredefinedPDF;

		case SBML_DISTRIB_EXPLICT_PMF:
			return SWIGTYPE_p_ExplicitPMF;

		case SBML_DISTRIB_EXPLICT_PDF:
			return SWIGTYPE_p_ExplicitPDF;

		default:
			return SWIGTYPE_p_SBase;
	}
}

#endif // USE_DISTRIB 

