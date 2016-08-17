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
 * @file    echoNUML.cpp
 * @brief   Echos (and pretty prints) an NUML model.
 * Original author:  Ben Bornstein
 * Modified for NuML by: Joseph O. Dada
 */


#include <iostream>
#include <numl/NUMLTypes.h>

using namespace std;
LIBNUML_CPP_NAMESPACE_USE

int
main (int argc, char* argv[])
{
  if (argc != 3)
  {
    cout << endl << "Usage: echoNUML input-filename output-filename"
         << endl << endl;
    return 2;
  }

  writeNUML(readNUML(argv[1]), argv[2]);
  return 0;
}
