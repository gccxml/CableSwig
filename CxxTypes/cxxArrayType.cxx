/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxArrayType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxArrayType.h"

#include <strstream>

namespace _cxx_
{


/**
 * Retrieve what kind of Type this is.
 */
RepresentationType
ArrayType
::GetRepresentationType() const
{
  return ArrayType_id;
}


/**
 * Try to cast the given Type to an ArrayType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
ArrayType* ArrayType::SafeDownCast(Type* t)
{
  ArrayType* result = dynamic_cast<ArrayType*>(t);
  if(!result) { throw TypeDownCastException(t, ArrayType_id); }
  return result;
}


/**
 * Try to cast the given Type to an ArrayType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
const ArrayType* ArrayType::SafeDownCast(const Type* t)
{
  const ArrayType* result = dynamic_cast<const ArrayType*>(t);
  if(!result) { throw TypeDownCastException(t, ArrayType_id); }
  return result;
}


String ArrayType::GenerateName(const String& outerType,
                               bool isConst, bool isVolatile) const
{
  String lengthStr = this->GenerateLengthString();
  String outerString = "("+outerType+")["+lengthStr+"]";  
  return m_ElementType.GenerateName(outerString, isConst, isVolatile);
}


/**
 * Get the name of the type as it would be used in a declaration with the
 * given name.
 */
String ArrayType::GenerateDeclaration(const String& name,
                                      bool isConst, bool isVolatile) const
{
  String lengthStr = this->GenerateLengthString();
  String outerString = name+"["+lengthStr+"]";  
  return m_ElementType.GenerateName(outerString, isConst, isVolatile);
}


/**
 * Called by GenerateName and GenerateDeclaration to prepare a string
 * holding the function type's arguments.
 */
String ArrayType::GenerateLengthString() const
{
  std::strstream length;
  length << m_Length << std::ends;
  return length.str();
}


/**
 * Constructor takes the type of the elements in the array, and the length.
 */
ArrayType
::ArrayType(const CvQualifiedType& in_elementType, unsigned long in_length):
  m_ElementType(in_elementType),
  m_Length(in_length)
{
}

} // namespace _cxx_
