#!/usr/bin/env python
## 
## @file    printNUML.py
## @brief   Prints an overview of the elments in the given NUML document
## @author  Joseph O. Dada
## 
## <!--------------------------------------------------------------------------
## This file is part of libNUML.  Please visit http://sed-ml.org for more
## information about NUML, and the latest version of libNUML.
## 
## Copyright (c) 2013, University of Manchester  
## All rights reserved.
## 
## ------------------------------------------------------------------------ -.
## 

import sys
import os.path
import libnuml

def main (args):
  """Usage: print_NUML input-filename
  """
  if len(args) != 2:
    print(main.__doc__)
    sys.exit(1)

  doc = libnuml.readNUML(args[1]);
  if ( doc.getErrorLog().getNumFailsWithSeverity(libnuml.LIBNUML_SEV_ERROR) > 0):  
 #   print doc.getErrorLog().toString();
    sys.exit(2);   
    
 #   NUMLReader reader = new NUMLReader();
 # NUMLWriter writer = new NUMLWriter();  

  level   = doc.getLevel();
  version = doc.getVersion();

  print ("File:" , args[1]);
  print ("Level" , level , " version ", version, ")");
 
  nOTerms = doc.getNumOntologyTerms();
  nRComps = doc.getNumResultComponents();

  print ("Number of ontologyTerms in this document: ", nOTerms);
  print ("Number of resultComponent in this document: " , nRComps); 

  oTerms = doc.getOntologyTerms();
  rComps = doc.getResultComponents();

  if oTerms is None:
    print ("No OntologyTerms present.");
    sys.exit(2);
  
  #print all the ontologyTerms 
  print ("No.\t Term  \t Source Term Id \t Ontology URI");
  for i  in range(0, nOTerms):    
    oTerm = oTerms.get(i);
    print("", i+1,"\t", oTerm.getTerm(), "\t", oTerm.getSourceTermId(), "\t", oTerm.getOntologyURI());   

  if rComps is None:
    print ("No ResultComponents present.");
    sys.exit(2);
	
  # print the ResultComponents	
  print ("Number of ResultComponents ", nRComps);   
  for i in range(0, nRComps):
    print("Result Component:", i+1);
    print("Name of ResultComponent: ", rComps.get(i).getId());
    rComp = rComps.get(i);

  sys.exit(0);     
  
if __name__ == "__main__":
  main(sys.argv)  