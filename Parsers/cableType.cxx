/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableType.h"
#include "cxxCvQualifiedType.h"

namespace cable
{

//----------------------------------------------------------------------------
Type::Type():
  m_CxxType(*(new cxx::CvQualifiedType))
{
  m_Const = false;
  m_Volatile = false;
}

//----------------------------------------------------------------------------
Type::~Type()
{
  delete &m_CxxType;
}

//----------------------------------------------------------------------------
const cxx::CvQualifiedType& Type::GetCxxType() const
{
  return m_CxxType;
}

//----------------------------------------------------------------------------
bool Type::GetConst() const
{
  return m_Const;
}

//----------------------------------------------------------------------------
void Type::SetConst(bool c) 
{
  m_Const = c;
}

//----------------------------------------------------------------------------
bool Type::GetVolatile() const
{
  return m_Volatile;
}

//----------------------------------------------------------------------------
void Type::SetVolatile(bool v)
{
  m_Volatile = v;
}

//----------------------------------------------------------------------------
void Type::Print(std::ostream& os, Indent indent) const
{
  const cxx::Type* type = m_CxxType.GetType();
  String name = "<unknown>";
  
  if(type) { name = m_CxxType.GetName(); }
  
  os << indent << "/* type " << name.c_str() << " */\n";
}

} // namespace cable
