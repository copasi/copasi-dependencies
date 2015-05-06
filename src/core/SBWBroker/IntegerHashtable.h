#ifndef SBW_INTEGER_HASHTABLE
#define SBW_INTEGER_HASHTABLE

#include <vector>
#include <map>

#include "SBW/SBWApplicationException.h"

using namespace std;


/* \def DEFAULT_CAPACITY 
*
* Default capacity of the hash table, in terms of number of buckets. \
*
**/			
#define DEFAULT_CAPACITY 31

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{		
		/**
		* \ingroup C++Broker
		*
		* Internal class representing entries in the hash table.  Buckets
		* in the hash table point to linked lists of these objects.
		*
		* \date 10-19-2004
		*
		* \author fbergman
		*
		*
		*/
		template<class T> class Entry
		{
		private:
			int key;                ///< The key for the entry.
			T value;				///< The value stored with this key.
			Entry<T> *next;         ///< Pointer to next entry in list.
		public:

			/**
			* constructor
			* \param key key for this Entry
			* \param value value for this Entry
			* \return new Entry
			*/
			Entry(int key, T value) 
			{ 
				this->key = key; 
				this->value = value; 
			}			
			/**
			*
			* \return Get the key for this entry.
			*/
			int getKey() 
			{
				return key; 
			}

			/**
			*
			* \return Get the value for this entry. 
			*/
			T getValue() 
			{
				return value; 
			}			
			/**
			* Set the value for this entry and return the old value. 
			* \param newValue new value
			* \return old value
			*/
			T setValue(T newValue) 
			{ 
				T old = value; 
				value = newValue; 
				return old; 
			}			
			/**
			* Reset the value for this entry to something else. 
			* \param newValue the new value to set to
			*/
			void changeValue(T newValue) 
			{ 
				value = newValue; 
			} 
		};
		/**
		* \ingroup C++Broker
		*
		* Simple, synchronized hash table implementation that keys objects by
		* integers.  Integer keys are mapped into an array of buckets, with each
		* bucket pointing to a singly-linked list of Entry objects.
		* <p>
		* Implementation note: this class does not reallocate if the table
		* becomes full; searches will simply take longer down the bucket chains.
		* 
		* \date 10-19-2004
		*
		* \author fbergman
		*
		*
		*/
		template<class T>class IntegerHashtable
		{
		public:
			/**
			* Default constructor; sets the capacity to the internal default
			* value #DEFAULT_CAPACITY.
			**/
			IntegerHashtable() 
			{
				capacity = DEFAULT_CAPACITY;
				m_size = 0;
			}
			/**
			* Constructor that takes a single argument indicating the desired
			* size of the internal hash table.
			* <p>
			* @param capacity an integer giving the desired capacity
			**/
			IntegerHashtable(int capacity)
			{
				this->capacity = capacity;
				m_size = 0;
			}
			/**
			* Returns the size of the hash table in terms of the number of
			* entries currently in it.
			* <p>
			* @returns the current size of the hash table
			**/
			int size() 
			{
				return table.size(); 
			}
			/**
			* Returns <code>true</code> if the given key is found in the
			* hash table, <code>false</code> otherwise.
			* <p>
			* @param key the integer key to search for
			* @returns <code>true</code> or <code>false</code> depending
			* on whether the key is found in the table
			**/
			bool exists(int key)
			{
				return find(key) != NULL;
			}
			/**
			* Retrieves the object keyed by the given key.
			* <p>
			* @param key the search key to use
			* @returns the object, if any, stored with the key
			**/
			T get(int key)
			{
				Entry<T> *e = find(key);
				return (e == NULL ? (T)0 : e->getValue());				
			}
			/**
			* Returns all values in the hash table as a linked list.
			* <p>
			* @returns a std::vector object containing the values
			* of the hash table.
			**/
			vector<T> getAll()
			{
				vector<T> oList;

				for (unsigned int i = 0; i < table.size(); i++)
					oList.push_back(table[i]->getValue());
				return oList;
			}
			/**
			* Add an entry into the hash table.  This maps the specified
			* <code>key</code> to the specified value <code>val</code> in this
			* hashtable.  The key can be an positive or negative integer, or zero.
			* The value can be null.  The value can be retrieved by calling the
			* #get method with a key that is equal to the original key.
			* <p>
			* @param key an integer that serves as the key for this entry
			* @param val the value to store and associate with the key
			**/
			void put(int key, T val)
			{
				table.push_back( new Entry<T>(key,val) );
			}
			/**
			* Remove an entry from the hash table.  This removes the
			* <code>key</code> and its corresponding value from the table, and
			* returns the object that was previously stored.  This method does
			* nothing if the key is not in the table.
			* <p>
			* @param key the key of the entry to be removed
			**/
			T remove(int key)
			{
/*#ifdef LINUX
			  for (unsigned int i=0; i < table.size(); i++)
			    {
			      if (table[i]->getKey() == key)
				{
				  T oTemp = table[i]->getValue();
				  table.erase(table.begin() + i);
				  return oTemp;
				}
			    }
			  return NULL;
#else*/
				typename vector< Entry< T >* >::iterator  oIterator;

				for ( oIterator = table.begin(); oIterator != table.end(); oIterator ++)
				{
					if ((*oIterator)->getKey() == key)
					{
						T oTemp = (*oIterator)->getValue();
						table.erase(oIterator);
						return oTemp;
					}
				}
				return 0;//(T)NULL;
//#endif
			}
		private:
			/**
			* Find and return an #Entry object stored under the given
			* <code>key</code>.
			* <p>
			* @param key the integer key to look up
			**/
			Entry<T> *find(int key)
			{
				for (unsigned int i = 0; i < table.size(); i++) 
				{
					Entry<T> *oTemp = table[i];
					if(oTemp->getKey() == key)
						return oTemp;
				}
				return NULL;					
			}
			/** Set capacity of the hash table, in terms of number of buckets. **/
			int capacity;
			/** Current size of the hash table (i.e., number of entries in it). **/
			int m_size;
			/** The hash table itself. **/
			vector < Entry < T > *>	table;			
		};
	}
}

#endif

