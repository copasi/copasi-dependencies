#
# @file    TestInitialAssignment.py
# @brief   SBML InitialAssignment unit tests
#
# @author  Akiya Jouraku (Python conversion)
# @author  Sarah Keating 
# 
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/sbml/test/TestInitialAssignment.c
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


class TestInitialAssignment(unittest.TestCase):

  global IA
  IA = None

  def setUp(self):
    self.IA = libsbml.InitialAssignment(2,4)
    if (self.IA == None):
      pass    
    pass  

  def tearDown(self):
    _dummyList = [ self.IA ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_InitialAssignment_create(self):
    self.assertTrue( self.IA.getTypeCode() == libsbml.SBML_INITIAL_ASSIGNMENT )
    self.assertTrue( self.IA.getMetaId() == "" )
    self.assertTrue( self.IA.getNotes() == None )
    self.assertTrue( self.IA.getAnnotation() == None )
    self.assertTrue( self.IA.getSymbol() == "" )
    self.assertTrue( self.IA.getMath() == None )
    pass  

  def test_InitialAssignment_createWithNS(self):
    xmlns = libsbml.XMLNamespaces()
    xmlns.add( "http://www.sbml.org", "testsbml")
    sbmlns = libsbml.SBMLNamespaces(2,3)
    sbmlns.addNamespaces(xmlns)
    object = libsbml.InitialAssignment(sbmlns)
    self.assertTrue( object.getTypeCode() == libsbml.SBML_INITIAL_ASSIGNMENT )
    self.assertTrue( object.getMetaId() == "" )
    self.assertTrue( object.getNotes() == None )
    self.assertTrue( object.getAnnotation() == None )
    self.assertTrue( object.getLevel() == 2 )
    self.assertTrue( object.getVersion() == 3 )
    self.assertTrue( object.getNamespaces() != None )
    self.assertTrue( object.getNamespaces().getLength() == 2 )
    _dummyList = [ object ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_InitialAssignment_free_NULL(self):
    _dummyList = [ None ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_InitialAssignment_setMath(self):
    math = libsbml.parseFormula("2 * k")
    self.IA.setMath(math)
    math1 = self.IA.getMath()
    self.assertTrue( math1 != None )
    formula = libsbml.formulaToString(math1)
    self.assertTrue( formula != None )
    self.assertTrue((  "2 * k" == formula ))
    self.assertTrue( self.IA.getMath() != math )
    self.assertEqual( True, self.IA.isSetMath() )
    self.IA.setMath(self.IA.getMath())
    math1 = self.IA.getMath()
    self.assertTrue( math1 != None )
    formula = libsbml.formulaToString(math1)
    self.assertTrue( formula != None )
    self.assertTrue((  "2 * k" == formula ))
    self.assertTrue( self.IA.getMath() != math )
    self.IA.setMath(None)
    self.assertEqual( False, self.IA.isSetMath() )
    if (self.IA.getMath() != None):
      pass    
    _dummyList = [ math ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_InitialAssignment_setSymbol(self):
    Symbol =  "k2";
    self.IA.setSymbol(Symbol)
    self.assertTrue(( Symbol == self.IA.getSymbol() ))
    self.assertEqual( True, self.IA.isSetSymbol() )
    if (self.IA.getSymbol() == Symbol):
      pass    
    self.IA.setSymbol(self.IA.getSymbol())
    self.assertTrue(( Symbol == self.IA.getSymbol() ))
    self.IA.setSymbol("")
    self.assertEqual( False, self.IA.isSetSymbol() )
    if (self.IA.getSymbol() != None):
      pass    
    pass  

def suite():
  suite = unittest.TestSuite()
  suite.addTest(unittest.TestLoader.loadTestsFromTestCase(TestInitialAssignment))

  return suite

if __name__ == "__main__":
  if unittest.TextTestRunner(verbosity=1).run(suite()).wasSuccessful() :
    sys.exit(0)
  else:
    sys.exit(1)
