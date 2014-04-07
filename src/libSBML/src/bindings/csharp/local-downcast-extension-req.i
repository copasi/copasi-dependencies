/**
 * casting to most specific SBMLExtension object
 */
 
#ifdef USE_REQUIREDELEMENTS
%pragma(csharp) modulecode =
%{
		if (pkgName == "req")
			return new RequiredElementsExtension(cPtr, owner);
%}
#endif /* USE_REQUIREDELEMENTS */
