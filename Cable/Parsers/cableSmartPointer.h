/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableSmartPointer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableSmartPointer_h
#define _cableSmartPointer_h

#include "cableUtils.h"

namespace cable
{

/**  SmartPointer allows automatic reference counting.  */
template <class T>
class SmartPointer
{
public:
  SmartPointer(): m_Pointer(0) {}
  SmartPointer(const SmartPointer& p): m_Pointer(p.m_Pointer)
    { this->Register(); }
  SmartPointer(T* p): m_Pointer(p) { this->Register(); }
  ~SmartPointer() { this->Unregister(); }

  /** Allow equality comparison with another pointer.  */
  bool operator == (SmartPointer r) const { return (m_Pointer==r.m_Pointer); }

  /** Allow equality comparison with another pointer.  */
  bool operator == (T* r) const { return (m_Pointer==r); }
  
  /** Allow inequality comparison with another pointer.  */
  bool operator != (SmartPointer r) const { return (m_Pointer!=r.m_Pointer); }

  /** Allow inequality comparison with another pointer.  */
  bool operator != (T* r) const { return (m_Pointer!=r); }
  
  /** Allow assignment.  */
  SmartPointer& operator = (const SmartPointer& r)
    { SmartPointer(r).Swap(*this); return *this; }  

  /** Allow assignment.  */
  SmartPointer& operator = (T* r)
    { SmartPointer(r).Swap(*this); return *this; }  

  /** Allow access to members of T as if this were a normal pointer.  */
  T* operator -> () const { return m_Pointer;  }

  /** Let smart pointer look like normal pointer to T.  */
  operator T* () const { return m_Pointer; }

  /** Allow read access to real pointer to make casting easier.  */
  T* RealPointer() const { return m_Pointer; }

private:
  /** Increment reference count of object.  */
  void Register() { if(m_Pointer) { m_Pointer->Register(); } }
  
  /** Decrement reference count of object.  */
  void Unregister() { if(m_Pointer) { m_Pointer->Unregister(); } }
  
  void Swap(SmartPointer& r)
    { T* temp = m_Pointer; m_Pointer = r.m_Pointer; r.m_Pointer = temp; }
  
  T* m_Pointer;
};

} // namespace cable
  
#endif
