/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableMethodType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableMethodType.h"
#include "cableClass.h"
#include "cxxTypeSystem.h"

#include <vector>

namespace cable
{

//----------------------------------------------------------------------------
MethodType::MethodType()
{
  m_Class = 0;
}

//----------------------------------------------------------------------------
MethodType::~MethodType()
{
}

//----------------------------------------------------------------------------
Type::TypeIdType MethodType::GetTypeId() const
{
  return MethodTypeId;
}

//----------------------------------------------------------------------------
bool MethodType::CreateCxxType(cxx::TypeSystem* ts)
{
  // Make sure we haven't already created the type.
  if(m_CxxType.GetType())
    {
    return true;
    }
  
  if(!m_Class)
    {
    cableErrorMacro("No class set.");
    return false;
    }
  const cxx::ClassType* ct = m_Class->GetCxxClassType(ts);
  if(!ct)
    {
    cableErrorMacro("Error creating cxx::ClassType for MethodType.");
    return false;
    }
  const cxx::FunctionType* ft = this->GetCxxFunctionType(ts);
  if(!ft)
    {
    cableErrorMacro("Error creating cxx::FunctionType for MethodType.");
    return false;
    }
  
  // There isn't really a MethodType.  Use a pointer to member
  // function.
  cxx::CvQualifiedType cvft = ft->GetCvQualifiedType(m_Const, false);
  const cxx::PointerToMemberType* pt = ts->GetPointerToMemberType(cvft, ct);  
  if(pt)
    {
    m_CxxType = pt->GetCvQualifiedType(false, false);
    return true;
    }
  cableErrorMacro("Couldn't create cxx::MethodType.");
  return false;
}

//----------------------------------------------------------------------------
Class* MethodType::GetClass() const
{
  return m_Class;
}

//----------------------------------------------------------------------------
void MethodType::SetClass(Class* c)
{
  m_Class = c;
}


} // namespace cable
