/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctArgument.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctArgument.h"
#include "ctTypeInfo.h"

namespace _cable_tcl_
{

/**
 * Constructor just sets argument as uninitialized.
 */
Argument::Argument():
  m_ArgumentId(Uninitialized_id)
{
}


/**
 * Copy constructor ensures m_Anything still points to the right place.
 */
Argument::Argument(const Argument& a):
  m_Anything(a.m_Anything),
  m_Type(a.m_Type),
  m_ArgumentId(a.m_ArgumentId),
  m_Temp(a.m_Temp)
{
  // Make sure m_Anything points to the right place in the copy.
  switch (m_ArgumentId)
    {
    case bool_id:    m_Anything.object = &m_Temp.m_bool; break;
    case int_id:     m_Anything.object = &m_Temp.m_int; break;
    case long_id:    m_Anything.object = &m_Temp.m_long; break;
    case double_id:  m_Anything.object = &m_Temp.m_double; break;
    case Object_id:
    case Pointer_id:
    case Function_id:
    case Uninitialized_id:
    default: break;
    }
}


/**
 * Assignment operator just duplicates functionality of copy constructor.
 */
Argument& Argument::operator=(const Argument& a)
{
  // Copy the values.
  m_Anything = a.m_Anything;
  m_Type = a.m_Type;
  m_ArgumentId = a.m_ArgumentId;
  m_Temp = a.m_Temp;
  
  // Make sure m_Anything points to the right place in the copy.
  switch (m_ArgumentId)
    {
    case bool_id:    m_Anything.object = &m_Temp.m_bool; break;
    case int_id:     m_Anything.object = &m_Temp.m_int; break;
    case long_id:    m_Anything.object = &m_Temp.m_long; break;
    case double_id:  m_Anything.object = &m_Temp.m_double; break;
    case Object_id:
    case Pointer_id:
    case Function_id:
    case Uninitialized_id:
    default: break;
    }
  
  return *this;
}


/**
 * Get the value of the Argument for passing to the conversion function.
 */
Anything Argument::GetValue() const
{
  // TODO: Throw exception for uninitalized argument.
  return m_Anything;
}


/**
 * Get the type of the Argument for selecting a conversion function.
 */
const CvQualifiedType& Argument::GetType() const
{
  // TODO: Throw exception for uninitalized argument.
  return m_Type;
}


/**
 * Set the type of the Argument for selecting a conversion function.
 * This should only be used to dereference the implicit object argument.
 */
void Argument::SetType(const CvQualifiedType& type)
{
  m_Type = type;
}


/**
 * Set the Argument to be the object pointed to by the given pointer.
 */
void Argument::SetToObject(ObjectType object,
                           const CvQualifiedType& type)
{
  m_Anything.object = object;
  m_Type = type;
  m_ArgumentId = Object_id;
}


/**
 * Set the Argument to be the given bool value.
 */
void Argument::SetToBool(bool b)
{
  m_Temp.m_bool = b;
  m_Anything.object = &m_Temp.m_bool;
  m_Type = CvPredefinedType<bool>::type;
  m_ArgumentId = bool_id;
}


/**
 * Set the Argument to be the given int value.
 */
void Argument::SetToInt(int i)
{
  m_Temp.m_int = i;
  m_Anything.object = &m_Temp.m_int;
  m_Type = CvPredefinedType<int>::type;
  m_ArgumentId = int_id;
}


/**
 * Set the Argument to be the given long value.
 */
void Argument::SetToLong(long l)
{
  m_Temp.m_long = l;
  m_Anything.object = &m_Temp.m_long;
  m_Type = CvPredefinedType<long>::type;
  m_ArgumentId = long_id;
}


/**
 * Set the Argument to be the given double value.
 */
void Argument::SetToDouble(double d)
{
  m_Temp.m_double = d;
  m_Anything.object = &m_Temp.m_double;
  m_Type = CvPredefinedType<double>::type;
  m_ArgumentId = double_id;
}


/**
 * Set the Argument to be the given pointer value.
 */
void Argument::SetToPointer(ObjectType v,
                            const CvQualifiedType& pointerType)
{
  m_Anything.object = v;
  m_Type = pointerType;
  m_ArgumentId = Pointer_id;
}


/**
 * Set the Argument to be the given function pointer value.
 */
void Argument::SetToFunction(FunctionType f,
                             const CvQualifiedType& functionPointerType)
{
  m_Anything.function = f;
  m_Type = functionPointerType;
  m_ArgumentId = Function_id;
}

} // namespace _cable_tcl_
