/**
* Begin svn Header
* $Rev: 26 $:	Revision of last commit
* $Author: josephodada@gmail.com $:	Author of last commit
* $Date: 2013-04-24 17:06:40 +0100 (Wed, 24 Apr 2013) $:	Date of last commit
* $HeadURL: https://numl.googlecode.com/svn/trunk/libnuml/examples/c++/createExampleNUMLDoc.cpp $
* $Id: echoNUML.cpp 26 2013-04-24 16:06:40Z josephodada@gmail.com $
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
import org.numl.libnuml.NUMLReader;
import org.numl.libnuml.NUMLWriter;
import org.numl.libnuml.NUMLDocument;

public class echoNUML
{

public static void main (String[] args)
{
  if (args.length != 2)
  {
    System.out.println("Usage: echoNUML input-filename output-filename");
    System.exit(2);
  }  
		System.loadLibrary("numlj");

		NUMLReader reader = new NUMLReader();
		NUMLWriter writer = new NUMLWriter();

		NUMLDocument doc = reader.readNUML(args[0]);
		if (doc.getNumErrors() > 0) {
			doc.printErrors();
		} else {
			writer.writeNUML(doc, args[1]);
		}

		System.exit(0);
	}

}