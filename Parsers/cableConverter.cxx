/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableConverter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableConverter.h"
#include "cableFunctionType.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
Converter::Converter()
{
}

//----------------------------------------------------------------------------
Converter::~Converter()
{
}

//----------------------------------------------------------------------------
Function::FunctionIdType Converter::GetFunctionId() const
{
  return ConverterId;
}

//----------------------------------------------------------------------------
void Converter::Print(std::ostream& os, Indent indent) const
{
  if(m_FunctionType && m_FunctionType->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_FunctionType->GetCxxType();
    cvType = cvType.GetMoreQualifiedType(m_Const, false);
    String declaration = cvType.GenerateDeclaration(" ");
    os << indent << "operator " << declaration.c_str() << ";\n";
    }
  else
    {
    os << indent
       << "/* broken converter " << m_Name.c_str() << "; */\n";
    }
}

} // namespace cable
