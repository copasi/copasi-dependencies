#
# @file    TestSBMLError.py
# @brief   SBMLError unit tests, C++ version
#
# @author  Akiya Jouraku (Python conversion)
# @author  Sarah Keating 
# 
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/sbml/test/TestSBMLError.cpp
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


class TestSBMLError(unittest.TestCase):


  def test_SBMLError_create(self):
    error = libsbml.SBMLError()
    self.assertTrue( error != None )
    error = None
    error = libsbml.SBMLError(libsbml.EmptyListInReaction)
    self.assertTrue( error.getErrorId() == libsbml.EmptyListInReaction )
    #self.assertTrue( error.getSeverity() == libsbml.LIBSBML_SEV_ERROR )
    #self.assertTrue( error.getSeverityAsString() ==  "Error"  )
    self.assertTrue( error.getCategory() == libsbml.LIBSBML_CAT_SBML )
    self.assertTrue( error.getCategoryAsString() ==  "General SBML conformance" )
    error = None
    error = libsbml.SBMLError(libsbml.OverdeterminedSystem,2,1)
    self.assertTrue( error.getErrorId() == libsbml.OverdeterminedSystem )
    self.assertTrue( error.getSeverity() == libsbml.LIBSBML_SEV_WARNING )
    self.assertTrue( error.getSeverityAsString() ==  "Warning"  )
    self.assertTrue( error.getCategory() == libsbml.LIBSBML_CAT_OVERDETERMINED_MODEL )
    self.assertTrue( error.getCategoryAsString() ==  "Overdetermined model" )
    error = None
    error = libsbml.SBMLError(libsbml.OffsetNoLongerValid,2,2)
    self.assertTrue( error.getErrorId() == libsbml.OffsetNoLongerValid )
    self.assertTrue( error.getSeverity() == libsbml.LIBSBML_SEV_ERROR )
    self.assertTrue( error.getSeverityAsString() ==  "Error"  )
    self.assertTrue( error.getCategory() == libsbml.LIBSBML_CAT_GENERAL_CONSISTENCY )
    self.assertTrue( error.getCategoryAsString() ==  "SBML component consistency" )
    error = None
    error = libsbml.SBMLError(libsbml.NoSBOTermsInL1,2,2)
    self.assertTrue( error.getErrorId() == libsbml.NoSBOTermsInL1 )
    self.assertTrue( error.getSeverity() == libsbml.LIBSBML_SEV_WARNING )
    self.assertTrue( error.getSeverityAsString() ==  "Warning"  )
    self.assertTrue( error.getCategory() == libsbml.LIBSBML_CAT_SBML_L1_COMPAT )
    self.assertTrue( error.getCategoryAsString() ==  "Translation to SBML L1V2" )
    error = None
    error = libsbml.SBMLError(libsbml.DisallowedMathMLEncodingUse,2,2)
    self.assertTrue( error.getErrorId() == libsbml.DisallowedMathMLEncodingUse )
    self.assertTrue( error.getSeverity() == libsbml.LIBSBML_SEV_ERROR )
    self.assertTrue( error.getSeverityAsString() ==  "Error"  )
    self.assertTrue( error.getCategory() == libsbml.LIBSBML_CAT_MATHML_CONSISTENCY )
    self.assertTrue( error.getShortMessage() ==  "Use of the MathML 'encoding' attribute is not allowed on this element" )
    error = None
    error = libsbml.SBMLError(libsbml.DisallowedMathMLEncodingUse,1,2)
    self.assertTrue( error.getErrorId() == libsbml.DisallowedMathMLEncodingUse )
    self.assertTrue( error.getSeverity() == libsbml.LIBSBML_SEV_NOT_APPLICABLE )
    self.assertTrue( error.getCategory() == libsbml.LIBSBML_CAT_MATHML_CONSISTENCY )
    error = None
    error = libsbml.SBMLError(libsbml.UnknownError,2,4)
    self.assertTrue( error.getErrorId() == libsbml.UnknownError )
    self.assertTrue( error.getSeverity() == libsbml.LIBSBML_SEV_FATAL )
    self.assertTrue( error.getSeverityAsString() ==  "Fatal"  )
    self.assertTrue( error.getCategory() == libsbml.LIBSBML_CAT_INTERNAL )
    self.assertTrue( error.getShortMessage() ==  "Encountered unknown internal libSBML error" )
    error = None
    pass  

def suite():
  suite = unittest.TestSuite()
  suite.addTest(unittest.TestLoader.loadTestsFromTestCase(TestSBMLError))

  return suite

if __name__ == "__main__":
  if unittest.TextTestRunner(verbosity=1).run(suite()).wasSuccessful() :
    sys.exit(0)
  else:
    sys.exit(1)
