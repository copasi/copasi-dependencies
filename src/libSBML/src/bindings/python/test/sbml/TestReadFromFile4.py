#
# @file    TestReadFromFile4.py
# @brief   Reads tests/l1v1-minimal.xml into memory and tests it.
#
# @author  Akiya Jouraku (Python conversion)
# @author  Ben Bornstein 
# 
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/sbml/test/TestReadFromFile4.c
# using the conversion program dev/utilities/translateTests/translateTests.pl.
# Any changes made here will be lost the next time the file is regenerated.
#
# -----------------------------------------------------------------------------
# This file is part of libSBML.  Please visit http://sbml.org for more
# information about SBML, and the latest version of libSBML.
#
# Copyright 2005-2010 California Institute of Technology.
# Copyright 2002-2005 California Institute of Technology and
#                     Japan Science and Technology Corporation.
# 
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation.  A copy of the license agreement is provided
# in the file named "LICENSE.txt" included with this software distribution
# and also available online as http://sbml.org/software/libsbml/license.html
# -----------------------------------------------------------------------------

import sys
import unittest
import libsbml


class TestReadFromFile4(unittest.TestCase):


  def test_read_l1v1_minimal(self):
    filename = "../../sbml/test/test-data/l1v1-minimal.xml"
    d = libsbml.readSBML(filename)
    if (d == None):
      pass    
    self.assertTrue( d.getLevel() == 1 )
    self.assertTrue( d.getVersion() == 1 )
    m = d.getModel()
    self.assertTrue( m.getNumCompartments() == 1 )
    c = m.getCompartment(0)
    self.assertTrue((  "x" == c.getName() ))
    self.assertTrue( m.getNumSpecies() == 1 )
    s = m.getSpecies(0)
    self.assertTrue((  "y"  == s.getName() ))
    self.assertTrue((  "x"  == s.getCompartment() ))
    self.assertTrue( s.getInitialAmount() == 1 )
    self.assertTrue( s.getBoundaryCondition() == False )
    self.assertTrue( m.getNumReactions() == 1 )
    r = m.getReaction(0)
    self.assertTrue((  "r" == r.getName() ))
    self.assertTrue( r.getReversible() != False )
    self.assertTrue( r.getFast() == False )
    self.assertTrue( r.getNumReactants() == 1 )
    self.assertTrue( r.getNumProducts() == 1 )
    sr = r.getReactant(0)
    self.assertTrue((  "y" == sr.getSpecies() ))
    self.assertTrue( sr.getStoichiometry() == 1 )
    self.assertTrue( sr.getDenominator() == 1 )
    sr = r.getProduct(0)
    self.assertTrue((  "y" == sr.getSpecies() ))
    self.assertTrue( sr.getStoichiometry() == 1 )
    self.assertTrue( sr.getDenominator() == 1 )
    _dummyList = [ d ]; _dummyList[:] = []; del _dummyList
    pass  

def suite():
  suite = unittest.TestSuite()
  suite.addTest(unittest.TestLoader.loadTestsFromTestCase(TestReadFromFile4))

  return suite

if __name__ == "__main__":
  if unittest.TextTestRunner(verbosity=1).run(suite()).wasSuccessful() :
    sys.exit(0)
  else:
    sys.exit(1)
