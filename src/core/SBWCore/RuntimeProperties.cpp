/**
* @file RuntimeProperties.cpp
* @brief implementation of RuntimeProperties class
* @author SBW Development Group <sysbio-team@caltech.edu>
*
* Organization: Caltech ERATO Kitano Systems Biology Project
*
* Created: @date 2002-05-20
* $Id: RuntimeProperties.cpp,v 1.3 2008/12/12 17:52:23 fbergmann Exp $
* $Source: /cvsroot/sbw/core/SBWCore/RuntimeProperties.cpp,v $
*/

/*
** Copyright 2001 California Institute of Technology and
** Japan Science and Technology Corporation.
** 
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 2.1 of the License, or
** any later version.
** 
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
** MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software and
** documentation provided hereunder is on an "as is" basis, and the
** California Institute of Technology and Japan Science and Technology
** Corporation have no obligations to provide maintenance, support,
** updates, enhancements or modifications.  In no event shall the
** California Institute of Technology or the Japan Science and Technology
** Corporation be liable to any party for direct, indirect, special,
** incidental or consequential damages, including lost profits, arising
** out of the use of this software and its documentation, even if the
** California Institute of Technology and/or Japan Science and Technology
** Corporation have been advised of the possibility of such damage.  See
** the GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with this library; if not, write to the Free Software Foundation,
** Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
**
** The original code contained here was initially developed by:
**
**     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri
**     The Systems Biology Workbench Development Group
**     ERATO Kitano Systems Biology Project
**     Control and Dynamical Systems, MC 107-81
**     California Institute of Technology
**     Pasadena, CA, 91125, USA
**
**     http://www.cds.caltech.edu/erato
**     mailto:sysbio-team@caltech.edu
**
** Contributor(s):
**
*/

#include "stdafx.h"
#include "SBWConfig.h"
#include "SBWRawException.h"
#include "SBWCommunicationException.h"
#include "RuntimeProperties.h"
#include "SBWModuleRPC.h"
#include <stdlib.h> 

#ifdef WIN32
#include <io.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#define LINE_BUFFER_SIZE 50
#define MODULE_PORT_FIELD "sbw.module.port="
#define BROKER_PORT_FIELD "sbw.broker.port="


using namespace SystemsBiologyWorkbench;

std::string RuntimeProperties::getPropertiesFile()
{
	std::string dir = Config::getRuntimeFilesDirectory();
	std::string file(dir);
	file += SBWOS::DirectorySeparator();
	file += "run.properties";

	return file ;
}

int RuntimeProperties::getSBWModulePort()
{
	std::string file = getPropertiesFile();

	if (!SBWModuleRPC::fileExists(file.c_str())) return -1;

	TRACE("Loading runtime properties from " << file.c_str());

	FILE *fp;
	if ((fp = fopen(file.c_str(), "r")) != NULL)
	{
		size_t taglength = strlen(MODULE_PORT_FIELD);
		char line[LINE_BUFFER_SIZE];

		while (! feof(fp) && (fgets(line, LINE_BUFFER_SIZE, fp) != NULL))
		{
			if (strncmp(line, MODULE_PORT_FIELD, taglength) == 0)
			{
				// Why doesn't fscanf work like I expect it to?
				// fscanf(fp, "sbw.module.port=%d", &port)

				char *newline = strchr(line, '\n');
				size_t end = newline - line;

				// If the broker isn't running, the value will be empty

				if ((size_t)(line + taglength) != end)
				{			    
					char buf[10];
					memset (buf, 0, 10 * sizeof(char));
					strncpy(buf, line + taglength, end - taglength);
					fclose(fp);
					return atoi(buf);
				}
				else
				{
					TRACE("Value of sbw.module.port is empty");
					fclose(fp);
					return -1;
				}
			}
		}

		fclose(fp);
		TRACE("Unable to read sbw.module.port from runtime file");
		return -1;
	}
	else
		TRACE("Unable to open " << file.c_str());

	TRACE("Cannot load sbw.module.port");
	return -1;
}
int RuntimeProperties::getSBWBrokerPort()
{
	std::string file = getPropertiesFile();
	if (!SBWModuleRPC::fileExists(file.c_str())) return -1;

	TRACE("Loading runtime properties from " << file.c_str());

	FILE *fp;
	if ((fp = fopen(file.c_str(), "r")) != NULL)
	{
		size_t taglength = strlen(BROKER_PORT_FIELD);
		char line[LINE_BUFFER_SIZE];

		while (! feof(fp) && (fgets(line, LINE_BUFFER_SIZE, fp) != NULL))
		{
			if (strncmp(line, BROKER_PORT_FIELD, taglength) == 0)
			{
				// Why doesn't fscanf work like I expect it to?
				// fscanf(fp, "sbw.module.port=%d", &port)

				char *newline = strchr(line, '\n');
				size_t end = newline - line;

				// If the broker isn't running, the value will be empty

				if ((size_t)(line + taglength) != end)
				{			    
					char buf[10];
					memset (buf, 0, 10 * sizeof(char));
					strncpy(buf, line + taglength, end - taglength);
					fclose(fp);
					return atoi(buf);
				}
				else
				{
					TRACE("Value of sbw.broker.port is empty");
					fclose(fp);
					return -1;
				}
			}
		}

		fclose(fp);
		TRACE("Unable to read sbw.broker.port from runtime file");
		return -1;

	}
	else
		TRACE("Unable to open " << file.c_str());

	TRACE("Cannot load sbw.broker.port");
	return -1;
}
