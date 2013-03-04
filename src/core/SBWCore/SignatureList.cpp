/**
 * @file SignatureList.cpp
 * @brief represents a list type inside a signature
 * @author SBW Development Group <sysbio-team@caltech.edu>
 *
 * Organization: Caltech ERATO Kitano Systems Biology Project
 *
 * Created: @date 18th June 2001
 * $Id: SignatureList.cpp,v 1.1 2005/12/15 01:06:44 fbergmann Exp $
 * $Source: /cvsroot/sbw/core/SBWCore/SignatureList.cpp,v $
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
// sbwSignatureList.cpp: implementation of the sbwSignatureList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#ifndef WIN32
#include <stdarg.h>
//#endif

#include "SignatureList.h"

#include "SignatureListContents.h"
#include "Signature.h"
#include "ImplDataBlockWriter.h"
#include "ImplDataBlockReader.h"

using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/// deallocates the content list
sbwSignatureList::~sbwSignatureList()
{
	delete contents;
}

/**
 * constructs a list type object given a string pointing to the start of a list type definition.
 * @param signature string prefixed with list type definition.  The string pointer is chnaged to point
 *        to the first character after the type definition
 *
 */
sbwSignatureList::sbwSignatureList(const char *&signature)
{
	sbwSignature::consume("{", signature);
	contents = new SignatureListContents(signature);
	sbwSignature::consume("}", signature);
}

/**
 * For the C API :stores a sbwDataBlockWriter object in a datablock.
 * @param writer datablock in which list will be stored.
 * @param ap variable argument list containing a pointer to a sbwDataBlockerWriter which will be stored in
 *			 the data block as a list object.  The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::write(DataBlockWriter writer, va_list &ap)
{
	sbwDataBlockWriter *list = va_arg(ap, sbwDataBlockWriter *);

	writer << list ;
}

/**
 * For the C API :reads a list from a datablock.  The list is stored in a new sbwDataBlockWriter object 
 * @param reader datablock from which list will be read.
 * @param ap variable argument list containing a pointer to a pointer to a sbwDataBlockerWriter which will be set
 *           to a new sbwDataBlockWriter object allocated on the C Heap.  
 *			 The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::read(DataBlockReader reader, va_list &ap)
{
	sbwDataBlockReader **list = va_arg(ap, sbwDataBlockReader **);

	reader.getForC(*list) ;
}

/**
 * For the C API :stores an array of sbwDataBlockWriter objects in a datablock as an array of lists
 * @param writer datablock in which array will be stored.
 * @param ap variable argument list containing a pointer to a sbwDataBlockerWriter array which will be stored in
 *			 the data block as a list array.  The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::write1DArray(DataBlockWriter writer, Integer size, va_list &ap)
{
	sbwDataBlockWriter **x = va_arg(ap, sbwDataBlockWriter **);

	writer.add(size, x);
}

/**
 * For the C API :reads a list array from a datablock.  The array is stored in a new sbwDataBlockWriter * array 
 * @param reader datablock from which array will be read.
 * @param ap variable argument list containing a pointer to a pointer to a sbwDataBlockerWriter array which will be set
 *           to a new sbwDataBlockWriter array allocated on the C Heap.  
 *			 The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::read1DArray(DataBlockReader reader, Integer &size, va_list &ap)
{
	sbwDataBlockReader ***x = va_arg(ap, sbwDataBlockReader ***);

	reader.getForC(size, *x);
}


/**
 * For the C API :stores an array of sbwDataBlockWriter objects in a datablock as an array of lists
 * @param writer datablock in which array will be stored.
 * @param ap variable argument list containing a pointer to a sbwDataBlockerWriter array which will be stored in
 *			 the data block as a list array.  The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::write2DArray(DataBlockWriter writer, Integer xsize, Integer ysize, va_list &ap)
{
	sbwDataBlockWriter ***x = va_arg(ap, sbwDataBlockWriter ***);

	writer.add(xsize, ysize, x);
}

/**
 * For the C API :reads a list 2D array from a datablock.  The array is stored in a new sbwDataBlockWriter ** array 
 * @param reader datablock from which array will be read.
 * @param ap variable argument list containing a pointer to a pointer to a sbwDataBlockerWriter * array which will be
 *           set to a new sbwDataBlockWriter * array allocated on the C Heap.  
 *			 The argument pointer is changed to point to the next argument.
 */
void sbwSignatureList::read2DArray(DataBlockReader reader, Integer &xsize, Integer &ysize, va_list &ap)
{
	sbwDataBlockReader ****x = va_arg(ap, sbwDataBlockReader ****);

	reader.getForC(xsize, ysize, *x);
}

/**
 * destroy this object returning the contents as a SignatureListContents object.
 * @return the contents as a SignatureListContents object.  
 *         The returned object should deleted using the delete operator.
 */
SignatureListContents *sbwSignatureList::discardShell()
{
	SignatureListContents *result = contents ;
	contents = NULL ;
	delete this ;

	return result ;
}

/**
 * return the contents of this list type definition.
 * @return the contents of this list type definition.
 *         The returned vector object should deleted using the delete operator.
 */
std::vector<SignatureElement> *sbwSignatureList::getContents()
{
	return contents->copyVector();
}

/**
 * returns whether this type is equal to another type.
 * This implementation checks to see if the list contents match if neither are empty.
 * @param other other type.
 * @return whether this type is equal to another type.
 */
bool sbwSignatureList::match(sbwSignatureType *other)
{
	if (other->getType() != ListType)
		return false;

	sbwSignatureList *otherList = (sbwSignatureList *)other ;

	return otherList->contents->size() == 0 || contents->size() == 0 || contents->match(*(otherList->contents));
}
