/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableMethod.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableMethod.h"
#include "cableFunctionType.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
Method::Method()
{
  m_Static = false;
  m_Const = false;
}

//----------------------------------------------------------------------------
Method::~Method()
{
}

//----------------------------------------------------------------------------
Function::FunctionIdType Method::GetFunctionId() const
{
  return MethodId;
}

//----------------------------------------------------------------------------
bool Method::GetStatic() const
{
  return m_Static;
}

//----------------------------------------------------------------------------
void Method::SetStatic(bool s)
{
  m_Static = s;
}

//----------------------------------------------------------------------------
bool Method::GetConst() const
{
  return m_Const;
}

//----------------------------------------------------------------------------
void Method::SetConst(bool c)
{
  m_Const = c;
}  

//----------------------------------------------------------------------------
void Method::Print(std::ostream& os, Indent indent) const
{
  if(m_FunctionType && m_FunctionType->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_FunctionType->GetCxxType();
    cvType = cvType.GetMoreQualifiedType(m_Const, false);
    String declaration = cvType.GenerateDeclaration(m_Name);
    os << indent;
    if(m_Static) { os << "static "; }
    os << declaration.c_str() << ";\n";
    }
  else
    {
    os << indent << "/* broken method " << m_Name.c_str() << "; */\n";
    }
}

} // namespace cable
