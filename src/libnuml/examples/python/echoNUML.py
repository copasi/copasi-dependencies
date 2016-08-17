#!/usr/bin/env python
## 
## @file    echo_sedml.py
## @brief   Echos (and prints) a NuML data.
## @author  Joseph O. Dada
## 
## <!--------------------------------------------------------------------------
## This file is part of libNUML.  Please visit http://numl.org for more
## information about NUML, and the latest version of libNUML.
## 
## Copyright (c) 2013, University of Manchester 
## All rights reserved.
## 
## 

import sys
import os.path
import libnuml

def main (args):
  """Usage: echo_numl input-filename output-filename
  """
  if len(args) != 3:
    print(main.__doc__)
    sys.exit(1)

  doc = libnuml.readNUML(args[1]);
  ##if ( doc.getErrorLog().getNumFailsWithSeverity(libsedml.LIBSNUML_SEV_ERROR) > 0):
  ##  print doc.getErrorLog().toString();
##  else:
  libnuml.writeNUML(doc, args[2]);

  return 0;

if __name__ == '__main__':
  main(sys.argv)  