/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxEnumerationType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxEnumerationType.h"

namespace _cxx_
{

  
/**
 * Retrieve what kind of Type this is.
 */
RepresentationType EnumerationType::GetRepresentationType() const
{
  return EnumerationType_id;
}


/**
 * Try to cast the given Type to an EnumerationType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
EnumerationType* EnumerationType::SafeDownCast(Type* t)
{
  EnumerationType* result = dynamic_cast<EnumerationType*>(t);
  if(!result) { throw TypeDownCastException(t, EnumerationType_id); }
  return result;
}


/**
 * Try to cast the given Type to an EnumerationType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
const EnumerationType* EnumerationType::SafeDownCast(const Type* t)
{
  const EnumerationType* result = dynamic_cast<const EnumerationType*>(t);
  if(!result) { throw TypeDownCastException(t, EnumerationType_id); }
  return result;
}


/**
 * Get the name of the class.
 */
String EnumerationType::GetName() const
{
  return m_Name;
}


String EnumerationType::GenerateName(const String& outerType,
                                     bool isConst, bool isVolatile) const
{
  String cv = this->GetLeftCvString(isConst, isVolatile);
  return cv+m_Name+this->PrepareOuterStringForPostfix(outerType);
}


/**
 * Constructor for EnumerationType just takes the name of the type.
 */
EnumerationType::EnumerationType(const String& name):
  m_Name(name)
{
}


} // namespace _cxx_
