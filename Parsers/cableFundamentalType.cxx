/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableFundamentalType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableFundamentalType.h"
#include "cxxTypeSystem.h"

namespace cable
{

//----------------------------------------------------------------------------
FundamentalType::FundamentalType()
{
}

//----------------------------------------------------------------------------
FundamentalType::~FundamentalType()
{
}

//----------------------------------------------------------------------------
Type::TypeIdType FundamentalType::GetTypeId() const
{
  return FundamentalTypeId;
}

//----------------------------------------------------------------------------
bool FundamentalType::CreateCxxType(cxx::TypeSystem* ts)
{
  // Make sure we haven't already created the type.
  if(m_CxxType.GetType())
    {
    return true;
    }
  
  const cxx::FundamentalType* t = 0;
  const String& n = m_TypeName;
  if(n == "unsigned char")
    { t = ts->GetFundamentalType(cxx::FundamentalType::UnsignedChar); }
  else if((n == "unsigned short") || (n == "short unsigned int"))
    { t = ts->GetFundamentalType(cxx::FundamentalType::UnsignedShortInt); }
  else if(n == "unsigned int")
    { t = ts->GetFundamentalType(cxx::FundamentalType::UnsignedInt); }
  else if((n == "unsigned long") || (n == "long unsigned int"))
    { t = ts->GetFundamentalType(cxx::FundamentalType::UnsignedLongInt); }
  else if((n == "unsigned long long") || (n == "long long unsigned int"))
    { t = ts->GetFundamentalType(cxx::FundamentalType::UnsignedLongLongInt); }
  else if(n == "signed char")
    { t = ts->GetFundamentalType(cxx::FundamentalType::SignedChar); }
  else if(n == "char")
    { t = ts->GetFundamentalType(cxx::FundamentalType::Char); }
  else if((n == "short") || (n == "signed short") || (n == "short int"))
    { t = ts->GetFundamentalType(cxx::FundamentalType::ShortInt); }
  else if((n == "int") || (n == "signed int"))
    { t = ts->GetFundamentalType(cxx::FundamentalType::Int); }
  else if((n == "long") || (n == "signed long") || (n == "long int"))
    { t = ts->GetFundamentalType(cxx::FundamentalType::LongInt); }
  else if((n == "long long") || (n == "long long int"))
    { t = ts->GetFundamentalType(cxx::FundamentalType::LongLongInt); }
  else if(n == "wchar_t")
    { t = ts->GetFundamentalType(cxx::FundamentalType::WChar_t); }
  else if(n == "bool")
    { t = ts->GetFundamentalType(cxx::FundamentalType::Bool); }
  else if(n == "float")
    { t = ts->GetFundamentalType(cxx::FundamentalType::Float); }
  else if(n == "double")
    { t = ts->GetFundamentalType(cxx::FundamentalType::Double); }
  else if(n == "long double")
    { t = ts->GetFundamentalType(cxx::FundamentalType::LongDouble); }
  else if(n == "complex float")
    { t = ts->GetFundamentalType(cxx::FundamentalType::ComplexFloat); }
  else if(n == "complex double")
    { t = ts->GetFundamentalType(cxx::FundamentalType::ComplexDouble); }
  else if(n == "complex long double")
    { t = ts->GetFundamentalType(cxx::FundamentalType::ComplexLongDouble); }
  else if(n == "void")
    { t = ts->GetFundamentalType(cxx::FundamentalType::Void); }
   
  if(t)
    {
    m_CxxType = t->GetCvQualifiedType(m_Const, m_Volatile);
    return true;
    }
  else
    {
    cableErrorMacro("Invalid type name \""
                    << n.c_str() << "\" for FundamentalType.");
    return false;
    }
}

//----------------------------------------------------------------------------
const char* FundamentalType::GetTypeName() const
{
  return m_TypeName.c_str();
}

//----------------------------------------------------------------------------
void FundamentalType::SetTypeName(const char* name)
{
  m_TypeName = name;
}

} // namespace cable
