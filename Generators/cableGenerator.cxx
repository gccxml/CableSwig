/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableGenerator.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableGenerator.h"

#include <fstream>

namespace cable
{

//----------------------------------------------------------------------------
Generator::Generator()
{
  m_Stream = 0;
}

//----------------------------------------------------------------------------
Generator::~Generator()
{
}

//----------------------------------------------------------------------------
SourceRepresentation* Generator::GetSourceRepresentation() const
{
  return m_SourceRepresentation;
}

//----------------------------------------------------------------------------
void Generator::SetSourceRepresentation(SourceRepresentation* sr)
{
  m_SourceRepresentation = sr;
}

//----------------------------------------------------------------------------
std::ostream* Generator::GetStream() const
{
  return m_Stream;
}

//----------------------------------------------------------------------------
void Generator::SetStream(std::ostream* stream)
{
  m_Stream = stream;
}

//----------------------------------------------------------------------------
bool Generator::Generate()
{
  if(!m_Stream)
    {
    cableErrorMacro("Generate called with no Stream set.");
    return false;
    }
  if(!m_SourceRepresentation)
    {
    cableErrorMacro("Generate called with no SourceRepresentation set.");
    return false;
    }
  
  // Generate the wrappers.
  return this->GenerateWrappers();
}

//----------------------------------------------------------------------------
const char* Generator::GetOperatorName(const char* name) const
{
  // Try to translate an operator name into a valid C identifier.  If
  // there is no translation, the empty string is returned.
  String n = name;
  if(n == "=") { return "assign"; }
  else if(n == "+") { return "plus"; }
  else if(n == "+=") { return "aplus"; }
  else if(n == "-") { return "minus"; }
  else if(n == "-=") { return "aminus"; }
  else if(n == "*") { return "multiply"; }
  else if(n == "*=") { return "amultiply"; }
  else if(n == "/") { return "divide"; }
  else if(n == "/=") { return "adivide"; }
  return "";
}

} // namespace cable
