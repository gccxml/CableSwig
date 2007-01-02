/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableSourceObject.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableSourceObject.h"

namespace cable
{

//----------------------------------------------------------------------------
SourceObject::SourceObject()
{
}

//----------------------------------------------------------------------------
SourceObject::~SourceObject()
{
}

//----------------------------------------------------------------------------
const char* SourceObject::GetAttributes() const
{
  return m_Attributes.c_str();
}

//----------------------------------------------------------------------------
void SourceObject::SetAttributes(const char* atts)
{
  if (atts)
    {
    m_Attributes = atts;
    }
  else
    {
    m_Attributes = "";
    }
}

} // namespace cable
