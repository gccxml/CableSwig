/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableNamespaceAlias.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableNamespaceAlias.h"
#include "cableNamespace.h"

namespace cable
{

//----------------------------------------------------------------------------
NamespaceAlias::NamespaceAlias()
{
  m_Namespace = 0;
}

//----------------------------------------------------------------------------
NamespaceAlias::~NamespaceAlias()
{
}

//----------------------------------------------------------------------------
void NamespaceAlias::Print(std::ostream& os, Indent indent) const
{
  os << indent << "namespace " << this->GetName();
  if(m_Namespace)
    {
    os << " = " << m_Namespace->GetQualifiedName().c_str();
    }
  else
    {
    os << " /* broken alias */ ";
    }
  os << ";\n";
}

//----------------------------------------------------------------------------
Namespace* NamespaceAlias::GetNamespace() const
{
  return m_Namespace;
}

//----------------------------------------------------------------------------
void NamespaceAlias::SetNamespace(Namespace* ns)
{
  m_Namespace = ns;
}

//----------------------------------------------------------------------------
void NamespaceAlias::AddNamed(Named* named, Access access)
{
  if(m_Namespace)
    {
    m_Namespace->AddNamed(named, access);
    }
  else
    {
    cableErrorMacro("Cannot add " << named->GetNameOfClass()
                    << " to broken namespace alias.");
    }
}

} // namespace cable
