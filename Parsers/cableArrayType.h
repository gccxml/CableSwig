/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableArrayType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableArrayType_h
#define _cableArrayType_h

#include "cableType.h"

namespace cable
{

/** Represent an array type.  */
class CABLE_PARSERS_EXPORT ArrayType: public Type
{
public:
  cableTypeMacro(ArrayType, Type);
  static Pointer New() { return new Self; }
  
  /** Get the type id for this Type.  */
  TypeIdType GetTypeId() const;
  
  /** Create the CxxTypes representation.  */
  bool CreateCxxType(cxx::TypeSystem* ts);
  
  /** Get/Set the target of the array type.  */
  Type* GetTarget() const;
  void SetTarget(Type* target);
  
  /** Get/Set the length of the array.  */
  unsigned long GetLength() const;
  void SetLength(unsigned long length);
  
protected:
  ArrayType();
  ~ArrayType();  
  
  // The target of the array type.
  Type* m_Target;
  
  // The length of the array.
  unsigned long m_Length;
  
private:
  ArrayType(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
