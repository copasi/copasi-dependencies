/**
 * @file SedConstructorException.h
 * @brief Definition of the SedConstructorException class.
 * @author DEVISER
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSEDML. Please visit http://sed-ml.org for more
 * information about SED-ML. The latest version of libSEDML can be found on
 * github: https://github.com/fbergmann/libSEDML/
 * 

 * Copyright (c) 2013-2019, Frank T. Bergmann
 * All rights reserved.
 * 

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 

 * 1. Redistributions of source code must retain the above copyright notice,
 * this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation. A copy of the license agreement is provided in the
 * file named "LICENSE.txt" included with this software distribution and also
 * available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class SedConstructorException
 * @sbmlbrief{} TODO:Definition of the SedConstructorException class.
 */


#ifndef SEDML_CONSTRUCTOR_EXCEPTION_H
#define SEDML_CONSTRUCTOR_EXCEPTION_H

#include <sedml/common/extern.h>
#include <sedml/SedNamespaces.h>

#ifdef __cplusplus

#include <string>
#include <stdexcept>
#include <algorithm>

LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedConstructorException : public std::invalid_argument
{
public:

  /** @cond doxygenLibsedmlInternal */
  
  /* constructor */
  SedConstructorException (std::string errmsg = "");
  SedConstructorException (std::string errmsg, std::string sedmlErrMsg);
  SedConstructorException (std::string elementName, SedNamespaces* xmlns);
  virtual ~SedConstructorException () throw();
  
 /** @endcond */

  /**
   * Returns the message associated with this Sed exception.
   *
   * @return the message string.
   */
  const std::string getSedErrMsg() const { return mSedErrMsg; }

private:
  std::string mSedErrMsg;
};


LIBSEDML_CPP_NAMESPACE_END

#endif /* __cplusplus */

#endif /* SEDML_CONSTRUCTOR_EXCEPTION_H */

