/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableOperatorFunction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableOperatorFunction.h"
#include "cableFunctionType.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
OperatorFunction::OperatorFunction()
{
}

//----------------------------------------------------------------------------
OperatorFunction::~OperatorFunction()
{
}

//----------------------------------------------------------------------------
Function::FunctionIdType OperatorFunction::GetFunctionId() const
{
  return OperatorFunctionId;
}

//----------------------------------------------------------------------------
String OperatorFunction::GetCallName() const
{
  return String("operator ")+this->GetName();
}

//----------------------------------------------------------------------------
void OperatorFunction::Print(std::ostream& os, Indent indent) const
{
  if(m_FunctionType && m_FunctionType->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_FunctionType->GetCxxType();
    String declaration = cvType.GenerateDeclaration("operator "+m_Name);
    os << indent << declaration.c_str() << ";\n";
    }
  else
    {
    os << indent
       << "/* broken operator function " << m_Name.c_str() << "; */\n";
    }
}

} // namespace cable
