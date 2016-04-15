/**
 * @file:   MultiSBMLError.h
 * @brief:  Implementation of the MultiSBMLError class
 * @author: Fengkai Zhang
 * @author: SBMLTeam
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
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
 * ------------------------------------------------------------------------ -->
 */


#ifndef MultiSBMLError_H__
#define MultiSBMLError_H__



LIBSBML_CPP_NAMESPACE_BEGIN

BEGIN_C_DECLS


typedef enum
{
  MultiUnknownError                 	= 7010100 /*!< Unknown error from multi */

  // General rules about the Multi package
, MultiNSUndeclared                     = 7010101 /*!< The Multi ns is not correctly declared */
, MultiElementNotInNs                   = 7010102 /*!< Element not in Multi namespace */
, MultiSBML_RequiredAttMissing          = 7010103 /*!< The 'multi:required' attribute is required on <code>&lt;sbml&gt;</code> */
, MultiSBML_RequiredAttMustBeBoolean    = 7010104 /*!< The 'multi:required' attribute must be Boolean */
, MultiSBML_RequiredAttMustBeTrue       = 7010105 /*!< The 'multi:required' attribute must be 'true' */

  // General rules about identifiers
, MultiDupClaId			        = 7010301 /*!< Duplicate 'id' attribute value */
, MultiInvSIdSyn                        = 7010302 /*!< Invalid SId syntax */
, MultiInvNamSyn                       	= 7010303 /*!< Invalid name syntax */

  // scoping rules for identifiers, sec 3.28
, MultiUnqId_SptIns                     = 7010401 /*!< SpeciesTypeInstance must have unique ids within the parent speciesType */
, MultiUnqId_SptCpoInd       		= 7010402 /*!< SpeciesTypeComponentIndex must have unique ids within the parent speciesType */
, MultiUnqId_InSptBnd               	= 7010403 /*!< InSpeciesTypeBond must have unique ids within the parent speciesType */
, MultiUnqId_Sft                        = 7010404 /*!< SpeciesFeatureType must have unique ids within the parent speciesType */
, MultiUnqId_SubListOfSfs               = 7010405 /*!< SubListOfSpeciesFeatures must have unique ids within a species */
, MultiUnqId_SpeFtr                  	= 7010406 /*!< SpeciesFeature must have unique ids within a species */
, MultiUnqId_SpeFtrChg            	= 7010407 /*!< SpeciesFeatureChange must have unique ids within a speciesTypeComponentMapInProduct */
, MultiUnqId_CpaRef           		= 7010408 /*!< CompartmentReference must have unique ids within a compartment */

  // SIdRef under SpeciesType
, MultiInvSIdRefSyn_Spt_CpaAtt		= 7010501 /*!< Invalid SIdRef syntax: 'compartment' attribute of SpeciesType */
, MultiInvSIdRefSyn_PslSpeFtrVal_NumAtt	= 7010502 /*!< Invalid SIdRef syntax: 'numericValue' attribute of PossibleSpeciesFeatureValue */
, MultiInvSIdRefSyn_SptIns_SptAtt	= 7010503 /*!< Invalid SIdRef syntax: 'speciesType' attribute of SpeciesTypeInstance */
, MultiInvSIdRefSyn_SptIns_CpaRefAtt	= 7010504 /*!< Invalid SIdRef syntax: 'compartmentReference' attribute of SpeciesTypeInstance */
, MultiInvSIdRefSyn_SptCpoInd_CpoAtt	= 7010505 /*!< Invalid SIdRef syntax: 'component' attribute of SpeciesTypeComponentIndex */
, MultiInvSIdRefSyn_SptCpoInd_ParAtt 	= 7010506 /*!< Invalid SIdRef syntax: 'identifyingParent' attribute of SpeciesTypeComponentIndex */
//, MultiInvSIdRefSyn_DenSptCpoInd_IndAtt	= 7010507 /*!< Invalid SIdRef syntax: 'speciesTypeComponentIndex' attribute of DenotedSpeciesTypeComponentIndex */
, MultiInvSIdRefSyn_InSptBnd_Bst1Att	= 7010508 /*!< Invalid SIdRef syntax: 'bindingSite1' attribute of InSpeciesTypeBond */
, MultiInvSIdRefSyn_InSptBnd_Bst2Att	= 7010509 /*!< Invalid SIdRef syntax: 'bindingSite2' attribute of InSpeciesTypeBond */

  // SIdRef under Species
, MultiInvSIdRefSyn_Spe_SptAtt		= 7010601 /*!< Invalid SIdRef syntax: 'speciesType' attribute of extended Species */
, MultiInvSIdRefSyn_OutBst_CpoAtt	= 7010602 /*!< Invalid SIdRef syntax: 'component' attribute of OutwardBindingSite */
, MultiInvSIdRefSyn_SpeFtr_SpeFtrTypAtt	= 7010603 /*!< Invalid SIdRef syntax: 'speciesFeatureType' attribute of SpeciesFeature */
, MultiInvSIdRefSyn_SpeFtr_CpoAtt 	= 7010604 /*!< Invalid SIdRef syntax: 'component' attribute of SpeciesFeature */
, MultiInvSIdRefSyn_SpeFtrVal_ValAtt 	= 7010605 /*!< Invalid SIdRef syntax: 'value' attribute of SpeciesFeatureValue */

  // SIdRef under Reaction
, MultiInvSIdRefSyn_SplSpeRef_CompRefAtt = 7010701 /*!< Invalid SIdRef syntax: 'compartmentReference' attribute of extended SimpleSpeciesReference */
, MultiInvSIdRefSyn_StpCpoMapInPro_RctAtt = 7010702 /*!< Invalid SIdRef syntax: 'reactant' attribute of SpeciesTypeComponentMapInProduct */
, MultiInvSIdRefSyn_StpCpoMapInPro_RctCpoAtt = 7010703 /*!< Invalid SIdRef syntax: 'reactantComponent' attribute of SpeciesTypeComponentMapInProduct */
, MultiInvSIdRefSyn_StpCpoMapInPro_ProCpoAtt = 7010704 /*!< Invalid SIdRef syntax: 'productComponent' attribute of SpeciesTypeComponentMapInProduct */
, MultiInvSIdRefSyn_SpeFtrChg_RctSpeFtrAtt = 7010705 /*!< Invalid SIdRef syntax: 'reactantSpeciesFeature' attribute of SpeciesFeatureChange */
, MultiInvSIdRefSyn_SpeFtrChg_ProSpeFtrAtt = 7010706 /*!< Invalid SIdRef syntax: 'productSpeciesFeature' attribute of SpeciesFeatureChange */

  // SIdRef under Compartment
, MultiInvSIdRefSyn_Cpa_CpaTypAtt 	= 7010801 /*!< Invalid SIdRef syntax: 'compartmentType' attribute of extended Compartment */
, MultiInvSIdRefSyn_CpaRef_CpaAtt 	= 7010802 /*!< Invalid SIdRef syntax: 'compartment' attribute of CompartmentReference */


// Rules by class, 702****

// Rules for extended Model objects
, MultiLofStps_OnlyOne       		= 7020101 /*!< ListOfSpeciesTypes: Only one object allowed in a model */
, MultiLofStps_NoEmpty          	= 7020102 /*!< ListOfSpeciesTypes: Must not be empty */
, MultiLofStps_AllowedAtts  		= 7020103 /*!< ListOfSpeciesTypes: Allowed attributes */
, MultiLofStps_AllowedElts  		= 7020104 /*!< ListOfSpeciesTypes: Allowed elements */

// Rules for extended Compartment objects
, MultiExCpa_AllowedMultiAtts           = 7020201 /*!< Extended Compartment: Allowed Multi attributes */
, MultiExCpa_IsTypeAtt_Invalid          = 7020202 /*!< Extended Compartment: Invalid boolean syntax of 'isType' attribute */
, MultiExCpa_IsTypeAtt_Required         = 7020203 /*!< Extended Compartment: 'isType' attribute is requried */
, MultiExCpa_IsTypeAtt_SameAsParent     = 7020204 /*!< Extended Compartment: 'isType' attribute, if referenced, must be same as that of the containing compartment */
, MultiExCpa_CpaTypAtt_Restrict  	= 7020205 /*!< Extended Compartment: Compartment type can not reference another compartment type */
, MultiLofCpaRefs_OnlyOne     		= 7020206 /*!< ListOfCompartmentReferences: Only one object allowed in a compartment */
, MultiLofCpaRefs_NoEmpty         	= 7020207 /*!< ListOfCompartmentReferences: Must not be empty */
, MultiLofCpaRefs_AllowedAtts 		= 7020208 /*!< ListOfCompartmentReferences: Allowed attributes */
, MultiLofCpaRefs_AllowedElts 		= 7020209 /*!< ListOfCompartmentReferences: Allowed elements */

// Rules for CompartmentReference objects
, MultiCpaRef_AllowedCoreAtts  		= 7020301 /*!< CompartmentReference: Allowed SBML core attributes  */
, MultiCpaRef_AllowedCoreElts 		= 7020302 /*!< CompartmentReference: Allowed SBML core elements */
, MultiCpaRef_AllowedMultiAtts 		= 7020303 /*!< CompartmentReference: Allowed Multi attributes */
, MultiCpaRef_CompartmentAtt_Ref        = 7020304 /*!< CompartmentReference: 'compartment' must be the 'id' of a compartment */
, MultiCpaRef_IdRequiredOrOptional      = 7020305 /*!< CompartmentReference: 'multi:id' is required when referencing the same compartment */

// Rules for SpeciesType objects
, MultiSpt_AllowedCoreAtts  		= 7020401 /*!< SpeciesType: Allowed SBML core attributes */
, MultiSpt_AllowedCoreElts 		= 7020402 /*!< SpeciesType: Allowed SBML core children */
, MultiSpt_AllowedMultiAtts 		= 7020403 /*!< SpeciesType: Allowed Multi attributes */
, MultiSpt_CompartmentAtt_Ref 		= 7020404 /*!< SpeciesType: 'compartment' must be the 'id' of a compartment */
, MultiSpt_ListOfDefs_NoEmpty 		= 7020405 /*!< SpeciesType: ListOf- subobjects must not be empty */
, MultiLofSpeFtrTyps_onlyOne            = 7020406 /*!< ListOfSpeciesFeatureTypes: Only one <listOfSpeciesFeatureTypes> is allowed in a <speciesType> */
, MultiLofSpeFtrTyps_Elts               = 7020407 /*!< ListOfSpeciesFeatureTypes: Only allow speciesFeatureType elements */
, MultiLofSpeFtrTyps_AllowedAtts 	= 7020408 /*!< ListOfSpeciesFeatureTypes: Allowed attributes */
, MultiLofSptInss_onlyOne               = 7020409 /*!< ListOfSpeciesTypeInstances: Only one <listOfSpeciesTypeInstances> is allowed in a <speciesType> */
, MultiLofSptInss_Elts       		= 7020410 /*!< ListOfSpeciesTypeInstances: Only allow speciesTypeInstance elements */
, MultiLofSptInss_AllowedAtts 	        = 7020411 /*!< ListOfSpeciesTypeInstances: Allowed attributes */
, MultiLofSptCpoInds_onlyOne            = 7020412 /*!< ListOfSpeciesTypeComponentIndexes: Only one <listOfSpeciesTypeComponentIndexes> is allowed in a <speciesType> */
, MultiLofSptCpoInds_Elts	        = 7020413 /*!< ListOfSpeciesTypeComponentIndexes: Only allow speciesTypeComponentIndex elements */
, MultiLofSptCpoInds_AllowedAtts	= 7020414 /*!< ListOfSpeciesTypeComponentIndexes: Allowed attributes */
, MultiLofInSptBnds_onlyOne             = 7020415 /*!< ListOfInSpeciesTypeBonds: Only one <listOfInSpeciesTypeBonds> is allowed in a <speciesType> */
, MultiLofInSptBnds_Elts   		= 7020416 /*!< ListOfInSpeciesTypeBonds: Only allow inSpeciesTypeBond elements */
, MultiLofInSptBnds_AllowedAtts         = 7020417 /*!< ListOfInSpeciesTypeBonds: Allowed attributes */

// Rules for BindingSiteSpeciesType objects
, MultiBstSpt_Restrict  		= 7020501 /*!< BindingSiteSpeciesType: Not permitted to have listOfSpeciesTypeInstances */

// Rules for SpeciesFeatureType objects
, MultiSpeFtrTyp_AllowedCoreAtts  	= 7020601 /*!< SpeciesFeatureType: Allowed SBML core attributes */
, MultiSpeFtrTyp_AllowedCoreElts  	= 7020602 /*!< SpeciesFeatureType: Allowed SBML core elements */
, MultiSpeFtrTyp_AllowedMultiAtts  	= 7020603 /*!< SpeciesFeatureType: Allowed Multi attributes */
, MultiSpeFtrTyp_OccAtt_Ref 	 	= 7020604 /*!< SpeciesFeatureType: 'occur' must be a positiveInteger */
, MultiSpeFtrTyp_RestrictElt  		= 7020605 /*!< SpeciesFeatureType: Required to have one listOfPossibleSpeciesFeatureValues */
, MultiLofPsbSpeFtrVals_AllowedAtts	= 7020606 /*!< ListOfPossibleSpeciesFeatureValues: Allowed attributes */
, MultiLofPsbSpeFtrVals_Elts  		= 7020607 /*!< ListOfPossibleSpeciesFeatureValues: Only allow possibleSpeciesFeatureValue elements */
, MultiLofPsbSpeFtrVals_NoEmpty  	= 7020608 /*!< ListOfPossibleSpeciesFeatureValues: Must not be empty */

// Rules for PossibleSpeciesFeatureValue objects
, MultiPsbSpeFtrVal_AllowedCoreAtts	= 7020701 /*!< PossibleSpeciesFeatureValue: Allowed SBML core attributes */
, MultiPsbSpeFtrVal_AllowedCoreElts	= 7020702 /*!< PossibleSpeciesFeatureValue: Allowed SBML core elements */
, MultiPsbSpeFtrVal_AllowedMultiAtts 	= 7020703 /*!< PossibleSpeciesFeatureValue: Allowed Multi attributes */
, MultiPsbSpeFtrVal_NumAtt_Ref		= 7020704 /*!< PossibleSpeciesFeatureValue: 'numbericValue' must be the 'id' of a parameter */

// Rules for SpeciesTypeInstance objects
, MultiSptIns_AllowedCoreAtts  		= 7020801 /*!< SpeciesTypeInstance: Allowed SBML core attributes */
, MultiSptIns_AllowedCoreElts  		= 7020802 /*!< SpeciesTypeInstance: Allowed SBML core elements */
, MultiSptIns_AllowedMultiAtts  	= 7020803 /*!< SpeciesTypeInstance: Allowed Multi attributes */
//, MultiSptIns_OccAtt_Ref  		= 7020804 /*!< SpeciesTypeInstance: 'occur' must be a positiveInteger */
, MultiSptIns_SptAtt_Ref		= 7020805 /*!< SpeciesTypeInstance: 'speciesType' must be the 'id' of a speciesType */
, MultiSptIns_CpaRefAtt_Ref		= 7020806 /*!< SpeciesTypeInstance: 'compartmentReference' must be the 'id' of a compartmentReference */

// Rules for SpeciesTypeComponentIndex objects
, MultiSptCpoInd_AllowedCoreAtts	= 7020901 /*!< SpeciesTypeComponentIndex: Allowed SBML core attributes */
, MultiSptCpoInd_AllowedCoreElts	= 7020902 /*!< SpeciesTypeComponentIndex: Allowed SBML core elements */
, MultiSptCpoInd_AllowedMultiAtts  	= 7020903 /*!< SpeciesTypeComponentIndex: Allowed Multi attributes */
, MultiSptCpoInd_CpoAtt_Ref		= 7020904 /*!< SpeciesTypeComponentIndex: 'component' must be the 'id' of a component */
//, MultiSptCpoInd_RestrictElts		= 7020905 /*!< SpeciesTypeComponentIndex: Can not have a listOfDenotedSpeciesTypeComponentIndexes if component references an index  */
//, MultiSptCpoInd_SameOccurAsRefIndex	= 7020906 /*!< SpeciesTypeComponentIndex: 'occur' attribute must have the same value as that of the referenced index */
, MultiSptCpoInd_IdParAtt_Ref		= 7020907 /*!< SpeciesTypeComponentIndex: 'identifyingParent' must be the 'id' of a component */
//, MultiSptCpoInd_OccAtt_Ref		= 7020908 /*!< SpeciesTypeComponentIndex: 'occur' must be a positiveInteger */
//, MultiLofDenSptCpoInds_NoEmpty  	= 7020909 /*!< ListOfDenotedSpeciesTypeComponentIndexes: Must not be empty */
//, MultiLofDenSptCpoInds_AllowedAtts	= 7020910 /*!< ListOfDenotedSpeciesTypeComponentIndexes: Allowed attributes */
//, MultiLofDenSptCpoInds_Elts  		= 7020911 /*!< ListOfDenotedSpeciesTypeComponentIndexes: Only allow denotedSpeciesTypeComponentIndex elements */

//// Rules for DenotedSpeciesTypeComponentIndex objects
//, MultiDenSptCpoInd_AllowedCoreAtts	= 7021001 /*!< DenotedSpeciesTypeComponentIndex: Allowed SBML core attributes */
//, MultiDenSptCpoInd_AllowedCoreElts	= 7021002 /*!< DenotedSpeciesTypeComponentIndex: Allowed SBML core elements */
//, MultiDenSptCpoInd_AllowedMultiAtts 	= 7021003 /*!< DenotedSpeciesTypeComponentIndex: Allowed Multi attributes */
//, MultiDenSptCpoInd_SpTypeCpoIndAtt_Ref = 7021004 /*!< DenotedSpeciesTypeComponentIndex: 'speciesTypeComponentIndex' must the 'id' of a speciesTypeComponentIndex */

// Rules for InSpeciesTypeBond objects
, MultiInSptBnd_AllowedCoreAtts  	= 7021101 /*!< InSpeciesTypeBond: Allowed SBML core attributes */
, MultiInSptBnd_AllowedCoreElts  	= 7021102 /*!< InSpeciesTypeBond: Allowed SBML core elements */
, MultiInSptBnd_AllowedMultiAtts  	= 7021103 /*!< InSpeciesTypeBond: Allowed Multi attributes */
, MultiInSptBnd_Bst1Att_Ref		= 7021104 /*!< InSpeciesTypeBond: 'bindingSite1' must be the 'id' of a speciesTypeInstance or speciesTypeComponentIndex */
, MultiInSptBnd_Bst2Att_Ref		= 7021105 /*!< InSpeciesTypeBond: 'bindingSite2' must be the 'id' of a speciesTypeInstance or speciesTypeComponentIndex */
, MultiInSptBnd_TwoBstAtts_NotSame	= 7021106 /*!< InSpeciesTypeBond: 'bindingSite1' and 'bindingSite2' can not reference the same binding site */

// Rules for extended Species objects
, MultiExSpe_AllowedMultiAtts         	= 7021201 /*!< Extended Species: Allowed Multi attributes */
, MultiExSpe_RestrictSpeciesTypeAtt	= 7021202 /*!< Extended Species: SpeciesType attribute must have value of the id of a speciesType */
, MultiExSpe_NoEmptyListOfDefs 		= 7021203 /*!< Extended Species: ListOf- subobjects must not be empty */
, MultiLofOutBsts_AllowedAtts     	= 7021204 /*!< ListOfOutwardBindingSites: Allowed attributes */
, MultiLofOutBsts_AllowedElts 		= 7021205 /*!< ListOfOutwardBindingSites: Allowed elements */
, MultiLofSpeFtrs_AllowedAtts     	= 7021206 /*!< ListOfSpeciesFeatures: Allowed SBML attributes */
, MultiSubLofSpeFtrs_AllowedMultiAtts 	= 7021207 /*!< SubListOfSpeciesFeatures: Allowed Multi attributes */
, MultiSubLofSpeFtrs_RelationAtt_Ref	= 7021208 /*!< ListOfSpeciesFeatures: 'relation' must be a value of 'Relation' */
, MultiLofSpeFtrs_AllowedElts 	        = 7021209 /*!< ListOfSpeciesFeatures: Allowed elements */
, MultiSubLofSpeFtrs_AllowedCoreAtts 	= 7021210 /*!< SubListOfSpeciesFeatures: Allowed SBML core attributes */
, MultiSubLofSpeFtrs_AllowedElts 	= 7021211 /*!< SubListOfSpeciesFeatures: Allowed elements */
, MultiSubLofSpeFtrs_CpoAtt_Ref		= 7021212 /*!< SubListOfSpeciesFeatures: 'component' must be the 'id' of a 'SpeciesType' component */


// Rules for OutwardBindingSite objects
, MultiOutBst_AllowedCoreAtts  		= 7021301 /*!< OutwardBindingSite: Allowed SBML core attributes */
, MultiOutBst_AllowedCoreElts  		= 7021302 /*!< OutwardBindingSite: Allowed SBML core elements */
, MultiOutBst_AllowedMultiAtts  	= 7021303 /*!< OutwardBindingSite: Allowed Multi attributes */
, MultiOutBst_BdgStaAtt_Ref		= 7021304 /*!< OutwardBindingSite: 'bindingStatus' must have a value of 'BindingStatus' */
, MultiOutBst_CpoAtt_Ref		= 7021305 /*!< OutwardBindingSite: 'component' must be the 'id' of a 'BindingSiteSpeciesType' component */

// Rules for SpeciesFeature objects
, MultiSpeFtr_AllowedCoreAtts  		= 7021401 /*!< SpeciesFeature: Allowed SBML core attributes */
, MultiSpeFtr_AllowedCoreElts  		= 7021402 /*!< SpeciesFeature: Allowed SBML core elements */
, MultiSpeFtr_AllowedMultiAtts  	= 7021403 /*!< SpeciesFeature: Allowed Multi attributes */
, MultiSpeFtr_SpeFtrTypAtt_Ref		= 7021404 /*!< SpeciesFeature: 'speciesFeatureType' must be the 'id' of a speciesFeatureType */
, MultiSpeFtr_OccAtt_Ref	        = 7021405 /*!< SpeciesFeature: 'occur' must be a positiveInteger with restriction */
, MultiSpeFtr_CpoAtt_Ref		= 7021406 /*!< SpeciesFeature: 'component' must be the 'id' of a component */
, MultiSpeFtr_RestrictElts  		= 7021407 /*!< SpeciesFeature: Required one listOfSpeciesFeatureValues  */
, MultiLofSpeFtrVals_NoEmpty		= 7021408 /*!< ListOfSpeciesFeatureValues: Must not be empty */
, MultiLofSpeFtrVals_AllowedAtts  	= 7021409 /*!< ListOfSpeciesFeatureValues: Allowed attributes */
, MultiLofSpeFtrVals_AllowedElts	= 7021410 /*!< ListOfSpeciesFeatureValues: Allowed elements */

// Rules for SpeciesFeatureValue objects
, MultiSpeFtrVal_AllowedCoreAtts	= 7021501 /*!< SpeciesFeatureValue: Allowed SBML core attributes */
, MultiSpeFtrVal_AllowedCoreElts	= 7021502 /*!< SpeciesFeatureValue: Allowed SBML core elements */
, MultiSpeFtrVal_AllowedMultiAtts 	= 7021503 /*!< SpeciesFeatureValue: Allowed Multi attributes */
, MultiSpeFtrVal_ValAtt_Ref		= 7021504 /*!< SpeciesFeatureValue: 'value' must be the 'id' of a possibleSpeciesFeatureValue */

// Rules for IntraSpeciesReaction objects
, MultiIntSpeRec_AllowedAtts		= 7021601 /*!< IntraSpeciesReaction: Allowed attributes */
, MultiIntSpeRec_AllowedCoreElts	= 7021602 /*!< IntraSpeciesReaction: Allowed SBML core elements */

// Rules for extended SimpleSpeciesReference objects
, MultiExSplSpeRef_AllowedMultiAtts	= 7021701 /*!< Extended SimpleSpeciesReference: Allowed Multi attributes */
, MultiExSplSpeRef_CpaRefAtt_Ref	= 7021702 /*!< Extended SimpleSpeciesReference: 'compartmentReference' must be the 'id' of a compartmentReference */

// Rules for extended SpeciesReference objects
, MultiLofSptCpoMapsInPro_NoEmpty	= 7021801 /*!< ListOfSpeciesTypeComponentMapsInProduct: Must not be empty */
, MultiLofSptCpoMapsInPro_AllowedAtts 	= 7021802 /*!< ListOfSpeciesTypeComponentMapsInProduct: Allowed attributes */
, MultiLofSptCpoMapsInPro_AllowedElts 	= 7021803 /*!< ListOfSpeciesTypeComponentMapsInProduct: Allowed elements */

// Rules for SpeciesTypeComponentMapInProduct objects
, MultiSptCpoMapInPro_AllowedCoreAtts  	= 7021901 /*!< SpeciesTypeComponentMapInProduct: Allowed SBML core attributes */
, MultiSptCpoMapInPro_AllowedCoreElts	= 7021902 /*!< SpeciesTypeComponentMapInProduct: Allowed SBML core elements */
, MultiSptCpoMapInPro_AllowedMultiAtts  = 7021903 /*!< SpeciesTypeComponentMapInProduct: Allowed Multi attributes */
, MultiSptCpoMapInPro_RctAtt_Ref  	= 7021904 /*!< SpeciesTypeComponentMapInProduct: 'reactant' must be the 'id' of a reactant speciesReference */
, MultiSptCpoMapInPro_RctCpoAtt_Ref	= 7021905 /*!< SpeciesTypeComponentMapInProduct: 'reactantComponent' must be the 'id' of a reactant component */
, MultiSptCpoMapInPro_ProCpoAtt_Ref	= 7021906 /*!< SpeciesTypeComponentMapInProduct: 'productComponent' must be the 'id' of a product component */
, MultiLofSpeFtrChgs_NoEmpty		= 7021907 /*!< ListOfSpeciesFeatureChanges: Must not be empty */
, MultiLofSpeFtrChgs_AllowedAtts  	= 7021908 /*!< ListOfSpeciesFeatureChanges: Allowed attributes */
, MultiLofSpeFtrChgs_AllowedElts	= 7021909 /*!< ListOfSpeciesFeatureChanges: Allowed elements */

// Rules for SpeciesFeatureChange objects
, MultiSpeFtrChg_AllowedCoreAtts  	= 7022001 /*!< SpeciesFeatureChange: Allowed SBML core attributes */
, MultiSpeFtrChg_AllowedCoreElts	= 7022002 /*!< SpeciesFeatureChange: Allowed SBML core elements */
, MultiSpeFtrChg_AllowedMultiAtts  	= 7022003 /*!< SpeciesFeatureChange: Allowed Multi attributes */
, MultiSpeFtrChg_RctSpeFtrAtt_Ref	= 7022004 /*!< SpeciesFeatureChange: 'reactantSpeciesFeature' must be the 'id' of a speciesFeature */
, MultiSpeFtrChg_ProSpeFtrAtt_Ref	= 7022005 /*!< SpeciesFeatureChange: 'productSpeciesFeature' must be the 'id' of a speciesFeature */


// Rules for extended ci elements in Math objects
, MultiMathCi_AllowedMultiAtts  	= 7022101 /*!< Math ci element: Allowed Multi attributes */
, MultiMathCi_SpeRefAtt_Ref		= 7022102 /*!< Math ci element: 'speciesReference' must be the 'id' of a speciesReference within the same reaction */
, MultiMathCi_RepTypAtt_Ref	        = 7022103 /*!< Math ci element: 'representationType' must be a value of the Multi data type 'RepresentationType' */

}  MultiSBMLErrorCode_t;

END_C_DECLS

LIBSBML_CPP_NAMESPACE_END

#endif  /*  MultiSBMLError_h__  */

