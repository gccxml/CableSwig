/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctCalls.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctCalls_txx
#define _ctCalls_txx

namespace _cable_tcl_
{


/**
 * Convert the given Argument to an object of T.
 */
template <typename T>
T ArgumentAsInstanceOf<T>::operator()(const Argument& argument)
{
  // Try to find a conversion.
  if(this->FindConversionFunction(argument.GetType()))
    {
    // No conversion is needed.  It is safe do to an identity cast.
    return *reinterpret_cast<T*>(argument.GetValue().object);
    }    
  
  // Perform the conversion and return the result.
  return (reinterpret_cast<T(*)(Anything)>(m_ConversionFunction))(argument.GetValue());
}


/**
 * Convert the given Argument to a pointer to T.
 */
template <typename T>
T* ArgumentAsPointerTo<T>::operator()(const Argument& argument)
{
  // Try to find a conversion.
  if(this->FindConversionFunction(argument.GetType()))
    {
    // No conversion is needed.  It is safe do to an identity cast.
    return reinterpret_cast<T*>(argument.GetValue().object);
    }
    
  // Perform the conversion and return the result.
  return (reinterpret_cast<T*(*)(Anything)>(m_ConversionFunction))(argument.GetValue());
}


/**
 * Convert the given Argument to a pointer to T.  This version can optionally
 * point to an array of T.
 */
template <typename T>
T* ArgumentAsPointerTo_array<T>::operator()(const Argument& argument)
{
  CvQualifiedType from = argument.GetType();
  
  // Try to find a conversion.
  if(this->FindConversionFunction(from))
    {
    // No conversion is needed.  It is safe do to an identity cast.
    return reinterpret_cast<T*>(argument.GetValue().object);
    }
  else if(m_NeedArray)
    {
    const Argument* elements = reinterpret_cast<const Argument*>(argument.GetValue().object);
    unsigned long length = ArrayType::SafeDownCast(from.GetType())->GetLength();
    m_Array = new NoCvT[length];
    for(unsigned int i = 0; i < length; ++i)
      {
      m_Array[i] = ElementFor(m_WrapperFacility)(elements[i]);
      }
    return m_Array;
    }
    
  // Perform the conversion and return the result.
  return (reinterpret_cast<T*(*)(Anything)>(m_ConversionFunction))(argument.GetValue());
}


/**
 * Convert the given Argument to a pointer to function T.
 */
template <typename T>
T* ArgumentAsPointerToFunction<T>::operator()(const Argument& argument)
{
  // Try to find a conversion.
  if(this->FindConversionFunction(argument.GetType()))
    {
    // No conversion is needed.  It is safe do to an identity cast.
    return reinterpret_cast<T*>(argument.GetValue().function);
    }
    
  // Perform the conversion and return the result.
  return (reinterpret_cast<T*(*)(Anything)>(m_ConversionFunction))(argument.GetValue());
}


/**
 * Convert the given Argument to a reference to T.
 */
template <typename T>
T& ArgumentAsReferenceTo<T>::operator()(const Argument& argument)
{
  // Try to find a conversion.
  if(this->FindConversionFunction(argument.GetType()))
    {
    // No conversion is needed.  It is safe do to an identity cast.
    return *reinterpret_cast<T*>(argument.GetValue().object);
    }
    
  // Perform the conversion and return the result.  Note that
  // conversion functions that are supposed to return a reference
  // actually return a pointer since the only difference in their
  // implementations would be an extra dereference.  We instead add
  // the dereference here.
  return *((reinterpret_cast<T*(*)(Anything)>(m_ConversionFunction))(argument.GetValue()));
}


/**
 * A function object to convert an Argument to a reference to const T.
 * In this case, a temporary may be constructed that must persist throughout
 * the call to a wrapped function.
 */
template <typename T>
const T& ArgumentAsReferenceTo_const<T>::operator()(const Argument& argument)
{
  // Try to find a conversion.
  if(this->FindConversionFunction(argument.GetType()))
    {
    // No conversion is needed.  It is safe do to an identity cast.
    return *reinterpret_cast<T*>(argument.GetValue().object);
    }
    
  if(m_NeedTemporary)
    {
    // Allocate a temporary instance with a copy-construction of
    // the result.        
    m_Temporary = new T((reinterpret_cast<T(*)(Anything)>(m_ConversionFunction))(argument.GetValue()));
    return *m_Temporary;
    }

  // Perform the conversion and return the result.  Note that
  // conversion functions that are supposed to return a reference
  // actually return a pointer since the only difference in their
  // implementations would be an extra dereference.  We instead add
  // the dereference here.
  return *((reinterpret_cast<const T*(*)(Anything)>(m_ConversionFunction))(argument.GetValue()));
}


} // namespace _cable_tcl_

#endif
