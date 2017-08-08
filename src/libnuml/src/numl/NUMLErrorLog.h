/*
* ****************************************************************************
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
* Contributors:
* Joseph O. Dada, The University of Manchester - initial API and implementation
* ****************************************************************************
*/

/**
 * @class NUMLErrorLog
 * @brief the error log containing all errors / warnings encountered
 */


#ifndef NUMLErrorLog_h
#define NUMLErrorLog_h


#include <numl/common/extern.h>
#include <sbml/xml/XMLError.h>
#include <sbml/xml/XMLErrorLog.h>
#include <numl/NUMLError.h>


#ifdef __cplusplus

LIBNUML_CPP_NAMESPACE_BEGIN

class LIBNUML_EXTERN NUMLErrorLog : public LIBSBML_CPP_NAMESPACE_QUALIFIER XMLErrorLog
{
public:

  /**
   * Returns the nth NUMLError in this log.
   *
   * Callers should first inquire about the number of items in the log by
   * using the NUMLErrorLog::getNumErrors() method.  (This method is
   * inherited from the parent class, XMLErrorLog).  Attempting to using an
   * error index number that exceed the number of errors in the log will
   * result in a NULL being returned.
   *
   * @param n unsigned int number of the error to retrieve.
   *
   * @return the <code>n</code>th NUMLError in this log.
   */
  const NUMLError* getError (unsigned int n) const;


  /**
   * Returns the number of errors that have been logged with the given
   * severity code.
   *
   * LibNUML associates severity levels with every NUMLError object to
   * provide an indication of how serious the problem is.  Severities range
   * from informational diagnostics to fatal (irrecoverable) errors.  Given
   * an NUMLError object instance, a caller can interrogate it for its
   * severity level using methods such as NUMLError::getSeverity(),
   * NUMLError::isFatal(), and so on.  The present method encapsulates
   * iteration and interrogation of all objects in an NUMLErrorLog, making
   * it easy to check for the presence of error objects with specific
   * severity levels.
   *
   * @if clike @param severity a value from
   * #NUMLErrorSeverity_t @endif@if java @param severity a
   * value from the set of <code>LIBNUML_SEV_</code> constants defined by
   * the interface class <code><a
   * href="libnumlConstants.html">libnumlConstants</a></code> @endif
   *
   * @return a count of the number of errors with the given severity code
   */
  unsigned int getNumFailsWithSeverity(unsigned int severity);


  /**
   * Creates a new, empty NUMLErrorLog.
   */
  NUMLErrorLog ();


  /**
   * Destroys this NUMLErrorLog.
   */
  virtual ~NUMLErrorLog ();


  /**
   * Convenience function that combines creating an NUMLError object and
   * adding it to the log.
   *
   * @param errorId an unsigned int, the identification number of the error.
   * 
   * @param details a string containing additional details about the error.
   * If the error code in @p errorId is one that is recognized by NUMLError,
   * the given message is @em appended to a predefined message associated
   * with the given code.  If the error code is not recognized, the message
   * is stored as-is as the text of the error.
   * 
   * @param line an unsigned int, the line number at which the error occured.
   * 
   * @param column an unsigned int, the column number at which the error occured.
   * 
   * @param severity an integer indicating severity of the error.
   * 
   * @param category an integer indicating the category to which the error
   * belongs.
   *
   * @docnote The native C++ implementation of this method defines a
   * default argument value.  In the documentation generated for different
   * libNUML language bindings, you may or may not see corresponding
   * arguments in the method declarations.  For example, in Java, a default
   * argument is handled by declaring two separate methods, with one of
   * them having the argument and the other one lacking the argument.
   * However, the libNUML documentation will be @em identical for both
   * methods.  Consequently, if you are reading this and do not see an
   * argument even though one is described, please look for descriptions of
   * other variants of this method near where this one appears in the
   * documentation.
   */
  void logError
  (
      const unsigned int errorId  = 0
    , const unsigned int level    = NUML_DEFAULT_LEVEL
    , const unsigned int version  = NUML_DEFAULT_VERSION
    , const std::string& details  = ""
    , const unsigned int line     = 0
    , const unsigned int column   = 0
    , const unsigned int severity = LIBNUML_SEV_ERROR
    , const unsigned int category = LIBNUML_CAT_NUML
  );


  /**
   * Adds the given NUMLError to the log.
   *
   * @param error NUMLError, the error to be logged.
   */
  void add (const NUMLError& error);


  /**
   * Adds (copies) the NUMLErrors in the given NUMLError list to this
   * NUMLErrorLog.
   *
   * @param errors list, a list of NUMLError to be added to the log.
   */
  void add (const std::list<NUMLError>& errors);


  /**
   * Removes an error having errorId from the NUMLError list.
   *  
   * Only the first item will be removed if there are multiple errors
   * with the given errorId.
   *
   * @param errorId the error identifier of the error to be removed.
   */
  void remove (const unsigned int errorId);


};

LIBNUML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* NUMLErrorLog_h */
