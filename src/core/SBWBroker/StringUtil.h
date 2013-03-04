#ifndef SBW_STRING_UTIL
#define SBW_STRING_UTIL

#include <string>
#include <vector>

using namespace std;

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		* \ingroup C++Broker
		*
		* Utility class containing help functions for string processing. 
		*
		*
		* \date 10-19-2004
		*
		* \author fbergman
		*
		*
		*/
		class StringUtil
		{
		public:
			/**
			* checks whether a string is empty or not
			* \param s string to be checked
			* \return true if string is empty
			*/
			static bool empty(std::string s)
			{
				return (s == "" || s.length() == 0);
			}

			/**
			* parses a string for a range of integers
			* \param rangeStr string to parse
			* \return vector of integers containing the range
			*/
			static std::vector<int> parseRange(std::string rangeStr);

			/** 
			* Method to return a hex representation of a byte array.
			*
			* \param data[] the byte array
			* \param nLength the length of the array
			* \return 
			*/
			static std::string toHex(char data[], int nLength);
			static std::string toString(int nInt);
			static std::string EMPTY;
			static int toInt(std::string sString);
		private:
			/** std::string used for toHex(). **/
			static  std::string HEX;
		};
	}
}
#endif
