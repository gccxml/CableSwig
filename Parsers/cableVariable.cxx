/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableVariable.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableVariable.h"
#include "cableType.h"
#include "cableContext.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
Variable::Variable()
{
  m_Type = 0;
}

//----------------------------------------------------------------------------
Variable::~Variable()
{
}

//----------------------------------------------------------------------------
Type* Variable::GetType() const
{
  return m_Type;
}

//----------------------------------------------------------------------------
void Variable::SetType(Type* type)
{
  m_Type = type;
}

//----------------------------------------------------------------------------
const char* Variable::GetInitializer() const
{
  return m_Initializer.c_str();
}

//----------------------------------------------------------------------------
void Variable::SetInitializer(const char* init)
{
  m_Initializer = init;
}

//----------------------------------------------------------------------------
void Variable::Print(std::ostream& os, Indent indent) const
{
  if(m_Type && m_Type->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_Type->GetCxxType();
    String declaration = cvType.GenerateDeclaration(m_Name);
    os << indent;
    if(m_Context && (String(m_Context->GetNameOfClass()) != "Namespace"))
      {
      os << "static ";
      }
    os << declaration.c_str();
    if(m_Initializer.length() > 0)
      {
      os << " = " << m_Initializer.c_str();
      }
    os << ";\n";
    }
  else
    {
    os << indent << "/* broken variable " << m_Name.c_str();
    if(m_Initializer.length() > 0)
      {
      os << " = " << m_Initializer.c_str();
      }
    os << "; */\n";
    }
}

} // namespace cable
