/**
 * @file    TestSBMLTransforms.cpp
 * @brief   SBMLTransforms unit tests
 * @author  Sarah Keating
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2020 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *     3. University College London, London, UK
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *  
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#include <sbml/common/common.h>
#include <sbml/common/extern.h>
#include <sbml/SBMLReader.h>
#include <sbml/SBMLTypes.h>

#include <sbml/SBMLTransforms.h>
#include <sbml/conversion/ConversionProperties.h>

#include <check.h>

#include <iostream>

LIBSBML_CPP_NAMESPACE_USE

BEGIN_C_DECLS


extern char *TestDataDirectory;


START_TEST (test_SBMLTransforms_replaceFD)
{
  SBMLReader        reader;
  SBMLDocument*     d;
  Model*            m;
  ASTNode           ast;
  FunctionDefinition*     fd;
  ListOfFunctionDefinitions * lofd;

  std::string filename(TestDataDirectory);
  filename += "multiple-functions.xml";


  d = reader.readSBML(filename);
  
  if (d == NULL)
  {
    fail("readSBML(\"multiple-functions.xml\") returned a NULL pointer.");
  }

  m = d->getModel();

  fail_unless( m->getNumFunctionDefinitions() == 3 );

  /* one function definition */
  ast = *m->getReaction(2)->getKineticLaw()->getMath();
  
  char* math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "f(S1, p) * compartmentOne / t"), NULL);
  safe_free(math);

  fd = m->getFunctionDefinition(0);
  SBMLTransforms::replaceFD(&ast, fd);
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "S1 * p * compartmentOne / t"), NULL);
  safe_free(math);

  /* https://github.com/sbmlteam/libsbml/issues/299 */
  /* fd: f_relabelled(p, S1) = p - S1 */
  /* ast: f_relabelled(S1, p) * compartmentOne / t */
  /* ast after replaceFD: (S1 - p) * compartmentOne / t */
  
  // use initial assignment instead of reaction, because the reaction
  // got flagged as having invalid units
  ast = *m->getInitialAssignment(0)->getMath();

  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "f_relabelled(S1, p) * compartmentOne / t"), NULL);
  safe_free(math);

  fd = m->getFunctionDefinition(2);
  SBMLTransforms::replaceFD(&ast, fd);

  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "(S1 - p) * compartmentOne / t"), NULL);
  safe_free(math);

  /* one function definition - nested */
  ast = *m->getReaction(1)->getKineticLaw()->getMath();
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "f(f(S1, p), compartmentOne) / t"), NULL);
  safe_free(math);

  // need to get function definition for 'f' back
  fd = m->getFunctionDefinition(0);
  SBMLTransforms::replaceFD(&ast, fd);
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "S1 * p * compartmentOne / t"), NULL);
  safe_free(math);

  /* two function definitions - nested */
  ast = *m->getReaction(0)->getKineticLaw()->getMath();
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "g(f(S1, p), compartmentOne) / t"), NULL);
  safe_free(math);

  SBMLTransforms::replaceFD(&ast, fd);
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "g(S1 * p, compartmentOne) / t"), NULL);
  safe_free(math);

  fd = m->getFunctionDefinition(1);

  SBMLTransforms::replaceFD(&ast, fd);
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "f(S1 * p, compartmentOne) / t"), NULL);
  safe_free(math);

  ast = *m->getReaction(0)->getKineticLaw()->getMath();
  lofd = m->getListOfFunctionDefinitions();
  SBMLTransforms::replaceFD(&ast, lofd);
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "S1 * p * compartmentOne / t"), NULL);
  safe_free(math);

  d->expandFunctionDefinitions();

  fail_unless( d->getModel()->getNumFunctionDefinitions() == 0 );
  
  ast = *d->getModel()->getReaction(0)->getKineticLaw()->getMath();
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "S1 * p * compartmentOne / t"), NULL);
  safe_free(math);

  ast = *d->getModel()->getReaction(1)->getKineticLaw()->getMath();
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "S1 * p * compartmentOne / t"), NULL);
  safe_free(math);

  ast = *d->getModel()->getReaction(2)->getKineticLaw()->getMath();
  
  math = SBML_formulaToString(&ast);
  fail_unless (!strcmp(math, "S1 * p * compartmentOne / t"), NULL);
  safe_free(math);

  delete d;
}
END_TEST


START_TEST(test_SBMLTransforms_evaluateAST)
{
  double temp;
  const char * mathml;
  ASTNode * node = new ASTNode();
  node->setValue((int)(2));
  
  fail_unless(SBMLTransforms::evaluateASTNode(node) == 2);

  node->setValue((double) (3.2));

  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 3.2));

  node->setValue((long)(1), (long)(4));

  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.25));

  node->setValue((double) (4.234), (int) (2));

  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 423.4));

  node->setType(AST_NAME_AVOGADRO);

  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 6.02214179e23));

  node->setType(AST_NAME_TIME);

  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  node->setType(AST_NAME);

  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node)));

  node->setType(AST_CONSTANT_E);

  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), exp(1.0)));

  node->setType(AST_CONSTANT_FALSE);

  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  node->setType(AST_CONSTANT_PI);

  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 4.0*atan(1.0)));

  node->setType(AST_CONSTANT_TRUE);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("2.5 + 6.1");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 8.6));

  delete node;
  node = SBML_parseFormula("-4.3");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), -4.3));

  delete node;
  node = SBML_parseFormula("9.2-4.3");
  temp = 9.2-4.3;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("2*3");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 6));

  delete node;
  node = SBML_parseFormula("1/5");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.2));

  delete node;
  node = SBML_parseFormula("pow(2, 3)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 8));

  delete node;
  node = SBML_parseFormula("3^3");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 27));

  delete node;
  node = SBML_parseFormula("abs(-9.456)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 9.456));

  delete node;
  node = SBML_parseFormula("ceil(9.456)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 10));

  delete node;
  node = SBML_parseFormula("exp(2.0)");
  temp = exp(2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("floor(2.04567)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 2));

  delete node;
  node = SBML_parseFormula("ln(2.0)");
  temp = log(2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("log10(100.0)");
  temp = log10(100.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("sin(2.0)");
  temp = sin(2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("cos(4.1)");
  temp = cos(4.1);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("tan(0.345)");
  temp = tan(0.345);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arcsin(0.456)");
  temp = asin(0.456);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arccos(0.41)");
  temp = acos(0.41);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arctan(0.345)");
  temp = atan(0.345);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("sinh(2.0)");
  temp = sinh(2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("cosh(4.1)");
  temp = cosh(4.1);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("tanh(0.345)");
  temp = tanh(0.345);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("and(1, 0)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  delete node;
  node = SBML_parseFormula("or(1, 0)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("not(1)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  delete node;
  node = SBML_parseFormula("xor(1, 0)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("xor(1, 1)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  delete node;
  node = SBML_parseFormula("eq(1, 2)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  delete node;
  node = SBML_parseFormula("eq(1, 1)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("geq(2,1)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("geq(2,4)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  delete node;
  node = SBML_parseFormula("geq(2,2)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("gt(2,1)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("gt(2,4)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  delete node;
  node = SBML_parseFormula("leq(2,1)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  delete node;
  node = SBML_parseFormula("leq(2,4)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("leq(2,2)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("lt(2,1)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  delete node;
  node = SBML_parseFormula("lt(2,4)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));
  
  delete node;
  node = SBML_parseFormula("neq(2,2)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 0.0));

  delete node;
  node = SBML_parseFormula("neq(3,2)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.0));

  delete node;
  node = SBML_parseFormula("cot(2.0)");
  temp = 1.0/tan(2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("csc(4.1)");
  temp = 1.0/sin(4.1);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("sec(0.345)");
  temp = 1.0/cos(0.345);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("coth(2.0)");
  temp = cosh(2.0)/sinh(2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));
  
  delete node;
  node = SBML_parseFormula("sech(2.0)");
  temp = 1.0/cosh(2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("csch(2.0)");
  temp = 1.0/sinh(2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arccot(2.0)");
  temp = atan(1/2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arccsc(2.0)");
  temp = asin(1/2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arcsec(2.0)");
  temp = acos(1/2.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arccosh(2.0)");
  temp = log(2.0 + pow(3.0, 0.5));
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arccoth(2.0)");
  temp = 0.5 * log(3.0);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arcsech(0.2)");
  temp = log(2*pow(6, 0.5)+5);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));
  
  delete node;
  node = SBML_parseFormula("arccsch(0.2)");
  /* temp = log(5 +pow(26, 0.5));
   * only matches to 15 dp and therefore fails !!
   */
  temp = 2.312438341272753;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arcsinh(3.0)");
  temp = log(3.0 +pow(10.0, 0.5));
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("arctanh(0.2)");
  temp = 0.5 * log(1.5);
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  node->setType(AST_FUNCTION_DELAY);
  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node)));

  delete node;
  node = SBML_parseFormula("factorial(3)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 6));
  
  delete node;
  node= SBML_parseFormula("piecewise()");
  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node)));

  delete node;
  node= SBML_parseFormula("piecewise(1,false)");
  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node)));

  delete node;
  node= SBML_parseFormula("piecewise(1,true)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1));

  delete node;
  node = SBML_parseFormula("piecewise(1.0, true, 0.0)");
  temp = 1.0;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("piecewise(1.0, false, 0.0)");
  temp = 0.0;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("piecewise(4.5)");
  temp = 4.5;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("piecewise(4.5, false, 5.5, true)");
  temp = 5.5;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("piecewise(4.5, true, 5.5, false)");
  temp = 4.5;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("piecewise(4.5, false, 5.5, false, 6.5)");
  temp = 6.5;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node= SBML_parseFormula("piecewise(4.5, true, 5.5, true)");

  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node)));

  delete node;
  node = SBML_parseFormula("piecewise(4.5, true, 4.5, true)");
  temp = 4.5;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  delete node;
  node = SBML_parseFormula("root(2, 4)");

  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 2));
  
  mathml = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">"
           "<apply><plus/></apply></math>";
  
  delete node;
  node = readMathMLFromString(mathml);

  temp = 0;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  mathml = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">"
           "<apply><plus/><cn>2.3</cn></apply></math>";
  
  delete node;
  node = readMathMLFromString(mathml);

  temp = 2.3;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  mathml = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">"
           "<apply><times/></apply></math>";
  
  delete node;
  node = readMathMLFromString(mathml);

  temp = 1.0;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));

  mathml = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">"
           "<apply><times/><cn>6.5</cn></apply></math>";
  
  delete node;
  node = readMathMLFromString(mathml);

  temp = 6.5;
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), temp));
  delete node;

}
END_TEST


START_TEST (test_SBMLTransforms_replaceIA)
{
  SBMLReader        reader;
  SBMLDocument*     d;
  Model*            m;

  std::string filename(TestDataDirectory);
  filename += "initialAssignments.xml";


  d = reader.readSBML(filename);

  if (d == NULL)
  {
    fail("readSBML(\"initialAssignments.xml\") returned a NULL pointer.");
  }

  m = d->getModel();

  fail_unless( m->getNumInitialAssignments() == 2 );
  fail_unless( !(m->getCompartment(0)->isSetSize()));
  fail_unless( m->getParameter(1)->getValue() == 2);

  d->expandInitialAssignments();

  m = d->getModel();

  fail_unless( m->getNumInitialAssignments() == 0 );
  fail_unless( m->getCompartment(0)->isSetSize());
  fail_unless( m->getCompartment(0)->getSize() == 25.0);
  fail_unless( m->getParameter(1)->getValue() == 50);
  
  delete d;
}
END_TEST

START_TEST (test_SBMLTransforms_expandFD)
{
  std::string filename(TestDataDirectory);
  filename += "multiple-functions.xml";


  // test 1: skip expansion of 'f'
  SBMLDocument *doc = readSBMLFromFile(filename.c_str());

  ConversionProperties props; 
  props.addOption("expandFunctionDefinitions", true);
  props.addOption("skipIds", "f");

  fail_unless(doc->convert(props) == LIBSBML_OPERATION_SUCCESS);
  fail_unless(doc->getModel() != NULL);
  fail_unless(doc->getModel()->getNumFunctionDefinitions() == 1);
  fail_unless(doc->getModel()->getFunctionDefinition("f") != NULL);
  
  delete doc;

  // test 2: expand all
  doc = readSBMLFromFile(filename.c_str());

  props = ConversionProperties(); 
  props.addOption("expandFunctionDefinitions", true);

  fail_unless(doc->convert(props) == LIBSBML_OPERATION_SUCCESS);
  fail_unless(doc->getModel() != NULL);
  fail_unless(doc->getModel()->getNumFunctionDefinitions() == 0);
  
  delete doc;

  // test 3: don't expand f and g
  doc = readSBMLFromFile(filename.c_str());

  props = ConversionProperties(); 
  props.addOption("expandFunctionDefinitions", true);
  props.addOption("skipIds", "f,g");

  fail_unless(doc->convert(props) == LIBSBML_OPERATION_SUCCESS);
  fail_unless(doc->getModel() != NULL);
  fail_unless(doc->getModel()->getNumFunctionDefinitions() == 2);
  
  delete doc;

  // test 4: even though comma separated is advertized, make sure that ';' works
  doc = readSBMLFromFile(filename.c_str());

  props = ConversionProperties(); 
  props.addOption("expandFunctionDefinitions", true);
  props.addOption("skipIds", "f;g");

  fail_unless(doc->convert(props) == LIBSBML_OPERATION_SUCCESS);
  fail_unless(doc->getModel() != NULL);
  fail_unless(doc->getModel()->getNumFunctionDefinitions() == 2);
  
  delete doc;

  // test 5: or space
  doc = readSBMLFromFile(filename.c_str());

  props = ConversionProperties(); 
  props.addOption("expandFunctionDefinitions", true);
  props.addOption("skipIds", "f g");

  fail_unless(doc->convert(props) == LIBSBML_OPERATION_SUCCESS);
  fail_unless(doc->getModel() != NULL);
  fail_unless(doc->getModel()->getNumFunctionDefinitions() == 2);
  
  delete doc;

  // test 6: or tab
  doc = readSBMLFromFile(filename.c_str());

  props = ConversionProperties(); 
  props.addOption("expandFunctionDefinitions", true);
  props.addOption("skipIds", "f\tg");

  fail_unless(doc->convert(props) == LIBSBML_OPERATION_SUCCESS);
  fail_unless(doc->getModel() != NULL);
  fail_unless(doc->getModel()->getNumFunctionDefinitions() == 2);
  
  delete doc;

  // test 7: or a combination
  doc = readSBMLFromFile(filename.c_str());

  props = ConversionProperties(); 
  props.addOption("expandFunctionDefinitions", true);
  props.addOption("skipIds", "f; g");

  fail_unless(doc->convert(props) == LIBSBML_OPERATION_SUCCESS);
  fail_unless(doc->getModel() != NULL);
  fail_unless(doc->getModel()->getNumFunctionDefinitions() == 2);
  
  delete doc;

}
END_TEST

START_TEST(test_SBMLTransforms_evaluateCustomAST)
{
  std::map<std::string, double> values;
  const char* formula = "a + b + c"; 

  ASTNode* node = SBML_parseFormula(formula);
  fail_unless(node != NULL);
  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node, values)));
  values["a"] = 1;
  values["b"] = 1;
  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node, values)));
  values["c"] = 1;
  fail_unless(SBMLTransforms::evaluateASTNode(node, values) == 3);

  delete node;
}
END_TEST

START_TEST (test_SBMLTransforms_replaceIA_species)
{
  SBMLReader        reader;
  SBMLDocument*     d;
  Model*            m;

  std::string filename(TestDataDirectory);
  filename += "initialAssignments_species.xml";


  d = reader.readSBML(filename);

  if (d == NULL)
  {
    fail("readSBML(\"initialAssignments_species.xml\") returned a NULL pointer.");
  }

  m = d->getModel();

  fail_unless( m->getNumInitialAssignments() == 3 );
  fail_unless( m->getParameter(1)->getValue() == 0.75);
  fail_unless( !(m->getParameter(2)->isSetValue()));
  fail_unless( m->getSpecies(2)->isSetInitialAmount());
  fail_unless( m->getSpecies(2)->getInitialAmount() == 2);

  d->expandInitialAssignments();

  m = d->getModel();

  fail_unless( m->getNumInitialAssignments() == 0 );
  fail_unless( m->getParameter(1)->getValue() == 3);
  fail_unless( m->getParameter(2)->isSetValue());
  fail_unless( m->getParameter(2)->getValue() == 0.75);
  fail_unless( !(m->getSpecies(2)->isSetInitialAmount()));
  fail_unless( m->getSpecies(2)->getInitialConcentration() == 2);
  
  delete d;
}
END_TEST


START_TEST(test_SBMLTransforms_evaluateL3V2AST)
{
  ASTNode * node;

  node = SBML_parseL3Formula("max(2, 3, 4, 5)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 5.0));

  delete node;
  node = SBML_parseL3Formula("min(2, 4.5, 1.2, 3)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1.2));

  delete node;
  node = SBML_parseL3Formula("rem(4,3)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 1));

  delete node;
  node = SBML_parseL3Formula("quotient(623,23)");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node), 27));

  delete node;
  node = SBML_parseL3Formula("undefinedfunc(x)");
  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node)));

  delete node;
  node = SBML_parseL3Formula("cos(undefinedfunc2(3.1))");
  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node)));

  delete node;
}
END_TEST


START_TEST(test_SBMLTransforms_evaluateL3V2ASTWithModel)
{
  SBMLReader        reader;
  SBMLDocument*     d;
  Model*            m;
  std::string filename(TestDataDirectory);
  filename += "multiple-functions.xml";

  d = reader.readSBML(filename);
  if (d == NULL)
  {
    fail("readSBML(\"multiple-functions.xml\") returned a NULL pointer.");
  }

  m = d->getModel();

  ASTNode * node;
  node = SBML_parseL3Formula("undefinedfunc(x)");
  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node, m)));

  delete node;
  node = SBML_parseL3Formula("f(2, g(1, undefinedfunc(0)))");
  fail_unless(util_isNaN(SBMLTransforms::evaluateASTNode(node, m)));

  delete node;
  node = SBML_parseL3Formula("f(2, g(4, cos(0)))");
  fail_unless(util_isEqual(SBMLTransforms::evaluateASTNode(node, m), 8));

  delete node;
  delete d;
}
END_TEST


START_TEST(test_SBMLTransforms_replaceIAWithFD)
{
  SBMLReader        reader;
  SBMLDocument*     d;
  Model*            m;

  std::string filename(TestDataDirectory);
  filename += "initialAssignmentsWithFD.xml";


  d = reader.readSBML(filename);

  if (d == NULL)
  {
    fail("readSBML(\"initialAssignments.xml\") returned a NULL pointer.");
  }

  m = d->getModel();

  fail_unless(m->getNumInitialAssignments() == 5);
  fail_unless(m->getParameter(0)->isSetValue() == false);
  fail_unless(m->getParameter(1)->isSetValue() == false);
  fail_unless(m->getParameter(2)->isSetValue() == false);
  fail_unless(m->getParameter(3)->isSetValue() == false);
  fail_unless(m->getParameter(4)->isSetValue() == false);

  d->expandInitialAssignments();

  m = d->getModel();

  fail_unless(m->getNumInitialAssignments() == 0);
  fail_unless(m->getParameter(0)->isSetValue() == true);
  fail_unless(m->getParameter(1)->isSetValue() == true);
  fail_unless(m->getParameter(2)->isSetValue() == true);
  fail_unless(m->getParameter(3)->isSetValue() == true);
  fail_unless(m->getParameter(4)->isSetValue() == true);
  fail_unless(util_isEqual(m->getParameter(0)->getValue(), 0.0));
  fail_unless(util_isEqual(m->getParameter(1)->getValue(), 1.0));
  fail_unless(util_isEqual(m->getParameter(2)->getValue(), 1.0));
  fail_unless(util_isEqual(m->getParameter(3)->getValue(), 0.0));
  fail_unless(util_isEqual(m->getParameter(4)->getValue(), 0.0));

  delete d;
}
END_TEST


START_TEST(test_SBMLTransforms_L3V2AssignmentNoMath)
{
  std::string filename(TestDataDirectory);
  filename += "l3v2-assignment-nomath.xml";

  SBMLDocument* d = readSBMLFromFile(filename.c_str());

  SBMLTransforms::clearComponentValues();
  fail_unless(d->getModel() != NULL);
  SBMLTransforms::IdValueMap values;
  SBMLTransforms::getComponentValuesForModel(d->getModel(), values);

  fail_unless(values["S1_stoich"].first == 2);

  
  delete d;

}
END_TEST

START_TEST(test_SBMLTransforms_StoichiometryMath)
{
  std::string filename(TestDataDirectory);
  filename += "l2v5-stoichiometrymath.xml";

  SBMLDocument* d = readSBMLFromFile(filename.c_str());

  SBMLTransforms::clearComponentValues();
  fail_unless(d->getModel() != NULL);
  SBMLTransforms::IdValueMap values;
  SBMLTransforms::getComponentValuesForModel(d->getModel(), values);

  fail_unless(values["generatedId_0"].first == 2);


  delete d;
}
END_TEST

START_TEST(test_SBMLTransforms_evaluateAST_L2SpeciesReference)
{
  SBMLDocument doc(2, 2);
  Model* pMod = doc.createModel();
  Compartment* pComp = pMod->createCompartment();
  pComp->initDefaults();
  pComp->setId("C");

  Species* pSpecies = pMod->createSpecies();
  pSpecies->initDefaults();
  pSpecies->setId("A");
  pSpecies->setCompartment("C");

  Reaction* pReaction = pMod->createReaction(); 
  pReaction->initDefaults();
  pReaction->setId("J1");
  SpeciesReference* pSR = pReaction->createReactant();
  pSR->setId("SR");
  pSR->setSpecies("A");
  KineticLaw* pKL = pReaction->createKineticLaw();
  ASTNode * node = SBML_parseL3Formula("A*0.1");
  pKL->setMath(node);
  delete node;

  node = SBML_parseFormula("SR");
  double value = SBMLTransforms::evaluateASTNode(node, pMod);

  fail_unless(value == 1.0);

  delete node;
}
END_TEST


START_TEST(test_SBMLTransforms_multipleMaps)
{
  SBMLDocument d1(3, 1);
  Model* m1 = d1.createModel();
  Compartment* c1 = m1->createCompartment();
  c1->setId("c");
  c1->setConstant(true);
  c1->setSize(1.0);

  Species* s1 = m1->createSpecies();
  s1->setId("s");
  s1->setCompartment("c");
  s1->setInitialConcentration(1.0);
  s1->setHasOnlySubstanceUnits(false);

  // at this point there shouldn't be a map for this model
  fail_unless(SBMLTransforms::getComponentValues(m1).size() == 0);

  // create a map for this model
  IdList list1 = SBMLTransforms::mapComponentValues(m1);
	fail_unless(list1.size() == 0);

  SBMLDocument d2(3, 2);
	Model* m2 = d2.createModel();
	c1 = m2->createCompartment();
	c1->setId("c");
	c1->setConstant(true);
	c1->setSize(2.0);

	s1 = m2->createSpecies();
	s1->setId("s");
	s1->setCompartment("c");
	s1->setInitialConcentration(2.0);
	s1->setHasOnlySubstanceUnits(false);

  fail_unless(SBMLTransforms::getComponentValues(m2).size() == 0);

  IdList list2 = SBMLTransforms::mapComponentValues(m2);
  fail_unless(list2.size() == 0);

  SBMLTransforms::IdValueMap values1 = SBMLTransforms::getComponentValues(m1);
  SBMLTransforms::IdValueMap values2 = SBMLTransforms::getComponentValues(m2);

  fail_unless(values1["c"].first == 1);
  fail_unless(values1["s"].first == 1);
	fail_unless(values2["c"].first == 2);
  fail_unless(values2["s"].first == 2);

  SBMLTransforms::clearComponentValues(m1);
  SBMLTransforms::clearComponentValues(m2);
  
}
END_TEST

Suite *
create_suite_SBMLTransforms (void)
{
  Suite *suite = suite_create("SBMLTransforms");
  TCase *tcase = tcase_create("SBMLTransforms");


  tcase_add_test(tcase, test_SBMLTransforms_expandFD);
  tcase_add_test(tcase, test_SBMLTransforms_replaceFD);
  tcase_add_test(tcase, test_SBMLTransforms_evaluateAST);
  tcase_add_test(tcase, test_SBMLTransforms_evaluateCustomAST);
  tcase_add_test(tcase, test_SBMLTransforms_evaluateAST_L2SpeciesReference);
  tcase_add_test(tcase, test_SBMLTransforms_replaceIA);
  tcase_add_test(tcase, test_SBMLTransforms_replaceIA_species);
  tcase_add_test(tcase, test_SBMLTransforms_evaluateL3V2AST);
  tcase_add_test(tcase, test_SBMLTransforms_evaluateL3V2ASTWithModel);
  tcase_add_test(tcase, test_SBMLTransforms_L3V2AssignmentNoMath);
  tcase_add_test(tcase, test_SBMLTransforms_StoichiometryMath);
	tcase_add_test(tcase, test_SBMLTransforms_multipleMaps);


  suite_add_tcase(suite, tcase);

  return suite;
}


END_C_DECLS

