/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableReferenceType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableReferenceType_h
#define _cableReferenceType_h

#include "cableType.h"

namespace cable
{

/** Represent a reference type.  */
class CABLE_PARSERS_EXPORT ReferenceType: public Type
{
public:
  cableTypeMacro(ReferenceType, Type);
  static Pointer New() { return new Self; }
  
  /** Get the type id for this Type.  */
  TypeIdType GetTypeId() const;
  
  /** Create the CxxTypes representation.  */
  bool CreateCxxType(cxx::TypeSystem* ts);
  
  /** Get/Set the target of the reference type.  */
  Type* GetTarget() const;
  void SetTarget(Type* target);
  
protected:
  ReferenceType();
  ~ReferenceType();  
  
  // The target of the reference type.
  Type* m_Target;
  
private:
  ReferenceType(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
