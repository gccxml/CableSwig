/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxDllAllocator.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxUtils.h"

namespace _cxx_
{

/**
 * Allocate "length" bytes of memory, and return a pointer to it.
 */
void* DllAllocate(size_t length)
{
  return ((void*)(new char[length]));
}


/**
 * Free "length" bytes of memory.
 */
void DllDeallocate(void* buffer, size_t)
{
  delete [] ((char*)buffer);
}
  
} // namespace _cxx_
