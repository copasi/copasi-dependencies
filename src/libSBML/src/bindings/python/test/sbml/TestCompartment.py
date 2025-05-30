#
# @file    TestCompartment.py
# @brief   Compartment unit tests
#
# @author  Akiya Jouraku (Python conversion)
# @author  Ben Bornstein 
# 
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/sbml/test/TestCompartment.c
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


class TestCompartment(unittest.TestCase):

  global C
  C = None

  def setUp(self):
    self.C = libsbml.Compartment(2,4)
    if (self.C == None):
      pass    
    pass  

  def tearDown(self):
    _dummyList = [ self.C ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_Compartment_create(self):
    self.assertTrue( self.C.getTypeCode() == libsbml.SBML_COMPARTMENT )
    self.assertTrue( self.C.getMetaId() == "" )
    self.assertTrue( self.C.getNotes() == None )
    self.assertTrue( self.C.getAnnotation() == None )
    self.assertTrue( self.C.getId() == "" )
    self.assertTrue( self.C.getName() == "" )
    self.assertTrue( self.C.getUnits() == "" )
    self.assertTrue( self.C.getOutside() == "" )
    self.assertTrue( self.C.getSpatialDimensions() == 3 )
    self.assertTrue( self.C.getVolume() == 1.0 )
    self.assertTrue( self.C.getConstant() == True )
    self.assertEqual( False, self.C.isSetId() )
    self.assertEqual( False, self.C.isSetName() )
    self.assertEqual( False, self.C.isSetSize() )
    self.assertEqual( False, self.C.isSetVolume() )
    self.assertEqual( False, self.C.isSetUnits() )
    self.assertEqual( False, self.C.isSetOutside() )
    pass  

  def test_Compartment_createWith(self):
    c = libsbml.Compartment(2,4)
    c.setId( "A")
    self.assertTrue( c.getTypeCode() == libsbml.SBML_COMPARTMENT )
    self.assertTrue( c.getMetaId() == "" )
    self.assertTrue( c.getNotes() == None )
    self.assertTrue( c.getAnnotation() == None )
    self.assertTrue( c.getName() == "" )
    self.assertTrue( c.getSpatialDimensions() == 3 )
    self.assertTrue((  "A"      == c.getId() ))
    self.assertTrue( c.getConstant() == True )
    self.assertEqual( True, c.isSetId() )
    self.assertEqual( False, c.isSetName() )
    _dummyList = [ c ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_Compartment_createWithNS(self):
    xmlns = libsbml.XMLNamespaces()
    xmlns.add( "http://www.sbml.org", "testsbml")
    sbmlns = libsbml.SBMLNamespaces(2,1)
    sbmlns.addNamespaces(xmlns)
    c = libsbml.Compartment(sbmlns)
    self.assertTrue( c.getTypeCode() == libsbml.SBML_COMPARTMENT )
    self.assertTrue( c.getMetaId() == "" )
    self.assertTrue( c.getNotes() == None )
    self.assertTrue( c.getAnnotation() == None )
    self.assertTrue( c.getLevel() == 2 )
    self.assertTrue( c.getVersion() == 1 )
    self.assertTrue( c.getNamespaces() != None )
    self.assertTrue( c.getNamespaces().getLength() == 2 )
    self.assertTrue( c.getName() == "" )
    self.assertTrue( c.getSpatialDimensions() == 3 )
    self.assertTrue( c.getConstant() == True )
    _dummyList = [ c ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_Compartment_free_NULL(self):
    _dummyList = [ None ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_Compartment_getSpatialDimensions(self):
    self.C.setSpatialDimensions(1)
    self.assertTrue( self.C.getSpatialDimensions() == 1 )
    pass  

  def test_Compartment_getsetConstant(self):
    self.C.setConstant(True)
    self.assertTrue( self.C.getConstant() == True )
    pass  

  def test_Compartment_getsetType(self):
    self.C.setCompartmentType( "cell")
    self.assertTrue((  "cell"  == self.C.getCompartmentType() ))
    self.assertEqual( True, self.C.isSetCompartmentType() )
    self.C.unsetCompartmentType()
    self.assertEqual( False, self.C.isSetCompartmentType() )
    pass  

  def test_Compartment_initDefaults(self):
    c = libsbml.Compartment(2,4)
    c.setId( "A")
    c.initDefaults()
    self.assertTrue((  "A" == c.getId() ))
    self.assertTrue( c.getName() == "" )
    self.assertTrue( c.getUnits() == "" )
    self.assertTrue( c.getOutside() == "" )
    self.assertTrue( c.getSpatialDimensions() == 3 )
    self.assertTrue( c.getVolume() == 1.0 )
    self.assertTrue( c.getConstant() == True )
    self.assertEqual( True, c.isSetId() )
    self.assertEqual( False, c.isSetName() )
    self.assertEqual( False, c.isSetSize() )
    self.assertEqual( False, c.isSetVolume() )
    self.assertEqual( False, c.isSetUnits() )
    self.assertEqual( False, c.isSetOutside() )
    self.assertEqual( True, c.isSetSpatialDimensions() )
    self.assertEqual( True, c.isSetConstant() )
    _dummyList = [ c ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_Compartment_setId(self):
    id =  "mitochondria";
    self.C.setId(id)
    self.assertTrue(( id == self.C.getId() ))
    self.assertEqual( True, self.C.isSetId() )
    if (self.C.getId() == id):
      pass    
    self.C.setId(self.C.getId())
    self.assertTrue(( id == self.C.getId() ))
    self.C.setId("")
    self.assertEqual( False, self.C.isSetId() )
    if (self.C.getId() != None):
      pass    
    pass  

  def test_Compartment_setName(self):
    name =  "My_Favorite_Factory";
    self.C.setName(name)
    self.assertTrue(( name == self.C.getName() ))
    self.assertEqual( True, self.C.isSetName() )
    if (self.C.getName() == name):
      pass    
    self.C.setName(self.C.getName())
    self.assertTrue(( name == self.C.getName() ))
    self.C.setName("")
    self.assertEqual( False, self.C.isSetName() )
    if (self.C.getName() != None):
      pass    
    pass  

  def test_Compartment_setOutside(self):
    outside =  "cell";
    self.C.setOutside(outside)
    self.assertTrue(( outside == self.C.getOutside() ))
    self.assertEqual( True, self.C.isSetOutside() )
    if (self.C.getOutside() == outside):
      pass    
    self.C.setOutside(self.C.getOutside())
    self.assertTrue(( outside == self.C.getOutside() ))
    self.C.setOutside("")
    self.assertEqual( False, self.C.isSetOutside() )
    if (self.C.getOutside() != None):
      pass    
    pass  

  def test_Compartment_setUnits(self):
    units =  "volume";
    self.C.setUnits(units)
    self.assertTrue(( units == self.C.getUnits() ))
    self.assertEqual( True, self.C.isSetUnits() )
    if (self.C.getUnits() == units):
      pass    
    self.C.setUnits(self.C.getUnits())
    self.assertTrue(( units == self.C.getUnits() ))
    self.C.setUnits("")
    self.assertEqual( False, self.C.isSetUnits() )
    if (self.C.getUnits() != None):
      pass    
    pass  

  def test_Compartment_unsetSize(self):
    self.C.setSize(0.2)
    self.assertTrue( self.C.getSize() == 0.2 )
    self.assertEqual( True, self.C.isSetSize() )
    self.C.unsetSize()
    self.assertEqual( False, self.C.isSetSize() )
    pass  

  def test_Compartment_unsetVolume(self):
    self.C.setVolume(1.0)
    self.assertTrue( self.C.getVolume() == 1.0 )
    self.C.unsetVolume()
    self.assertEqual( False, self.C.isSetVolume() )
    pass  

def suite():
  suite = unittest.TestSuite()
  suite.addTest(unittest.TestLoader.loadTestsFromTestCase(TestCompartment))

  return suite

if __name__ == "__main__":
  if unittest.TextTestRunner(verbosity=1).run(suite()).wasSuccessful() :
    sys.exit(0)
  else:
    sys.exit(1)
