/**
* Begin svn Header
* $Rev: 26 $:	Revision of last commit
* $Author: josephodada@gmail.com $:	Author of last commit
* $Date: 2013-04-24 17:06:40 +0100 (Wed, 24 Apr 2013) $:	Date of last commit
* $HeadURL: https://numl.googlecode.com/svn/trunk/libnuml/examples/c++/createExampleNUMLDoc.cpp $
* $Id: createNUML.cpp 26 2013-04-24 16:06:40Z josephodada@gmail.com $
* End svn Header
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
**/

#include <iostream>
#include <numl/NUMLTypes.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_USE

//
// Functions for creating the example NUML documents.
//
NUMLDocument* createExampleTwoDimensionNuMLDoc();            /* Two Dimensions Document */
NUMLDocument* createExampleOneDimensionNuMLDoc();               /* One Dimension Document */
NUMLDocument* createExample52();

//
// Helper functions for validating and writing the NUML documents created.
//
bool writeExampleNUML(const NUMLDocument *numlDoc, const string& filename);

//
// These variables are used in writeExampleNUML when writing an NUML
// document.  They are handed to libNUML functions in order to include
// the program information into comments within the NUML file.
//
const static string ProgramName    = "createExampleNuMLDoc";
const static string ProgramVersion = "1.0.0";

//
// The NUML Level and Version of the example NUML models.
//
const static unsigned int Level   = 1;
const static unsigned int Version = 1;


//===============================================================================
//
// Main routine
//
//  Creates NUML models represented in "Example models expressed in XML using
//  NUML" in Section 7 of the NUML Level 1 Version 1 specification(*).
//
//   (*) The specification document is available at the following URL:
//       http://numl.org/Documents/Specifications
//
//===============================================================================
//
int
main (int argc, char *argv[])
{
  NUMLDocument* numlDoc = 0;
  try
  {
    //-------------------------------------------------
    // Two Dimensional example
    //-------------------------------------------------

    numlDoc = createExampleTwoDimensionNuMLDoc();
    writeExampleNUML(numlDoc, "twoDimensionDoc.xml");
    delete numlDoc;

    // example 52
    numlDoc = createExample52();
    writeExampleNUML(numlDoc, "example52");
    delete numlDoc;


  }
  catch (std::bad_alloc& e)
  {
    cerr << e.what() << ": Unable to allocate memory." << endl;
    return 1;
  }
  catch (...)
  {
    cerr << "Unexpected exceptional condition encountered." << endl;
    return 1;
  }

  // Return status 0 to show that "all ok".
  return 0;

}

/**
 *
 * Creates a Two Dimensional NUML Document"
 *
 */
NUMLDocument* createExampleTwoDimensionNuMLDoc()
{
  const unsigned int level   = Level;
  const unsigned int version = Version;

  //---------------------------------------------------------------------------
  //
  // Creates an NUMLDocument object for this time course data (a Two Dimensional Data)
  //
  //---------------------------------------------------------------------------

  NUMLDocument* numlDoc = new NUMLDocument(level,version);

  // Header Information encoded in OntologyTerm objects

  /*
  No.     Term    			Source Term Id 	Ontology URI
  1       concentration   	SBO:0000196     http://www.ebi.ac.uk/sbo/
  2       metabolite      	SBO:0000299     http://www.ebi.ac.uk/sbo/
  3       Time    			SBO:0000345     http://www.ebi.ac.uk/sbo/
  */

  //---------------------------------------------------------------------------
  //
  // Creates a OntologyTerm object inside OntologyTerms in NUMLDocument object.
  //
  //---------------------------------------------------------------------------

  OntologyTerm* oTerm = numlDoc->createOntologyTerm();
  oTerm->setId("term1");
  oTerm->setTerm("Time");
  oTerm->setSourceTermId("SBO:0000345");
  oTerm->setOntologyURI("http://www.ebi.ac.uk/sbo/");

  oTerm = numlDoc->createOntologyTerm();
  oTerm->setId("term2");
  oTerm->setTerm("metabolite");
  oTerm->setSourceTermId("SBO:0000299");
  oTerm->setOntologyURI("http://www.ebi.ac.uk/sbo/");

  oTerm = numlDoc->createOntologyTerm();
  oTerm->setId("term3");
  oTerm->setTerm("concentration");
  oTerm->setSourceTermId("SBO:0000196");
  oTerm->setOntologyURI("http://www.ebi.ac.uk/sbo/");

  //---------------------------------------------------------------------------
  //
  // Creates a ResultComponent object inside ResultComponents in NUMLDocument object.
  //
  //---------------------------------------------------------------------------

  //The time course data
    /*
    Time    BL      B       DLL
    0       0       1.66058 0.0884913
    20      0.23    1.76058 0.0984913
    */

	ResultComponent* rComp = numlDoc->createResultComponent();
	rComp->setId("result_comp1");

	//---------------------------------------------------------------------------
	//
	// Creates a CompositeDescription object inside DimesionDescription in the ResultComponent object.
	//
	//---------------------------------------------------------------------------

	//1st dimension is the time
	CompositeDescription *compDesc = rComp->createCompositeDescription();
	compDesc->setName("Time");
	compDesc->setIndexType("float");
	compDesc->setOntologyTerm("term1");

	//2nd dimension is Metabilites
	CompositeDescription *subCompDesc = compDesc->createCompositeDescription();
	subCompDesc->setName("Metabolites");
	subCompDesc->setIndexType("string");
	subCompDesc->setOntologyTerm("term2");

	//the actual data description of data
	AtomicDescription *aDesc= subCompDesc->createAtomicDescription();
	aDesc->setMetaId("double");
	aDesc->setName("Concentration");
	aDesc->setOntologyTerm("term3");

	//encoding the data
	//row 1
	//first column in row 1
	CompositeValue *compValue = rComp->createCompositeValue();
	compValue->setIndexValue("0");

	//2nd column in row 1
	CompositeValue *subCompValue = compValue->createCompositeValue();
	subCompValue->setIndexValue("BL");
	AtomicValue *aValue = subCompValue->createAtomicValue();
	aValue->setValue("0");

	//3rd column in this row 1
	subCompValue = compValue->createCompositeValue();
	subCompValue->setIndexValue("B");
	aValue = subCompValue->createAtomicValue();
	aValue->setValue("1.66058");

	//4th column in this row 1
	subCompValue = compValue->createCompositeValue();
	subCompValue->setIndexValue("DLL");
	aValue = subCompValue->createAtomicValue();
	aValue->setValue("0.0884913");


	//row 2
	//first column in row 2
	compValue = rComp->createCompositeValue();
	compValue->setIndexValue("20");

	//2nd column in row 2
	subCompValue = compValue->createCompositeValue();
	subCompValue->setIndexValue("BL");
	aValue = subCompValue->createAtomicValue();
	aValue->setValue("0.23");

	//3rd column in this row 2
	subCompValue = compValue->createCompositeValue();
	subCompValue->setIndexValue("B");
	aValue = subCompValue->createAtomicValue();
	aValue->setValue("1.76058");

	//4th column in this row 2
	subCompValue = compValue->createCompositeValue();
	subCompValue->setIndexValue("DLL");
	aValue = subCompValue->createAtomicValue();
	aValue->setValue("0.0984913");

  return numlDoc;
}

NUMLDocument* createExample52()
{
  NUMLDocument* doc = new NUMLDocument();
  ResultComponent* r = doc->createResultComponent();
  r->setId("main_fitting_result");
  DimensionDescription* d = r->getDimensionDescription();

  TupleDescription* t = d->createTupleDescription();
  t->setName("Main");

  AtomicDescription* a = t->createAtomicDescription();
  a->setName( "Objective Value");
  a->setValueType("float");

  a = t->createAtomicDescription();
  a->setName( "Root Mean Square");
  a->setValueType("float");

  a = t->createAtomicDescription();
  a->setName( "Standard Deviation");
  a->setValueType("float");

  Dimension* dim = r->getDimension();

  Tuple* tuple = dim->createTuple();

  AtomicValue* val = tuple->createAtomicValue();
  val->setValue("12.5015");

  val = tuple->createAtomicValue();
  val->setValue("0.158123");

  val = tuple->createAtomicValue();
  val->setValue("0.159242");


  return doc;
}

/**
 *
 * Writes the given NUMLDocument to the given file.
 *
 */ 
bool writeExampleNUML(const NUMLDocument* numlDoc, const string& filename)
{
  NUMLWriter numlWriter;

  bool result = numlWriter.writeNUML(numlDoc, filename);

  if (result)
  {
    cout << "Wrote file \"" << filename << "\"" << endl;
    return true;
  }
  else
  {
    cerr << "Failed to write \"" << filename << "\"" << endl;
    return false;
  }
}
