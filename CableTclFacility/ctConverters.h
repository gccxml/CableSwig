/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctConverters.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctConverters_h
#define _ctConverters_h

#include "ctAnything.h"

namespace _cable_tcl_
{

namespace Converter
{

// Conversion functions returning objects:


/**
 * A conversion function for a derived-to-base object conversion.
 */
template <typename From, typename To>
struct ObjectDerivedToBase
{
  static To Convert(Anything in)
    {
    return static_cast<To>(*reinterpret_cast<From*>(in.object));
    }
  inline static ConversionFunction GetConversionFunction()
    {
    return reinterpret_cast<ConversionFunction>(&ObjectDerivedToBase::Convert);
    }
};


/**
 * A conversion function for calling a type conversion operator.
 */
template <typename From, typename To>
struct ConversionOperator
{
  static To Convert(Anything in)
    {
    return reinterpret_cast<From*>(in.object)->operator To();
    }
  inline static ConversionFunction GetConversionFunction()
    {
    return reinterpret_cast<ConversionFunction>(&ConversionOperator::Convert);
    }
};


/**
 * A conversion function for performing conversion by constructor.
 */
template <typename From, typename To>
struct ConversionByConstructor
{
  static To Convert(Anything in)
    {
    return To(*reinterpret_cast<From*>(in.object));
    }
  inline static ConversionFunction GetConversionFunction()
    {
    return reinterpret_cast<ConversionFunction>(&ConversionByConstructor::Convert);
    }
};


/**
 * A conversion function for performing a reinterpret_cast on an object.
 */
template <typename From, typename To>
struct ObjectReinterpret
{
  static To Convert(Anything in)
    {
    return reinterpret_cast<To>(*reinterpret_cast<From*>(in.object));
    }
  inline static ConversionFunction GetConversionFunction()
    {
    return reinterpret_cast<ConversionFunction>(&ObjectReinterpret::Convert);
    }
};


/**
 * A bug in GCC 2.95.3 prevents static_cast from adding a const like this:
 * void* v; const int* i = static_cast<const int*>(v);
 * However, we can't use a const_cast to add it because it is not allowed
 * when we aren't really adding a const.  This class creates an inline
 * function to add a const_cast if T is a const type.
 */
namespace CastHack
{
  template <typename> struct Caster { inline static void* Cast(void* v) { return v; } };
#ifdef _cable_tcl_CONST_CAST_HACK
  template <typename T> struct Caster<const T> { inline static const void* Cast(void* v) { return const_cast<const void*>(v); } };
#endif
} // namespace CastHack


// Conversion functions returning pointers:

/**
 * A conversion function for a derived-to-base pointer conversion.
 */
template <typename From, typename To>
struct PointerDerivedToBase
{
  static To* Convert(Anything in)
    {
    return static_cast<To*>(reinterpret_cast<From*>(in.object));
    }
  inline static ConversionFunction GetConversionFunction()
    {
    return reinterpret_cast<ConversionFunction>(&PointerDerivedToBase::Convert);
    }
};


// Conversion functions returning references.  Since pointer and
// reference casting are effectively the same, we use the pointer
// implementations to save space.  The input is a void* anyway, the
// reference implementations would just have an extra dereference.
// Instead, we move this dereference to the ArgumentAsReferenceTo and
// ArgumentAsReferenceTo_const implementations in wrapCalls.  For
// clarity, we still want to use the reference names, though.
#define ReferenceDerivedToBase PointerDerivedToBase

} // namespace Converter

} // namespace _cable_tcl_

#endif
