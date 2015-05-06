/**
 * @file RuntimeProperties.cpp
 * @brief implementation of RuntimeProperties class
 * 
 * This file is part of SBW.  Please visit http://sbw.sf.org for more
 * information about SBW, and the latest version of libSBW.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the New BSD license.
 *
 * Copyright (c) 2010-2014, Frank T. Bergmann and 
 *                          University of Washington
 * Copyright (c) 2008-2010, University of Washington and 
 *                          Keck Graduate Institute.
 * Copyright (c) 2005-2008, Keck Graduate Institute.
 * Copyright (c) 2001-2004, California Institute of Technology and
 *               Japan Science and Technology Corporation.
 * 
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are 
 * met: 
 * 
 * 1. Redistributions of source code must retain the above 
 *    copyright notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 
 * 3. Neither the name of the copyright holder nor the names of its 
 *    contributors may be used to endorse or promote products derived from 
 *    this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * The original code contained here was initially developed by:
 *
 *     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri
 *     The Systems Biology Workbench Development Group
 *     ERATO Kitano Systems Biology Project
 *     Control and Dynamical Systems, MC 107-81
 *     California Institute of Technology
 *     Pasadena, CA, 91125, USA
 *
 *
 * Contributor(s):
 *
 */


#include "stdafx.h"
#include "SBWConfig.h"
#include "SBWRawException.h"
#include "SBWCommunicationException.h"
#include "RuntimeProperties.h"
#include "SBWModuleRPC.h"
#include <stdlib.h> 

#include <SBW/portableOS.h>

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
