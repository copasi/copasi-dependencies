/**
 * @file   FbcVisitor.h
 * @brief  Visitor Design Pattern for the SBML object tree
 * @author Sarah Keating
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EBML-EBI), Hinxton, UK
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
 * ---------------------------------------------------------------------- -->
 *
 * @class FbcVisitor
 * @ingroup FBC
 * @brief @htmlinclude pkg-marker-fbc.html
 * Support class for performing operations on SBML 'fbc' package objects.
 * 
 * <em style='color: #555'>This class of objects is defined by libSBML only
 * and has no direct equivalent in terms of SBML components.</em>
 * 
 * This is a class that supports the use of the <a target="_blank"
 * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
 * Pattern</i></a> to perform operations on SBML objects.  The LibSBML
 * implementation of the Visitor Pattern couples the notion of visitation
 * with the traversal of an SBML model oject hierarchy.
 *
 * This class (FbcVisitor) does not provide stand-alone functionality;
 * rather, it defines the interface that user visitor classes must
 * implement.  All of the individual SBML object classes have methods named
 * <code>accept</code> that are used for invoking an object of class
 * FbcVisitor.  An example of its use is in the SBML validation system,
 * which is internally implemented using this Visitor Pattern facility.
 */

#ifndef FbcVisitor_h
#define FbcVisitor_h


#ifdef __cplusplus

#include <sbml/SBMLTypes.h>
#include <sbml/packages/fbc/common/FbcExtensionTypes.h>

LIBSBML_CPP_NAMESPACE_BEGIN

/**
 * Forward class name declarations avoid cyclic dependencies.
 */

class SBase;

class SBMLDocument;
class Model;
class FluxBound;
class FluxObjective;
class Objective;
class Species;

class ListOf;


class FbcVisitor
{
public:

  /**
   * Destructor method.
   */
  virtual ~FbcVisitor ();

  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on SBMLDocument objects.
   *
   * @param x the SBMLDocument object to visit.
   */
  virtual void visit (const SBMLDocument &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on Model objects.
   *
   * @param x the Model object to visit.
   */
  virtual void visit (const Model        &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on ListOf objects.
   *
   * @param x the ListOf object to visit.
   *
   * @param type the object type code.
   */
  virtual void visit (const ListOf       &x, int type);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on SBase objects.
   *
   * @param x the SBase object to visit.
   */
  virtual bool visit (const SBase                    &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on FunctionDefinition objects.
   *
   * @param x the FluxBound object to visit.
   */
  virtual bool visit (const FluxBound       &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on FunctionDefinition objects.
   *
   * @param x the FluxObjective object to visit.
   */
  virtual bool visit (const FluxObjective       &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on UnitDefinition objects.
   *
   * @param x the Objective object to visit.
   */
  virtual bool visit (const Objective           &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on ReplacedElement objects.
   *
   * @param x the Species object to visit.
   */
  virtual bool visit (const Species           &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on ReplacedElement objects.
   *
   * @param x the ListOfObjectives object to visit.
   */
  virtual bool visit (const ListOfObjectives           &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on SBMLDocument objects.
   *
   * @param x the SBMLDocument object to visit.
   */
  virtual void leave (const SBMLDocument &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on Model objects.
   *
   * @param x the Model object to visit.
   */
  virtual void leave (const Model        &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on Reaction objects.
   *
   * @param x the Reaction object to visit.
   */
  virtual void leave (const SBase     &x);


  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on ListOf objects.
   *
   * @param x the ListOf object to visit.
   *
   * @param type the object type code.
   *
   */
  virtual void leave (const ListOf &x, int type);

  /**
   * Interface method for using the <a target="_blank" 
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on ListOf objects.
   *
   * @param x the Objective object to visit.
   *
   * @param type the object type code.
   *
   */
  virtual void leave (const Objective &x);

  protected:

    friend class FbcValidatingVisitor;
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* FbcVisitor_h */

