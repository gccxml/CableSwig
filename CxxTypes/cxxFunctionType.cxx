/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxFunctionType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxFunctionType.h"

namespace _cxx_
{

/**
 * Retrieve what kind of Type this is.
 */
RepresentationType FunctionType::GetRepresentationType() const
{
  return FunctionType_id;
}


/**
 * Try to cast the given Type to an FunctionType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
FunctionType* FunctionType::SafeDownCast(Type* t)
{
  FunctionType* result = dynamic_cast<FunctionType*>(t);
  if(!result) { throw TypeDownCastException(t, FunctionType_id); }
  return result;
}


/**
 * Try to cast the given Type to an FunctionType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
const FunctionType* FunctionType::SafeDownCast(const Type* t)
{
  const FunctionType* result = dynamic_cast<const FunctionType*>(t);
  if(!result) { throw TypeDownCastException(t, FunctionType_id); }
  return result;
}


String FunctionType::GenerateName(const String& outerType,
                                  bool isConst, bool) const
{
  String arguments = this->GenerateArgumentString();
  String outerString = "("+outerType+")( "+arguments+" )"+this->GetRightCvString(isConst, false);
  return m_ReturnType.GenerateName(outerString);
}


/**
 * Get the name of the type as it would be used in a declaration with the
 * given name.
 */
String FunctionType::GenerateDeclaration(const String& name,
                                         bool isConst, bool) const
{
  String arguments = this->GenerateArgumentString();
  String outerString;
  if(name.length() > 0)
    {
    outerString = name+"("+arguments+")"+this->GetRightCvString(isConst, false);
    }
  else
    {
#ifndef _MSC_VER
    outerString = "(("+arguments+"))"+this->GetRightCvString(isConst, false);
#else
    outerString = "__cdecl("+arguments+")"+this->GetRightCvString(isConst, false);
#endif
    }
  return m_ReturnType.GenerateName(outerString);
}


/**
 * Called by GenerateName and GenerateDeclaration to prepare a string
 * holding the function type's arguments.
 */
String FunctionType::GenerateArgumentString() const
{
  String arguments = "";
  CvQualifiedTypes::const_iterator arg = m_Arguments.begin();
  if(arg != m_Arguments.end())
    {    
    arguments += arg->GetName();
    for(++arg;arg != m_Arguments.end(); ++arg)
      {
      arguments += ", "+arg->GetName();
      }
    }
  return arguments;
}


/**
 * Constructor takes the return type of the function.
 */
FunctionType::FunctionType(const CvQualifiedType& returnType,
                           const CvQualifiedTypes& arguments):
  m_ReturnType(returnType),
  m_Arguments(arguments)
{
}


} // namespace _cxx_
