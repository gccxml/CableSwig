/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableReferenceType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableReferenceType.h"
#include "cxxTypeSystem.h"

namespace cable
{

//----------------------------------------------------------------------------
ReferenceType::ReferenceType()
{
  m_Target = 0;
}

//----------------------------------------------------------------------------
ReferenceType::~ReferenceType()
{
}

//----------------------------------------------------------------------------
Type::TypeIdType ReferenceType::GetTypeId() const
{
  return ReferenceTypeId;
}

//----------------------------------------------------------------------------
bool ReferenceType::CreateCxxType(cxx::TypeSystem* ts)
{
  // Make sure we haven't already created the type.
  if(m_CxxType.GetType())
    {
    return true;
    }
  
  // Make sure there is a valid target type.
  if(!m_Target || !m_Target->CreateCxxType(ts))
    {
    cableErrorMacro("Invalid target type for ReferenceType.");
    return false;
    }
  
  const cxx::ReferenceType* t = ts->GetReferenceType(m_Target->GetCxxType());
  
  if(t)
    {
    // Reference types have no top-level cv-qualifiers.
    m_CxxType = t->GetCvQualifiedType(false, false);
    return true;
    }
  else
    {
    cableErrorMacro("Couldn't create cxx::ReferenceType.");
    return false;
    }
}

//----------------------------------------------------------------------------
Type* ReferenceType::GetTarget() const
{
  return m_Target;
}

//----------------------------------------------------------------------------
void ReferenceType::SetTarget(Type* target)
{
  m_Target = target;
}

} // namespace cable
