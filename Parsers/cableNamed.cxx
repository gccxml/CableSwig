/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableNamed.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableNamed.h"
#include "cableContext.h"

namespace cable
{

//----------------------------------------------------------------------------
Named::Named()
{
  m_Line = 0;
  m_Context = 0;
}

//----------------------------------------------------------------------------
Named::~Named()
{
}

//----------------------------------------------------------------------------
const char* Named::GetName() const
{
  return m_Name.c_str();
}

//----------------------------------------------------------------------------
void Named::SetName(const char* name)
{
  m_Name = name;
}

//----------------------------------------------------------------------------
const char* Named::GetFile() const
{
  return m_File.c_str();
}

//----------------------------------------------------------------------------
void Named::SetFile(const char* file)
{
  m_File = file;
}

//----------------------------------------------------------------------------
unsigned long Named::GetLine() const
{
  return m_Line;
}

//----------------------------------------------------------------------------
void Named::SetLine(unsigned long line)
{
  m_Line = line;
}

//----------------------------------------------------------------------------
Context* Named::GetContext() const
{
  return m_Context;
}

//----------------------------------------------------------------------------
void Named::SetContext(Context* context)
{
  m_Context = context;
}

//----------------------------------------------------------------------------
String Named::GetQualifiedName() const
{
  if(m_Context)
    {
    String name = m_Context->GetQualifiedName();
    if(name == "::")
      {
      name = "";
      }
    else
      {
      name += "::";
      }
    name += m_Name;
    return name;
    }
  else
    {
    return m_Name;
    }
}

} // namespace cable
