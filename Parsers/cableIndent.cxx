/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableIndent.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableIndent.h"

namespace cable
{

//----------------------------------------------------------------------------
Indent::Indent(): m_Indent(0)
{
}

//----------------------------------------------------------------------------
Indent::Indent(unsigned int indent): m_Indent(indent)
{
}

//----------------------------------------------------------------------------
Indent Indent::GetNextIndent() const
{
  return (m_Indent+2);
}

//----------------------------------------------------------------------------
void Indent::Print(std::ostream& os) const
{
  if(!m_Indent) { return; }
  
  // Use blocks of 8 spaces to speed up big indents.
  unsigned int blockCount = m_Indent >> 3;
  unsigned int singleCount = m_Indent & 7;
  while(blockCount-- > 0)
    {
    os << "        ";
    }
  while(singleCount-- > 0)
    {
    os << " ";
    }
}

//----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Indent& indent)
{
  indent.Print(os);
  return os;
}

} // namespace cable

