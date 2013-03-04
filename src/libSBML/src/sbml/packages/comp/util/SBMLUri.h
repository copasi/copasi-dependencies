/**
 * @file    SBMLUri.h
 * @brief   Definition of SBMLUri, the utility class for handling uris.
 * @author  Frank Bergmann
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
 * ------------------------------------------------------------------------ -->
 *
 * @class SBMLUri
 * @ingroup Comp
 * @brief @htmlinclude pkg-marker-comp.html
 * utility class for handling uris.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * The SBMLUri parses uris and provides information about them.
 */

#ifndef SBMLUri_h
#define SBMLUri_h

#include <sbml/common/sbmlfwd.h>

#ifdef __cplusplus

#include <string>


LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN SBMLUri
{
public:

  /**
   * Creates a new SBMLUri from the given string uri.
   */
  SBMLUri(const std::string& uri); 


  /**
   * Copy constructor; creates a copy of an SBMLUri object.
   *
   * @param orig the SBMLUri object to copy.
   *
   * @throws @if python ValueError @else SBMLConstructorException @endif@~
   * Thrown if the argument @p orig is @c NULL.
   */
  SBMLUri(const SBMLUri& orig); 


  /**
   * Destroy this SBMLUri object.
   */
  virtual ~SBMLUri(); 


  /**
   * Assignment operator for SBMLUri.
   *
   * @param rhs The object whose values are used as the basis of the
   * assignment.
   *
   * @throws @if python ValueError @else SBMLConstructorException @endif@~
   * Thrown if the argument @p rhs is @c NULL.
   */  
  SBMLUri& operator=(const SBMLUri& rhs);


  /**
   * Assignment operator for SBMLUri, allows parsing of the given string.
   *
   * @param uri the uri to parse
   */
  SBMLUri& operator=(const std::string& uri);


  /**
   * Creates and returns a deep copy of this SBMLUri object.
   *
   * @return a (deep) copy of this SBMLFileResolver object.
   */
  SBMLUri* clone () const;


  /**
   * @return the parsed scheme like 'file' or 'http'
   */
  const std::string& getScheme() const;


  /**
   * @return the host of the uri (empty in case of files)
   */
  const std::string& getHost() const;


  /**
   * @return the path of the uri (i.e: the full filename with path)
   */
  const std::string& getPath() const;


  /**
   * @return the query any additional query bit
   */
  const std::string& getQuery() const;


  /**
   * @return the original query (with only backslashes replaced to slashes)
   */
  const std::string& getUri() const;


  /**
   * Constructs a new uri relative to this object and the given uri. 
   * 
   * For example,  SBMLUri("c:\\test").relativeTo("test.xml") would 
   * construct a new file uri, with path c:/test/test.xml.
   * 
   * @param uri uri to be added
   * 
   * @return the new uri
   */
  SBMLUri relativeTo(const std::string& uri) const;


private: 
  void parse(const std::string& uri);
private: 
  std::string mScheme;
  std::string mHost;
  std::string mPath;
  std::string mQuery;
  std::string mUri;
};


LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* SBMLUri_h */

