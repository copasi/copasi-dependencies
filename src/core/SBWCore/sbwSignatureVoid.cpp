/**
 * @file sbwSignatureVoid.cpp
 * @brief Implementation of the sbwSignatureVoid class
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
#include "sbwSignatureVoid.h"
#include "sbwSignatureByte.h"


using namespace SystemsBiologyWorkbench ;

/// placeholder - does nothing
DataBlockType sbwSignatureVoid::getType() { return VoidType; }

void sbwSignatureVoid::write(DataBlockWriter /*d*/, va_list &/*a*/)
{
}

/// placeholder - does nothing
void sbwSignatureVoid::read(DataBlockReader /*d*/, va_list &/*a*/)
{
}

/// placeholder - does nothing
void sbwSignatureVoid::write1DArray(DataBlockWriter /*d*/, Integer /*size*/, va_list &/*a*/)
{
}

/// placeholder - does nothing
void sbwSignatureVoid::read1DArray(DataBlockReader /*d*/, Integer &/*size*/, va_list &/*a*/)
{
}

/// placeholder - does nothing
void sbwSignatureVoid::write2DArray(DataBlockWriter /*d*/, Integer /*xsize*/, Integer /*ysize*/, va_list &/*a*/)
{
}

/// placeholder - does nothing
void sbwSignatureVoid::read2DArray(DataBlockReader /*d*/, Integer &/*xsize*/, Integer &/*ysize*/, va_list &/*a*/)
{
}



DataBlockType sbwSignatureByte::getType() { return ByteType; }

void sbwSignatureByte::write(DataBlockWriter writer, va_list& ap)
{
  // The C++ C99 standard requires that "the type used in va_arg is
  // supposed to match the actual type **after default promotions**."

  unsigned char x = (unsigned char) va_arg(ap, int /* unsigned char */);

  writer << x ;
}

void sbwSignatureByte::read(DataBlockReader reader, va_list& ap)
{
  unsigned char *x = (unsigned char *) va_arg(ap, unsigned char *);

  reader.getForC(*x) ;
}

void sbwSignatureByte::write1DArray(DataBlockWriter writer, Integer size, va_list& ap)
{
  unsigned char *x = (unsigned char *) va_arg(ap, unsigned char *);

  writer.add(size, x);
}

void sbwSignatureByte::read1DArray(DataBlockReader reader, Integer& size, va_list& ap)
{
  unsigned char **x = (unsigned char **)va_arg(ap, unsigned char **);

  reader.getForC(size, *x);
}

void sbwSignatureByte::write2DArray(DataBlockWriter writer, Integer xsize, Integer ysize, va_list& ap)
{
  unsigned char **x = (unsigned char **)va_arg(ap, unsigned char **);

  writer.add(xsize, ysize, x);
}

void sbwSignatureByte::read2DArray(DataBlockReader reader, Integer& xsize, Integer& ysize, va_list& ap)
{
  unsigned char ***x = (unsigned char ***)va_arg(ap, unsigned char ***);

  reader.getForC(xsize, ysize, *x);
}

