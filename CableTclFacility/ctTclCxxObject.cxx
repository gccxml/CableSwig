/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctTclCxxObject.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctTclCxxObject.h"
#include "ctWrapperFacility.h"
#include "ctTypeInfo.h"

#include <strstream>
#include <string.h>

namespace _cable_tcl_
{


/**
 * Given a Tcl_Obj, test whether its internal representation type
 * matches that implemented by this class.
 */
bool TclCxxObject::TclObjTypeMatches(Tcl_Obj* objPtr)
{
  return (objPtr->typePtr == &Self::TclObjTypeStruct);
}


/**
 * Tcl object interface:
 * Create a new Tcl object which represents the given object.
 */
Tcl_Obj* TclCxxObject::NewObj(CxxObject* obj)
{
  Tcl_Obj* objPtr = Tcl_NewObj();
  _cable_tcl_DEBUG_OUTPUT(obj->GetWrapperFacility(),
                          "Creating Tcl_Obj at " << objPtr
                          << " for CxxObject at " << obj << std::endl);
  Self::SetTclObj(objPtr, obj);
  return objPtr;
}


/**
 * Tcl object interface:
 * Set a Tcl object to represent a given object.
 */
void TclCxxObject::SetObj(Tcl_Obj* objPtr, CxxObject* obj)
{
  _cable_tcl_DEBUG_OUTPUT(obj->GetWrapperFacility(),
                          "Setting Tcl_Obj at " << objPtr
                          << " to CxxObject at " << obj << std::endl);
  // If this is a shared object, we are not supposed to be setting it.
  if(Tcl_IsShared(objPtr))
    {
    panic("Tcl_SetCxxObjectObj called with shared object");
    }
  Self::DeleteOldRepresentation(objPtr);
  Self::SetTclObj(objPtr, obj);
}


/**
 * Tcl object interface:
 * Get a pointer to CxxObject from the given Tcl object.  Convert
 * the Tcl object's representation if necessary.
 */
int TclCxxObject::GetFromObj(Tcl_Interp* interp, Tcl_Obj* objPtr,
                             CxxObject** obj)
{
  if(!Self::TclObjTypeMatches(objPtr))
    {
    int error;
    if((error = Self::SetFromAny(interp, objPtr)) != TCL_OK) { return error; }
    }
  if(obj) { *obj = Self::Cast(objPtr); }
  return TCL_OK;
}


// Protected implementation members.

/**
 * Sets objPtr's internal representation to point at the given object.
 */
void TclCxxObject::SetTclObj(Tcl_Obj* objPtr, CxxObject* obj)
{
  obj->Increment();
  objPtr->internalRep.otherValuePtr = static_cast<VOID*>(obj);
  objPtr->typePtr = &Self::TclObjTypeStruct;
  Tcl_InvalidateStringRep(objPtr);
}


/**
 * Cast the objPtr's internal representation pointer to a CxxObject
 * pointer.
 */
CxxObject* TclCxxObject::Cast(Tcl_Obj* objPtr)
{
  return static_cast<CxxObject*>(objPtr->internalRep.otherValuePtr);
}


/**
 * Given a Tcl object, delete any existing internal representation.
 */
void TclCxxObject::DeleteOldRepresentation(Tcl_Obj* objPtr)
{
  Tcl_ObjType* oldTypePtr = objPtr->typePtr;
  if((oldTypePtr != NULL) && (oldTypePtr->freeIntRepProc != NULL))
    {
    oldTypePtr->freeIntRepProc(objPtr);
    }
}


/**
 * Tcl object implementation:
 * Free the memory used by the internal representation of the type.
 */
void TclCxxObject::FreeInternalRep(Tcl_Obj* objPtr)
{
  CxxObject* cxxObject = Self::Cast(objPtr);
  _cable_tcl_DEBUG_OUTPUT(cxxObject->GetWrapperFacility(),
                          "Freeing rep for Tcl_Obj at " << objPtr
                          << " (cxxObject=" << cxxObject
                          << ",refCount=" << objPtr->refCount << ")" << std::endl);
  cxxObject->Decrement();
  
  // There is one simple rule that controls when a CxxObject is
  // deleted.  When the last Tcl_Obj still referencing the CxxObject
  // in binary form is destroyed, the CxxObject is destroyed.
  if((cxxObject->GetReferenceCount() <= 0) && (objPtr->refCount <= 0))
    {
    cxxObject->Delete();
    }
}


/**
 * Tcl object implementation:
 * Set the internal representation of destPtr to a copy of that of srcPtr.
 */
void TclCxxObject::DupInternalRep(Tcl_Obj* srcPtr, Tcl_Obj* destPtr)
{
  _cable_tcl_DEBUG_OUTPUT(Self::Cast(srcPtr)->GetWrapperFacility(),
                          "Duplicating rep of Tcl_Obj from " << srcPtr
                          << " to " << destPtr << std::endl);
  Self::SetTclObj(destPtr, Self::Cast(srcPtr));
}


namespace
{

/**
 * Internal function used to convert the value in a CxxObject that
 * references a predefined type into a string describing the value.
 *
 * This is called by TclCxxObject::UpdateString() to get the string
 * representation for the values.
 */
template <typename T>
struct GetCxxObjectValue
{
  static String From(CxxObject* cxxObject)
    {
    std::ostrstream value;
    value << *reinterpret_cast<T*>(cxxObject->GetObject()) << std::ends;
    String result = value.str();
    value.rdbuf()->freeze(0);
    return result;
    }
};

template <>
struct GetCxxObjectValue<char>
{
  static String From(CxxObject* cxxObject)
    {
    std::ostrstream value;
    value << int(*reinterpret_cast<char*>(cxxObject->GetObject()))
          << std::ends;
    String result = value.str();
    value.rdbuf()->freeze(0);
    return result;
    }
};

template <>
struct GetCxxObjectValue<signed char>
{
  static String From(CxxObject* cxxObject)
    {
    std::ostrstream value;
    value << int(*reinterpret_cast<signed char*>(cxxObject->GetObject()))
          << std::ends;
    String result = value.str();
    value.rdbuf()->freeze(0);
    return result;
    }
};

template <>
struct GetCxxObjectValue<unsigned char>
{
  static String From(CxxObject* cxxObject)
    {
    std::ostrstream value;
    value << int(*reinterpret_cast<unsigned char*>(cxxObject->GetObject()))
          << std::ends;
    String result = value.str();
    value.rdbuf()->freeze(0);
    return result;
    }
};

template <>
struct GetCxxObjectValue<bool>
{
  static String From(CxxObject* cxxObject)
    {
    if(*reinterpret_cast<bool*>(cxxObject->GetObject())) { return "1"; }
    else { return "0"; }
    }
};
  
} // anonymous namespace


/**
 * Tcl object implementation:
 * Generate the string representation of the Tcl object from the internal
 * representation.
 *
 * This will be a string capable of re-constructing a pointer to the
 * internal CxxObject instance in all cases except values that are
 * references to non-const fundamental types.  In these cases, the
 * value of the predefined type will be used to construct the string
 * representation.  This allows references to predefined types to be
 * passed through arguments as references, but still be displayed and
 * used by Tcl as their values.
 */
void TclCxxObject::UpdateString(Tcl_Obj *objPtr)
{
  String stringRep;
  CxxObject* cxxObject = Self::Cast(objPtr);
  const Type* type = cxxObject->GetType();
  
  if(type == CvPredefinedType<bool&>::type.GetType())
    { stringRep = GetCxxObjectValue<bool>::From(cxxObject); }
  else if(type == CvPredefinedType<char&>::type.GetType())
    { stringRep = GetCxxObjectValue<char>::From(cxxObject); }
  else if(type == CvPredefinedType<signed char&>::type.GetType())
    { stringRep = GetCxxObjectValue<signed char>::From(cxxObject); }
  else if(type == CvPredefinedType<unsigned char&>::type.GetType())
    { stringRep = GetCxxObjectValue<unsigned char>::From(cxxObject); }
  else if(type == CvPredefinedType<short&>::type.GetType())
    { stringRep = GetCxxObjectValue<short>::From(cxxObject); }
  else if(type == CvPredefinedType<unsigned short&>::type.GetType())
    { stringRep = GetCxxObjectValue<unsigned short>::From(cxxObject); }
  else if(type == CvPredefinedType<int&>::type.GetType())
    { stringRep = GetCxxObjectValue<int>::From(cxxObject); }
  else if(type == CvPredefinedType<unsigned int&>::type.GetType())
    { stringRep = GetCxxObjectValue<unsigned int>::From(cxxObject); }
  else if(type == CvPredefinedType<long&>::type.GetType())
    { stringRep = GetCxxObjectValue<long>::From(cxxObject); }
  else if(type == CvPredefinedType<unsigned long&>::type.GetType())
    { stringRep = GetCxxObjectValue<unsigned long>::From(cxxObject); }
  else if(type == CvPredefinedType<float&>::type.GetType())
    { stringRep = GetCxxObjectValue<float>::From(cxxObject); }
  else if(type == CvPredefinedType<double&>::type.GetType())
    { stringRep = GetCxxObjectValue<double>::From(cxxObject); }
  else if(type == CvPredefinedType<long double&>::type.GetType())
    { stringRep = GetCxxObjectValue<long double>::From(cxxObject); }  
  else
    { stringRep = cxxObject->GetStringRepresentation(); }
  
  objPtr->bytes = Tcl_Alloc(stringRep.length()+1);
  objPtr->length = stringRep.length();
  strcpy(objPtr->bytes, stringRep.c_str());
}


/**
 * Tcl object implementation:
 * Attempt to construct the internal representation from the string
 * representation.
 */
int TclCxxObject::SetFromAny(Tcl_Interp* interp, Tcl_Obj* objPtr)
{
  const char* stringRep = Tcl_GetStringFromObj(objPtr, NULL);
  CxxObject* cxxObject = CxxObject::GetFromStringRepresentation(stringRep);
  
  if(!cxxObject)
    {
    Tcl_ResetResult(interp);
    Tcl_AppendResult(interp,
                     "Expected CxxObject, but got \"",
                     const_cast<char*>(stringRep), "\"", 0);
    return TCL_ERROR;
    }
  else
    {
    WrapperFacility* wrapperFacility = WrapperFacility::GetForInterpreter(interp);
    if(!wrapperFacility->CxxObjectExists(cxxObject))
      {
      Tcl_ResetResult(interp);
      Tcl_AppendResult(interp,
                       "No object exists at address given by \"",
                       const_cast<char*>(stringRep), "\"", 0);
      return TCL_ERROR;
      }
    _cable_tcl_DEBUG_OUTPUT(wrapperFacility,
                            "Setting rep for Tcl_Obj at " << objPtr
                            << " (refCount=" << objPtr->refCount << ")"
                            << " from \"" << stringRep << "\"" << std::endl);
    }
  
  Self::DeleteOldRepresentation(objPtr);
  Self::SetTclObj(objPtr, cxxObject);
  return TCL_OK;
}


/**
 * This will be called when the program begins.  It registers the Tcl
 * object type implemented by this class.
 */
void TclCxxObject::ClassInitialize()
{
  Tcl_RegisterObjType(&Self::TclObjTypeStruct);
}


/**
 * The Tcl_ObjType structure instance that is registered with Tcl.
 */
Tcl_ObjType TclCxxObject::TclObjTypeStruct =
{
  "CxxObject",            // name
  &Self::FreeInternalRep, // freeIntRepProc
  &Self::DupInternalRep,  // dupIntRepProc
  &Self::UpdateString,    // updateStringProc
  &Self::SetFromAny       // setFromAnyProc
};


/**
 * Tcl object interface:
 * Get a pointer to CxxObject from the given Tcl object.  Convert
 * the Tcl object's representation if necessary.
 */
int Tcl_GetCxxObjectFromObj(Tcl_Interp* interp, Tcl_Obj* objPtr,
                            CxxObject** obj)
{
  return TclCxxObject::GetFromObj(interp, objPtr, obj);
}


/**
 * Tcl object interface:
 * Set a Tcl object to represent a given CxxObject pointer.
 */
void Tcl_SetCxxObjectObj(Tcl_Obj* objPtr, CxxObject* obj)
{
  TclCxxObject::SetObj(objPtr, obj);
}


/**
 * Tcl object interface:
 * Create a new Tcl object which represents the given CxxObject pointer.
 */
Tcl_Obj* Tcl_NewCxxObjectObj(CxxObject* obj)
{
  return TclCxxObject::NewObj(obj);
}


/**
 * Check whether the given object's type is "CxxObject".
 */
bool TclObjectTypeIsCxxObject(Tcl_Obj* o)
{
  return TclCxxObject::TclObjTypeMatches(o);
}


/**
 * Check whether the given string representation is that of a CxxObject.
 */
bool StringRepIsCxxObject(const String& s)
{
  return (((s).substr(0, 4)) == "_cxx");
}
  
} // namespace _cable_tcl_
