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
**/

#include <algorithm>
#include <functional>
#include <string>
#include <list>

#include <sbml/xml/XMLToken.h>
#include <sbml/xml/XMLParser.h>
#include <numl/NUMLError.h>
#include <numl/NUMLErrorLog.h>



using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

/** Most of the methods are internal.  A few visible ones are at the end. */


/*
 * Creates a new empty NUMLErrorLog.
 */
NUMLErrorLog::NUMLErrorLog ()
{
}


/**
 * Used by the Destructor to delete each item in mErrors.
 */
struct Delete
{
  void operator() (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLError* error) { delete error; }
};


/*
 * Destroys this NUMLErrorLog.
 */
NUMLErrorLog::~NUMLErrorLog ()
{
/*
  //
  // debug code for NUMLErrorLog::remove(const unsigned int)
  //
  vector<XMLError*>::iterator iter;

  int count = 0;
  iter = mErrors.begin();
  while(iter != mErrors.end() )
  {
    ++count;
    unsigned int errid  = (*iter)->getErrorId();
    unsigned int column = (*iter)->getColumn();
    cout << "(" << count << ") ErrorId " << errid << " column " << column << endl;
    remove (errid);
    cout << "Size of mErrors " << mErrors.size() << endl;
    iter = mErrors.begin();
  }
*/
}


/*
 * See NUMLError for a list of NUML error codes and XMLError
 * for a list of system and XML-level error codes.
 */
void
NUMLErrorLog::logError ( const unsigned int errorId
                       , const unsigned int level
                       , const unsigned int version
                       , const std::string& details
                       , const unsigned int line
                       , const unsigned int column
                       , const unsigned int severity
                       , const unsigned int category )
{
  add( NUMLError( errorId, level, version, details, line, column,
                  severity, category ));
}


/*
 * Adds the given NUMLError to the log.
 *
 * @param error NUMLError, the error to be logged.
 */
void
NUMLErrorLog::add (const NUMLError& error)
{
  if (error.getSeverity() != LIBNUML_SEV_NOT_APPLICABLE)
    XMLErrorLog::add(error);
}


/*
 * Logs (copies) the NUMLErrors in the given NUMLError list to this
 * NUMLErrorLog.
 *
 * @param errors list, a list of NUMLError to be added to the log.
 */
void
NUMLErrorLog::add (const std::list<NUMLError>& errors)
{
  list<NUMLError>::const_iterator end = errors.end();
  list<NUMLError>::const_iterator iter;

  for (iter = errors.begin(); iter != end; ++iter)
    XMLErrorLog::add( *iter );
}


/*
 * Helper class used by NUMLErrorLog::remove.
 */
class MatchErrorId
{
public:
  MatchErrorId(const unsigned int theId) : idToFind(theId) {}

  bool operator() (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLError* e) const
  {
    return e->getErrorId() == idToFind;
  };

private:
  unsigned int idToFind;
};


/*
 * Removes an error having errorId from the NUMLError list.
 *
 * Only the first item will be removed if there are multiple errors
 * with the given errorId.
 *
 * @param errorId the error identifier of the error to be removed.
 */
void
NUMLErrorLog::remove (const unsigned int errorId)
{
  //
  // "mErrors.erase( remove_if( ...))" can't be used for removing
  // the matched items from the list, because the type of the vector container is pointer 
  // of XMLError object. 
  //
  // (Effective STL 50 Specific Ways to Improve Your Use of the Standard Template Library
  //  Scott Meyers
  //  Item 33: Be wary of remove-like algorithms on containers of pointers. 143)
  //
  //
  vector<LIBSBML_CPP_NAMESPACE_QUALIFIER XMLError*>::iterator delIter;

  // finds an item with the given errorId (the first item will be found if 
  // there are two or more items with the same Id)  
  delIter = find_if(mErrors.begin(), mErrors.end(), MatchErrorId(errorId));

  if ( delIter != mErrors.end() )
  {
    // deletes (invoke delete operator for the matched item) and erases (removes 
    // the pointer from mErrors) the matched item (if any)
    delete *delIter;
    mErrors.erase(delIter);    
  }
}


/*
 * Helper class used by 
 * NUMLErrorLog::getNumFailsWithSeverity(NUMLErrorSeverity_t).
 */
class MatchSeverity
{
public:
  MatchSeverity(const unsigned int s) : severity(s) {}

  bool operator() (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLError* e) const
  {
    return e->getSeverity() == severity;
  };

private:
  unsigned int severity;
};




/**
  * Returns number of errors that are logged with severity Error
  */
unsigned int
NUMLErrorLog::getNumFailsWithSeverity(unsigned int severity)
{
  int n = 0;

#if defined(__SUNPRO_CC)
  // Workaround for Sun cc which is missing:
  count_if(mErrors.begin(), mErrors.end(), MatchSeverity(severity), n);
#else
  n = count_if(mErrors.begin(), mErrors.end(), MatchSeverity(severity));
#endif

  return n;
}


/*
 * Returns the nth NUMLError in this log.
 *
 * @param n unsigned int number of the error to retrieve.
 *
 * @return the nth NUMLError in this log.
 */
const NUMLError*
NUMLErrorLog::getError (unsigned int n) const
{
  return static_cast<const NUMLError*>(XMLErrorLog::getError(n));
}

LIBNUML_CPP_NAMESPACE_END

