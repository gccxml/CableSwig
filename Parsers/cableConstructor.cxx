/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableConstructor.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableConstructor.h"
#include "cableFunctionType.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
Constructor::Constructor()
{
}

//----------------------------------------------------------------------------
Constructor::~Constructor()
{
}

//----------------------------------------------------------------------------
Function::FunctionIdType Constructor::GetFunctionId() const
{
  return ConstructorId;
}

//----------------------------------------------------------------------------
void Constructor::Print(std::ostream& os, Indent indent) const
{
  if(m_FunctionType && m_FunctionType->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_FunctionType->GetCxxType();
    String declaration = cvType.GenerateDeclaration(m_Name);
    // Strip off the first 5 characters ("void ").  Constructor has no
    // return type.
    os << indent << declaration.substr(5).c_str() << ";\n";
    }
  else
    {
    os << indent << "/* broken constructor " << m_Name.c_str() << "; */\n";
    }
}

} // namespace cable
