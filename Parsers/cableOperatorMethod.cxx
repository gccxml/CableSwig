/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableOperatorMethod.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableOperatorMethod.h"
#include "cableFunctionType.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
OperatorMethod::OperatorMethod()
{
}

//----------------------------------------------------------------------------
OperatorMethod::~OperatorMethod()
{
}

//----------------------------------------------------------------------------
Function::FunctionIdType OperatorMethod::GetFunctionId() const
{
  return OperatorMethodId;
}

//----------------------------------------------------------------------------
String OperatorMethod::GetCallName() const
{
  return String("operator")+this->GetName();
}

//----------------------------------------------------------------------------
void OperatorMethod::Print(std::ostream& os, Indent indent) const
{
  if(m_FunctionType && m_FunctionType->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_FunctionType->GetCxxType();
    cvType = cvType.GetMoreQualifiedType(m_Const, false);
    String declaration = cvType.GenerateDeclaration("operator"+m_Name);
    os << indent;
    if(m_Static) { os << "static "; }
    os << declaration.c_str() << ";\n";
    }
  else
    {
    os << indent
       << "/* broken operator method " << m_Name.c_str() << "; */\n";
    }
}

} // namespace cable
