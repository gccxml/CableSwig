/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableStruct.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableStruct.h"

namespace cable
{

//----------------------------------------------------------------------------
Struct::Struct()
{
}

//----------------------------------------------------------------------------
Struct::~Struct()
{
}

//----------------------------------------------------------------------------
void Struct::Print(std::ostream& os, Indent indent) const
{
  os << indent << "struct " << this->GetName() << "\n";
  Indent nextIndent = indent.GetNextIndent();
  os << nextIndent << "{\n";
  this->PrintMembers(os, nextIndent);
  os << nextIndent << "};\n";
}

} // namespace cable
