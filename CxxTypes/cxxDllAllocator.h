/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxDllAllocator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxDllAllocator_h
#define _cxxDllAllocator_h

#ifndef _cxxUtils_DllAllocator_include
do_not_include_cxxDllAllocator_directly__use_cxxUtils;
#endif

#include <memory>

namespace _cxx_
{

template <typename T>
class DllAllocator;

/**
 * Special allocator for "void".
 */
template <> class DllAllocator<void>: public std::allocator<void> 
{
public:
  typedef std::allocator<void>::const_pointer const_pointer;
};

_cxx_EXPORT void* DllAllocate(size_t length);
_cxx_EXPORT void DllDeallocate(void* buffer, size_t);

/**
 * Define a replacement for the std::allocator class that is DLL-boundary
 * safe.  This is identical to the original class, except that it
 * re-implements the allocate and deallocate methods.  The new
 * implementations call non-templated allocate/deallocate functions that
 * exist in only one DLL.  This way the memory is always allocated and freed
 * on the heap for one DLL, and no boundary errors are created.
 */
template <typename T>
class DllAllocator: public std::allocator<T>
{
public:
  /**
   * Easy access to the allocator subclass from which we are taking most
   * of the implementation.
   */
  typedef std::allocator<T>  StdAllocator;

  typedef typename StdAllocator::pointer pointer;

  /**
   * Default constructor just invokes the real allocator's default constructor.
   */
  DllAllocator(): StdAllocator() {}
  
  /**
   * Copy constructor just invokes the real allocator's copy constructor.
   */
  DllAllocator(const DllAllocator& a): StdAllocator(a) {}

#ifndef _cxx_STATIC_ALLOCATOR_METHODS
  /**
   * Re-implement the allocate() method to be sure to allocate memory on only
   * one DLL's heap.
   */
  pointer allocate(size_type n, DllAllocator<void>::const_pointer = 0)
    {
      return (pointer)DllAllocate(n*sizeof(T));
    }
  
  /**
   * Re-implement the deallocate() method to free memory allocated by the
   * re-implementation of the allocate() mehtod.
   */
  void deallocate(pointer p, size_type n)
    {
      DllDeallocate(p, n);
    }
#else
  /**
   * Re-implement the allocate() method to be sure to allocate memory on only
   * one DLL's heap.
   */
  static pointer allocate(size_type n, DllAllocator<void>::const_pointer = 0)
    {
      return (pointer)DllAllocate(n*sizeof(T));
    }

  /**
   * Re-implement the deallocate() method to free memory allocated by the
   * re-implementation of the allocate() mehtod.
   */
  static void deallocate(void* p, size_type n)
    {
      DllDeallocate(p, n);
    }
#endif
};

} // namespace _cxx_
  
#endif
