/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrapInstanceTable.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#include "wrapInstanceTable.h"
#include "wrapException.h"

namespace _wrap_
{


/**
 * Constructor takes interpreter to which the InstanceTable will be
 * attached.  It also initializes the temporary object number to zero.
 */
InstanceTable::InstanceTable(Tcl_Interp* interp):
  m_Interpreter(interp),
  m_TempNameNumber(0)
{
}


/**
 * Set a mapping from "name" to object "object" with type "type".
 * This deletes any instance of an object already having the given name.
 */
void InstanceTable::SetObject(const String& name, void* object,
                              const CvQualifiedType& type)
{
  if(this->Exists(name))
    {
    this->DeleteObject(name);
    }
  m_InstanceMap[name] = Reference(object, type);
  m_AddressToNameMap[object] = name;  
}
  
  
/**
 * Delete the object corresponding to the given name.
 * This looks up the function pointer that was registered for the object's
 * type, and calls it to do the actual deletion.
 */
void InstanceTable::DeleteObject(const String& name)
{
  this->CheckExists(name);
  
  const Type* type = m_InstanceMap[name].GetCvQualifiedType().GetType();
  void* object = m_InstanceMap[name].GetObject();

  // Make sure we know how to delete this object.
  if(m_DeleteFunctionMap.count(type) < 1)
    {
    throw _wrap_UndefinedObjectTypeException(type->Name());
    }
  
  // Remove the object's address from our table.
  m_AddressToNameMap.erase(object);

  // Call the registered delete function.
  m_DeleteFunctionMap[type](object);
  
  // Remove from the instance table.
  m_InstanceMap.erase(name);
 
  // Remove the Tcl command for this instance.
  Tcl_DeleteCommand(m_Interpreter, const_cast<char*>(name.c_str()));
}
  
  
/**
 * Check if there is an object with the given name.
 */
bool InstanceTable::Exists(const String& name) const
{
  return (m_InstanceMap.count(name) > 0);
}


/**
 * Get the Reference holding the object with the given name.
 */
const Reference& InstanceTable::GetEntry(const String& name) const
{
  this->CheckExists(name);
  InstanceMap::const_iterator i = m_InstanceMap.find(name);
  return i->second;
}
 

/**
 * Get a pointer to the object with the given name.
 */
void* InstanceTable::GetObject(const String& name)
{
  this->CheckExists(name);
  return m_InstanceMap[name].GetObject();
}

  
/**
 * Get the type string for the object with the given name.
 */
const CvQualifiedType& InstanceTable::GetType(const String& name)
{
  this->CheckExists(name);
  return m_InstanceMap[name].GetCvQualifiedType();
}

  
/**
 * Allow object type deletion functions to be added.
 */
void InstanceTable::SetDeleteFunction(const Type* type,
                                           DeleteFunction func)
{
  m_DeleteFunctionMap[type] = func;
}


/**
 * Create a unique name for a temporary object, and set the given object
 * to have this name.  The name chosen is returned.
 */
String InstanceTable::CreateTemporary(void* object,
                                      const CvQualifiedType& type)
{
  char tempName[15];
  sprintf(tempName, "__temp%x", m_TempNameNumber++);
  String name = tempName;
  this->SetObject(name, object, type);
  return name;
}


/**
 * If the given object name was generated by InsertTemporary(), the object
 * is deleted.
 */
void InstanceTable::DeleteIfTemporary(const String& name)
{
  this->CheckExists(name);
  if(name.substr(0,6) == "__temp")
    {
    this->DeleteObject(name);
    }
}


/**
 * When an instance deletes itself, this callback is made to remove it from
 * the instance table.
 */
void InstanceTable::DeleteCallBack(void* object)
{
  if(m_AddressToNameMap.count(object) > 0)
    {
    String name = m_AddressToNameMap[object];
    this->DeleteObject(name);
    }
}


/**
 * Make sure an object with the given name exists.
 * Throw an exception if it doesn't exist.
 */
void InstanceTable::CheckExists(const String& name) const
{
  if(!this->Exists(name))
    {
    throw _wrap_UndefinedInstanceNameException(name);
    }
}


/**
 * Get an InstanceTable object set up to deal with the given Tcl interpreter.
 * If one exists, it will be returned.  Otherwise, a new one will be
 * created.
 */
InstanceTable* InstanceTable::GetForInterpreter(Tcl_Interp* interp)
{
  // See if an InstanceTable exists for the given interpreter.
  if(interpreterInstanceTableMap.count(interp) == 0)
    {
    // No, we must create a new InstanceTable for this interpreter.
    interpreterInstanceTableMap[interp] = new InstanceTable(interp);
    }
  
  // Return the InstanceTable.
  return interpreterInstanceTableMap[interp];  
}


/**
 * Map from a Tcl interpreter to the InstanceTable for it.
 */
InstanceTable::InterpreterInstanceTableMap InstanceTable::interpreterInstanceTableMap;

} // namespace _wrap_
