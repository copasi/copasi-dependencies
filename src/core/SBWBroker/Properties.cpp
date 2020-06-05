#include "Properties.h"
#include "BrokerDefines.h"
#include "Sys.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>

#include "SBW/SBW.h"
#include "SBW/SBWConfig.h"
#include "SBW/SBWApplicationException.h"

#ifdef WIN32
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif


using namespace std;
using namespace SystemsBiologyWorkbench::Broker;

Properties* Properties::oInstance = NULL;

void Properties::deleteInstance()
{
  if (oInstance == NULL) 
    return;

  delete oInstance;
  oInstance = NULL;
}

Properties* Properties::getInstance() 
{
	if (oInstance == NULL)
  {
		oInstance = new Properties();
    std::atexit(&deleteInstance);
  }
	return oInstance;
}
bool Properties::hasProperty(std::string sName)
{	
	Properties *oProp = getInstance();
	map< string, string > ::iterator oIterator;
	oIterator = oProp->m_oProperties.find( sName );
	if (oIterator != oProp->m_oProperties.end())
		return true;
	return false;
}
void Properties::changeProperty(std::string sName, std::string sValue)
{
	Properties *oProp = getInstance();
	map< string, string > ::iterator oIterator;
	oIterator = oProp->m_oProperties.find( sName );
	if (oIterator != oProp->m_oProperties.end())
	{
		oProp->m_oProperties.erase(oIterator);
		oProp->m_oProperties.insert( StringString(sName, sValue) );
	}
}
void Properties::setProperty(std::string sName, std::string sValue)
{
	Properties *oProp = getInstance();
	map< string, string > ::iterator oIterator;
	oIterator = oProp->m_oProperties.find( sName );
	if (oIterator != oProp->m_oProperties.end())
	{
		oProp->m_oProperties.erase(oIterator);
	}
	oProp->m_oProperties.insert( StringString(sName, sValue) );
}
void Properties::setProperty(std::string sName, std::string sValue, std::string sDefault)
{
	if (sValue == "") 
		setProperty(sName,sDefault);
	else
		setProperty(sName,sValue);
}
std::string Properties::getProperty(std::string sName)
{
	Properties *oProp = getInstance();
	map< string, string > ::iterator oIterator;
	oIterator = oProp->m_oProperties.find( sName );
	if (oIterator != oProp->m_oProperties.end())
	{
		return oIterator->second;
	}
	return "";
}
std::string Properties::getProperty(std::string sName, std::string sDefault) 
{
	std::string sResult = getProperty(sName);
	if (sResult == "")
	{
		setProperty(sName, sDefault);
		return sDefault;
	}
	return sResult;
}
int	Properties::getPropertyAsInt(std::string sName)
{
	return toInt(getProperty(sName));
}
double Properties::getPropertyAsDouble(std::string sName)
{
	return toDouble(getProperty(sName));
}
void Properties::removeProperty(std::string sName)
{
	Properties *oProp = getInstance();
	map< string, string > ::iterator oIterator;
	oIterator = oProp->m_oProperties.find( sName );
	if (oIterator != oProp->m_oProperties.end())
	{
		oProp->m_oProperties.erase(oIterator);
	}		
}
std::string Properties::toString(int nValue)
{
	std::stringstream sTemp;
	sTemp << nValue;
	return sTemp.str();
}
std::string Properties::toString(double dValue)
{
	std::stringstream sTemp;
	sTemp << dValue;
	return sTemp.str();
}
int Properties::toInt (std::string sValue)
{
	stringstream sTemp (sValue);
	int nResult = -1;
	sTemp >> nResult;
	return nResult;
}
double Properties::toDouble (std::string sValue)
{
	stringstream sTemp (sValue);
	double dResult = -1.0;
	sTemp >> dResult;
	return dResult;
}

bool Properties::assertRuntimeProperty(std::string sPropertyName, std::string sExpression)
{
	if(sExpression == getProperty(sPropertyName))
		return true;
	return false;	
}

void Properties::save ()
{

	std::string sFilename = Config::getRuntimeFilesDirectory() + SBWOS::DirectorySeparator() + "run.properties";
	ofstream oFile (sFilename.c_str());
	if (oFile.is_open()) 
	{
		Properties *oProp = getInstance();
		map< string, string >::iterator oIterator;

		for ( oIterator = oProp->m_oProperties.begin(); oIterator != oProp->m_oProperties.end(); oIterator++) 
		{
			oFile << oIterator->first << "=" << oIterator->second << "\n";
		}
		oFile.flush();
		oFile.close();
	}
	else
	{
		Sys::createDirectory(Config::getRuntimeFilesDirectory() + SBWOS::DirectorySeparator());
		ofstream oNewFile (sFilename.c_str());
		if (oNewFile.is_open()) 
		{

			Properties *oProp = getInstance();
			map< string, string >::iterator oIterator;

			for ( oIterator = oProp->m_oProperties.begin(); oIterator != oProp->m_oProperties.end(); oIterator++) 
			{
				oNewFile << oIterator->first << "=" << oIterator->second << "\n";
			}

			oNewFile.flush();
			oNewFile.close();
		}
		else 
		{
			TRACE("couldn't save runtime properties ...");
		}
	}
}

