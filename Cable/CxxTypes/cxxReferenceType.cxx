/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxReferenceType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxReferenceType.h"

namespace _cxx_
{


/**
 * Retrieve what kind of Type this is.
 */
RepresentationType ReferenceType::GetRepresentationType() const
{
  return ReferenceType_id;
}


/**
 * Try to cast the given Type to an ReferenceType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
ReferenceType* ReferenceType::SafeDownCast(Type* t)
{
  ReferenceType* result = dynamic_cast<ReferenceType*>(t);
  if(!result) { throw TypeDownCastException(t, ReferenceType_id); }
  return result;
}


/**
 * Try to cast the given Type to an ReferenceType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
const ReferenceType* ReferenceType::SafeDownCast(const Type* t)
{
  const ReferenceType* result = dynamic_cast<const ReferenceType*>(t);
  if(!result) { throw TypeDownCastException(t, ReferenceType_id); }
  return result;
}


// Can't have cv qualifiers.
String ReferenceType::GenerateName(const String& outerType, bool, bool) const
{
  String outerString = "&"+this->PrepareOuterStringForPostfix(outerType);
  return m_ReferencedType.GenerateName(outerString);
}


/**
 * Get the CvQualifiedType referenced by this ReferenceType.
 */
const CvQualifiedType& ReferenceType::GetReferencedType() const
{
  return m_ReferencedType;
}


/**
 * Constructor takes the cv-qualified type that is referenced.
 */
ReferenceType::ReferenceType(const CvQualifiedType& in_type):
  m_ReferencedType(in_type)
{
}


} // namespace _cxx_
