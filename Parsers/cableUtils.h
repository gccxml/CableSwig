/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableUtils.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableUtils_h
#define _cableUtils_h

// Include cxxUtils.h which does some work for us.
#include "cxxUtils.h"

#if defined(_WIN32) || defined(WIN32) /* Win32 version */
#  ifdef CableParsers_SHARED
#    ifdef CableParsers_EXPORTS
#      define CABLE_PARSERS_EXPORT __declspec(dllexport)
#    else
#      define CABLE_PARSERS_EXPORT __declspec(dllimport)
#    endif
#  else
#    define CABLE_PARSERS_EXPORT
#  endif
#else /* UNIX version */
#  define CABLE_PARSERS_EXPORT
#endif

// Visual C++ for-loop scoping hack.  We can use this inside the CABLE
// application without interfering with user code.
#ifdef _MSC_VER
#ifndef for
#define for if(false) {} else for
#endif
#endif

#define CABLE__LINE__AS_STRING CABLE__LINE__AS_STRING0(__LINE__)
#define CABLE__LINE__AS_STRING0(x) CABLE__LINE__AS_STRING1(x)
#define CABLE__LINE__AS_STRING1(x) #x

#include <iostream>

// Error reporting macro.
#define cableErrorMacro(x)                                              \
  {                                                                     \
    const Object* ptr = this;                                                 \
    std::cerr << "ERROR: In " __FILE__ ", line " CABLE__LINE__AS_STRING \
        << "\n" << this->GetNameOfClass() << " (" << ptr << "): "       \
        << x << "\n\n";                                                 \
  }

// Warning reporting macro.
#define cableWarningMacro(x)                                              \
  {                                                                       \
    const Object* ptr = this;                                                   \
    std::cerr << "WARNING: In " __FILE__ ", line " CABLE__LINE__AS_STRING \
        << "\n" << this->GetNameOfClass() << " (" << ptr << "): "         \
        << x << "\n\n";                                                   \
  }

// Macro to help define a new cable class.
#define cableTypeMacro(type, superclass)                \
  const char* GetNameOfClass() const { return #type; }  \
  static type* SafeDownCast(Object* obj)                \
    { return dynamic_cast<type*>(obj); }                \
  static const type* SafeDownCast(const Object* obj)    \
    { return dynamic_cast<const type*>(obj); }          \
  typedef type Self;                                    \
  typedef superclass Superclass;                        \
  typedef SmartPointer<Self> Pointer

// We don't have to worry about name conflicts in the cable
// application.
namespace cxx = _cxx_;

namespace cable
{

// Get the String type from cxxUtils.h.  We don't have to worry about
// a name conflict here.
typedef cxx::String String;

} // namespace cable

#endif
