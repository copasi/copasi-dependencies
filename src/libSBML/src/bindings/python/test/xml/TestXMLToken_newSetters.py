#
# @file    TestXMLToken_newSetters.py
# @brief   XMLToken_newSetters unit tests
#
# @author  Akiya Jouraku (Python conversion)
# @author  Sarah Keating 
# 
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/xml/test/TestXMLToken_newSetters.c
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


class TestXMLToken_newSetters(unittest.TestCase):


  def test_XMLToken_newSetters_addAttributes1(self):
    triple = libsbml.XMLTriple("test","","")
    attr = libsbml.XMLAttributes()
    token = libsbml.XMLToken(triple,attr)
    xt2 = libsbml.XMLTriple("name3", "http://name3.org/", "p3")
    i = token.addAttr( "name1", "val1")
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getAttributesLength() == 1 )
    self.assertTrue( token.isAttributesEmpty() == False )
    self.assertTrue( (  "name1" != token.getAttrName(0) ) == False )
    self.assertTrue( (  "val1"  != token.getAttrValue(0) ) == False )
    i = token.addAttr( "name2", "val2", "http://name1.org/", "p1")
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getAttributesLength() == 2 )
    self.assertTrue( token.isAttributesEmpty() == False )
    self.assertTrue( (  "name2" != token.getAttrName(1) ) == False )
    self.assertTrue( (  "val2"  != token.getAttrValue(1) ) == False )
    self.assertTrue( (  "http://name1.org/" != token.getAttrURI(1) ) == False )
    self.assertTrue( (  "p1"    != token.getAttrPrefix(1) ) == False )
    i = token.addAttr(xt2, "val2")
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getAttributesLength() == 3 )
    self.assertTrue( token.isAttributesEmpty() == False )
    self.assertTrue( (  "name3" != token.getAttrName(2) ) == False )
    self.assertTrue( (  "val2"  != token.getAttrValue(2) ) == False )
    self.assertTrue( (  "http://name3.org/" != token.getAttrURI(2) ) == False )
    self.assertTrue( (  "p3"    != token.getAttrPrefix(2) ) == False )
    _dummyList = [ xt2 ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ attr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_addAttributes2(self):
    triple = libsbml.XMLTriple("test","","")
    token = libsbml.XMLToken(triple)
    xt2 = libsbml.XMLTriple("name3", "http://name3.org/", "p3")
    i = token.addAttr( "name1", "val1")
    self.assertTrue( i == libsbml.LIBSBML_INVALID_XML_OPERATION )
    self.assertTrue( token.getAttributesLength() == 0 )
    self.assertTrue( token.isAttributesEmpty() == True )
    i = token.addAttr( "name2", "val2", "http://name1.org/", "p1")
    self.assertTrue( i == libsbml.LIBSBML_INVALID_XML_OPERATION )
    self.assertTrue( token.getAttributesLength() == 0 )
    self.assertTrue( token.isAttributesEmpty() == True )
    i = token.addAttr(xt2, "val2")
    self.assertTrue( i == libsbml.LIBSBML_INVALID_XML_OPERATION )
    self.assertTrue( token.getAttributesLength() == 0 )
    self.assertTrue( token.isAttributesEmpty() == True )
    _dummyList = [ xt2 ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_addNamespaces1(self):
    triple = libsbml.XMLTriple("test","","")
    attr = libsbml.XMLAttributes()
    token = libsbml.XMLToken(triple,attr)
    self.assertTrue( token.getNamespacesLength() == 0 )
    self.assertTrue( token.isNamespacesEmpty() == True )
    i = token.addNamespace( "http://test1.org/", "test1")
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getNamespacesLength() == 1 )
    self.assertTrue( token.isNamespacesEmpty() == False )
    _dummyList = [ attr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_addNamespaces2(self):
    triple = libsbml.XMLTriple("test","","")
    token = libsbml.XMLToken(triple)
    self.assertTrue( token.getNamespacesLength() == 0 )
    self.assertTrue( token.isNamespacesEmpty() == True )
    i = token.addNamespace( "http://test1.org/", "test1")
    self.assertTrue( i == libsbml.LIBSBML_INVALID_XML_OPERATION )
    self.assertTrue( token.getNamespacesLength() == 0 )
    self.assertTrue( token.isNamespacesEmpty() == True )
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_clearAttributes1(self):
    triple = libsbml.XMLTriple("test","","")
    attr = libsbml.XMLAttributes()
    token = libsbml.XMLToken(triple,attr)
    nattr = libsbml.XMLAttributes()
    xt1 = libsbml.XMLTriple("name1", "http://name1.org/", "p1")
    nattr.add(xt1, "val1")
    i = token.setAttributes(nattr)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.isAttributesEmpty() == False )
    i = token.clearAttributes()
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.isAttributesEmpty() == True )
    _dummyList = [ nattr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ attr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ xt1 ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_clearNamespaces1(self):
    triple = libsbml.XMLTriple("test","","")
    attr = libsbml.XMLAttributes()
    token = libsbml.XMLToken(triple,attr)
    ns = libsbml.XMLNamespaces()
    self.assertTrue( token.getNamespacesLength() == 0 )
    self.assertTrue( token.isNamespacesEmpty() == True )
    ns.add( "http://test1.org/", "test1")
    i = token.setNamespaces(ns)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getNamespacesLength() == 1 )
    self.assertTrue( token.isNamespacesEmpty() == False )
    i = token.clearNamespaces()
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getNamespacesLength() == 0 )
    self.assertTrue( token.isNamespacesEmpty() == True )
    _dummyList = [ attr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ ns ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_removeAttributes1(self):
    triple = libsbml.XMLTriple("test","","")
    attr = libsbml.XMLAttributes()
    token = libsbml.XMLToken(triple,attr)
    xt2 = libsbml.XMLTriple("name3", "http://name3.org/", "p3")
    xt1 = libsbml.XMLTriple("name5", "http://name5.org/", "p5")
    i = token.addAttr( "name1", "val1")
    i = token.addAttr( "name2", "val2", "http://name1.org/", "p1")
    i = token.addAttr(xt2, "val2")
    i = token.addAttr( "name4", "val4")
    self.assertTrue( token.getAttributes().getLength() == 4 )
    i = token.removeAttr(7)
    self.assertTrue( i == libsbml.LIBSBML_INDEX_EXCEEDS_SIZE )
    i = token.removeAttr( "name7")
    self.assertTrue( i == libsbml.LIBSBML_INDEX_EXCEEDS_SIZE )
    i = token.removeAttr( "name7", "namespaces7")
    self.assertTrue( i == libsbml.LIBSBML_INDEX_EXCEEDS_SIZE )
    i = token.removeAttr(xt1)
    self.assertTrue( i == libsbml.LIBSBML_INDEX_EXCEEDS_SIZE )
    self.assertTrue( token.getAttributes().getLength() == 4 )
    i = token.removeAttr(3)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getAttributes().getLength() == 3 )
    i = token.removeAttr( "name1")
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getAttributes().getLength() == 2 )
    i = token.removeAttr( "name2", "http://name1.org/")
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getAttributes().getLength() == 1 )
    i = token.removeAttr(xt2)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getAttributes().getLength() == 0 )
    _dummyList = [ xt1 ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ xt2 ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ attr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_removeNamespaces(self):
    triple = libsbml.XMLTriple("test","","")
    attr = libsbml.XMLAttributes()
    token = libsbml.XMLToken(triple,attr)
    token.addNamespace( "http://test1.org/", "test1")
    self.assertTrue( token.getNamespacesLength() == 1 )
    i = token.removeNamespace(4)
    self.assertTrue( i == libsbml.LIBSBML_INDEX_EXCEEDS_SIZE )
    self.assertTrue( token.getNamespacesLength() == 1 )
    i = token.removeNamespace(0)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getNamespacesLength() == 0 )
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ attr ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_removeNamespaces1(self):
    triple = libsbml.XMLTriple("test","","")
    attr = libsbml.XMLAttributes()
    token = libsbml.XMLToken(triple,attr)
    token.addNamespace( "http://test1.org/", "test1")
    self.assertTrue( token.getNamespacesLength() == 1 )
    i = token.removeNamespace( "test2")
    self.assertTrue( i == libsbml.LIBSBML_INDEX_EXCEEDS_SIZE )
    self.assertTrue( token.getNamespacesLength() == 1 )
    i = token.removeNamespace( "test1")
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getNamespacesLength() == 0 )
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ attr ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_setAttributes1(self):
    triple = libsbml.XMLTriple("test","","")
    attr = libsbml.XMLAttributes()
    token = libsbml.XMLToken(triple,attr)
    nattr = libsbml.XMLAttributes()
    xt1 = libsbml.XMLTriple("name1", "http://name1.org/", "p1")
    nattr.add(xt1, "val1")
    i = token.setAttributes(nattr)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.isAttributesEmpty() == False )
    _dummyList = [ nattr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ attr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ xt1 ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_setAttributes2(self):
    triple = libsbml.XMLTriple("test","","")
    token = libsbml.XMLToken(triple)
    nattr = libsbml.XMLAttributes()
    xt1 = libsbml.XMLTriple("name1", "http://name1.org/", "p1")
    nattr.add(xt1, "val1")
    i = token.setAttributes(nattr)
    self.assertTrue( i == libsbml.LIBSBML_INVALID_XML_OPERATION )
    self.assertTrue( token.isAttributesEmpty() == True )
    _dummyList = [ nattr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ xt1 ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_setEOF(self):
    token = libsbml.XMLToken()
    self.assertTrue( token.isEnd() == False )
    i = token.setEOF()
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.isEnd() == False )
    self.assertTrue( token.isStart() == False )
    self.assertTrue( token.isText() == False )
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_setEnd(self):
    token = libsbml.XMLToken()
    self.assertTrue( token.isEnd() == False )
    i = token.setEnd()
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.isEnd() == True )
    i = token.unsetEnd()
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.isEnd() == False )
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_setNamespaces1(self):
    triple = libsbml.XMLTriple("test","","")
    attr = libsbml.XMLAttributes()
    token = libsbml.XMLToken(triple,attr)
    ns = libsbml.XMLNamespaces()
    self.assertTrue( token.getNamespacesLength() == 0 )
    self.assertTrue( token.isNamespacesEmpty() == True )
    ns.add( "http://test1.org/", "test1")
    i = token.setNamespaces(ns)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue( token.getNamespacesLength() == 1 )
    self.assertTrue( token.isNamespacesEmpty() == False )
    _dummyList = [ attr ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ ns ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_setNamespaces2(self):
    triple = libsbml.XMLTriple("test","","")
    token = libsbml.XMLToken(triple)
    ns = libsbml.XMLNamespaces()
    self.assertTrue( token.getNamespacesLength() == 0 )
    self.assertTrue( token.isNamespacesEmpty() == True )
    ns.add( "http://test1.org/", "test1")
    i = token.setNamespaces(ns)
    self.assertTrue( i == libsbml.LIBSBML_INVALID_XML_OPERATION )
    self.assertTrue( token.getNamespacesLength() == 0 )
    self.assertTrue( token.isNamespacesEmpty() == True )
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ ns ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_setTriple1(self):
    triple = libsbml.XMLTriple("test","","")
    token = libsbml.XMLToken()
    i = token.setTriple(triple)
    self.assertTrue( i == libsbml.LIBSBML_OPERATION_SUCCESS )
    self.assertTrue((  "test" == token.getName() ))
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    pass  

  def test_XMLToken_newSetters_setTriple2(self):
    triple = libsbml.XMLTriple("test","","")
    token = libsbml.XMLToken("This is text")
    i = token.setTriple(triple)
    self.assertTrue( i == libsbml.LIBSBML_INVALID_XML_OPERATION )
    _dummyList = [ triple ]; _dummyList[:] = []; del _dummyList
    _dummyList = [ token ]; _dummyList[:] = []; del _dummyList
    pass  

def suite():
  suite = unittest.TestSuite()
  suite.addTest(unittest.TestLoader.loadTestsFromTestCase(TestXMLToken_newSetters))

  return suite

if __name__ == "__main__":
  if unittest.TextTestRunner(verbosity=1).run(suite()).wasSuccessful() :
    sys.exit(0)
  else:
    sys.exit(1)
