/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxFundamentalType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxFundamentalType.h"

namespace _cxx_
{

/**
 * Retrieve what kind of Type this is.
 */
RepresentationType FundamentalType::GetRepresentationType() const
{
  return FundamentalType_id;
}


/**
 * Try to cast the given Type to an FundamentalType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
FundamentalType* FundamentalType::SafeDownCast(Type* t)
{
  FundamentalType* result = dynamic_cast<FundamentalType*>(t);
  if(!result) { throw TypeDownCastException(t, FundamentalType_id); }
  return result;
}


/**
 * Try to cast the given Type to an FundamentalType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
const FundamentalType* FundamentalType::SafeDownCast(const Type* t)
{
  const FundamentalType* result = dynamic_cast<const FundamentalType*>(t);
  if(!result) { throw TypeDownCastException(t, FundamentalType_id); }
  return result;
}


String FundamentalType::GenerateName(const String& outerType,
                                     bool isConst, bool isVolatile) const
{
  String cv = this->GetLeftCvString(isConst, isVolatile);
  return cv+fundamentalTypeNames[m_Id]+this->PrepareOuterStringForPostfix(outerType);
}


/**
 * Constructor takes the Id of the fundamental type.
 */
FundamentalType::FundamentalType(Id in_id):
  m_Id(in_id)
{
}

const char* FundamentalType::fundamentalTypeNames[NumberOfTypes] =
{
  "unsigned char", "unsigned short", "unsigned int", "unsigned long",
  "unsigned long long", "signed char", "char", "short", "int", "long",
  "long long", "wchar_t", "bool", "float", "double", "long double",
  "std::complex<long double>", "void"
};

} // namespace _cxx_
