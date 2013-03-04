/**
 * casting to most specific SBMLNamespaces object
 */


#ifdef USE_REQUIREDELEMENTS
%pragma(csharp) modulecode =
%{
	
	if (ns.hasURI(RequiredElementsExtension.getXmlnsL3V1V1()))
	{
		return new RequiredElementsPkgNamespaces(cPtr, owner);
	}
%}
#endif /* USE_REQUIREDELEMENTS */
