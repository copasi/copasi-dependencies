/**
* Begin svn Header
* $Rev: 26 $:	Revision of last commit
* $Author: josephodada@gmail.com $:	Author of last commit
* $Date: 2013-04-24 17:06:40 +0100 (Wed, 24 Apr 2013) $:	Date of last commit
* $HeadURL: https://numl.googlecode.com/svn/trunk/libnuml/examples/c++/createExampleNUMLDoc.cpp $
* $Id: printNUML.cpp 26 2013-04-24 16:06:40Z josephodada@gmail.com $
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
import org.numl.libnuml.*;

public class printNUML
{


public static void main (String[] args)
{
  if (args.length != 1)
  {
    System.out.println("Usage: printNUML input-filename\n" );         
    System.exit(2);
  }

  try
  {
    System.loadLibrary("numlj");
    // For extra safety, check that the jar file is in the classpath.
    Class.forName("org.numl.libnuml.libnuml");
  }
  catch (UnsatisfiedLinkError e)
  {
	  System.err.println("Error encountered while attempting to load libNUML:");
      e.printStackTrace();     
      System.exit(1);
	  
  }
  catch (ClassNotFoundException e)
  {
    System.err.println("Error: unable to load the file 'libnumlj.jar'." +
                       " It is likely that your -classpath command line " +
                       " setting or your CLASSPATH environment variable " +
                       " do not include the file 'libnumlj.jar'.");
    System.exit(1);
  }
  catch (SecurityException e)
  {
    System.err.println("Error encountered while attempting to load libNUML:");
    e.printStackTrace();
    System.err.println("Could not load the libNUML library files due to a"+
                       " security exception.\n");
    System.exit(1);
  }
  
 // String filename   = args[0];
  
  NUMLReader reader = new NUMLReader();
  //NUMLWriter writer = new NUMLWriter();
  NUMLDocument document = reader.readNUML(args[0]);

  if (document.getErrorLog().getNumFailsWithSeverity(libnuml.LIBNUML_SEV_ERROR) > 0)
  {
	System.out.println("Encountered the following NUML errors:");
	System.out.println(document.getErrorLog().toString());
    System.exit(2);
  }

  long level   = document.getLevel();
  long version = document.getVersion();

   System.out.println("File: " + args[0]);
   System.out.println(" (Level " + level + ", version " + version + ")");
 
  long nOTerms = document.getNumOntologyTerms();
  long nRComps = document.getNumResultComponents();

  System.out.println("Number of ontologyTerms in this document: " + nOTerms);
  System.out.println("Number of resultComponent in this document: " + nRComps); 

  OntologyTerms oTerms = document.getOntologyTerms();
  ResultComponents rComps = document.getResultComponents();

  if (oTerms == null)
  {
	  System.out.println("No OntologyTerms present.");
	  System.exit(2);
  }
  //print all the ontologyTerms 
  System.out.println("No."+"\t"+"Term"+"\t"+"Source Term Id" +"\t"+"Ontology URI");
     for(int i=0; i<nOTerms; i++){
    	 OntologyTerm oTerm = oTerms.get(i);
    	 System.out.println(i+1 +"\t"+oTerm.getTerm()+ "\t" + oTerm.getSourceTermId()+"\t"+oTerm.getOntologyURI());
    	// delete oTerm;
     }   

  if (rComps == null)
    {
	  System.out.println("No ResultComponents present.");
	  System.exit(2);
    }

  System.out.println("Number of ResultComponents " + nRComps);  
  

  System.exit(0);
}

}