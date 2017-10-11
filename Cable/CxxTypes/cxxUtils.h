/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxUtils.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxUtils_h
#define _cxxUtils_h

// Disable some Visual C++ warnings.
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4251 )
#endif

// Disable some Intel C++ Compiler warnings.
#ifdef __ICL
#pragma warning ( disable : 985 )
#endif

// Visual C++ for-loop scoping hack.
#if defined(_MSC_VER) && _MSC_VER < 1700
#ifndef for
#define for if(false) {} else for
#endif
#endif

#include <string>

/**
 * Setup a few utilities used by almost all source files.  There are some
 * differences between UNIX and Win32 platforms that are addressed here.
 */
#if defined(_WIN32) || defined(WIN32) /* Win32 version */

#  ifndef _cxx_STATIC
#    ifdef CxxTypes_EXPORTS
#      define _cxx_EXPORT __declspec(dllexport)
#    else
#      define _cxx_EXPORT __declspec(dllimport)
#    endif
#  else
#    define _cxx_EXPORT
#  endif

#  ifndef __CYGWIN__
#    define _cxx_char_traits char_traits
#  else
#    define _cxx_STATIC_ALLOCATOR_METHODS
#    define _cxx_char_traits string_char_traits
#  endif

#else /* UNIX version */

#  define _cxx_EXPORT

#endif


namespace _cxx_
{

typedef std::string  StringBase;

/** Wrapper around standard string class.  Keeps name short.  */
class String: public StringBase
{
public:
  typedef StringBase::value_type             value_type;
  typedef StringBase::pointer                pointer;
  typedef StringBase::reference              reference;
  typedef StringBase::const_reference        const_reference;
  typedef StringBase::size_type              size_type;
  typedef StringBase::difference_type        difference_type;
  typedef StringBase::iterator               iterator;
  typedef StringBase::const_iterator         const_iterator;
  typedef StringBase::reverse_iterator       reverse_iterator;
  typedef StringBase::const_reverse_iterator const_reverse_iterator;
  
  String(): StringBase() {}
  String(const value_type* s): StringBase(s) {}
  String(const value_type* s, size_type n): StringBase(s, n) {}
  String(const StringBase& s, size_type pos=0, size_type n=npos):
    StringBase(s, pos, n) {}
};

} // namespace _cxx_

#endif
