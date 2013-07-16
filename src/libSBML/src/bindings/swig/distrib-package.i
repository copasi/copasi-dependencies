#ifdef USE_DISTRIB

%newobject removeDistribInput;

%template (DistribPkgNamespaces) SBMLExtensionNamespaces<DistribExtension>;

%include <sbml/packages/distrib/extension/DistribExtension.h>
%include <sbml/packages/distrib/extension/DistribFunctionDefinitionPlugin.h>
%include <sbml/packages/distrib/sbml/DrawFromDistribution.h>
%include <sbml/packages/distrib/sbml/DistribInput.h>
%include <sbml/packages/distrib/sbml/PredefinedPDF.h>
%include <sbml/packages/distrib/sbml/ExplicitPMF.h>
%include <sbml/packages/distrib/sbml/ExplicitPDF.h>

#endif /* USE_DISTRIB */

