/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableField.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableField.h"
#include "cableType.h"
#include "cableContext.h"
#include "cxxCvQualifiedType.h"
#include <stdlib.h>

namespace cable
{

//----------------------------------------------------------------------------
Field::Field()
{
  this->m_Offset = 0;
}

//----------------------------------------------------------------------------
Field::~Field()
{
}

//----------------------------------------------------------------------------
void Field::ParseOffset(const char* offset)
{
  if (offset)
    {
    this->SetOffset(atoi(offset));
    }
  else
    {
    this->SetOffset(0);
    }
}

//----------------------------------------------------------------------------
int Field::GetOffset(void) const
{
  return this->m_Offset;
}

//----------------------------------------------------------------------------
void Field::SetOffset(int offset)
{
  this->m_Offset = offset;
}

//----------------------------------------------------------------------------
void Field::Print(std::ostream& os, Indent indent) const
{
  if(m_Type && m_Type->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_Type->GetCxxType();
    String declaration = cvType.GenerateDeclaration(m_Name);
    os << indent << declaration.c_str() << ";\n";
    }
  else
    {
    os << indent << "/* broken field " << m_Name.c_str() << "; */\n";
    }
}

} // namespace cable
