/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableTypedef.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableTypedef.h"
#include "cableType.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
Typedef::Typedef()
{
  m_Type = 0;
}

//----------------------------------------------------------------------------
Typedef::~Typedef()
{
}

//----------------------------------------------------------------------------
Type* Typedef::GetType() const
{
  return m_Type;
}

//----------------------------------------------------------------------------
void Typedef::SetType(Type* type)
{
  m_Type = type;
}

//----------------------------------------------------------------------------
void Typedef::Print(std::ostream& os, Indent indent) const
{
  if(m_Type && m_Type->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_Type->GetCxxType();
    String declaration = cvType.GenerateDeclaration(m_Name);
    os << indent << "typedef " << declaration.c_str() << ";\n";
    }
  else
    {
    os << indent << "/* broken typedef " << m_Name.c_str() << "; */\n";
    }
}

} // namespace cable
