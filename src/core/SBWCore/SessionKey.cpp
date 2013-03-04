/**
 * @file SessionKey.cpp
 * @brief implementation of SessionKey class
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 2002-05-20
 * $Id: SessionKey.cpp,v 1.3 2007/09/03 01:04:01 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SessionKey.cpp,v $
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

// Config.cpp: implementation of the Config class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SBWConfig.h"
#include "SBWRawException.h"
#include "SBWCommunicationException.h"
#include "SessionKey.h"

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
