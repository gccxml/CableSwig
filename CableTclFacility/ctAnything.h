/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctAnything.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctAnything_h
#define _ctAnything_h

namespace _cable_tcl_
{


/**
 * The general argument to a conversion function.
 * Casts between data and function pointers are not allowed.  Therfore,
 * we need to represent a general object as a class type.  Since only one
 * of the pointers is needed at a time, it can be a union.
 */
union Anything
{
  typedef void* ObjectType;
  typedef void (*FunctionType)();
  ObjectType   object;
  FunctionType function;
  
  Anything() {}
  Anything(const void* obj): object(const_cast<void*>(obj)) {}
  Anything(FunctionType func): function(func) {}
};


/**
 * The general type of a conversion function.  A real conversion function
 * will return something, but they are all cast to this for storage
 * in the table.
 */
typedef void (*ConversionFunction)(Anything);


} // namespace _cable_tcl_

#endif
