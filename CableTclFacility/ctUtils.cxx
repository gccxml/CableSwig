/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctUtils.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctUtils.h"

#include <string.h>

namespace _cable_tcl_
{

/**
 * Check whether the given object's type pointer is NULL.
 */
bool TclObjectTypeIsNULL(Tcl_Obj* o)
{
  return ((o)->typePtr == NULL);
}


/**
 * Check whether the given object's type is "boolean".
 */
bool TclObjectTypeIsBoolean(Tcl_Obj* o)
{
  return (!TclObjectTypeIsNULL(o)
          && (strcmp("boolean", (o)->typePtr->name)==0));
}


/**
 * Check whether the given object's type is "int".
 */
bool TclObjectTypeIsInt(Tcl_Obj* o)
{
  return (!TclObjectTypeIsNULL(o)
          && (strcmp("int", (o)->typePtr->name)==0));
}
  

/**
 * Check whether the given object's type is "long".
 */
bool TclObjectTypeIsLong(Tcl_Obj* o)
{
  return (!TclObjectTypeIsNULL(o)
          && (strcmp("long", (o)->typePtr->name)==0));
}
  

/**
 * Check whether the given object's type is "double".
 */
bool TclObjectTypeIsDouble(Tcl_Obj* o)
{
  return (!TclObjectTypeIsNULL(o)
          && (strcmp("double", (o)->typePtr->name)==0));
}


/**
 * Check whether the given object's type is "string".
 */
bool TclObjectTypeIsString(Tcl_Obj* o)
{
  return (!TclObjectTypeIsNULL(o)
          && (strcmp("string", (o)->typePtr->name)==0));
}


/**
 * Check whether the given object is a command name object.
 */
bool TclObjectTypeIsCmdName(Tcl_Obj* o)
{
  return (!TclObjectTypeIsNULL(o)
          && (strcmp("cmdName", (o)->typePtr->name)==0));
}


} // namespace _cable_tcl_
