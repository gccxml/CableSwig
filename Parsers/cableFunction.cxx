/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableFunction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableFunction.h"
#include "cableFunctionType.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
Function::Function()
{
  m_FunctionType = 0;
}

//----------------------------------------------------------------------------
Function::~Function()
{
}

//----------------------------------------------------------------------------
Function::FunctionIdType Function::GetFunctionId() const
{
  return FunctionId;
}

//----------------------------------------------------------------------------
FunctionType* Function::GetFunctionType() const
{
  return m_FunctionType;
}

//----------------------------------------------------------------------------
void Function::SetFunctionType(FunctionType* type)
{
  m_FunctionType = type;
}

//----------------------------------------------------------------------------
void Function::Print(std::ostream& os, Indent indent) const
{
  if(m_FunctionType && m_FunctionType->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_FunctionType->GetCxxType();
    String declaration = cvType.GenerateDeclaration(m_Name);
    os << indent << declaration.c_str() << ";\n";
    }
  else
    {
    os << indent << "/* broken function " << m_Name.c_str() << "; */\n";
    }
}

//----------------------------------------------------------------------------
String Function::GetCallName() const
{
  return this->GetName();
}

} // namespace cable
