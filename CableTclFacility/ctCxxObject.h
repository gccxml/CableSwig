/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctCxxObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctCxxObject_h
#define _ctCxxObject_h

#include "ctUtils.h"
#include "ctAnything.h"

namespace _cable_tcl_
{

class WrapperFacility;

/**
 * An instance of this class is used to refer to a C++ object from one
 * or more Tcl objects.  It maintains a reference count of the number
 * of Tcl objects refering to it in their internal representations.
 * Such Tcl objects are of the type "CxxObject", which is defined in
 * "wrapTclCxxObject.h".
 *
 * When an instance is created, it automatically tries to create a Tcl
 * command to allow methods to be called on the referenced object.
 * When it is destroyed, the command is removed.  Also, if the
 * WrapperFacility knows how to delete the object, it will be deleted
 * when this instance is destroyed.
 */
class _cable_tcl_EXPORT CxxObject
{
public:
  typedef CxxObject Self;
  
  void Delete() const;
  
  void* GetObject() const;
  const Type* GetType() const;
  Tcl_Interp* GetInterpreter() const;
  const WrapperFacility* GetWrapperFacility() const;
  int GetReferenceCount() const;
  void Increment();
  void Decrement();
  
  String GetStringRepresentation() const;
  static CxxObject* GetFromStringRepresentation(const char*);  
  
private:
  // Make sure no one creates or destroys an instance directly.
  CxxObject(const Anything&, const Type*, const WrapperFacility*);
  ~CxxObject();
  
  void CreateTclCommand() const;
  void DeleteTclCommand() const;
  void CleanupObject() const;
  
  // Leave these unimplemented so that no one can copy an instance.
  CxxObject(const CxxObject&);
  void operator=(const CxxObject&);
  
  ///! The a pointer to the object or function.
  Anything m_Anything;
  
  ///! The type of the object.  Top level cv-qualifiers are never stored.
  const Type* m_Type;

  ///! The WrapperFacility instance for which the object has been created.
  const WrapperFacility* m_WrapperFacility;
  
  ///! Count the number of Object instances referencing this.
  int m_ReferenceCount;
  
  // Make sure that a WrapperFacility can create instances of
  // CxxObject.
  friend class WrapperFacility;
};

} // namespace _cable_tcl_

#endif
