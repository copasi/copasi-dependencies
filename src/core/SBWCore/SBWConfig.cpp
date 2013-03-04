/**
* @file          SBWConfig.cpp
* @brief         Access static and run-time SBW configuration information.
* @author        SBW Development Group <sysbio-team@caltech.edu>
* @date Created  18th June 2001
* $Id: SBWConfig.cpp,v 1.8 2008/12/12 17:52:23 fbergmann Exp $
* $Source: /cvsroot/sbw/core/SBWCore/SBWConfig.cpp,v $
*
* Copyright 2001-2002 California Institute of Technology and
* the Japan Science and Technology Corporation.
* 
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation; either version 2.1 of the License, or
* any later version.
* 
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
* MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software and
* documentation provided hereunder is on an "as is" basis, and the
* California Institute of Technology and Japan Science and Technology
* Corporation have no obligations to provide maintenance, support,
* updates, enhancements or modifications.  In no event shall the
* California Institute of Technology or the Japan Science and Technology
* Corporation be liable to any party for direct, indirect, special,
* incidental or consequential damages, including lost profits, arising
* out of the use of this software and its documentation, even if the
* California Institute of Technology and/or Japan Science and Technology
* Corporation have been advised of the possibility of such damage.  See
* the GNU Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation,
* Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*
* The original code contained here was initially developed by:
*
*     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri <br>
*     The Systems Biology Workbench Development Group <br>
*     ERATO Kitano Systems Biology Project <br>
*     Control and Dynamical Systems, MC 107-81 <br>
*     California Institute of Technology <br>
*     Pasadena, CA, 91125, USA <br>
*
*     http://www.cds.caltech.edu/erato <br>
*     sysbio-team@caltech.edu
*
* Contributor(s):
*
**/

#if  _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE

// avoid the following warning: 
// The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strdup. See online help for details.
#pragma warning(disable : 4996)
#endif


#include <stdio.h>
#include <stdlib.h> 
#include "stdafx.h"
#include "SBWConfig.h"
#include "SBWRawException.h"
#include "SBWCommunicationException.h"

#ifdef WIN32
#include "winsock2.h"
#include "TCHAR.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#if defined(LINUX) || defined(DARWIN) || defined (SOLARIS)
#include <limits.h>
#define MAX_PATH PATH_MAX
#endif

#define SBW_PROPERTIES_FILE "config.properties"
#define SBW_HOME_FIELD "sbw.home="

using namespace SystemsBiologyWorkbench;

/**
* This looks for the %SBW home directory, attempting different approaches
* one after the other until one succeeds or they all fail:
* <ol>
* <li>If the environment variable SBW_HOME is set, it returns the value.
*
* <li>Else, if this is a Windows system, it tries to read the registry entry
* for %SBW and returns the value stored with the key.
*
* <li>Else, it looks for a file called "config.properties" in a subdirectory
* named ".sbw" in the user's home directory, and tries to read the property
* setting for "sbw.home" out of that.
*
* <li>Else, if that file does not exist and this is running under
* Windows, it makes a final attempt to find the file in the Windows
* system directory (e.g., /WINNT/system32).
* </ol>
*
* @return a string representing the pathname to the %SBW home directory
* @throw SBWRawException if the attempt to read the Windows registry entry
*   for %SBW failed
* @throw SBWCommunicationException if it cannot determine the %SBW 
*   installation directory after trying all the approaches
**/
std::string Config::getSBWHome()
{
	static std::string sbwHome;

	// The 1st time this is called, this computes the value.  Subsequent
	// calls return a cached value.

	if (! sbwHome.empty())
	{
		return sbwHome;
	}

  const char *envVal = getenv("SBW_HOME");
	if (envVal != NULL)
	{
		// Environment variable set.  This overrides anything else.
		TRACE("Environment variable SBW_HOME = '" << envVal << "'");
		sbwHome = envVal;
		return sbwHome;
	}
	else
	{
		TRACE("Environment variable SBW_HOME not set");
		bool haveRegistryKey = false;

#ifdef WIN32
		TRACE("Trying to look up Windows registry entry for SBW");

		HKEY hKey;
		const char *regKey = "Software\\Systems Biology Workbench\\SBW Core";
		const char *item = "Directory";

		if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, regKey,
			0, KEY_READ, &hKey))
		{
			DWORD dwType = REG_SZ;
			DWORD dwSize = 512;
			BYTE arRegData[512];

			if (ERROR_SUCCESS == RegQueryValueEx(hKey, item, NULL, &dwType,
				arRegData, &dwSize)
				&& dwType == REG_SZ)
			{
				haveRegistryKey = true;
				TRACE("Found registry entry: '" << (LPCTSTR) arRegData << "'");
				sbwHome = (LPCTSTR) arRegData;
				return sbwHome;
			}
			RegCloseKey(hKey);
			hKey = NULL;
		}
		if (!haveRegistryKey)
		{
			HKEY hKey;
			const char *regKey = "Software\\Systems Biology Workbench\\SBW Core";
			const char *item = "Directory";

			if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, regKey,
				0, KEY_READ, &hKey))
			{
				DWORD dwType = REG_SZ;
				DWORD dwSize = 512;
				BYTE arRegData[512];

				if (ERROR_SUCCESS == RegQueryValueEx(hKey, item, NULL, &dwType,
					arRegData, &dwSize)
					&& dwType == REG_SZ)
				{
					haveRegistryKey = true;
					TRACE("Found registry entry: '" << (LPCTSTR) arRegData << "'");
					sbwHome = (LPCTSTR) arRegData;
					return sbwHome;
				}
				RegCloseKey(hKey);
				hKey = NULL;
			}
		}
#endif

		if (!haveRegistryKey)       // Not Windows, or have no registry key.
		{
#ifdef WIN32
			TRACE("Could not find SBW registry entry in Windows registry");
#endif
			// Try to read the config.properties file from the ~/.sbw dir.

			std::string userDir = getSBWUserDir();
			if (! userDir.empty())
			{
				TRACE(userDir << " exists");
				sbwHome = readSBWHomeFromPropFile(userDir);
			}

			if (! sbwHome.empty())
				return sbwHome;
			else
			{
#ifdef WIN32
				// Last-ditch effort under Windows: try to find the
				// config.properties file in the Windows system directory.

				TRACE("Could not find user's .sbw directory");

				std::string sysDir = getWindowsSystemDirectory();
				if (! sysDir.empty())
				{
					TRACE(sysDir << " exists");
					sbwHome = readSBWHomeFromPropFile(sysDir);
				}

				if (! sbwHome.empty())
					return sbwHome;
				else
				{
					TRACE("Could not find config.properties file; giving up.");

					std::string details("Registry Key ");

					details += regKey;
					details += '\\';
					details += item;

					throw new SBWRawException(
						"SBW_HOME not set and SBW registry key not found."
						" This indicates that the SBW installation is"
						" corrupted.  You may be able to fix the installation"
						" by setting the environment variable SBW_HOME"
						" to the path to the parent of the folder containing"
						" the broker executable.",
						details.c_str());
#else
				throw new SBWCommunicationException(
					"Could not find ~/.sbw/config.properties and"
					" therefore could not determine SBW home directory",
					"Could not find ~/.sbw/config.properties and"
					" therefore could not determine SBW home directory."
					" This indicates that the SBW installation is"
					" corrupted.  You may be able to fix the installation"
					" by setting the environment variable SBW_HOME"
					" to the path to the parent of the folder containing"
					" the broker executable.");
#endif
#ifdef WIN32
			}
#endif
		}
	}

	// If we get here, we've failed to find the value anywhere.

	TRACE("Could not determine SBW home directory");
	throw new SBWCommunicationException(
		"SBW could not find the SBW home directory"
		"Multiple attempts to determine the SBW home directory"
		" have failed.  This indicates that the SBW installation is"
		" corrupted.  You may be able to fix the installation"
		" by setting the environment variable SBW_HOME"
		" to the path to the parent of the folder containing"
		" the broker executable.");
}
}

/**
* This returns the SBW "lib" directory for this %SBW installation.
* The directory pathname is obtained by appending "/lib" to the
* directory pathname returned by Config::getSBWHome().
*
* @return the full lib directory path for this %SBW installation
* @throw SBWCommunicationException if the SBW installation directory
*   is unknown (and thus the lib directory cannot be determined)
**/
std::string Config::getLibDirectory()
{
	std::string path = Config::getSBWHome();
	if (! path.empty())
	{
		path += SBWOS::DirectorySeparator();
		path += "lib";
		TRACE("Using SBW lib directory = '" << path << "'");
		return path;
	}
	else
	{
		TRACE("Can't find SBW lib directory because can't"
			" determine the SBW home directory");
		throw new SBWCommunicationException(
			"Cannot locate the SBW installation directory",
			"The SBW installation is corrupted.  You"
			" may be able to fix the installation by"
			" setting the environment variable SBW_HOME"
			" to the parent of the folder containing the"
			" broker executable.");
	}
}


#ifdef WIN32
typedef BOOL (STDMETHODCALLTYPE FAR * LPFNGETUSERPROFILEDIR) (
	HANDLE hToken,
	LPTSTR lpProfileDir,
	LPDWORD lpcchSize
	);
#endif

/**
* Returns the path to the user's ".sbw" subdirectory.  This subdirectory
* should be located in the user's home directory.  The approach to find
* the user's home directory depends on the operating system:
* <ul>
* <li> If the operating system is Windows, the user's home folder is 
* determined using Windows API calls.
*
* <li> If the operating system is Unix/Linux, the user's home directory is
* taken to be the value of the environment variable "HOME".
* </ul>
*
* @return a string, the pathname to the SBW user directory
* @throw SBWCommunicationException if the user's home directory cannot
*   be determined under Unix
**/
std::string Config::getSBWUserDir()
{
	static std::string userDir;

	if (! userDir.empty())
	{
		return userDir;
	}

	const char *envSBW_USER = getenv("SBW_USER");

	if (envSBW_USER != NULL)         // SBW_USER variable set.
	{
		userDir = envSBW_USER;
		return userDir;
	}

#ifdef WIN32
	OSVERSIONINFO osVersionInfo ;

	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO) ;

	if (!GetVersionEx(&osVersionInfo))
		SBWOS::ThrowError();

	if (osVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) 
	{
		// NT, 2000 and XP

		HMODULE  hUserEnvLib = NULL;

		hUserEnvLib = LoadLibrary( _T("userenv.dll") );
		if ( !hUserEnvLib )
			SBWOS::ThrowError();

		LPFNGETUSERPROFILEDIR GetUserProfileDirectory = NULL;

#ifdef UNICODE
		GetUserProfileDirectory =
			(LPFNGETUSERPROFILEDIR) GetProcAddress(
			hUserEnvLib, "GetUserProfileDirectoryW" );
#else
		GetUserProfileDirectory =
			(LPFNGETUSERPROFILEDIR) GetProcAddress(
			hUserEnvLib, "GetUserProfileDirectoryA" );
#endif

		if (!GetUserProfileDirectory)
			SBWOS::ThrowError();

		HANDLE hUserToken;

		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hUserToken))
			SBWOS::ThrowError();

		TCHAR buffer[MAX_PATH + 1];
		DWORD size = MAX_PATH + 1;

		if (!GetUserProfileDirectory(hUserToken, buffer, &size))
			SBWOS::ThrowError();

		// Release USERENV.DLL
		FreeLibrary( hUserEnvLib );

		userDir = buffer ;
	}
	else
	{
		// 95, 98 & ME
		TCHAR buffer[MAX_PATH + 1];

		if (!GetWindowsDirectory(buffer, MAX_PATH + 1))
			SBWOS::ThrowError();

		userDir = buffer ;
	}

#else // UNIX
	const char *envVal = getenv("HOME");

	if (envVal == NULL)         // Environment variable not set.
	{
		TRACE("Environment variable HOME not set -- can't find user home dir");
		throw new SBWCommunicationException(
			"User environment variable HOME is not set");
	}
	else                                // Environment variable set.
	{
		userDir = envVal;
	}

#endif

	userDir += SBWOS::DirectorySeparator();
	userDir += ".sbw";

	TRACE("Using user directory = '" << userDir << "'");
	return userDir;
}

/**
* This method returns the pathname of the %SBW runtime information directory
* for this user.  The runtime directory has the form
* 
* HOME/.sbw/runtime/IPADDRESS
*
* where HOME/.sbw is the path to the ".sbw" subdirectory in the user's
* home directory (i.e., the path returned by Config::getSBWUserDir())
* and IPADDRESS is the IP address of this computer.
*
* @return a string, the pathname of the %SBW runtime files directory
* @throw SBWCommunicationException if this method cannot determine the
*   host name or network address.
**/
std::string Config::getRuntimeFilesDirectory()
{
	static std::string runtimeDir;

	if (! runtimeDir.empty())
	{
		return runtimeDir;
	}

	runtimeDir = getSBWUserDir();
	runtimeDir += SBWOS::DirectorySeparator();
	runtimeDir += "runtime";

#ifdef OLD_RUNTIME_DIR 	
	char shortname[256];
	if (gethostname(shortname, 255) < 0)
		throw new SBWCommunicationException("Cannot determine host name");

	struct hostent *host = gethostbyname(shortname);
	if (host == NULL)
		throw new SBWCommunicationException(
		"Cannot determine local host's network address");
	else
	{
		char tmp[sizeof "255.255.255.255"];
		char *a = host->h_addr_list[0];
		sprintf(tmp, "%u.%u.%u.%u", (u_char) a[0], (u_char) a[1],
			(u_char) a[2], (u_char) a[3]);

		TRACE("Local host address is " << tmp);

		runtimeDir += SBWOS::DirectorySeparator();
		runtimeDir += tmp;
#else 
		runtimeDir += SBWOS::DirectorySeparator();
		runtimeDir += "127.0.0.1";
		
#endif
		TRACE("Runtime files directory = '" << runtimeDir << "'");

		return runtimeDir;
#ifdef OLD_RUNTIME_DIR 	
	}
#endif
}

/**
* Returns the pathname of the %SBW session key file for this %SBW session.
* The pathname is the result of appending the file name "key" to the
* directory returned by Config::getRuntimeFilesDirectory().
*
* @return a string, the pathname of the %SBW session key file
**/
std::string Config::getSessionKeyFile()
{
	std::string file = getRuntimeFilesDirectory();
	file += SBWOS::DirectorySeparator();
	file += "key";
	TRACE("Using session key file = '" << file << "'");
	return file;
}

#ifdef WIN32
/**
* Returns the pathname of the Windows system directory for the
* version of Microsoft Windows running on the current computer.
* This directory is determined using Windows API calls.
*
* @return a string representing the pathname to the Windows system
* diretory for this host computer.
**/
std::string Config::getWindowsSystemDirectory()
{
	char *dir = (char*)malloc(sizeof(char)*MAX_PATH); 
	memset(dir, 0, sizeof(char)*MAX_PATH);
	
	WIN32_FIND_DATA fd;

	GetSystemDirectory(dir, MAX_PATH);
	FindFirstFile(dir, &fd);
	strcpy(strrchr(dir, '\\') + 1, fd.cFileName);
	std::string sysDir(dir);

	free(dir);

	TRACE("The Windows system directory appears to be " << sysDir);
	return sysDir;
}
#endif

/**
* Parses the "config.properties" file and returns the value
* of the "sbw.home" entry.
**/
std::string Config::readSBWHomeFromPropFile(std::string dir)
{
	std::string file = dir;
	file += SBWOS::DirectorySeparator();
	file += SBW_PROPERTIES_FILE;

	FILE *fp;
	if ((fp = fopen(file.c_str(), "r")) != NULL)
	{
		TRACE("Reading sbw.home from " << file);

		size_t taglength = strlen(SBW_HOME_FIELD);
		char line[MAX_PATH];

		while (! feof(fp) && (fgets(line, MAX_PATH, fp) != NULL))
			if (strncmp(line, SBW_HOME_FIELD, taglength) == 0)
			{
				char *newline = strchr(line, '\n');
				size_t end = newline - line;

				if ((size_t)(line + taglength) != end)
				{                           
					char buf[MAX_PATH];
					strncpy(buf, line + taglength, end - taglength);
					buf[end - taglength] = (char)NULL;
					fclose(fp);
					std::string sbwHome(buf);
					TRACE("sbw.home = " << sbwHome);
					return sbwHome;
				}
				else
				{
					fclose(fp);
					TRACE("Value of sbw.home in config.properties is empty");
					throw new SBWRawException(
						"Empty sbw.home entry in config.properties file",
						"config.properties file exists but contains an empty"
						" value for sbw.home. This indicates that the SBW"
						" installation is corrupted.  You may be able to fix"
						" the installation by setting the environment variable"
						" SBW_HOME to the path to the parent of the folder"
						" containing the broker executable.");
				}
			}

			fclose(fp);
			TRACE("No sbw.home entry in config.properties file");
			throw new SBWRawException(
				"No sbw.home entry in config.properties file",
				"config.properties file exists but does not contain a"
				" value for sbw.home. This indicates that the SBW"
				" installation is corrupted.  You may be able to fix"
				" the installation by setting the environment variable"
				" SBW_HOME to the path to the parent of the folder"
				" containing the broker executable.");
	}
	else
		TRACE("Unable to open " << file.c_str());

	return "";
}

char* Config::getSBWHomeC() { return strdup(getSBWHome().c_str()); }
char* Config::getSBWUserDirC() { return strdup(getSBWUserDir().c_str()); } 
char* Config::getLibDirectoryC() { return strdup(getLibDirectory().c_str()); }
char* Config::getRuntimeFilesDirectoryC() { return strdup(getRuntimeFilesDirectory().c_str()); }
char* Config::getSessionKeyFileC() { return strdup(getSessionKeyFile().c_str());}
#ifdef WIN32
char* Config::getWindowsSystemDirectoryC() { return strdup(getWindowsSystemDirectory().c_str());}
#endif


