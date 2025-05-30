#
# @file    TestConstraint.py
# @brief   SBML Constraint unit tests
#
# @author  Akiya Jouraku (Python conversion)
# @author  Sarah Keating 
# 
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/sbml/test/TestConstraint.c
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


class TestConstraint(unittest.TestCase):

  global C
  C = None

  def setUp(self):
    self.C = libsbml.Constraint(2,4)
    if (self.C == None):
      pass    
    pass  

  def tearDown(self):
    _dummyList = [ self.C ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_Constraint_create(self):
    self.assertTrue( self.C.getTypeCode() == libsbml.SBML_CONSTRAINT )
    self.assertTrue( self.C.getMetaId() == "" )
    self.assertTrue( self.C.getNotes() == None )
    self.assertTrue( self.C.getAnnotation() == None )
    self.assertEqual( False, self.C.isSetMessage() )
    self.assertEqual( False, self.C.isSetMath() )
    pass  

  def test_Constraint_createWithNS(self):
    xmlns = libsbml.XMLNamespaces()
    xmlns.add( "http://www.sbml.org", "testsbml")
    sbmlns = libsbml.SBMLNamespaces(2,2)
    sbmlns.addNamespaces(xmlns)
    object = libsbml.Constraint(sbmlns)
    self.assertTrue( object.getTypeCode() == libsbml.SBML_CONSTRAINT )
    self.assertTrue( object.getMetaId() == "" )
    self.assertTrue( object.getNotes() == None )
    self.assertTrue( object.getAnnotation() == None )
    self.assertTrue( object.getLevel() == 2 )
    self.assertTrue( object.getVersion() == 2 )
    self.assertTrue( object.getNamespaces() != None )
    self.assertTrue( object.getNamespaces().getLength() == 2 )
    _dummyList = [ object ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_Constraint_free_NULL(self):
    _dummyList = [ None ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_Constraint_setMath(self):
    math = libsbml.parseFormula("2 * k")
    self.C.setMath(math)
    self.assertTrue( self.C.getMath() != math )
    self.assertEqual( True, self.C.isSetMath() )
    self.C.setMath(self.C.getMath())
    self.assertTrue( self.C.getMath() != math )
    self.C.setMath(None)
    self.assertEqual( False, self.C.isSetMath() )
    if (self.C.getMath() != None):
      pass    
    _dummyList = [ math ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_Constraint_setMessage(self):
    text = libsbml.XMLNode.convertStringToXMLNode(" Some text ",None)
    triple = libsbml.XMLTriple("p", "http://www.w3.org/1999/xhtml", "")
    att = libsbml.XMLAttributes()
    xmlns = libsbml.XMLNamespaces()
    xmlns.add( "http://www.w3.org/1999/xhtml", "")
    p = libsbml.XMLNode(triple,att,xmlns)
    p.addChild(text)
    triple1 = libsbml.XMLTriple("message", "", "")
    att1 = libsbml.XMLAttributes()
    node = libsbml.XMLNode(triple1,att1)
    node.addChild(p)
    self.C.setMessage(node)
    self.assertTrue( self.C.getMessage() != node )
    self.assertTrue( self.C.isSetMessage() == True )
    self.C.setMessage(self.C.getMessage())
    self.assertTrue( self.C.getMessage() != node )
    self.assertTrue( self.C.getMessageString() != None )
    self.C.unsetMessage()
    self.assertEqual( False, self.C.isSetMessage() )
    if (self.C.getMessage() != None):
      pass    
    _dummyList = [ node ]; _dummyList[:] = []; del _dummyList
    pass  

def suite():
  suite = unittest.TestSuite()
  suite.addTest(unittest.TestLoader.loadTestsFromTestCase(TestConstraint))

  return suite

if __name__ == "__main__":
  if unittest.TextTestRunner(verbosity=1).run(suite()).wasSuccessful() :
    sys.exit(0)
  else:
    sys.exit(1)
