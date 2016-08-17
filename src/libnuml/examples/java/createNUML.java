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


import java.io.*;
import org.numl.libnuml.AtomicDescription;
import org.numl.libnuml.AtomicValue;
import org.numl.libnuml.CompositeDescription;
import org.numl.libnuml.CompositeValue;
import org.numl.libnuml.Dimension;
import org.numl.libnuml.DimensionDescription;
import org.numl.libnuml.OntologyTerm;
import org.numl.libnuml.OntologyTerms;
import org.numl.libnuml.ResultComponent;
import org.numl.libnuml.NUMLDocument;
import org.numl.libnuml.NUMLReader;
import org.numl.libnuml.NUMLWriter;
import org.numl.libnuml.Tuple;
import org.numl.libnuml.TupleDescription;

public class createNUML
{
	//
	// The NUML Level and Version of the example NUML models.
	//
	private static int Level = 1;
	private static int Version = 1;

	public static void main(String[] args) {

		if (args.length != 1) {
			System.out.println("Usage: createNUML output-filename.");
			System.exit(2);
		}

		System.loadLibrary("numlj");

		// create the document
		NUMLDocument numlDoc = createExampleTwoDimensionNuMLDoc();

		NUMLWriter numlWriter = new NUMLWriter();

		boolean result = numlWriter.writeNUML(numlDoc, args[0]);

		if (result) {
			System.out.println("Wrote file  " + args[0]);
		} else {
			System.out.println("Failed to write " + args[0]);
			System.exit(2);
		}

		System.exit(0);
	}

/**
*
* Creates a Two Dimensional NUML Document"
*
*/
static NUMLDocument createExampleTwoDimensionNuMLDoc()
{
 int level = Level;
 int version = Version;

 //---------------------------------------------------------------------------
 //
 // Creates an NUMLDocument object for this time course data (a Two Dimensional Data)
 //
 //---------------------------------------------------------------------------

 NUMLDocument numlDoc = new NUMLDocument(level,version);

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

 OntologyTerm oTerm = numlDoc.createOntologyTerm();
 oTerm.setId("term1");
 oTerm.setTerm("Time");
 oTerm.setSourceTermId("SBO:0000345");
 oTerm.setOntologyURI("http://www.ebi.ac.uk/sbo/");

 oTerm = numlDoc.createOntologyTerm();
 oTerm.setId("term2");
 oTerm.setTerm("metabolite");
 oTerm.setSourceTermId("SBO:0000299");
 oTerm.setOntologyURI("http://www.ebi.ac.uk/sbo/");

 oTerm = numlDoc.createOntologyTerm();
 oTerm.setId("term3");
 oTerm.setTerm("concentration");
 oTerm.setSourceTermId("SBO:0000196");
 oTerm.setOntologyURI("http://www.ebi.ac.uk/sbo/");

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

	ResultComponent rComp = numlDoc.createResultComponent();
	rComp.setId("result_comp1");

	//---------------------------------------------------------------------------
	//
	// Creates a CompositeDescription object inside DimesionDescription in the ResultComponent object.
	//
	//---------------------------------------------------------------------------

	//1st dimension is the time
	CompositeDescription compDesc = rComp.createCompositeDescription();
	compDesc.setName("Time");
	compDesc.setIndexType("float");
	compDesc.setOntologyTerm("term1");

	//2nd dimension is Metabilites
	CompositeDescription subCompDesc = compDesc.createCompositeDescription();
	subCompDesc.setName("Metabolites");
	subCompDesc.setIndexType("string");
	subCompDesc.setOntologyTerm("term2");

	//the actual data description of data
	AtomicDescription aDesc= subCompDesc.createAtomicDescription();
	aDesc.setMetaId("double");
	aDesc.setName("Concentration");
	aDesc.setOntologyTerm("term3");

	//encoding the data
	//row 1
	//first column in row 1
	CompositeValue compValue = rComp.createCompositeValue();
	compValue.setIndexValue("0");

	//2nd column in row 1
	CompositeValue subCompValue = compValue.createCompositeValue();
	subCompValue.setIndexValue("BL");
	AtomicValue aValue = subCompValue.createAtomicValue();
	aValue.setValue("0");

	//3rd column in this row 1
	subCompValue = compValue.createCompositeValue();
	subCompValue.setIndexValue("B");
	aValue = subCompValue.createAtomicValue();
	aValue.setValue("1.66058");

	//4th column in this row 1
	subCompValue = compValue.createCompositeValue();
	subCompValue.setIndexValue("DLL");
	aValue = subCompValue.createAtomicValue();
	aValue.setValue("0.0884913");


	//row 2
	//first column in row 2
	compValue = rComp.createCompositeValue();
	compValue.setIndexValue("20");

	//2nd column in row 2
	subCompValue = compValue.createCompositeValue();
	subCompValue.setIndexValue("BL");
	aValue = subCompValue.createAtomicValue();
	aValue.setValue("0.23");

	//3rd column in this row 2
	subCompValue = compValue.createCompositeValue();
	subCompValue.setIndexValue("B");
	aValue = subCompValue.createAtomicValue();
	aValue.setValue("1.76058");

	//4th column in this row 2
	subCompValue = compValue.createCompositeValue();
	subCompValue.setIndexValue("DLL");
	aValue = subCompValue.createAtomicValue();
	aValue.setValue("0.0984913");

 return numlDoc;
}

}