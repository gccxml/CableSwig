/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctUtils.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctUtils_h
#define _ctUtils_h

// Include the C++ type representation classes.
// This also includes cxxUtils.h which does some work for us.
#include "CxxTypes/cxxTypeSystem.h"
#include "CxxTypes/cxxConversions.h"

// Force debug support on for now.
#define _cable_tcl_DEBUG_SUPPORT

/**
 * Setup a few utilities used by almost all source files.  There are some
 * differences between UNIX and Win32 platforms that are addressed here.
 */
#if defined(_WIN32) || defined(WIN32) /* Win32 version */
#  ifndef _cable_tcl_STATIC
#    ifdef CableTclFacility_EXPORTS
#      define _cable_tcl_EXPORT __declspec(dllexport)
#    else
#      define _cable_tcl_EXPORT __declspec(dllimport)
#    endif
#  else
#    define _cable_tcl_EXPORT
#  endif
#  define CABLE_TCL_WRAPPER_EXPORT __declspec(dllexport)
#else /* UNIX version */
#  define _cable_tcl_EXPORT
#  define CABLE_TCL_WRAPPER_EXPORT
#endif

// Visual C++ can't distinguish void types with different cv qualifiers.
#ifdef _MSC_VER
#define _cable_tcl_NO_CV_VOID
#endif

// Both Visual C++ and the Intel C/C++ compiler define wchar_t as
// unsigned short.  Type information will fall through to unsinged short
// if wchar_t is used, so we can't let the information be duplicated.
#if defined(_MSC_VER) || defined(__ICL)
#define _cable_tcl_NO_WCHAR_T
#endif

// GCC and MipsPro won't let static_cast add a const qualifier.
#if defined(__GNUC__) || defined(__sgi)
#define _cable_tcl_CONST_CAST_HACK
#endif

// Include Tcl headers.
#include <tcl.h>

namespace _cable_tcl_
{
// Some functions to make Tcl type checking easy.
_cable_tcl_EXPORT bool TclObjectTypeIsNULL(Tcl_Obj*);
_cable_tcl_EXPORT bool TclObjectTypeIsBoolean(Tcl_Obj*);
_cable_tcl_EXPORT bool TclObjectTypeIsInt(Tcl_Obj*);
_cable_tcl_EXPORT bool TclObjectTypeIsLong(Tcl_Obj*);
_cable_tcl_EXPORT bool TclObjectTypeIsDouble(Tcl_Obj*);
_cable_tcl_EXPORT bool TclObjectTypeIsString(Tcl_Obj*);
_cable_tcl_EXPORT bool TclObjectTypeIsCmdName(Tcl_Obj*);

/**
 * The String type defined in the _cxx_ namespace.
 */
typedef ::_cxx_::String String;

/*@{
 * Pull this representation type out of _cxx_ namespace into _cable_tcl_ namespace.
 */
typedef ::_cxx_::Type                 Type;
  
typedef ::_cxx_::ArrayType            ArrayType;
typedef ::_cxx_::ClassType            ClassType;
typedef ::_cxx_::FunctionType         FunctionType;
typedef ::_cxx_::FundamentalType      FundamentalType;
typedef ::_cxx_::PointerType          PointerType;
typedef ::_cxx_::PointerToMemberType  PointerToMemberType;
typedef ::_cxx_::ReferenceType        ReferenceType;

typedef ::_cxx_::CvQualifiedType      CvQualifiedType;
typedef ::_cxx_::TypeSystem           TypeSystem;

typedef ::_cxx_::CvQualifiedTypes     CvQualifiedTypes;
typedef ::_cxx_::ClassTypes           ClassTypes;
  
typedef ::_cxx_::Conversions          Conversions;
//@}

  
/**
 * Comparison function object for sorting based on void pointer type.
 */
struct VoidPointerCompare
{
  bool operator()(const void* l, const void* r) const
    {
    return (reinterpret_cast<unsigned long>(l)
            < reinterpret_cast<unsigned long>(r));
    }
};
  
} // namespace _cable_tcl_

// Setup debug support if it is on.
#ifdef _cable_tcl_DEBUG_SUPPORT
#  if !defined(CMAKE_NO_ANSI_STRING_STREAM)
#    include <sstream>
#  elif !defined(CMAKE_NO_ANSI_STREAM_HEADERS)
#    include <strstream>
#    define CABLE_NO_ANSI_STRING_STREAM
#  else
#    include <strstream.h>
#    define CABLE_NO_ANSI_STRING_STREAM
#  endif
#  ifndef CABLE_NO_ANSI_STRING_STREAM
#    define _cable_tcl_DEBUG_OUTPUT(wf, x)        \
    { if((wf)->DebugIsOn())                       \
        { std::ostringstream msg;                 \
          msg << x;                               \
          wf->OutputDebugText(msg.str().c_str()); } }
#  else
#    define _cable_tcl_DEBUG_OUTPUT(wf, x)        \
    { if((wf)->DebugIsOn())                       \
        { std::ostrstream msg;                    \
          msg << x << std::ends;                  \
          wf->OutputDebugText(msg.str());         \
          msg.rdbuf()->freeze(0); } }
#  endif
#else
#  define _cable_tcl_DEBUG_OUTPUT(wf, x)
#endif

#endif
