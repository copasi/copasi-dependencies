#
# @file    TestReadFromFile8.py
# @brief   Reads test-data/l2v4-new.xml into memory and tests it.
#
# @author  Akiya Jouraku (Python conversion)
# @author  Sarah Keating 
# 
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/sbml/test/TestReadFromFile8.cpp
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


class TestReadFromFile8(unittest.TestCase):


  def test_read_l2v4_new(self):
    reader = libsbml.SBMLReader()
    filename = "../../sbml/test/test-data/"
    filename += "l2v4-new.xml"
    d = reader.readSBML(filename)
    if (d == None):
      pass    
    self.assertTrue( d.getLevel() == 2 )
    self.assertTrue( d.getVersion() == 4 )
    m = d.getModel()
    self.assertTrue( m != None )
    self.assertTrue( m.getId() ==  "l2v4_all" )
    self.assertTrue( m.getNumCompartments() == 1 )
    c = m.getCompartment(0)
    self.assertTrue( c != None )
    self.assertTrue( c.getId() ==  "a" )
    self.assertTrue( c.getSize() == 1 )
    self.assertEqual( False, c.getConstant() )
    self.assertTrue( m.getNumEvents() == 1 )
    e = m.getEvent(0)
    self.assertTrue( e != None )
    self.assertEqual( True, e.getUseValuesFromTriggerTime() )
    self.assertEqual( True, e.isSetTrigger() )
    trigger = e.getTrigger()
    self.assertTrue( trigger != None )
    ast = trigger.getMath()
    self.assertTrue((  "lt(x, 3)" == libsbml.formulaToString(ast) ))
    self.assertTrue( e.getNumEventAssignments() == 1 )
    ea = e.getEventAssignment(0)
    self.assertTrue( ea != None )
    self.assertTrue( ea.getVariable() ==  "a" )
    ast = ea.getMath()
    self.assertTrue((  "x * p3" == libsbml.formulaToString(ast) ))
    d = None
    pass  

def suite():
  suite = unittest.TestSuite()
  suite.addTest(unittest.TestLoader.loadTestsFromTestCase(TestReadFromFile8))

  return suite

if __name__ == "__main__":
  if unittest.TextTestRunner(verbosity=1).run(suite()).wasSuccessful() :
    sys.exit(0)
  else:
    sys.exit(1)

