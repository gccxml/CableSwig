/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableObject.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableObject.h"

namespace cable
{

//----------------------------------------------------------------------------
Object::Object(): m_ReferenceCount(0)
{
}

//----------------------------------------------------------------------------
Object::~Object()
{
}

//----------------------------------------------------------------------------
void Object::Register()
{
  if(++m_ReferenceCount < 0)
    {
    delete this;
    }
}
  
//----------------------------------------------------------------------------
void Object::Unregister()
{
  if(--m_ReferenceCount < 0)
    {
    delete this;
    }
}

} // namespace cable
