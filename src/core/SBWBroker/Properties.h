#ifndef SBW_PROPERTIES
#define SBW_PROPERTIES

#include <string>
#include <vector>
#include <map>

#if    _MSC_VER > 1000
#pragma once
#pragma warning (disable: 4996)
#endif

using namespace std;

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 *
		 * The Properties class is a help class that stores any properties needed during the runtime of the 
		 * Broker. 
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class Properties 
		{
		public:
			/**
			 *
			 * \return the one and only Properties object
			 */
			static Properties* getInstance();

      /**
       * delete static instance (will be called atexit)
       */
      static void deleteInstance();

			/**
			 * sets the value of a property
			 * \param sName  the name of the property to set
			 * \param sValue the value of the property
			 */
			static void setProperty(std::string sName, std::string sValue);
			/**
			 * sets the value of a property or, if the Value is NULL or EMPTY sets the Default value
			 * \param sName the name of the property
			 * \param sValue the value of the property (possibly NULL or EMPTY)
			 * \param sDefault the default value for this property
			 */
			static void setProperty(std::string sName, std::string sValue, std::string sDefault);
			/**
			 * return the value of a property
			 * \param sName the property to return
			 * \return the value or "" in case of a non-existing property
			 */
			static std::string getProperty(std::string sName);
			/**
			 * return the value of a property or the Default value for that property if the value is EMPTY
			 * \param sName the name of the property
			 * \param sDefault the default value of the property
			 * \return the value of a property or the Default value for that property if the value is EMPTY
			 */
			static std::string getProperty(std::string sName, std::string sDefault);
			/**
			 * removes a property from storage
			 * \param sName the name of the property to remove
			 */
			static void	removeProperty(std::string sName);
			/**
			 * this method checks whether a property with a given name exists or not
			 * \param sName name of the property
			 * \return <b>true</b> if the proerty exists, <b>false</b> otherwise
			 */
			static bool	hasProperty(std::string sName);
			/**
			 * change the value of a property 
			 * \param sName the name of the property
			 * \param sValue the new value of the property
			 */
			static void changeProperty(std::string sName, std::string  sValue);
			/**
			 * converts an integer value to a string
			 * \param nValue integer value
			 * \return string value
			 */
			static std::string toString(int nValue);
			/**
			 * converts a double value to a string
			 * \param dValue double value
			 * \return string value
			 */
			static std::string toString(double dValue);
			
			/**
			 * returns the named property as integer value
			 * \param sName the name of the property to return
			 * \return the integer representation of the property value
			 */
			static int		getPropertyAsInt(std::string sName);
			/**
			 * returns a named property as double value
			 * \param sName the name of the property to return
			 * \return the double representation of the property value
			 */
			static double	getPropertyAsDouble(std::string sName);

			/**
			 * converts a string to an integer
			 * \param sValue string value
			 * \return integer value
			 */
			static int toInt (std::string sValue);
			/**
			 * converts a string to a double 
			 * \param sValue string value
			 * \return double value
			 */
			static double toDouble (std::string sValue);

			/**
			 * checks whether a runtime property has a certain value 
			 * \param sPropertyName name of the property
			 * \param sExpression expression to meet
			 * \return <b>true</b> if expression is met by the value, <b>false</b> otherwise
			 */
			static bool assertRuntimeProperty(std::string sPropertyName, std::string sExpression);
			/**
			 * saves runtime properties onto harddrive 
			 */
			static void save();

		private:
			static Properties* oInstance;			///< the one and only Property instance
			typedef std::pair<std::string, std::string> StringString; ///< type definition for the hashmap			
			std::map < std::string, std::string > m_oProperties;	  ///< hashmap storing all properties
			
		};
	}
}

#endif
