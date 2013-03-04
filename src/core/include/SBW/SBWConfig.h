/**
 * @file          SBWConfig.h
 * @brief         Access static and run-time SBW configuration information.
 * @author        SBW Development Group <sysbio-team@caltech.edu>
 * @date Created  18th June 2001
 * $Id: SBWConfig.h,v 1.2 2007/09/03 01:04:01 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/SBWConfig.h,v $
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

/// MSVC generated include loop block
#if !defined(AFX_CONFIG_H__E5A3E453_D308_45EE_BC8E_637BC9123C1E__INCLUDED_)
#define AFX_CONFIG_H__E5A3E453_D308_45EE_BC8E_637BC9123C1E__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sbwdefs.h"

#include <string>

namespace SystemsBiologyWorkbench
{

/**
 * This class implements methods for accessing static and run-time
 * %SBW configuration information.  The information includes the
 * %SBW installation directory, the user runtime directory,
 * and the %SBW session key file.  This class is used internally by
 * %SBW and is not intended to be used by user modules.
 **/
class Config  
{
public:
    SBW_API static std::string getSBWHome();
	SBW_API static char* getSBWHomeC();
    SBW_API static std::string getSBWUserDir();
	SBW_API static char* getSBWUserDirC();
    SBW_API static std::string getLibDirectory();
	SBW_API static char* getLibDirectoryC();
    SBW_API static std::string getRuntimeFilesDirectory();
	SBW_API static char* getRuntimeFilesDirectoryC();
    SBW_API static std::string getSessionKeyFile();
	SBW_API static char* getSessionKeyFileC();
#ifdef WIN32
    SBW_API static std::string getWindowsSystemDirectory();
	SBW_API static char* getWindowsSystemDirectoryC();
#endif

private:
    SBW_API static std::string readSBWHomeFromPropFile(std::string dir);
};

} // SystemsBiologyWorkbench

#endif // !defined(AFX_CONFIG_H__E5A3E453_D308_45EE_BC8E_637BC9123C1E__INCLUDED_)
