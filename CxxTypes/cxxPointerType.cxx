/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxPointerType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxPointerType.h"

namespace _cxx_
{


/**
 * Retrieve what kind of Type this is.
 */
RepresentationType PointerType::GetRepresentationType() const
{
  return PointerType_id;
}


/**
 * Try to cast the given Type to an PointerType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
PointerType* PointerType::SafeDownCast(Type* t)
{
  PointerType* result = dynamic_cast<PointerType*>(t);
  if(!result) { throw TypeDownCastException(t, PointerType_id); }
  return result;
}


/**
 * Try to cast the given Type to an PointerType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
const PointerType* PointerType::SafeDownCast(const Type* t)
{
  const PointerType* result = dynamic_cast<const PointerType*>(t);
  if(!result) { throw TypeDownCastException(t, PointerType_id); }
  return result;
}


String PointerType::GenerateName(const String& outerType,
                                 bool isConst, bool isVolatile) const
{
  String cv = this->GetRightCvString(isConst, isVolatile);
  String outerString = "*"+cv+this->PrepareOuterStringForPostfix(outerType);
  return m_PointedToType.GenerateName(outerString);
}


/**
 * Get the CvQualifiedType to which this PointerType points.
 */
const CvQualifiedType& PointerType::GetPointedToType() const
{
  return m_PointedToType;
}


/**
 * Constructor takes the cv-qualified type to which the pointer points.
 */
PointerType::PointerType(const CvQualifiedType& in_type):
  m_PointedToType(in_type)
{
}

} // namespace _cxx_
