/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableOffsetType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableOffsetType.h"
#include "cableClass.h"
#include "cxxTypeSystem.h"

namespace cable
{

//----------------------------------------------------------------------------
OffsetType::OffsetType()
{
  m_MemberType = 0;
  m_Class = 0;
}

//----------------------------------------------------------------------------
OffsetType::~OffsetType()
{
}

//----------------------------------------------------------------------------
Type::TypeIdType OffsetType::GetTypeId() const
{
  return OffsetTypeId;
}

//----------------------------------------------------------------------------
bool OffsetType::CreateCxxType(cxx::TypeSystem* ts)
{
  // Make sure we haven't already created the type.
  if(m_CxxType.GetType())
    {
    return true;
    }
  
  // Make sure there is a valid member type.
  if(!m_MemberType || !m_MemberType->CreateCxxType(ts))
    {
    cableErrorMacro("Invalid member type for OffsetType.");
    return false;
    }
  if(!m_Class)
    {
    cableErrorMacro("No class set.");
    return false;
    }
  const cxx::ClassType* ct = m_Class->GetCxxClassType(ts);
  if(!ct)
    {
    cableErrorMacro("Error creating cxx::ClassType for OffseType.");
    return false;
    }
  
  // There isn't really an OffsetType.  Use a pointer to member data.
  cxx::CvQualifiedType ot = m_MemberType->GetCxxType();
  cxx::CvQualifiedType cvot = ot.GetMoreQualifiedType(m_Const, m_Volatile);
  const cxx::PointerToMemberType* pt = ts->GetPointerToMemberType(cvot, ct);
  if(pt)
    {
    m_CxxType = pt->GetCvQualifiedType(false, false);
    return true;
    }
  cableErrorMacro("Couldn't create cxx::OffsetType.");
  return false;
}

//----------------------------------------------------------------------------
Type* OffsetType::GetMemberType() const
{
  return m_MemberType;
}

//----------------------------------------------------------------------------
void OffsetType::SetMemberType(Type* type)
{
  m_MemberType = type;
}
  
//----------------------------------------------------------------------------
Class* OffsetType::GetClass() const
{
  return m_Class;
}

//----------------------------------------------------------------------------
void OffsetType::SetClass(Class* c)
{
  m_Class = c;
}

} // namespace cable
