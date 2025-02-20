#
# @file    TestModelHistory_newSetters.py
# @brief   ModelHistory unit tests
#
# @author  Akiya Jouraku (Python conversion)
# @author  Sarah Keating 
# 
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/annotation/test/TestModelHistory_newSetters.c
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


class TestModelHistory_newSetters(unittest.TestCase):


  def test_ModelHistory_addCreator1(self):
    mh = libsbml.ModelHistory()
    mc = libsbml.ModelCreator()
    mc.setFamilyName( "Keating")
    mc.setGivenName( "Sarah")
    i = mh.addCreator(mc)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( mh.getNumCreators() == 1 )
    _dummyList = [ mc ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ mh ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_ModelHistory_addCreator2(self):
    mh = libsbml.ModelHistory()
    mc = libsbml.ModelCreator()
    mc.setGivenName( "Sarah")
    i = mh.addCreator(mc)
    self.assertTrue( i == libsbml.LIBSBML_INVALID_OBJECT )
    self.assertTrue( mh.getNumCreators() == 0 )
    _dummyList = [ mc ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ mh ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_ModelHistory_addCreator3(self):
    mh = libsbml.ModelHistory()
    mc = None
    i = mh.addCreator(mc)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_FAILED )
    self.assertTrue( mh.getNumCreators() == 0 )
    _dummyList = [ mh ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_ModelHistory_setCreatedDate1(self):
    mh = libsbml.ModelHistory()
    self.assertTrue( mh != None )
    date = libsbml.Date("2005-12-30T12:15:32+02:00")
    i = mh.setCreatedDate(date)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( mh.isSetCreatedDate() == True )
    self.assertTrue( date != mh.getCreatedDate() )
    dateChar = mh.getCreatedDate().getDateAsString()
    self.assertTrue((  "2005-12-30T12:15:32+02:00" == dateChar ))
    i = mh.setCreatedDate(None)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( mh.isSetCreatedDate() == False )
    _dummyList = [ date ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ mh ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_ModelHistory_setCreatedDate2(self):
    mh = libsbml.ModelHistory()
    self.assertTrue( mh != None )
    date = libsbml.Date("Jan 12")
    i = mh.setCreatedDate(date)
    self.assertTrue( i == libsbml.LIBSBML_INVALID_OBJECT )
    self.assertTrue( mh.isSetCreatedDate() == False )
    _dummyList = [ date ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ mh ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_ModelHistory_setModifiedDate1(self):
    mh = libsbml.ModelHistory()
    self.assertTrue( mh != None )
    date = libsbml.Date("2005-12-30T12:15:32+02:00")
    i = mh.setModifiedDate(date)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( mh.isSetModifiedDate() == True )
    self.assertTrue( date != mh.getModifiedDate() )
    dateChar = mh.getModifiedDate().getDateAsString()
    self.assertTrue((  "2005-12-30T12:15:32+02:00" == dateChar ))
    i = mh.setModifiedDate(None)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_FAILED )
    self.assertTrue( mh.isSetModifiedDate() == True )
    _dummyList = [ date ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ mh ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_ModelHistory_setModifiedDate2(self):
    mh = libsbml.ModelHistory()
    self.assertTrue( mh != None )
    date = libsbml.Date(200,13,76,56,89,90,0,0,0)
    i = mh.setModifiedDate(date)
    self.assertTrue( i == libsbml.LIBSBML_INVALID_OBJECT )
    self.assertTrue( mh.isSetModifiedDate() == False )
    _dummyList = [ date ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ mh ]; _dummyList[:] = []; del _dummyList
    pass  

def suite():
  suite = unittest.TestSuite()
  suite.addTest(unittest.TestLoader.loadTestsFromTestCase(TestModelHistory_newSetters))

  return suite

if __name__ == "__main__":
  if unittest.TextTestRunner(verbosity=1).run(suite()).wasSuccessful() :
    sys.exit(0)
  else:
    sys.exit(1)

