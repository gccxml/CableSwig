/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableUnion.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableUnion.h"

namespace cable
{

//----------------------------------------------------------------------------
Union::Union()
{
}

//----------------------------------------------------------------------------
Union::~Union()
{
}

//----------------------------------------------------------------------------
void Union::Print(std::ostream& os, Indent indent) const
{
  os << indent << "union " << this->GetName() << "\n";
  Indent nextIndent = indent.GetNextIndent();
  os << nextIndent << "{\n";
  this->PrintMembers(os, nextIndent);
  os << nextIndent << "};\n";
}

} // namespace cable
