/**
 * Begin svn Header
 * $Rev$:	Revision of last commit
 * $Author$:	Author of last commit
 * $Date$:	Date of last commit
 * $HeadURL$
 * $Id$
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

/**
 * @brief   Prints one and two dimensional tab delimited data from NUML Document
 */


#include <iostream>
#include <sstream>
#include <numl/NUMLTypes.h>


using namespace std;
LIBNUML_CPP_NAMESPACE_USE

//Helper function definitions for generating the tab delimited data
int convertCompositeDescription2TSV(CompositeDescription *compDesc, int *numberOfDimensions, stringstream *header);
int convertCompositeValue2TSV(CompositeValue *compValue, int *dimIndex, stringstream *header, stringstream *data);

int
main (int argc, char* argv[])
{
  if (argc != 2)
  {
    cout << endl << "Usage: printNUML filename" << endl << endl;
    return 1;
  }

  const char* filename   = argv[1];
  NUMLDocument* document = readNUML(filename);

  if (document->getNumErrors() > 0)
  {
    cerr << "Encountered the following NUML errors:" << endl;
    document->printErrors(cerr);
    return 1;
  }

  unsigned int level   = document->getLevel  ();
  unsigned int version = document->getVersion();

  cout << endl
  << "File: " << filename
  << " (Level " << level << ", version " << version << ")" << endl;
  cout<<endl;


  unsigned int nOTerms = document->getNumOntologyTerms();
  cout << "Number of ontologyTerms in this document: " << nOTerms<< endl;
  OntologyTerms *oTerms = document->getOntologyTerms();
  //print all the ontologyTerms
  stringstream oTermData;
  oTermData <<"No."<<"\t"<<"Term"<<"\t"<<"Source Term Id"<<"\t"<<"Ontology URI"<<endl;
  for(unsigned int i=0; i<nOTerms; i++){
    OntologyTerm *oTerm = oTerms->get(i);
    oTermData << i+1 <<"\t"<<oTerm->getTerm()<< "\t" << oTerm->getSourceTermId()<<"\t"<<oTerm->getOntologyURI()<<endl;
    // delete oTerm;
  }
  cout<<oTermData.str()<<endl;


  unsigned int nRComps = document->getNumResultComponents();
  cout << "Number of resultComponent in this document: " << nRComps<< endl;
  cout<<endl;

  ResultComponents *rComps = document->getResultComponents();
  cout << "Number of ResultComponents " << nRComps<< endl;

  //write out the content of resultComponents
  string headerData, stringData;
  stringstream header, data;
  int numberOfDimDesc = 0;
  for(unsigned int i=0; i<nRComps; i++)
  {
    ResultComponent *rComp = rComps->get(i);
    cout<<"Result Component:"<< i+1<<endl;
    cout << "Name of ResultComponent: " << rComp->getId()<< endl;


    //process the result header i.e the DimensionDescription
    DimensionDescription *dimDesc = rComp->getDimensionDescription(); //get the data header

    for(unsigned int ii=0; ii<dimDesc->size(); ii++)
    {
      numberOfDimDesc = 1;
      DimensionDescription* currentDesc = dimDesc->get(ii);
      cout << "Type of DimensionDescription: " << currentDesc->getElementName()<< endl;
  CompositeDescription *compDesc = dynamic_cast<CompositeDescription*>(currentDesc);
      convertCompositeDescription2TSV(compDesc, &numberOfDimDesc, &header);
    }

    cout<<"Number of Dimensions: "<<numberOfDimDesc<<endl;

    //process the actual data, which is embedded in dimension element
    Dimension *dim = rComp->getDimension();
    int dimIndex = 0 ;
    for(unsigned int ii=0; ii<dim->size(); ii++)
    {
      Dimension* currentDimension =dim->get(ii);
      CompositeValue *compValue =  dynamic_cast<CompositeValue*>(currentDimension);
      if (compValue == NULL)
        continue;

      if(numberOfDimDesc>1)
      {
        dimIndex=ii; //only index at ii = 0 is needed, others are the same for 2 dimensional data
        data<<compValue->getIndexValue()<<"\t";
      }
      else
      {
        header<<compValue->getIndexValue()<<"\t";
      }

      convertCompositeValue2TSV(compValue, &dimIndex, &header, &data);

      if(numberOfDimDesc>1)
      {
        data<<"\n";
      }
    }

    cout<<endl;
    headerData = header.str();
    stringData = data.str();
    cout<<"Table data"<<endl;
    cout<<headerData<<endl;
    cout<<stringData<<endl;
    cout<<endl;
    data.str(string());
    header.str(string());
  }
  //clean up the mess
  delete document;
  return 0;
}

int convertCompositeDescription2TSV(CompositeDescription *compDesc, int *numberOfDimensions, stringstream *header) {

  if (compDesc == NULL)
  {
    cout << "Nothing to do. Document does not contain composite description" << endl;
    return 1;
  }

  if (compDesc->isContentCompositeDescription())
  {
				*numberOfDimensions+=1;
				(*header)<<compDesc->getName();
				(*header)<<"\t";

    for(int i=0; i<(int)compDesc->size(); i++)
    {
      CompositeDescription *subCompDesc = compDesc->get(i);
      convertCompositeDescription2TSV(subCompDesc, numberOfDimensions, header);
    }
  }
  else if (compDesc->isContentTupleDescription())
  {
				TupleDescription *tupleDesc = compDesc->getTupleDescription();
				for (int ii = 0; ii < (int) tupleDesc->size(); ii++)
        {
          AtomicDescription *tADesc = tupleDesc->getAtomicDescription(ii);
          std::cout << tADesc->getId() << std::endl;
        }
  }
  else if (compDesc->isContentAtomicDescription())
  {
				AtomicDescription *aDesc = compDesc->getAtomicDescription();
				std::cout << aDesc->getId() << std::endl;
  }

  return 0;
}

int convertCompositeValue2TSV(CompositeValue *compValue, int *dimIndex, stringstream *header, stringstream *data){
  //int compDept = 1; //monitor the dept of composite value

  if (compValue == NULL){
    cout << "Nothing to do. Document does not contain composite value data" << endl;
    return 1;
  }

  if (compValue->isContentCompositeValue())
  {
				for(int i=0; i<(int)compValue->size(); i++)
        {
          CompositeValue *subCompValue = compValue->get(i);
          if((*dimIndex)==0)
          {
            (*header)<<subCompValue->getIndexValue();
            (*header)<<"\t";
          }
          convertCompositeValue2TSV(subCompValue, dimIndex, header, data);
        }
  }
  else if (compValue->isContentTuple())
  {
				Tuple *compTuple = compValue->getTuple();
				for (int ii = 0; ii < (int) compTuple->size(); ii++)
        {
          AtomicValue *tValue = compTuple->getAtomicValue(ii);
          (*data)<<tValue->getDoubleValue()<<'\t';
        }
  }
  else if (compValue->isContentAtomicValue())
  {
				AtomicValue *aValue = compValue->getAtomicValue();
				(*data)<<aValue->getDoubleValue()<<'\t';
  }
  

  
  return 0;
}
