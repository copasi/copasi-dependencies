#ifndef SBW_BROKER_SESSION_KEY
#define SBW_BROKER_SESSION_KEY

#include "StringUtil.h"
#include "SRandom.h"
#include "Sys.h"

#include "SBW/sbwdefs.h"
#include "SBW/SessionKey.h"
#include "SBW/SBWConfig.h"
#include "SBW/portableOS.h"

#include <iostream>
#include <fstream>

/** The length of the session key, in bits. **/
#define KEY_LENGTH  256

namespace SystemsBiologyWorkbench 
{
	namespace Broker
	{
		/**
		* Subclass of SessionKey that adds functionality used by the Broker
		* to generate and manipulate the session authentication key.
		* <p>
		* The session authentication key is a secret key consisting of a long random
		* number.  It is used to verify that a module or Broker connecting to another
		* Broker is permitted to do so.  The security provided by this is only
		* moderate.  It would be difficult for someone to randomly guess the key
		* or write a program to guess it in a reasonable amount of time.  However,
		* the key is kept in a file on the file system and the security is only
		* as good as the security on that file.
		* <p>
		* @see SessionKey
		* 
		* @author Michael Hucka
		* @author $Author: fbergmann $
		* @version $Revision: 1.1 $
		**/
		class BrokerSessionKey : public SystemsBiologyWorkbench::SessionKey
		{
		private:
			static std::string sMyKey;		///< the session key for this instance
		public:
			/**
			* Generates a new key and stores it internally.
			**/
			static void generateKey()
			{
				TRACE("Generating session key");				
				SRandom *generator = new SRandom();								
				sMyKey = StringUtil::toHex(generator->generateData(KEY_LENGTH),KEY_LENGTH);	
				delete generator;
			}

			/**
			* Saves the key to the session key file.
			**/
			static void saveKey()
			{
				writeToKeyFile(sMyKey);
#ifdef WIN32
				//! lock the file till broker is stopped
				//std::string sLock (Config::getSessionKeyFile());
				//HANDLE hLock = CreateFile(sLock.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
#endif
			}

			/**
			* Removes the session key from the session key file.
			* It does this by writing a blank string to the file.
			**/
			static void removeKey()
			{
				writeToKeyFile("");
			}

		private:
			/**
			* Writes the given string <var>str</var> (which should
			* be the session key) to the session key file dictated by
			* Config#getSessionKeyFile().
			* <p>
			* @param str the session key to write to the file
			**/
			static void writeToKeyFile( std::string str)
			{
				Sys::createDirectory(Config::getRuntimeFilesDirectory()+ SBWOS::DirectorySeparator());
				std::string * sSessionKeyFile = new std::string(Config::getSessionKeyFile());
				ofstream oFile (sSessionKeyFile->c_str());
				if (oFile.is_open())
				{
					oFile << str;
					oFile.flush();
					oFile.close();
				} 
				else 
				{
					TRACE("Cannot write session key to file '" << sSessionKeyFile->c_str() << "'");					
				}
				delete sSessionKeyFile;
			}
		};
	}
}
#endif
