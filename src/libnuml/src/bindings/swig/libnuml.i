/**
* Begin svn Header
* $Rev$:	Revision of last commit
* $Author$:	Author of last commit
* $Date$:	Date of last commit
* $HeadURL$
* $Id$
* End svn Header
* ***************************************************************************
* This file is part of libNUML.  Please visit http://code.google.com/p/numl/for more
* information about NUML, and the latest version of libNUML. 
* Copyright (c) 2013 The University of Manchester.
* 
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation.  A copy of the license agreement is 
* provided in the file named "LICENSE.txt" included with this software 
* distribution and also available online as http://www.gnu.org/licenses/lgpl.html 
* 
* The original code was initially developed by:
* 
* 	SBML Team
* 	Control and Dynamical Systems, MC 107-81
* 	California Institute of Technology
* 	Pasadena, CA, 91125, USA
* 
*  	http://www.sbml.org
* 	mailto:sbml-team@caltech.edu
* 
* Adapted for NUML by:
* 
* 	NUML Team
* 	Computational Systems Biology Group
* 	School of Computer Science
* 	The University of Manchester
* 	Manchester, M1 7DN UK
* 	mailto:josephodada@gmail.com
* 
*   Contributor(s):
*   Joseph O. Dada, The University of Manchester - initial LIBNUML API and implementation
* ***************************************************************************
**/


%module libnuml

%pragma(java) moduleclassmodifiers="
/**
  * Wrapper class for global methods and constants defined by libNUML.
  * <p>
  * <em style='color: #555'>
  * This class of objects is defined by libNUML only and has no direct
  * equivalent in terms of NUML components.  This class is not prescribed by
  * the NUML specifications, although it is used to implement features
  * defined in NUML.
  * </em>
  * <p>
  * In the C++ and C versions of libNUML, there exists a small number of
  * methods that are global in scope; in addition, libNUML uses a number
  * of enum's to define such things as error codes in a way that can be
  * used by both C++ and C.  This poses a problem in languages such as
  * Java, where there is no concept of global method or global constant.
  * SWIG wraps these global identifiers in the class whose documentation
  * you see before you.
  */
public class"


%{
#include "libnuml.h"

LIBNUML_CPP_NAMESPACE_USE

#include "local.cpp"
%}

%import  numl/common/libnuml-namespace.h
%import  numl/common/extern.h
%import  numl/common/numlfwd.h
%import  sbml/common/extern.h
%import  sbml/xml/XMLExtern.h


/**
 * Wraps List class by ListWrapper<TYPENAME> template class.
 * TYPENAME is replaced with a corresponding data type which is
 * stored in the List object (e.g. ModelCreator, CVTerm and ASTNode). 
 *
 * ListWrapper<TYPENAME> class is wrapped as TYPENAMEList class
 * (e.g. ListWrapper<CVTerm> -> CVTermList)
 *
 */


%include "ListWrapper.h"
%template(ModelCreatorList) ListWrapper<ModelCreator>;
%template(DateList)         ListWrapper<Date>;
%template(CVTermList)       ListWrapper<CVTerm>;
%template(ASTNodeList)      ListWrapper<ASTNode>;

/**
 *
 * Includes a language specific interface file.
 *
 */

%include local.i

/**
 * Unfortunately, SWIG makes no distinction between const and non-const
 * member functions (SWIG 1.3 Manual, Section 6.25), but in libNUML C++ we
 * have both const and non-const versions of most getter methods.  To avoid
 * a ton of warning messages about 'const' methods not being wrapped, we
 * disable Warning(516).
 */
#pragma SWIG nowarn=516


/**
 * Ignore the Visitor pattern accept() method (for now) on all NUML
 * objects.
 */
%ignore *::accept;

/**
 * Ignore internal implementation methods in ASTNode.h
 */
%ignore ASTNode(Token_t*);
%ignore ASTNode::getListOfNodes(ASTNodePredicate predicate) const;
%ignore ASTNode::fillListOfNodes;
%ignore ASTNode::setSemanticsFlag;
%ignore ASTNode::unsetSemanticsFlag;
%ignore ASTNode::getSemanticsFlag;
%ignore ASTNode::setDefinitionURL;
%ignore ASTNode::setUserData;
%ignore ASTNode::getUserData;
%ignore ASTNode::setParentNUMLObject;
%ignore ASTNode::getClass;
/**
 * SWIG makes no distinction between int and long arguments.
 * (SWIG 1.3 Manual, Section 6.15.2)
 */
%ignore ASTNode::setValue(int);

/**
 * Ignore operator= and operator<< on all NUML objects.
 */
%ignore *::operator=;
%ignore *::operator<<;
%ignore operator==;
%ignore operator!=;

/**
 * Ignore certain internal implementation methods on all objects.
 */
%ignore *::writeElements;
%ignore *::getElementPosition;
%ignore *::setNUMLDocument;
%ignore *::setParentNUMLObject;
%ignore *::setInternalId;
%ignore *::getInternalId;


/**
 * Ignore methods whose pointer argument serves as both input and output
 */
%ignore XMLAttributes::readInto;

/**
 * Ignore methods which receive List*.
 */
%ignore RDFAnnotationParser::parseRDFAnnotation(const XMLNode * annotation, List * CVTerms);

/**
 * Ignore methods which receive std::list.
 */
%ignore XMLErrorLog::add(const std::list<XMLError>& errors);
%ignore NUMLErrorLog::add(const std::list<NUMLError>& errors);

/**
 * Ignore 'static ParentMap mParent;' in SBO.h
 */
%ignore mParent;

/**
 * Ignore 'struct xmlErrorTableEntry' in XMLError.h.
 */
%ignore xmlErrorTableEntry;

/**
 * Both "const std::string& NMBase::getMetaId() const" and
 * "std:string& NMBase::getMetaId()" are defined in NMBase.cpp.
 * By default, SWIG doesn't convert non-const std:string& to and from
 * target language string.
 * So we ignore the non-const version.
 */
%ignore NMBase::getMetaId();

/**
 * Ignore internal FormulaUnitsData methods on NMBase
 */
%ignore NMBase::removeDuplicateAnnotations;
%ignore NMBase::setNUMLNamespaces;
%ignore NMBase::getNUMLNamespaces;
%ignore NMBase::read;
%ignore NMBase::write;


#include "ListWrapper.h"

/**
 * Ignore internal implementation methods in XMLAttributes
 */
%ignore XMLAttributes::addResource;
%ignore XMLAttributes::write;
%ignore XMLAttributes::setErrorLog;

/**
 * Ignore internal implementation methods in Event
 */
%ignore Event::setInternalIdOnly;

/**
 * Ignore internal implementation methods in NUMLErrorLog
 */
%ignore NUMLErrorLog::logError;
%ignore NUMLErrorLog::add;
%ignore NUMLErrorLog::remove;
%ignore NUMLErrorLog::NUMLErrorLog;

/**
 * Ignore internal implementation methods in XMLErrorLog
 */
%ignore XMLErrorLog::XMLErrorLog;
%ignore XMLErrorLog::add;
%ignore XMLErrorLog::setParser;


/**
 * Ignore internal implementation methods in ModelCreator
 */
%ignore ModelCreator::getAdditionalRDF;

/**
 * Ignore internal implementation methods in RDFAnnotationParser
 */
%ignore RDFAnnotationParser::hasRDFAnnotation;
%ignore RDFAnnotationParser::hasAdditionalRDFAnnotation;
%ignore RDFAnnotationParser::hasCVTermRDFAnnotation;
%ignore RDFAnnotationParser::hasHistoryRDFAnnotation;

/**
 * Ignore internal implementation methods in SyntaxChecer
 */
%ignore SyntaxChecker::isAllowedElement;
%ignore SyntaxChecker::hasDeclaredNS;
%ignore SyntaxChecker::isCorrectHTMLNode;

/**
 * Ignore internal implementation methods in NUMLNamespces
 */
%ignore NUMLNamespaces::setLevel;
%ignore NUMLNamespaces::setVersion;
%ignore NUMLNamespaces::setNamespaces;

/**
 * Ignore internal implementation methods in XMLToken
 */
%ignore XMLToken::write;

/**
 * Ignore internal implementation methods in XMLNode
 */
%ignore XMLNode::XMLNode(XMLInputStream&);
%ignore XMLNode::write;

/**
 * Ignore internal implementation methods in XMLOutputStream
 */
%ignore XMLOutputStream::getStringStream;

/**
 * Ignore internal implementation classes
 */
%ignore XMLOutputStringStream;
%ignore XMLOutputFileStream;

/**
 * Ignore the unsigned int version of XMLOutputStream::writeAttribute method
 * in order to properly wrap the long version of XMLOutputStream::writeAttribute 
 * method which should be used instead of the unsigned int version.
 */
%ignore XMLOutputStream::writeAttribute(const std::string&, const unsigned int&);
%ignore XMLOutputStream::writeAttribute(const XMLTriple&,   const unsigned int&);

/**
 * The following methods will create new objects.  To prevent memory
 * leaks we must inform SWIG of this.
 */

%typemap(newfree) char * "free($1);";

%newobject *::clone;
%newobject NMBase::toNUML;
%newobject NUMLReader::readNUMLFromString;
%newobject NUMLReader::readNUMLFromFile;
%newobject NUMLReader::readNUML;
%newobject readNUML(const char *);
%newobject readNUMLFromString(const char *);
%newobject readNUMLFromFile(const char *);
%newobject NUMLWriter::writeToString;
%newobject writeNUMLToString;
%newobject *::remove;
//%newobject Result::removeResultComponent;
%newobject ResultComponent::removeDimension;
//%newobject ResultComponent::createDimensionDescription;
%newobject ResultComponent::removeDimensionDescription;
//%newobject ResultComponent::createCompositeDescription;
%newobject DimensionDescription::removeCompositeDescription;
%newobject DimensionDesription::removeTupleDescription;
%newobject DimensionDescription::removeAtomicDescription;
%newobject Dimension::removeCompositeValue;
%newobject Dimension::removeTupleValue;
%newobject Dimension::removeAtomicValue;
%newobject RDFAnnotationParser::parseRDFAnnotation(XMLNode *);
%newobject RDFAnnotationParser::deleteRDFAnnotation;
%newobject RDFAnnotationParser::parseCVTerms;
%newobject RDFAnnotationParser::parseModelHistory;
%newobject RDFAnnotationParser::createRDFAnnotation;
%newobject RDFAnnotationParser::createAnnotation;
%newobject RDFAnnotationParser::createRDFDescription;
%newobject RDFAnnotationParser::createCVTerms;
%newobject XMLNode::removeChild;
%newobject XMLNode::convertStringToXMLNode;

/**
 * 
 * wraps "List* ASTNode::getListOfNodes(ASTNodePredicate)" function
 * as "ListWrapper<ASTNode>* ASTNode::getListOfNodes()" function 
 * which returns a list of all ASTNodes. 
 *
 */


//%inline
//%{
//  int ASTNode_true(const ASTNode *node)
//  {
//    return 1;
//  }
//%}

%extend ASTNode
{
  ListWrapper<ASTNode>* getListOfNodes()
  {
    List *list = $self->getListOfNodes(ASTNode_true);
    return new ListWrapper<ASTNode>(list);
  }
}


/*
 * Wraps "static void RDFAnnotationParser::parseRDFAnnotation(const XMLNode *annotation, 
 * List *CVTerms)" function as 
 * "static void RDFAnnotationParser::parseRDFAnnotation(const XMLNode *annotation, 
 *  ListWrapper<CVTerm> *CVTerms);
 *
 */

%extend RDFAnnotationParser
{
  static void RDFAnnotationParser::parseRDFAnnotation(const XMLNode *annotation, 
                                                      ListWrapper<CVTerm> *CVTerms)
  {
    if (!CVTerms) return;

    List *list = CVTerms->getList();
    RDFAnnotationParser::parseRDFAnnotation(annotation,list);
  }
}



%ignore ASTBasePlugin;
%ignore Model;
%ignore List::begin;
%ignore List::end;

/**
 * Wrap these files.
 */

%include "std_string.i"

%include numl/common/libnuml-version.h
%include numl/common/operationReturnValues.h

%include numl/NUMLNamespaces.h
%include numl/NUMLReader.h
%include numl/NUMLWriter.h
%include numl/NUMLTypeCodes.h
%include numl/NMBase.h

// list
%include <sbml/util/List.h>

// annotations
%include <sbml/annotation/CVTerm.h>
%include <sbml/annotation/Date.h>
%include <sbml/annotation/ModelHistory.h>
%include <sbml/annotation/ModelCreator.h>
%include <sbml/annotation/RDFAnnotation.h>
%include <sbml/annotation/RDFAnnotationParser.h>


%include <numl/NUMLDocument.h>
%include <sbml/xml/XMLError.h>
%include <sbml/SBMLNamespaces.h>

%include <numl/NUMLList.h>

%include <numl/OntologyTerm.h>

%include <numl/ResultComponent.h>

%include <numl/Dimension.h>
%include <numl/DimensionDescription.h>

%include <numl/CompositeValue.h>
%include <numl/Tuple.h>
%include <numl/AtomicValue.h>

%include <numl/CompositeDescription.h>
%include <numl/TupleDescription.h>
%include <numl/AtomicDescription.h>

%include <sbml/xml/XMLAttributes.h>
%include sbml/xml/XMLConstructorException.h
%include <sbml/xml/XMLNamespaces.h>
%include <sbml/xml/XMLToken.h>
%include <sbml/xml/XMLNode.h>
%include <sbml/xml/XMLTriple.h>
%include <sbml/xml/XMLInputStream.h>
%include <sbml/xml/XMLOutputStream.h>
%include <sbml/xml/XMLError.h>
%include <sbml/xml/XMLErrorLog.h>
%include <sbml/math/ASTNode.h>
%include <sbml/annotation/RDFAnnotation.h>

%include numl/NUMLErrorLog.h
%include numl/NUMLError.h

