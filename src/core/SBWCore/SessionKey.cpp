/**
 * @file SessionKey.cpp
 * @brief implementation of SessionKey class
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


// Config.cpp: implementation of the Config class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SBWConfig.h"
#include "SBWRawException.h"
#include "SBWCommunicationException.h"
#include "SessionKey.h"

#include <SBW/portableOS.h>

#ifdef WIN32
#include <io.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

using namespace SystemsBiologyWorkbench;

#define SESSION_KEY_LENGTH 256
std::string SessionKey::key;
char* SessionKey::getKeyC()
{
	return strdup(getKey().c_str());
}
std::string SessionKey::getKey()
{
        if (SessionKey::key.empty())
	        SessionKey::loadKey();
        return SessionKey::key;
}

void SessionKey::loadKey()
{
        std::string keyFile = Config::getSessionKeyFile();

#ifdef LINUX
        struct stat statbuff;
#endif
#ifdef WIN32
#if  _MSC_VER >= 1400
	if (_access(keyFile.c_str(), 0 ) == 0)
#else
	if (access(keyFile.c_str(), 0 ) == 0)
#endif
#else
	if (stat(keyFile.c_str(), &statbuff) == 0)
#endif
	{
	    TRACE("Loading session key from " << keyFile.c_str());
	    
	    FILE *fp;
	    if ((fp = fopen(keyFile.c_str(), "r")) != NULL)
	    {
		char buffer[2*SESSION_KEY_LENGTH + 1];

		fgets(buffer, 2*SESSION_KEY_LENGTH + 1, fp);
		fclose(fp);
		std::string theKey(buffer);
		SessionKey::key = theKey;
		return;
	    }
	}

	TRACE("Cannot load session key from " << keyFile.c_str());
}
