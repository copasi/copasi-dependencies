/**
 * @file DataBlockReader.cpp
 * @brief decodes data blocks
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: DataBlockReader.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/DataBlockReader.cpp,v $
 */

/*
**
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

static const char rcsid[] = "$Id: DataBlockReader.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $";

// DataBlockReader.cpp: implementation of the DataBlockReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataBlockReader.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * constructs an empty reader.
 * Method creates an null handle to a sbwDataBlockReader
 */
SBW_API DataBlockReader::DataBlockReader() : SBWHandle<sbwDataBlockReader>()
{
}

/**
 * shares data between 2 or more DataBlockReader objects.
 * The constructed DataBlockReader is simply a handle to the sbwDataBlockReader refered to by x
 * The reference count on the sbwDataBlockReader is incremented
 */
SBW_API DataBlockReader::DataBlockReader(const DataBlockReader &x) : SBWHandle<sbwDataBlockReader>(x)
{
}

/**
 * shares data between a new DataBlockReader object and a sbwDataBlockReader.
 * The constructed DataBlockReader is simply a handle to the sbwDataBlockReader.
 * The reference count on the sbwDataBlockReader is incremented.
 */
SBW_API DataBlockReader::DataBlockReader(sbwDataBlockReader *x) : SBWHandle<sbwDataBlockReader>(x)
{
}

/** 
 * shares or copies data between a new DataBlockReader object and raw char data.
 * The constructed DataBlockReader is a handle to a new sbwDataBlockReader
 * @param x raw data with which to construct DatablockReader object (not a zero terminated text string)
 * @param length size of x
 * @param makeCopy if true then the sbwDataBlockReader creates a copy of x otherwise takes ownership of x
 */
SBW_API DataBlockReader::DataBlockReader(unsigned char *x, Integer length, bool makeCopy)
: SBWHandle<sbwDataBlockReader>(new sbwDataBlockReader(x, length, makeCopy))
{
}
