/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxPointerToMemberType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxPointerToMemberType.h"
#include "cxxClassType.h"

namespace _cxx_
{

/**
 * Retrieve what kind of Type this is.
 */
RepresentationType PointerToMemberType::GetRepresentationType() const
{
  return PointerToMemberType_id;
}


/**
 * Try to cast the given Type to an PointerToMemberType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
PointerToMemberType* PointerToMemberType::SafeDownCast(Type* t)
{
  PointerToMemberType* result = dynamic_cast<PointerToMemberType*>(t);
  if(!result) { throw TypeDownCastException(t, PointerToMemberType_id); }
  return result;
}


/**
 * Try to cast the given Type to an PointerToMemberType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
const PointerToMemberType* PointerToMemberType::SafeDownCast(const Type* t)
{
  const PointerToMemberType* result = dynamic_cast<const PointerToMemberType*>(t);
  if(!result) { throw TypeDownCastException(t, PointerToMemberType_id); }
  return result;
}


String PointerToMemberType::GenerateName(const String& outerType,
                                         bool isConst, bool isVolatile) const
{
  String cv = this->GetRightCvString(isConst, isVolatile);
  String outerString = m_ClassType->GetName()+"::*"+cv
    +this->PrepareOuterStringForPostfix(outerType);
  return m_PointedToType.GenerateName(outerString);
}


/**
 * Get the ClassType to whom's member the PointerType points.
 */
const ClassType* PointerToMemberType::GetClassType() const
{
  return m_ClassType;
}


/**
 * Constructor takes cv-qualified type of member, and the type of
 * the class in which the member resides.
 */
PointerToMemberType::PointerToMemberType(const CvQualifiedType& in_type,
                                         const ClassType* in_class):
  PointerType(in_type),
  m_ClassType(in_class)
{
}
  

} // namespace _cxx_
