/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cablePointerType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cablePointerType_h
#define _cablePointerType_h

#include "cableType.h"

namespace cable
{

/** Represent a pointer type.  */
class CABLE_PARSERS_EXPORT PointerType: public Type
{
public:
  cableTypeMacro(PointerType, Type);
  static Pointer New() { return new Self; }
  
  /** Get the type id for this Type.  */
  TypeIdType GetTypeId() const;
  
  /** Create the CxxTypes representation.  */
  bool CreateCxxType(cxx::TypeSystem* ts);
  
  /** Get/Set the target of the pointer type.  */
  Type* GetTarget() const;
  void SetTarget(Type* target);
  
protected:
  PointerType();
  ~PointerType();  
  
  // The target of the pointer type.
  Type* m_Target;
  
private:
  PointerType(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
