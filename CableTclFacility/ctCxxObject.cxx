/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctCxxObject.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctCxxObject.h"
#include "ctWrapperFacility.h"
#include "ctClassWrapper.h"

#include <stdio.h>
#include <map>

namespace _cable_tcl_
{

/**
 * Delete the instance of CxxObject on which this method is called.
 * This will clean up the object's Tcl command, if any, as well as delete
 * the object referenced if it is of C++ class type.
 */
void CxxObject::Delete() const
{
  // Ask the map of all instances to delete this object.
  m_WrapperFacility->DeleteCxxObjectFor(m_Anything, m_Type);
}


/**
 * Get a pointer to the C++ object.
 */
void* CxxObject::GetObject() const
{
  return m_Anything.object;
}
  

/**
 * Get the type represented by this CxxObject.
 */
const Type* CxxObject::GetType() const
{
  return m_Type;
}


/**
 * Get the interpreter to which this CxxObject is attached.
 */
Tcl_Interp* CxxObject::GetInterpreter() const
{
  return m_WrapperFacility->GetInterpreter();
}


/**
 * Get the WrapperFacility to which this CxxObject is attached.
 */
const WrapperFacility* CxxObject::GetWrapperFacility() const
{
  return m_WrapperFacility;
}


/**
 * Get the number of Tcl objects currently referencing this CxxObject
 * in their binary representations.  Note that this being zero does
 * not necessarily mean the object should be deleted, because a Tcl
 * object in string form may still refer to it.
 */
int CxxObject::GetReferenceCount() const
{
  return m_ReferenceCount;
}


/**
 * Increment the number of Tcl object currently referencing this
 * CxxObject in their binary representations.
 */
void CxxObject::Increment()
{
  ++m_ReferenceCount;
  _cable_tcl_DEBUG_OUTPUT(this->GetWrapperFacility(),
                          "Incrementing references to CxxObject at " << this
                          << " to " << m_ReferenceCount << std::endl);
}


/**
 * Decrement the number of Tcl object currently referencing this
 * CxxObject in their binary representations.
 */
void CxxObject::Decrement()
{
  --m_ReferenceCount;
  _cable_tcl_DEBUG_OUTPUT(this->GetWrapperFacility(),
                          "Decrementing references to CxxObject at " << this
                          << " to " << m_ReferenceCount << std::endl);
}

  
/**
 * Return the string representation for this CxxObject instance.
 * Passing the resulting string to
 * CxxObject::GetFromStringRepresentation() should result in a pointer
 * to this instance.
 */
String CxxObject::GetStringRepresentation() const
{
  char addrBuf[sizeof(CxxObject*)*2+7];
  sprintf(addrBuf, "_cxx%p", this);
  return addrBuf;
}


/**
 * Attempt to convert the given string representation into a pointer
 * to an instance of CxxObject.
 */
CxxObject* CxxObject::GetFromStringRepresentation(const char* stringRep)
{
  CxxObject* cxxObject = 0;
  sscanf(stringRep, "_cxx%p", &cxxObject);
  
  // Make sure the new object pointer converts back to the original
  // string representation.  Otherwise, the stringRep may have just
  // happened to begin in the correct form.  This can happen when
  // there is a Tcl list of CxxObject instances.
  if(cxxObject && cxxObject->GetStringRepresentation() != stringRep)
    {
    return 0;
    }
  return cxxObject;
}

// private implementation methods:


/**
 * Constructor creates the Tcl command to allow methods to be called
 * on an object if its type is wrapped.
 */
CxxObject::CxxObject(const Anything& anything,
                     const Type* type,
                     const WrapperFacility* wrapperFacility):
  m_Anything(anything),
  m_Type(type),
  m_WrapperFacility(wrapperFacility),
  m_ReferenceCount(0)
{
  _cable_tcl_DEBUG_OUTPUT(this->GetWrapperFacility(),
                          "Creating CxxObject at " << this <<
                          " with type " << m_Type->Name().c_str() << std::endl);
  this->CreateTclCommand();
}


/**
 * Destructor removes the Tcl command and cleans up.
 */
CxxObject::~CxxObject()
{
  _cable_tcl_DEBUG_OUTPUT(this->GetWrapperFacility(),
                          "Deleting CxxObject at " << this <<
                          " with type " << m_Type->Name().c_str() << std::endl);
  this->DeleteTclCommand();
  this->CleanupObject();  
}


/**
 * Create the Tcl command corresponding to this CxxObject.
 */
void CxxObject::CreateTclCommand() const
{
  // Walk through a pointer or reference type to its target.  This
  // allows methods to be called through pointers or references to an
  // object.
  const Type* targetType = m_Type;
  if(m_Type->IsPointerType())
    {
    targetType = PointerType::SafeDownCast(m_Type)
      ->GetPointedToType().GetType();
    }
  else if(m_Type->IsReferenceType())
    {
    targetType = ReferenceType::SafeDownCast(m_Type)
      ->GetReferencedType().GetType();
    }
  
  // If the type is a class type, it may be wrapped.
  if(targetType->IsClassType())
    {
    // Try to get the wrapper for the given type.
    ClassWrapper* wrapper = m_WrapperFacility->GetClassWrapper(
      ClassType::SafeDownCast(targetType));
    if(wrapper)
      {
      char addrBuf[sizeof(CxxObject*)*2+7];
      sprintf(addrBuf, "_cxx%p", this);
      
      // Found a wrapper.  Create the command to use it.
      Tcl_CreateObjCommand(m_WrapperFacility->GetInterpreter(), addrBuf,
                           wrapper->GetObjectWrapperFunction(), wrapper, 0);
      }
    else
      {
      // TODO: Create a dummy wrapper that reports that the wrapper
      // for the type has not been loaded.
      }
    }
}


/**
 * Delete the Tcl command corresponding to this CxxObject.
 */
void CxxObject::DeleteTclCommand() const
{
  String cmdName = this->GetStringRepresentation();

  // If this object is being deleted by the WrapperFacility's
  // destructor, then the interpreter is being deleted.  Don't bother
  // removing the command.
  Tcl_Interp* interp = m_WrapperFacility->GetInterpreter();
  if(interp)
    {
    Tcl_DeleteCommand(interp, const_cast<char*>(cmdName.c_str()));
    }
}


/**
 * If this CxxObject refers to an instance of a C++ class type, try to
 * delete it.
 */
void CxxObject::CleanupObject() const
{
  if(m_Type->IsClassType())
    {
    m_WrapperFacility->DeleteObject(this->GetObject(), m_Type);
    }
}

  
} // namespace _cable_tcl_
