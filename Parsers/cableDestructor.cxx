/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableDestructor.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableDestructor.h"
#include "cableFunctionType.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
Destructor::Destructor()
{
}

//----------------------------------------------------------------------------
Destructor::~Destructor()
{
}

//----------------------------------------------------------------------------
Function::FunctionIdType Destructor::GetFunctionId() const
{
  return DestructorId;
}

//----------------------------------------------------------------------------
void Destructor::Print(std::ostream& os, Indent indent) const
{
  if(m_FunctionType && m_FunctionType->GetCxxType().GetType())
    {
    cxx::CvQualifiedType cvType = m_FunctionType->GetCxxType();
    String declaration = cvType.GenerateDeclaration(m_Name);
    // Strip off the first 5 characters ("void ").  Destructor has no
    // return type.
    os << indent << "~" << declaration.substr(5).c_str() << ";\n";
    }
  else
    {
    os << indent << "/* broken destructor " << m_Name.c_str() << "; */\n";
    }
}

} // namespace cable
