#
# @file    TestL3KineticLaw.py
# @brief   L3 KineticLaw unit tests
#
# @author  Akiya Jouraku (Python conversion)
# @author  Sarah Keating 
# 
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/sbml/test/TestL3KineticLaw.c
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


class TestL3KineticLaw(unittest.TestCase):

  global KL
  KL = None

  def setUp(self):
    self.KL = libsbml.KineticLaw(3,1)
    if (self.KL == None):
      pass    
    pass  

  def tearDown(self):
    _dummyList = [ self.KL ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_L3_KineticLaw_addParameter1(self):
    kl = libsbml.KineticLaw(3,1)
    p = libsbml.Parameter(3,1)
    i = self.KL.addParameter(p)
    self.assertTrue( i == libsbml.LIBSBML_INVALID_OBJECT )
    p.setId( "p")
    i = self.KL.addParameter(p)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( self.KL.getNumParameters() == 1 )
    self.assertTrue( self.KL.getNumLocalParameters() == 1 )
    self.assertTrue( kl.getNumParameters() == 0 )
    self.assertTrue( kl.getNumLocalParameters() == 0 )
    i = kl.addParameter(p)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( self.KL.getNumParameters() == 1 )
    self.assertTrue( self.KL.getNumLocalParameters() == 1 )
    self.assertTrue( kl.getNumParameters() == 1 )
    self.assertTrue( kl.getNumLocalParameters() == 1 )
    _dummyList = [ p ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ kl ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_L3_KineticLaw_addParameter2(self):
    kl = libsbml.KineticLaw(3,1)
    lp = libsbml.LocalParameter(3,1)
    lp1 = libsbml.LocalParameter(3,1)
    i = kl.addLocalParameter(lp)
    self.assertTrue( i == libsbml.LIBSBML_INVALID_OBJECT )
    lp.setId( "p")
    lp1.setId("p1");
    i = kl.addLocalParameter(lp)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( kl.getNumParameters() == 1 )
    self.assertTrue( kl.getNumLocalParameters() == 1 )
    i = kl.addParameter(lp1)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( kl.getNumParameters() == 2 )
    self.assertTrue( kl.getNumLocalParameters() == 2 )
    _dummyList = [ lp ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ kl ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_L3_KineticLaw_create(self):
    self.assertTrue( self.KL.getTypeCode() == libsbml.SBML_KINETIC_LAW )
    self.assertTrue( self.KL.getMetaId() == "" )
    self.assertTrue( self.KL.getNotes() == None )
    self.assertTrue( self.KL.getAnnotation() == None )
    self.assertTrue( self.KL.getNumParameters() == 0 )
    self.assertTrue( self.KL.getNumLocalParameters() == 0 )
    pass  

  def test_L3_KineticLaw_free_NULL(self):
    _dummyList = [ None ]; _dummyList[:] = []; del _dummyList
    pass  

def suite():
  suite = unittest.TestSuite()
  suite.addTest(unittest.TestLoader.loadTestsFromTestCase(TestL3KineticLaw))

  return suite

if __name__ == "__main__":
  if unittest.TextTestRunner(verbosity=1).run(suite()).wasSuccessful() :
    sys.exit(0)
  else:
    sys.exit(1)
