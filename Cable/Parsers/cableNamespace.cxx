/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableNamespace.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableNamespace.h"

namespace cable
{

//----------------------------------------------------------------------------
Namespace::Namespace()
{
}

//----------------------------------------------------------------------------
Namespace::~Namespace()
{
}

//----------------------------------------------------------------------------
bool Namespace::IsGlobalNamespace() const
{
  return (m_Name == "::") && !m_Context;
}

//----------------------------------------------------------------------------
void Namespace::Print(std::ostream& os, Indent indent) const
{
  if(!this->IsGlobalNamespace())
    {
    os << indent << "namespace " << this->GetName() << "\n";
    Indent nextIndent = indent.GetNextIndent();
    os << nextIndent << "{\n";
    this->PrintMembers(os, nextIndent);
    os << nextIndent << "}\n";
    }
  else
    {
    this->PrintMembers(os, indent);
    }
}

} // namespace cable
