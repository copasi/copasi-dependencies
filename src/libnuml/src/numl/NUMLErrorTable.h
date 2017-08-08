/*
* ****************************************************************************
* This file is part of libNUML.  Please visit http://code.google.com/p/numl/for more
* information about NUML, and the latest version of libNUML.
* Copyright (c) 2013 The University of Manchester.
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation.  A copy of the license agreement is
* provided in the file named "LICENSE.txt" included with this software
* distribution and also available online as http://www.gnu.org/licenses/lgpl.html
*
* Contributors:
* Joseph O. Dada, The University of Manchester - initial API and implementation
* ****************************************************************************
*/


#include <string>
#include <iostream>
#include <sstream>

#include <sbml/xml/XMLError.h>
#include <numl/NUMLError.h>


LIBNUML_CPP_NAMESPACE_BEGIN

typedef struct {
  const char * ref_l1;
} referenceEntry;

typedef struct {
  unsigned int code;
  const char*  shortMessage;
  unsigned int category;
  unsigned int l1v1_severity;
  const char*  message;
  referenceEntry reference;
} numlErrorTableEntry;


static const numlErrorTableEntry errorTable[] =
{
  // 10000
  { NUMLUnknownError,
    "Unknown internal libNUML error",
    LIBNUML_CAT_INTERNAL,
    LIBNUML_SEV_FATAL,
    "Unrecognized error encountered by libNUML" },

  //10101
  {
    NUMLNotUTF8,
    "Not UTF8",
    LIBNUML_CAT_NUML,
    LIBNUML_SEV_ERROR,
    "An NUML XML file must use UTF-8 as the character encoding. More "
    "precisely, the 'encoding' attribute of the XML declaration at the "
    "beginning of the XML data stream cannot have a value other than "
    "'UTF-8'. An example valid declaration is "
    "'<?xml version=\"1.0\" encoding=\"UTF-8\"?>'. (References: L2V2 Section "
    "4.1; L2V3 Section 4.1; L2V4 Section 4.1)"
  },

  //10102
  {
    NUMLUnrecognizedElement,
    "Unrecognized element",
    LIBNUML_CAT_NUML,
    LIBNUML_SEV_ERROR,
    "An NUML XML document must not contain undefined elements or attributes "
    "in the NUML namespace. Documents containing unknown elements or "
    "attributes placed in the NUML namespace do not conform to the NUML "
    "specification. (References: L2V2 Section 4.1; L2V3 Section 4.1; L2V4 "
    "Section 4.1)"
  },

  //10103
  {
    NUMLNotSchemaConformant,
    "Not conformant to NUML XML schema",
    LIBNUML_CAT_NUML,
    LIBNUML_SEV_ERROR,
    "An NUML XML document must conform to the XML Schema for the corresponding "
    "NUML Level, Version and Release. The XML Schema for NUML defines the "
    "basic NUML object structure, the data types used by those objects, and the "
    "order in which the objects may appear in an NUML document."
  },

  //10401
    {
      NUMLMissingAnnotationNamespace,
      "Missing declaration of XML namespace for annotation",
      LIBNUML_CAT_NUML,
      LIBNUML_SEV_ERROR,
      "Every top-level element within an annotation element must "
      "have a namespace declared. (References: L1V1 Section ??"
    },

    //10402
    {
      NUMLDuplicateAnnotationNamespaces,
      "Multiple annotations using same XML namespace",
      LIBNUML_CAT_NUML,
      LIBNUML_SEV_ERROR,
      "There cannot be more than one top-level element using a "
      "given namespace inside a given annotation element. "
      "(References: L1V1 Section ??"
    },

    //10403
    {
      NUMLNamespaceInAnnotation,
      "Invalid use of NUML XML namespace in annotation",
      LIBNUML_CAT_NUML,
      LIBNUML_SEV_ERROR,
      "Top-level elements within an annotation element cannot use any NUML "
      "namespace, whether explicitly (by declaring the namespace to be one of "
      "the URIs \"http://www.numl.org/numl/level1\", Section ??"
    },

    //20101
      {
        NUMLInvalidNamespaceOnNUML,
        "Invalid XML namespace for NUML container",
        LIBNUML_CAT_NUML,
        LIBNUML_SEV_ERROR,
        "The <numl> container element must declare the XML Namespace for NUML, "
        "and this declaration must be consistent with the values of the 'level' "
        "and 'version' attributes on the <numl> element."
        "Section XXX TODO)"
      },
    //21003
      {
        NUMLConstraintNotInXHTMLNamespace,
        "Constraint message is not in XHTML XML namespace",
        LIBNUML_CAT_NUML,
        LIBNUML_SEV_ERROR,
        "The contents of the <message> element in a <constraint> must be "
        "explicitly placed in the XHTML XML namespace. (References: L1V1 Section TODO)"
      },

      //21004
      {
        NUMLConstraintContainsXMLDecl,
        "XML declarations not permitted in constraint messages",
        LIBNUML_CAT_NUML,
        LIBNUML_SEV_ERROR,
        "The contents of the <message> element must not contain an XML declaration "
        "(i.e., a string of the form \"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\" "
      },

      //21005
      {
        NUMLConstraintContainsDOCTYPE,
        "XML DOCTYPE not permitted in constraint messages",
        LIBNUML_CAT_NUML,
        LIBNUML_SEV_ERROR,
        "The contents of the message element must not contain an XML DOCTYPE "
        "declaration (i.e., a string beginning with the characters \"<!DOCTYPE\". "
        "(References: L1V1 Section TODO)"
      },

      //21006
      {
        NUMLInvalidConstraintContent,
        "Invalid content for constraint message",
        LIBNUML_CAT_NUML,
        LIBNUML_SEV_ERROR,
        "The XHTML content inside a <constraint>s message element can only take one "
        "of the following general forms: (1) a complete XHTML document beginning "
        "with the element <html> and ending with </html>; (2) the \"body\" portion "
        "of a document beginning with the element <body> and ending with </body>; "
        "or (3) XHTML content that is permitted within a <body> ... </body> "
        "elements. (References: L1V1 Section ??"
      },


  //99101
  {
    InvalidNUMLLevelVersion,
    "Invalid NUML Level and Version",
    LIBNUML_CAT_NUML,
    LIBNUML_SEV_ERROR,
    "The level and version specified for the document must be "
    "consistent with a valid published NUML specification.  These are "
    "Level 1, Versions 1"
  },

  //99301
  {
    NUMLNoTimeSymbolInFunctionDef,
    "<csymbol> for 'time' used within the <math> of a function definition",
    LIBNUML_CAT_GENERAL_CONSISTENCY,
    LIBNUML_SEV_ERROR,
    "The csymbol 'time' should not be used within a the <math> element "
    "of a <FunctionDefinition>. (References: L2V3 Section 4.3.2; "
    "L2V4 Section 4.3.2)"
  },

  //99502
  // This is an internal error that reverts to 10501
/*  {
    InconsistentArgUnitsWarnings,
    "",
    LIBNUML_SEV_GENERAL_WARNING,
    LIBNUML_SEV_WARNING,
    ""
  },
*/

  //99701
 /* {
    UnrecognisedSBOTerm,
    "Unrecognized 'sboTerm' value",
    LIBNUML_CAT_SBO_CONSISTENCY,
    LIBNUML_SEV_WARNING,
    "The SBOTerm used is not recognised by libNUML and therefore the correct "
    "parentage cannot be checked. However, since libNUML is referring to a "
    "snapshot of the SBO tree the term may now exist."
  },

  //99701
  {
    ObseleteSBOTerm,
    "Obsolete 'sboTerm' value",
    LIBNUML_CAT_SBO_CONSISTENCY,
    LIBNUML_SEV_WARNING,
    "The SBOTerm value used is considered obsolete."
  },
*/
  //99901
 /* {
    IncorrectCompartmentSpatialDimensions,
    "in NUML Level 1, only three-dimensional compartments are permitted",
    LIBNUML_CAT_INTERNAL_CONSISTENCY,
    LIBNUML_SEV_ERROR,
    "The 'spatialDimensions' attribute on <compartment> was not available "
    "in NUML Level 1. In order for the internal respresentation of a "
    "<compartment> to be correct, the value for the spatialDimensions "
    "member variable should be '3'."
  },
*/
  //99925
/*  {
    OffsetNotValidAttribute,
    "Attribute 'offset' on units only available in NUML Level 2 Version 1",
    LIBNUML_CAT_INTERNAL_CONSISTENCY,
    LIBNUML_SEV_ERROR,
    "The 'offset' attribute on <unit> was only available in NUML Level 2 "
    "Version 1. In order for the internal respresentation of a <unit> to "
    "be correct, the value for the offset member variable should be '0'."
  }
*/

};

LIBNUML_CPP_NAMESPACE_END


