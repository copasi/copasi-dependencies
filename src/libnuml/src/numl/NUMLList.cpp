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

#include <numl/NUMLVisitor.h>
#include <numl/NUMLList.h>

using namespace std;

LIBNUML_CPP_NAMESPACE_BEGIN

NUMLList::NUMLList (unsigned int level, unsigned int version) :
   NMBase ( level, version )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}

NUMLList::NUMLList (NUMLNamespaces *numlns) :
    NMBase                   ( numlns )
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw NUMLConstructorException();
}

/*
 * Creates a new NUMLList items.
 */
NUMLList::NUMLList ()
{
}


/**
 * Used by the Destructor to delete each item in mItems.
 */
struct Delete
{
  void operator() (NMBase* sb) { delete sb; }
};


/*
 * Destroys the given NUMLList and its constituent items.
 */
NUMLList::~NUMLList ()
{
  for_each( mItems.begin(), mItems.end(), Delete() );
}


/**
 * Used by the Copy Constructor to clone each item in mItems.
 */
struct Clone
{
  NMBase* operator() (NMBase* sb) { return sb->clone(); }
};


/*
 * Copy constructor. Creates a copy of this NUMLList items.
 */
NUMLList::NUMLList (const NUMLList& orig) : NMBase(orig)
{
  mItems.resize( orig.size() );
  transform( orig.mItems.begin(), orig.mItems.end(), mItems.begin(), Clone() );
}


/*
 * Assignment operator
 */
NUMLList& NUMLList::operator=(const NUMLList& rhs)
{
  if(&rhs!=this)
  {
    this->NMBase::operator =(rhs);
    // Deletes existing items
    for_each( mItems.begin(), mItems.end(), Delete() );
    mItems.resize( rhs.size() );
    transform( rhs.mItems.begin(), rhs.mItems.end(), mItems.begin(), Clone() );
  }

  return *this;
}

/*
 * Accepts the given NUMLVisitor.
 */
bool
NUMLList::accept (NUMLVisitor& v) const
{
  v.visit(*this, getItemTypeCode() );
  for (unsigned int n = 0 ; n < mItems.size() && mItems[n]->accept(v); ++n) ;
  v.leave(*this, getItemTypeCode() );

  return true;
}


/*
 * @return a (deep) copy of this NUMLList items.
 */
NMBase*
NUMLList::clone () const
{
  return new NUMLList(*this);
}


/*
 * Adds item to the end of this NUMLList items.  This NUMLList items assumes
 * ownership of item and will delete it.
 */
void
NUMLList::append (const NMBase* item)
{
  appendAndOwn( item->clone() );
}


/*
 * Adds item to the end of this NUMLList items.  This NUMLList items assumes
 * ownership of item and will delete it.
 */
void
NUMLList::appendAndOwn (NMBase* item)
{
  mItems.push_back( item );
  item->setNUMLDocument(mNUML);
  item->setParentNUMLObject(this);
}


/*
 * @return the nth item in this NUMLList items.
 */
const NMBase*
NUMLList::get (unsigned int n) const
{
  return (n < mItems.size()) ? mItems[n] : 0;
}


/*
 * @return the nth item in this NUMLList items.
 */
NMBase*
NUMLList::get (unsigned int n)
{
  return const_cast<NMBase*>( static_cast<const NUMLList&>(*this).get(n) );
}


/**
 * Used by NUMLList::get() to lookup an NMBase based by its id.
 */
//struct IdEq
//{
//  const string& id;
//
//  IdEq (const string& id) : id(id) { }
//  bool operator() (NMBase* sb) { return sb->getId() == id; }
//};


/*
 * @return item in this NUMLList items with the given id or NULL if no such
 * item exists.
 */
//const NMBase*
//NUMLList::get (const std::string& sid) const
//{
//  vector<NMBase*>::const_iterator result;
//
//  result = find_if( mItems.begin(), mItems.end(), IdEq(sid) );
//  return (result == mItems.end()) ? 0 : *result;
//}


/*
 * @return item in this NUMLList items with the given id or NULL if no such
 * item exists.
 */
//NMBase*
//NUMLList::get (const std::string& sid)
//{
//  return const_cast<NMBase*>( static_cast<const NUMLList&>(*this).get(sid) );
//}


/*
 * Removes all items in this NUMLList object.
 *
 * If doDelete is true (default), all items in this NUMLList object are deleted
 * and cleared, and thus the caller doesn't have to delete those items.
 * Otherwise, all items are just cleared from this NUMLList object and the caller
 * is responsible for deleting all items (In this case, pointers to all items
 * should be stored elsewhere before calling this function by the caller).
 */
void
NUMLList::clear (bool doDelete)
{
  if (doDelete)
    for_each( mItems.begin(), mItems.end(), Delete() );
  mItems.clear();
}


/*
 * Removes the nth item from this NUMLList items and returns a pointer to
 * it.  The caller owns the returned item and is responsible for deleting
 * it.
 */
NMBase*
NUMLList::remove (unsigned int n)
{
  NMBase* item = get(n);
  if (item) mItems.erase( mItems.begin() + n );
  return item;
}


/*
 * Removes item in this NUMLList items with the given id or NULL if no such
 * item exists.  The caller owns the returned item and is repsonsible for
 * deleting it.
 */
//NMBase*
//NUMLList::remove (const std::string& sid)
//{
//  NMBase* item = 0;
//  vector<NMBase*>::iterator result;
//
//  result = find_if( mItems.begin(), mItems.end(), IdEq(sid) );
//
//  if (result != mItems.end())
//  {
//    item = *result;
//    mItems.erase(result);
//  }
//
//  return item;
//}
//

/*
 * @return the number of items in this NUMLList items.
 */
unsigned int
NUMLList::size () const
{
  return mItems.size();
}


/**
 * Used by NUMLList::setNUMLDocument().
 */
struct SetNUMLDocument
{
  NUMLDocument* d;

  SetNUMLDocument (NUMLDocument* d) : d(d) { }
  void operator() (NMBase* sbase) { sbase->setNUMLDocument(d); }
};


/**
 * Used by NUMLList::setParentNUMLObject().
 */
struct SetParentNUMLObject
{
  NMBase* sb;

  SetParentNUMLObject (NMBase *sb) : sb(sb) { }
  void operator() (NMBase* sbase) { sbase->setParentNUMLObject(sb); }
};


/*
 * Sets the parent NUMLDocument of this NUML object.
 */
void
NUMLList::setNUMLDocument (NUMLDocument* d)
{
  mNUML = d;
  for_each( mItems.begin(), mItems.end(), SetNUMLDocument(d) );
}


/*
 * Sets the parent NUML object of this NUML object.
 */
void
NUMLList::setParentNUMLObject (NMBase* sb)
{
  mParentNUMLObject = sb;
  for_each( mItems.begin(), mItems.end(), SetParentNUMLObject(this) );
}


/*
 * @return the typecode (int) of this NUML object or NUML_UNKNOWN
 * (default).
 */
/*int
NUMLList::getTypeCode () const
{
  return NUML_NUMLLIST;
}
*/

/*
 * @return the typecode (int) of NUML objects contained in this NUMList or
 * NUML_UNKNOWN (default).
 */
/*int
NUMLList::getItemTypeCode () const
{
  return NUML_UNKNOWN;
}*/





/*
 * @return the NUMLTypeCode_t of this NUML object or NUML_UNKNOWN
 * (default).
 */
NUMLTypeCode_t
NUMLList::getTypeCode () const
{
  return NUML_NUMLLIST;
}


/*
 * @return the NUMLTypeCode_t of NUML objects contained in this NUMLList or
 * NUML_UNKNOWN (default).
 */
NUMLTypeCode_t
NUMLList::getItemTypeCode () const
{
  return NUML_UNKNOWN;
}


/*
 * @return the name of this element ie "listOf".

 */
const string&
NUMLList::getElementName () const
{
  static const string name = "NUMLList";
  return name;
}


/**
 * Used by NUMLList::writeElements().
 */
struct Write
{
  LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& stream;

  Write (LIBSBML_CPP_NAMESPACE_QUALIFIER XMLOutputStream& s) : stream(s) { }
  void operator() (NMBase* sbrbase) { sbrbase->write(stream); }
};


/*
 * Subclasses should override this method to write out their contained
 * NUML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.
 */
void
NUMLList::writeElements (LIBSBML_CPP_NAMESPACE_QUALIFIER  XMLOutputStream& stream) const
{
  NMBase::writeElements(stream);
  for_each( mItems.begin(), mItems.end(), Write(stream) );
}


/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
NUMLList::readAttributes (const LIBSBML_CPP_NAMESPACE_QUALIFIER  XMLAttributes& attributes)
{
  NMBase::readAttributes(attributes);

  //const unsigned int level   = getLevel  ();
  //const unsigned int version = getVersion();

  std::vector<std::string> expectedAttributes;
  expectedAttributes.clear();

    expectedAttributes.push_back("metaid");



  //TODO
  // check that all attributes are expected
/*  for (int i = 0; i < attributes.getLength(); i++)
  {
    std::vector<std::string>::const_iterator end = expectedAttributes.end();
    std::vector<std::string>::const_iterator begin = expectedAttributes.begin();
    std::string name = attributes.getName(i);
    if (std::find(begin, end, name) == end)
    {
      std::string component = "<" + getElementName() + ">";
      logUnknownAttribute(name, level, version, component);
    }
  }
*/
}

void
NUMLList::writeAttributes (LIBSBML_CPP_NAMESPACE_QUALIFIER  XMLOutputStream& stream) const
{
  NMBase::writeAttributes(stream);

  //const unsigned int level   = getLevel  ();
  //const unsigned int version = getVersion();

}

/**
 * Creates a new NUMLList.
 *
 * @return a pointer to created NUMLList.
 */
LIBNUML_EXTERN
NUMLList_t *
NUMLList_create ()
{
  return new(nothrow) NUMLList;
}


/**
 * Frees the given NUMLList and its constituent items.
 *
 * This function assumes each item in the list is derived from NMBase.
 */
LIBNUML_EXTERN
void
NUMLList_free (NUMLList_t *lo)
{
  delete lo;
}


/**
 * @return a (deep) copy of this NUMLList items.
 */
LIBNUML_EXTERN
NUMLList_t *
NUMLList_clone (const NUMLList_t *lo)
{
  return static_cast<NUMLList_t*>( lo->clone() );
}


/**
 * Adds a copy of item to the end of this NUMLList items.
 */
LIBNUML_EXTERN
void
NUMLList_append (NUMLList_t *lo, const NMBase *item)
{
  lo->append(item);
}


/**
 * Adds the given item to the end of this NUMLList items.
 */
LIBNUML_EXTERN
void
NUMLList_appendAndOwn (NUMLList_t *lo, NMBase_t *item)
{
  lo->appendAndOwn(item);
}


/**
 * Returns the nth item in this NUMLList items.
 */
LIBNUML_EXTERN
NMBase *
NUMLList_get (NUMLList_t *lo, unsigned int n)
{
  return lo->get(n);
}


/*
 * @return item in this NUMLList items with the given id or NULL if no such
 * item exists.
 */
//LIBNUML_EXTERN
//NMBase *
//NUMLList_getById (NUMLList_t *lo, const char *sid)
//{
//  return (sid != NULL) ? lo->get(sid) : NULL;
//}
//

/**
 * Removes all items in this NUMLList object.
 *
 * If doDelete is true (non-zero), all items in this NUMLList object are deleted
 * and cleared, and thus the caller doesn't have to delete those items.
 * Otherwise (zero), all items are just cleared from this NUMLList object and the
 * caller is responsible for deleting all items (In this case, pointers to all
 * items should be stored elsewhere before calling this function by the caller).
 */
LIBNUML_EXTERN
void
NUMLList_clear (NUMLList_t *lo, int doDelete)
{
  lo->clear(doDelete);
}


/**
 * Removes the nth item from this NUMLList items and returns a pointer to
 * it.  The caller owns the returned item and is responsible for deleting
 * it.
 */
LIBNUML_EXTERN
NMBase *
NUMLList_remove (NUMLList_t *lo, unsigned int n)
{
  return lo->remove(n);
}


/*
 * Removes item in this NUMLList items with the given id or NULL if no such
 * item exists.  The caller owns the returned item and is repsonsible for
 * deleting it.
 */
//LIBNUML_EXTERN
//NMBase *
//NUMLList_removeById (NUMLList_t *lo, const char *sid)
//{
//  return (sid != NULL) ? lo->remove(sid) : NULL;
//}


/**
 * Returns the number of items in this NUMLList items.
 */
LIBNUML_EXTERN
unsigned int
NUMLList_size (const NUMLList_t *lo)
{
  return lo->size();
}


/**
 * @return the NUMLTypeCode_t of NUML objects contained in this NUMLList or
 * NUML_UNKNOWN (default).
 */
LIBNUML_EXTERN
NUMLTypeCode_t
NUMLList_getItemTypeCode (const NUMLList_t *lo)
{
  return lo->getItemTypeCode();
}




LIBNUML_CPP_NAMESPACE_END
