/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableEnumerationType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableEnumerationType.h"
#include "cableEnumeration.h"
#include "cxxEnumerationType.h"

namespace cable
{

//----------------------------------------------------------------------------
EnumerationType::EnumerationType()
{
  m_Enumeration = 0;
}

//----------------------------------------------------------------------------
EnumerationType::~EnumerationType()
{
}

//----------------------------------------------------------------------------
Type::TypeIdType EnumerationType::GetTypeId() const
{
  return EnumerationTypeId;
}

//----------------------------------------------------------------------------
bool EnumerationType::CreateCxxType(cxx::TypeSystem* ts)
{
  // Make sure we haven't already created the type.
  if(m_CxxType.GetType())
    {
    return true;
    }
  
  if(m_Enumeration)
    {
    const cxx::EnumerationType* t = m_Enumeration->GetCxxEnumerationType(ts);
    if(t)
      {
      m_CxxType = t->GetCvQualifiedType(m_Const, m_Volatile);
      return true;
      }
    }
  cableErrorMacro("Couldn't create cxx::EnumerationType.");
  return false;
}

//----------------------------------------------------------------------------
Enumeration* EnumerationType::GetEnumeration() const
{
  return m_Enumeration;
}

//----------------------------------------------------------------------------
void EnumerationType::SetEnumeration(Enumeration* enumeration)
{
  m_Enumeration = enumeration;
}

} // namespace cable
