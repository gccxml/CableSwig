/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableClassType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableClassType.h"
#include "cableClass.h"
#include "cxxClassType.h"

namespace cable
{

//----------------------------------------------------------------------------
ClassType::ClassType()
{
  m_Class = 0;
}

//----------------------------------------------------------------------------
ClassType::~ClassType()
{
}

//----------------------------------------------------------------------------
Type::TypeIdType ClassType::GetTypeId() const
{
  return ClassTypeId;
}

//----------------------------------------------------------------------------
bool ClassType::CreateCxxType(cxx::TypeSystem* ts)
{
  // Make sure we haven't already created the type.
  if(m_CxxType.GetType())
    {
    return true;
    }
  
  if(m_Class)
    {
    const cxx::ClassType* t = m_Class->GetCxxClassType(ts);
    if(t)
      {
      m_CxxType = t->GetCvQualifiedType(m_Const, m_Volatile);
      return true;
      }
    }
  cableErrorMacro("Couldn't create cxx::ClassType.");
  return false;
}

//----------------------------------------------------------------------------
Class* ClassType::GetClass() const
{
  return m_Class;
}

//----------------------------------------------------------------------------
void ClassType::SetClass(Class* c)
{
  m_Class = c;
}

} // namespace cable
