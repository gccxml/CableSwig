/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctTclCxxObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctTclCxxObject_h
#define _ctTclCxxObject_h

#include "ctUtils.h"
#include "ctCxxObject.h"

namespace _cable_tcl_
{

class WrapperFacility;

/**
 * Implement the Tcl object type "CxxObject", which refers to an
 * instance of the CxxObject class.  This will be used to pass Tcl
 * command arguments and results that refer to C++ object.
 */
class _cable_tcl_EXPORT TclCxxObject
{
public:  
  typedef TclCxxObject Self;
  static bool TclObjTypeMatches(Tcl_Obj*);
  static Tcl_Obj* NewObj(CxxObject*);
  static void SetObj(Tcl_Obj*, CxxObject*);
  static int GetFromObj(Tcl_Interp*, Tcl_Obj*, CxxObject**);
protected:
  static void SetTclObj(Tcl_Obj*, CxxObject*);
  static CxxObject* Cast(Tcl_Obj*);
  static void DeleteOldRepresentation(Tcl_Obj*);

  static void FreeInternalRep(Tcl_Obj*);
  static void DupInternalRep(Tcl_Obj*, Tcl_Obj*);
  static void UpdateString(Tcl_Obj*);
  static int SetFromAny(Tcl_Interp*, Tcl_Obj*);

  static Tcl_ObjType TclObjTypeStruct;
  
private:
  static void ClassInitialize();
  friend class WrapperFacility;
};  

// Standard Tcl interface for its object types.
// This one is for the Pointer object.
_cable_tcl_EXPORT int Tcl_GetCxxObjectFromObj(Tcl_Interp*, Tcl_Obj*, CxxObject**);
_cable_tcl_EXPORT void Tcl_SetCxxObjectObj(Tcl_Obj*, CxxObject*);
_cable_tcl_EXPORT Tcl_Obj* Tcl_NewCxxObjectObj(CxxObject*);
  
// A couple useful utility functions for the type.
_cable_tcl_EXPORT bool TclObjectTypeIsCxxObject(Tcl_Obj*);
_cable_tcl_EXPORT bool StringRepIsCxxObject(const String&);
  
} // namespace _cable_tcl_

#endif
