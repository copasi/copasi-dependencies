/**
 * @file DataBlockReader.h
 * @brief decodes datablocks - inline methods
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: DataBlockReader.h,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/include/SBW/DataBlockReader.h,v $
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
// DataBlockReader.h: interface for the DataBlockReader class.
//
//////////////////////////////////////////////////////////////////////

/// include loop block - automatically created by VC++
#if !defined(AFX_DATABLOCKREADER_H__8EF37D30_0142_4A65_A9B7_629A9AC84853__INCLUDED_)
#define AFX_DATABLOCKREADER_H__8EF37D30_0142_4A65_A9B7_629A9AC84853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataBlockReaderClass.h"
#include "ImplDataBlockReader.h"

/**
 * ignore the next object in the DataBlockReader.
 */
inline void SystemsBiologyWorkbench::DataBlockReader::skipObject() { p->skipObject(); }

/**
 * returns the type of the next object in the DataBlockReader.
 * @return the type of the next object in the DataBlockReader
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::DataBlockReader::getNextType()
{ return p->getNextType(); }

/**
 * returns the type of objects contained in the next array object in the DataBlockReader.
 * @throws an SBWException * if the next object is not an array
 * @return the type of objects contained in the next array object in the DataBlockReader
 */
inline SystemsBiologyWorkbench::DataBlockType SystemsBiologyWorkbench::DataBlockReader::getNextArrayType()
{ return p->getNextArrayType(); }

/**
 * returns the number of dimensions of the next array object in the DataBlockReader.
 * @throws an SBWException * if the next object is not an array
 * @return the number of dimensions of the next array object in the DataBlockReader
 */
inline SystemsBiologyWorkbench::Integer SystemsBiologyWorkbench::DataBlockReader::getNextArrayDimensions()
{ return p->getNextArrayDimensions(); }

#endif // !defined(AFX_DATABLOCKREADER_H__8EF37D30_0142_4A65_A9B7_629A9AC84853__INCLUDED_)
