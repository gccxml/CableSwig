/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableObject_h
#define _cableObject_h

#include "cableSmartPointer.h"
#include "cableIndent.h"

namespace cable
{

/** Superclass for all internal cable objects.  */
class CABLE_PARSERS_EXPORT Object
{
public:
  typedef Object                    Self;
  typedef SmartPointer<Self>        Pointer;
  
  /** Create a new reference counted object.  */
  static Pointer New() { return new Self; }
  
  /** Get the name of this class.  */
  virtual const char* GetNameOfClass() const { return "Object"; }
  
  /** Safely cast down the hierarchy.  */
  static Object* SafeDownCast(Object* obj) { return obj; }

  /** Safely cast down the hierarchy.  */
  static const Object* SafeDownCast(const Object* obj) { return obj; }
  
  /** Increment object's reference count. */
  virtual void Register();
  
  /** Decrement object's reference count.  */
  virtual void Unregister();
  
protected:
  Object();
  virtual ~Object();
  
  int m_ReferenceCount;
  
private:
  Object(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
