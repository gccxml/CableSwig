/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cablePointerType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cablePointerType.h"
#include "cxxTypeSystem.h"

namespace cable
{

//----------------------------------------------------------------------------
PointerType::PointerType()
{
  m_Target = 0;
}

//----------------------------------------------------------------------------
PointerType::~PointerType()
{
}

//----------------------------------------------------------------------------
Type::TypeIdType PointerType::GetTypeId() const
{
  return PointerTypeId;
}

//----------------------------------------------------------------------------
bool PointerType::CreateCxxType(cxx::TypeSystem* ts)
{
  // Make sure we haven't already created the type.
  if(m_CxxType.GetType())
    {
    return true;
    }
  
  // Make sure there is a valid target type.
  if(!m_Target || !m_Target->CreateCxxType(ts))
    {
    cableErrorMacro("Invalid target type for PointerType.");
    return false;
    }
  
  // If pointing to a member, use cxx::PointerToMemberType.
  TypeIdType targetTypeId = m_Target->GetTypeId();
  if(targetTypeId == Type::MethodTypeId)
    {
    cxx::CvQualifiedType pmt = m_Target->GetCxxType();
    m_CxxType = pmt.GetMoreQualifiedType(m_Const, m_Volatile);
    return true;
    }
  else if(targetTypeId == Type::OffsetTypeId)
    {
    cxx::CvQualifiedType pmt = m_Target->GetCxxType();
    m_CxxType = pmt.GetMoreQualifiedType(m_Const, m_Volatile);
    return true;
    }
  
  // Use standard pointer.
  const cxx::PointerType* t = ts->GetPointerType(m_Target->GetCxxType());  
  if(t)
    {
    m_CxxType = t->GetCvQualifiedType(m_Const, m_Volatile);
    return true;
    }
  else
    {
    cableErrorMacro("Couldn't create cxx::PointerType.");
    return false;
    }
}

//----------------------------------------------------------------------------
Type* PointerType::GetTarget() const
{
  return m_Target;
}

//----------------------------------------------------------------------------
void PointerType::SetTarget(Type* target)
{
  m_Target = target;
}

} // namespace cable
