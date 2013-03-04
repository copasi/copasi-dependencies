/**
 * @cond doxygen-libsbml-internal
 *
 * @file    IdentifierConsistencyConstraints.cpp
 * @brief   IdentifierConsistency check constraints.  See SBML Wiki
 * @author  Sarah Keating
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EBML-EBI), Hinxton, UK
 *  
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#ifndef AddingConstraintsToValidator

#include <sbml/validator/VConstraint.h>
#include <sbml/packages/comp/sbml/Port.h>
#include <sbml/packages/comp/sbml/Submodel.h>
#include <sbml/packages/comp/sbml/ExternalModelDefinition.h>
#include <sbml/packages/comp/sbml/ModelDefinition.h>
#include <sbml/packages/comp/validator/CompSBMLError.h>
#include <sbml/packages/comp/extension/CompSBMLDocumentPlugin.h>
#include <sbml/packages/comp/util/SBMLResolverRegistry.h>
#include <sbml/packages/comp/util/SBMLUri.h>
#endif

#include <sbml/validator/ConstraintMacros.h>

/** @cond doxygen-ignored */

using namespace std;

/** @endcond */

// 20101 - caught at read
// 20102 - caught at read
// 20103 - caught at read
// 20104 - caught at read
// 20105 - caught at read

// 20201 - caught at read
// 20202 - caught at read
// 20203 - caught by checkConsistency
// 20204 - caught by checkConsistency
// 20205 - caught at read
// 20206 - caught at read
// 20207 - caught at read
// 20208 - caught at read
// 20209 - caught at read
// 20210 - caught at read
// 20211 - caught at read

// 20301 - caught at read
// 20302 - caught at read
// 20303 - caught at read

//20304
START_CONSTRAINT (CompReferenceMustBeL3, ExternalModelDefinition, emd)
{
  pre (emd.isSetSource() == true);
  pre (emd.isSetId() == true);
  
  const SBMLDocument* doc = emd.getSBMLDocument();
  pre(doc != NULL);
  string locationURI = doc->getLocationURI();
  string uri = emd.getSource();
  doc = NULL;

  bool fail = false;

  const SBMLResolverRegistry& registry = SBMLResolverRegistry::getInstance();

  SBMLUri* resolved = registry.resolveUri(uri, locationURI);
  pre(resolved != NULL )

  msg = "<externalModelDefinition> '";
  msg += emd.getId();
  msg += "' refers to a URI '";
  msg += emd.getSource();
  msg += "' which is not an SBML Level 3 document.";

  string resolvedURI = resolved->getUri();
  delete resolved;

  doc = registry.resolve(uri, locationURI);
  pre (doc != NULL);

  if (doc->getLevel() != 3) 
  {
    fail = true;
  }

  delete doc;
  
  inv(fail == false);
}
END_CONSTRAINT

//20305
START_CONSTRAINT (CompModReferenceMustIdOfModel, ExternalModelDefinition, emd)
{
  pre (emd.isSetSource() == true);
  pre (emd.isSetId() == true);
  pre (emd.isSetModelRef() == true);
  
  const SBMLDocument* doc = emd.getSBMLDocument();
  pre(doc != NULL);
  string locationURI = doc->getLocationURI();
  string uri = emd.getSource();

  const SBMLResolverRegistry& registry = SBMLResolverRegistry::getInstance();
  SBMLUri* resolved = registry.resolveUri(uri, locationURI);
  pre(resolved != NULL )
  string resolvedURI = resolved->getUri();
  delete resolved;
  doc = registry.resolve(uri, locationURI);
  pre(doc != NULL);
  pre(doc->getLevel() == 3);

  bool fail = false;

  msg = "<externalModelDefinition> '";
  msg += emd.getId() ;
  msg += "' refers to a model with id '";
  msg += emd.getModelRef();
  msg += "' that does not exist in the referenced document.";

  const CompSBMLDocumentPlugin* csdp = static_cast<const CompSBMLDocumentPlugin*>(doc->getPlugin(emd.getPrefix()));
  if (csdp == NULL) 
  {
    const Model* model = doc->getModel();
    if (model==NULL || (model->getId() != emd.getModelRef())) {
      fail = true;
    }
  }
  else 
  {
    const SBase* referencedmod = csdp->getModel(emd.getModelRef());
    if (referencedmod == NULL) 
    {
      fail = true;
    }
  }

  delete doc;
  inv(fail == false);
}
END_CONSTRAINT

//TODO: 20306 - caught at read md5
//TODO: 20307 - caught at read anyURI
// 20308 - caught at read
// 20309 - string
//TODO: 20310 

//20501 - caught at read
//20502 - caught at read
//20503 - caught at read
//20504 - caught at read
//20505 - caught at read
//20506 - caught at read


//Submodel constraints
//20601 - caught at read
//20602 - caught at read
//20603 - caught at read
//20604 - caught at read
//20605 - caught at read
//20606 - caught at read
//20607 - caught at read
//20608 - caught at read

//20609-20612 - non existant

//20613 - caught at read
//20614 - caught at read

//20615
START_CONSTRAINT (CompSubmodelMustReferenceModel, Submodel, s)
{
  pre (s.isSetModelRef() == true);

  bool fail = true;

  msg = "<submodel> '";
  msg += s.getId() ;
  msg += "' in ";
  const Model* mod = static_cast<const Model*>(s.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(s.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg += " refers to a model with id '";
  msg += s.getModelRef();
  msg += "' that does not exist in the referenced document.";

  // do we reference the actual model
  // do not report this here as it is another error
  if (s.getModelRef() == m.getId())
  {
    fail = false;
  }

  if (fail == true)
  {
    // do we refernce an external modelDefinition
    CompSBMLDocumentPlugin *docPlug = (CompSBMLDocumentPlugin*)
      (m.getSBMLDocument()->getPlugin("comp"));
    ModelDefinition * md = docPlug->getModelDefinition(s.getModelRef());
    if (md == NULL)
    {
      ExternalModelDefinition * ext = 
        docPlug->getExternalModelDefinition(s.getModelRef());

      if (ext != NULL)
      {
        fail = false;
      }
    }
    else
    {
      fail = false;
    }
  }

  inv(fail == false);
}
END_CONSTRAINT

//20616
START_CONSTRAINT (CompSubmodelCannotReferenceSelf, Submodel, s)
{
  pre (s.isSetModelRef() == true);

  bool fail = false;

  msg = "<submodel> '";
  msg += s.getId() ;
  msg += "' in ";
  const Model* mod = static_cast<const Model*>(s.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(s.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg += " refers to the enclosing model with id '";
  msg += s.getModelRef();
  msg += "'.";

  if (m.getId() == s.getModelRef())
  {
    fail = true;
  }
  
  inv(fail == false);

}
END_CONSTRAINT

//TODO: 20617 circular dependencies

// 20618 - 20621 non existant

//20622
START_CONSTRAINT (CompTimeConversionMustBeParameter, Submodel, s)
{
  pre (s.isSetTimeConversionFactor() == true);

  bool fail = false;

  msg = "The 'timeConversionFactor' of <submodel> '";
  msg += s.getId() ;
  msg += "' in ";
  const Model* mod = static_cast<const Model*>(s.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(s.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg += " is set to '";
  msg += s.getTimeConversionFactor();
  msg += "' which is not a <parameter> within the <model>.";

  if (m.getParameter(s.getTimeConversionFactor()) == NULL)
  {
    fail = true;
  }
  
  inv(fail == false);

}
END_CONSTRAINT

//20623
START_CONSTRAINT (CompExtentConversionMustBeParameter, Submodel, s)
{
  pre (s.isSetExtentConversionFactor() == true);

  bool fail = false;

  msg = "The 'extentConversionFactor' of <submodel> '";
  msg += s.getId() ;
  msg += "' in ";
  const Model* mod = static_cast<const Model*>(s.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(s.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg += " is set to '";
  msg += s.getExtentConversionFactor();
  msg += "' which is not a <parameter> within the <model>.";

  if (m.getParameter(s.getExtentConversionFactor()) == NULL)
  {
    fail = true;
  }
  
  inv(fail == false);

}
END_CONSTRAINT

//*************************************

//SBaseRef constraints
// -  need to implement for each object that derives from SBaseRef
//20701
// Port doesnt have portRef

// 20701 - deletion
START_CONSTRAINT (CompPortRefMustReferencePort, Deletion, d)
{
  pre(d.isSetPortRef());
  
  bool fail = false;

  const Submodel * sub = static_cast<const Submodel*>
    (d.getAncestorOfType(SBML_COMP_SUBMODEL, "comp"));


  msg = "The 'portRef' of a <deletion>";
  msg += " is set to '";
  msg += d.getPortRef();
  msg += "' which is not a <port> within the <model> referenced by ";
  msg += "submodel '";
  msg += sub->getId();
  msg += "'.";

  std::string modelId = sub->getModelRef();

  const SBMLDocument * doc = d.getSBMLDocument();
  CompSBMLDocumentPlugin * docPlug = 
    (CompSBMLDocumentPlugin*)(doc->getPlugin("comp"));
  Model * referencedModel = docPlug->getModelDefinition(modelId);
  if (referencedModel == NULL)
  {
    fail = true;
  }
  else
  {
    CompModelPlugin *plug1 = (CompModelPlugin*)(referencedModel->getPlugin("comp"));
    if (plug1->getPort(d.getPortRef()) == NULL)
    {
      fail = true;
    }
  }

  inv(fail == false);
}
END_CONSTRAINT

// 20701 - replacedElement
START_CONSTRAINT (CompPortRefMustReferencePort, ReplacedElement, repE)
{
  pre(repE.isSetPortRef());
  pre(repE.isSetSubmodelRef());

  bool fail = false;

  msg = "The 'portRef' of a <replacedElement>";
  msg += " is set to '";
  msg += repE.getPortRef();
  msg += "' which is not a <port> within the <model> referenced by ";
  msg += "submodel '";
  msg += repE.getSubmodelRef();
  msg += "'.";

  /* need to be using the correct model */
  CompModelPlugin *plug = (CompModelPlugin*)(m.getPlugin("comp"));
  std::string modelId = (plug->getSubmodel(repE.getSubmodelRef()))->getModelRef();

  const SBMLDocument * doc = repE.getSBMLDocument();
  CompSBMLDocumentPlugin * docPlug = 
    (CompSBMLDocumentPlugin*)(doc->getPlugin("comp"));
  Model * referencedModel = docPlug->getModelDefinition(modelId);
  if (referencedModel == NULL)
  {
    fail = true;
  }
  else
  {
    CompModelPlugin *plug1 = (CompModelPlugin*)(referencedModel->getPlugin("comp"));
    if (plug1->getPort(repE.getPortRef()) == NULL)
    {
      fail = true;
    }
  }

  inv(fail == false);
}
END_CONSTRAINT

// 20701 - replacedBy
START_CONSTRAINT (CompPortRefMustReferencePort, ReplacedBy, repBy)
{
  pre(repBy.isSetPortRef());
  pre(repBy.isSetSubmodelRef());

  bool fail = false;

  msg = "The 'portRef' of a <replacedBy>";
  msg += " is set to '";
  msg += repBy.getPortRef();
  msg += "' which is not a <port> within the <model> referenced by ";
  msg += "submodel '";
  msg += repBy.getSubmodelRef();
  msg += "'.";

  /* need to be using the correct model */
  CompModelPlugin *plug = (CompModelPlugin*)(m.getPlugin("comp"));
  std::string modelId = (plug->getSubmodel(repBy.getSubmodelRef()))->getModelRef();

  const SBMLDocument * doc = repBy.getSBMLDocument();
  CompSBMLDocumentPlugin * docPlug = 
    (CompSBMLDocumentPlugin*)(doc->getPlugin("comp"));
  Model * referencedModel = docPlug->getModelDefinition(modelId);
  if (referencedModel == NULL)
  {
    fail = true;
  }
  else
  {
    CompModelPlugin *plug1 = (CompModelPlugin*)(referencedModel->getPlugin("comp"));
    if (plug1->getPort(repBy.getPortRef()) == NULL)
    {
      fail = true;
    }
  }

  inv(fail == false);
}
END_CONSTRAINT

//20702 - to do

//20703 - port
START_CONSTRAINT (CompUnitRefMustReferenceUnitDef, Port, p)
{
  pre(p.isSetUnitRef());
  
  bool fail = false;

  msg = "The 'unitRef' of a <port>";
  msg += " is set to '";
  msg += p.getUnitRef();
  msg += "' which is not a <unitDefinition> within the <model>.";

  if (m.getUnitDefinition(p.getUnitRef()) == NULL)
  {
    fail = true;
  }

  inv(fail == false);
}
END_CONSTRAINT

// 20703 - deletion
START_CONSTRAINT (CompUnitRefMustReferenceUnitDef, Deletion, d)
{
  pre(d.isSetUnitRef());
  
  bool fail = false;

  const Submodel * sub = static_cast<const Submodel*>
    (d.getAncestorOfType(SBML_COMP_SUBMODEL, "comp"));


  msg = "The 'unitRef' of a <deletion>";
  msg += " is set to '";
  msg += d.getUnitRef();
  msg += "' which is not a <unitDefinition> within the <model> referenced by ";
  msg += "submodel '";
  msg += sub->getId();
  msg += "'.";

  std::string modelId = sub->getModelRef();

  const SBMLDocument * doc = d.getSBMLDocument();
  CompSBMLDocumentPlugin * docPlug = 
    (CompSBMLDocumentPlugin*)(doc->getPlugin("comp"));
  Model * referencedModel = docPlug->getModelDefinition(modelId);
  if (referencedModel == NULL)
  {
    fail = true;
  }
  else
  {
    if (referencedModel->getUnitDefinition(d.getUnitRef()) == NULL)
    {
      fail = true;
    }
  }

  inv(fail == false);
}
END_CONSTRAINT

// 20703 - replacedElement
START_CONSTRAINT (CompUnitRefMustReferenceUnitDef, ReplacedElement, repE)
{
  pre(repE.isSetUnitRef());
  pre(repE.isSetSubmodelRef());

  bool fail = false;

  msg = "The 'unitRef' of a <replacedElement>";
  msg += " is set to '";
  msg += repE.getUnitRef();
  msg += "' which is not a <unitDefinition> within the <model> referenced by ";
  msg += "submodel '";
  msg += repE.getSubmodelRef();
  msg += "'.";

  /* need to be using the correct model */
  CompModelPlugin *plug = (CompModelPlugin*)(m.getPlugin("comp"));
  std::string modelId = (plug->getSubmodel(repE.getSubmodelRef()))->getModelRef();

  const SBMLDocument * doc = repE.getSBMLDocument();
  CompSBMLDocumentPlugin * docPlug = 
    (CompSBMLDocumentPlugin*)(doc->getPlugin("comp"));
  Model * referencedModel = docPlug->getModelDefinition(modelId);
  if (referencedModel == NULL)
  {
    fail = true;
  }
  else
  {
    if (referencedModel->getUnitDefinition(repE.getUnitRef()) == NULL)
    {
      fail = true;
    }
  }

  inv(fail == false);
}
END_CONSTRAINT

// 20703 - replacedBy
START_CONSTRAINT (CompUnitRefMustReferenceUnitDef, ReplacedBy, repBy)
{
  pre(repBy.isSetUnitRef());
  pre(repBy.isSetSubmodelRef());

  bool fail = false;

  msg = "The 'unitRef' of a <replacedBy>";
  msg += " is set to '";
  msg += repBy.getUnitRef();
  msg += "' which is not a <unitDefinition> within the <model> referenced by ";
  msg += "submodel '";
  msg += repBy.getSubmodelRef();
  msg += "'.";

  /* need to be using the correct model */
  CompModelPlugin *plug = (CompModelPlugin*)(m.getPlugin("comp"));
  std::string modelId = (plug->getSubmodel(repBy.getSubmodelRef()))->getModelRef();

  const SBMLDocument * doc = repBy.getSBMLDocument();
  CompSBMLDocumentPlugin * docPlug = 
    (CompSBMLDocumentPlugin*)(doc->getPlugin("comp"));
  Model * referencedModel = docPlug->getModelDefinition(modelId);
  if (referencedModel == NULL)
  {
    fail = true;
  }
  else
  {
    if (referencedModel->getUnitDefinition(repBy.getUnitRef()) == NULL)
    {
      fail = true;
    }
  }

  inv(fail == false);
}
END_CONSTRAINT


//
//20704
//20705
//20706
//20707
//20708
//20709

//*************************************

//Port constraints

//20801
START_CONSTRAINT (CompPortMustReferenceObject, Port, p)
{
  pre (p.isSetId());

  bool idRef = p.isSetIdRef();
  bool unitRef = p.isSetUnitRef();
  bool metaidRef = p.isSetMetaIdRef();

  msg = "<port> '";
  msg += p.getId() ;
  msg += "' in ";
  const Model* mod = static_cast<const Model*>(p.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(p.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg += " does not refer to another object.";

  bool fail = true;

  if (idRef == true)
  {
    fail = false;
  }
  else if (unitRef == true)
  {
    fail = false;
  }
  else if (metaidRef == true)
  {
    fail = false;
  }

  inv(fail == false);


}
END_CONSTRAINT


//20802
START_CONSTRAINT (CompPortMustReferenceOnlyOneObject, Port, p)
{
  pre (p.isSetId());

  bool idRef = p.isSetIdRef();
  bool unitRef = p.isSetUnitRef();
  bool metaidRef = p.isSetMetaIdRef();

  bool fail = false;

  msg = "<port> '";
  msg += p.getId() ;
  msg += "' in ";
  const Model* mod = static_cast<const Model*>(p.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(p.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg += " refers to ";

  if (idRef == true)
  {
    msg += "object with id '";
    msg += p.getIdRef();
    msg += "' and also ";
    if (unitRef == true)
    {
      fail = true;
      msg += "unit with id '";
      msg += p.getUnitRef();
      msg += "'";

      if ( metaidRef == true)
      {
        msg += "and also object with metaid '";
        msg += p.getMetaIdRef();
        msg += "'.";
      }
    }
    else if (metaidRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += p.getMetaIdRef();
      msg += "'.";
    }
  }
  else if (unitRef == true)
  {
    msg += "unit with id '";
    msg += p.getUnitRef();
    msg += "' and also ";
    
    if (metaidRef == true)
    {
      fail = true;
      msg += "object with metaid '";
      msg += p.getMetaIdRef();
      msg += "'.";
    }
  }

  inv(fail == false);

}
END_CONSTRAINT


//20803 - caught at read

//20804 - to do

//*************************************

//Deletion constraints

//20901
START_CONSTRAINT (CompDeletionMustReferenceObject, Deletion, d)
{
  //pre (d.isSetId());

  bool idRef = d.isSetIdRef();
  bool unitRef = d.isSetUnitRef();
  bool metaidRef = d.isSetMetaIdRef();
  bool portRef  = d.isSetPortRef();

  msg = "<Deletion> '";
  msg += d.getId() ;
  msg += "' in ";
  const Model* mod = static_cast<const Model*>(d.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(d.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg += " does not refer to another object.";

  bool fail = true;

  if (idRef == true)
  {
    fail = false;
  }
  else if (unitRef == true)
  {
    fail = false;
  }
  else if (metaidRef == true)
  {
    fail = false;
  }
  else if (portRef == true)
  {
    fail = false;
  }

  inv(fail == false);


}
END_CONSTRAINT


//20902
START_CONSTRAINT (CompDeletionMustReferOnlyOneObject, Deletion, d)
{
  //pre (d.isSetId());

  bool idRef = d.isSetIdRef();
  bool unitRef = d.isSetUnitRef();
  bool metaidRef = d.isSetMetaIdRef();
  bool portRef = d.isSetPortRef();

  bool fail = false;

  msg = "<Deletion> '";
  msg += d.getId() ;
  msg += "' in ";
  const Model* mod = static_cast<const Model*>(d.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(d.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg += " refers to ";

  if (idRef == true)
  {
    msg += "object with id '";
    msg += d.getIdRef();
    msg += "'";
    if (unitRef == true)
    {
      fail = true;
      msg += "and also unit with id '";
      msg += d.getUnitRef();
      msg += "'";

      if ( metaidRef == true)
      {
        msg += "and also object with metaid '";
        msg += d.getMetaIdRef();
        msg += "'";
      }

      if (portRef == true)
      {
        msg += "and also port with id '";
        msg += d.getPortRef();
        msg += "'";
      }
      msg += ".";
    }
    else if (metaidRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += d.getMetaIdRef();
      msg += "'";
 
      if (portRef == true)
      {
        msg += "and also port with id '";
        msg += d.getPortRef();
        msg += "'";
      }
      msg += ".";
    }
    else if (portRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += d.getMetaIdRef();
      msg += "'.";
    }
  }
  else if (unitRef == true)
  {
    msg += "unit with id '";
    msg += d.getUnitRef();
    msg += "' and also ";
    
    if (metaidRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += d.getMetaIdRef();
      msg += "'";
 
      if (portRef == true)
      {
        msg += "and also port with id '";
        msg += d.getPortRef();
        msg += "'";
      }
      msg += ".";
    }
    else if (portRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += d.getMetaIdRef();
      msg += "'.";
    }
  }
  else if (metaidRef == true)
  {
    msg += "object with metaid '";
    msg += d.getMetaIdRef();
    msg += "'";

    if (portRef == true)
    {
      fail = true;
      msg += "and also port with id '";
      msg += d.getPortRef();
      msg += "'";
    }
    msg += ".";
  }

  inv(fail == false);

}
END_CONSTRAINT


//20903 - caught at read

//*************************************

//ReplacedElement constraints

//21001
START_CONSTRAINT (CompReplacedElementMustRefObject, ReplacedElement, repE)
{
  pre (repE.isSetSubmodelRef());

  bool idRef = repE.isSetIdRef();
  bool unitRef = repE.isSetUnitRef();
  bool metaidRef = repE.isSetMetaIdRef();
  bool portRef  = repE.isSetPortRef();
  bool deletion = repE.isSetDeletion();

  msg = "A <replacedElement> in ";
  const Model* mod = static_cast<const Model*>(repE.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(repE.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg = " does not refer to another object.";

  bool fail = true;

  if (idRef == true)
  {
    fail = false;
  }
  else if (unitRef == true)
  {
    fail = false;
  }
  else if (metaidRef == true)
  {
    fail = false;
  }
  else if (portRef == true)
  {
    fail = false;
  }
  else if (deletion == true)
  {
    fail = false;
  }

  inv(fail == false);


}
END_CONSTRAINT


//21002
START_CONSTRAINT (CompReplacedElementMustRefOnlyOne, ReplacedElement, repE)
{
  pre (repE.isSetSubmodelRef());

  bool idRef = repE.isSetIdRef();
  bool unitRef = repE.isSetUnitRef();
  bool metaidRef = repE.isSetMetaIdRef();
  bool portRef  = repE.isSetPortRef();
  bool deletion = repE.isSetDeletion();

  bool fail = false;

  msg = "<replacedElement> '";
  msg += repE.getId() ;
  msg += "' in ";
  const Model* mod = static_cast<const Model*>(repE.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(repE.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg += " refers to ";

  if (idRef == true)
  {
    msg += "object with id '";
    msg += repE.getIdRef();
    msg += "'";
    if (unitRef == true)
    {
      fail = true;
      msg += "and also unit with id '";
      msg += repE.getUnitRef();
      msg += "'";

      if ( metaidRef == true)
      {
        msg += "and also object with metaid '";
        msg += repE.getMetaIdRef();
        msg += "'";
      }

      if (portRef == true)
      {
        msg += "and also port with id '";
        msg += repE.getPortRef();
        msg += "'";
      }

      if (deletion == true)
      {
        msg += "and also deletion object '";
        msg += repE.getDeletion();
        msg += "'";
      }
      msg += ".";
    }
    else if (metaidRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += repE.getMetaIdRef();
      msg += "'";
 
      if (portRef == true)
      {
        msg += "and also port with id '";
        msg += repE.getPortRef();
        msg += "'";
      }

      if (deletion == true)
      {
        msg += "and also deletion object '";
        msg += repE.getDeletion();
        msg += "'";
      }
      msg += ".";
    }
    else if (portRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += repE.getMetaIdRef();

      if (deletion == true)
      {
        msg += "and also deletion object '";
        msg += repE.getDeletion();
        msg += "'";
      }
      msg += "'.";
    }
    else if (deletion == true)
    {
      fail = true;
      msg += "and also deletion object '";
      msg += repE.getDeletion();
      msg += "'.";
    }
  }
  else if (unitRef == true)
  {
    msg += "unit with id '";
    msg += repE.getUnitRef();
    msg += "' and also ";
    
    if (metaidRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += repE.getMetaIdRef();
      msg += "'";
 
      if (portRef == true)
      {
        msg += "and also port with id '";
        msg += repE.getPortRef();
        msg += "'";
      }
 
      if (deletion == true)
      {
        msg += "and also deletion object '";
        msg += repE.getDeletion();
        msg += "'";
      }
      msg += ".";
    }
    else if (portRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += repE.getMetaIdRef();
 
      if (deletion == true)
      {
        msg += "and also deletion object '";
        msg += repE.getDeletion();
        msg += "'";
      }
      msg += "'.";
    }
    else if (deletion == true)
    {
      fail = true;
      msg += "and also deletion object '";
      msg += repE.getDeletion();
      msg += "'.";
   }
  }
  else if (metaidRef == true)
  {
    msg += "object with metaid '";
    msg += repE.getMetaIdRef();
    msg += "'";

    if (portRef == true)
    {
      fail = true;
      msg += "and also port with id '";
      msg += repE.getPortRef();
      msg += "'";
    }
 
    if (deletion == true)
    {
      msg += "and also deletion object '";
      msg += repE.getDeletion();
      msg += "'";
    }
    msg += ".";
  }
  else if (portRef == true)
  {
    msg += "port with id '";
    msg += repE.getPortRef();
    msg += "'";

    if (deletion == true)
    {
      fail = true;
      msg += "and also deletion object '";
      msg += repE.getDeletion();
      msg += "'";
    }
     msg += ".";
  }

  inv(fail == false);

}
END_CONSTRAINT

//21003 - caught at read

//21004
START_CONSTRAINT (CompReplacedElementSubModelRef, ReplacedElement, repE)
{
  pre (repE.isSetSubmodelRef());

  msg = "The <replacedElement> refers to the submodel '";
  msg += repE.getSubmodelRef();
  msg += "' that is not part of the parent model.";

  bool fail = false;

  const CompModelPlugin * plug = static_cast<const CompModelPlugin*>(m.getPlugin("comp"));
  if (plug != NULL
    && plug->getSubmodel(repE.getSubmodelRef()) == NULL)
  {
    fail = true;
  }

  inv(fail == false);
}
END_CONSTRAINT


//21005
START_CONSTRAINT (CompReplacedElementDeletionRef, ReplacedElement, repE)
{
  pre (repE.isSetSubmodelRef());
  pre (repE.isSetDeletion());

  msg = "A <replacedElement> in ";
  const Model* mod = static_cast<const Model*>(repE.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(repE.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg = " refers to the deletion '";
  msg += repE.getDeletion();
  msg += "' that is not part of the parent model.";

  bool fail = false;

  const CompModelPlugin * plug = 
    static_cast<const CompModelPlugin*>(m.getPlugin("comp"));
  if (plug != NULL)
  {
    const Submodel * sub  = plug->getSubmodel(repE.getSubmodelRef());

    if (sub != NULL && sub->getDeletion(repE.getDeletion()) == NULL)
    {
      fail = true;
    }
  }

  inv(fail == false);
}
END_CONSTRAINT

//21006
START_CONSTRAINT (CompReplacedElementConvFactorRef, ReplacedElement, repE)
{
  pre (repE.isSetSubmodelRef());
  pre (repE.isSetConversionFactor());

  msg = "The 'timeConversionFactor' of a <replacedElement> in ";
  const Model* mod = static_cast<const Model*>(repE.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(repE.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg = " is set to '";
  msg += repE.getConversionFactor();
  msg += "' which is not a <parameter> within the model.";

  bool fail = false;

  if (m.getParameter(repE.getConversionFactor()) == NULL)
  {
    fail = true;
  }

  inv(fail == false);
}
END_CONSTRAINT

//21007 - repeat of 10308
//21008 - repeat of 10309
//21009 - repeat of 10310

//21010 - to do

//*************************************

//ReplacedBy constraints

//21101
START_CONSTRAINT (CompReplacedByMustRefObject, ReplacedBy, repBy)
{
  pre (repBy.isSetSubmodelRef());

  bool idRef = repBy.isSetIdRef();
  bool unitRef = repBy.isSetUnitRef();
  bool metaidRef = repBy.isSetMetaIdRef();
  bool portRef = repBy.isSetPortRef();

  msg = "A <replacedBy> in ";
  const Model* mod = static_cast<const Model*>(repBy.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(repBy.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg = " does not refer to another object.";

  bool fail = true;

  if (idRef == true)
  {
    fail = false;
  }
  else if (unitRef == true)
  {
    fail = false;
  }
  else if (metaidRef == true)
  {
    fail = false;
  }
  else if (portRef == true)
  {
    fail = false;
  }

  inv(fail == false);
}
END_CONSTRAINT


//21102
START_CONSTRAINT (CompReplacedByMustRefOnlyOne, ReplacedBy, repBy)
{
  pre (repBy.isSetSubmodelRef());

  bool idRef = repBy.isSetIdRef();
  bool unitRef = repBy.isSetUnitRef();
  bool metaidRef = repBy.isSetMetaIdRef();
  bool portRef = repBy.isSetPortRef();

  bool fail = false;

  msg = "A <replacedBy> object in ";
  const Model* mod = static_cast<const Model*>(repBy.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(repBy.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg = " refers to ";

  if (idRef == true)
  {
    msg += "object with id '";
    msg += repBy.getIdRef();
    msg += "'";
    if (unitRef == true)
    {
      fail = true;
      msg += "and also unit with id '";
      msg += repBy.getUnitRef();
      msg += "'";

      if ( metaidRef == true)
      {
        msg += "and also object with metaid '";
        msg += repBy.getMetaIdRef();
        msg += "'";
      }

      if (portRef == true)
      {
        msg += "and also port with id '";
        msg += repBy.getPortRef();
        msg += "'";
      }
      msg += ".";
    }
    else if (metaidRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += repBy.getMetaIdRef();
      msg += "'";
 
      if (portRef == true)
      {
        msg += "and also port with id '";
        msg += repBy.getPortRef();
        msg += "'";
      }
      msg += ".";
    }
    else if (portRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += repBy.getMetaIdRef();
      msg += "'.";
    }
  }
  else if (unitRef == true)
  {
    msg += "unit with id '";
    msg += repBy.getUnitRef();
    msg += "' and also ";
    
    if (metaidRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += repBy.getMetaIdRef();
      msg += "'";
 
      if (portRef == true)
      {
        msg += "and also port with id '";
        msg += repBy.getPortRef();
        msg += "'";
      }
      msg += ".";
    }
    else if (portRef == true)
    {
      fail = true;
      msg += "and also object with metaid '";
      msg += repBy.getMetaIdRef();
      msg += "'.";
    }
  }
  else if (metaidRef == true)
  {
    msg += "object with metaid '";
    msg += repBy.getMetaIdRef();
    msg += "'";

    if (portRef == true)
    {
      fail = true;
      msg += "and also port with id '";
      msg += repBy.getPortRef();
      msg += "'";
    }
    msg += ".";
  }

  inv(fail == false);

}
END_CONSTRAINT

//21103 - caught at read

//21104
START_CONSTRAINT (CompReplacedBySubModelRef, ReplacedBy, repBy)
{
  pre (repBy.isSetSubmodelRef());

  msg = "A <replacedElement> in ";
  const Model* mod = static_cast<const Model*>(repBy.getAncestorOfType(SBML_MODEL, "core"));
  if (mod == NULL) {
    mod = static_cast<const Model*>(repBy.getAncestorOfType(SBML_COMP_MODELDEFINITION, "comp"));
  }
  if (mod == NULL || !mod->isSetId()) {
    msg += "the main model in the document";
  }
  else {
    msg += "the model '";
    msg += mod->getId();
    msg += "'";
  }
  msg = " refers to the submodel '";
  msg += repBy.getSubmodelRef();
  msg += "' that is not part of the parent model.";

  bool fail = false;

  const CompModelPlugin * plug = static_cast<const CompModelPlugin*>(m.getPlugin("comp"));
  if (plug != NULL
    && plug->getSubmodel(repBy.getSubmodelRef()) == NULL)
  {
    fail = true;
  }

  inv(fail == false);
}
END_CONSTRAINT




/** @endcond */

