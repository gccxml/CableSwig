/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctFunction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctFunction.h"
#include "ctTypeInfo.h"

namespace _cable_tcl_
{

/**
 * The constructor passes the function name and pararmeter types down to
 * the FunctionBase.
 */
Function::Function(const WrapperFacility* wrapperFacility,
                   WrappedFunctionInvoker wrappedFunctionInvoker,
                   const String& name,
                   bool isOperator,
                   const CvQualifiedType& returnType,
                   const ParameterTypes& parameterTypes):
  FunctionBase(name, parameterTypes),
  m_WrapperFacility(wrapperFacility),
  m_WrappedFunctionInvoker(wrappedFunctionInvoker),
  m_IsOperator(isOperator)
{
  // Construct the function type.
  CvQualifiedTypes functionParameterTypes;  
  for(ParameterTypes::const_iterator arg = m_ParameterTypes.begin();
      arg != m_ParameterTypes.end(); ++arg)
    {
    functionParameterTypes.push_back((*arg)->GetCvQualifiedType(false, false));
    }
  m_FunctionType = TypeInfo::GetFunctionType(returnType, functionParameterTypes,
                                             false, false);
}


/**
 * Get a string representation of the function prototype.
 */
String Function::GetPrototype() const
{
  return m_FunctionType.GenerateDeclaration(this->GetCallName());
}


/**
 * Get the name of the function as it would be called with the standard
 * function_name(args) syntax.  This is used to add the operator keyword to
 * the name, if necessary.
 */
String Function::GetCallName() const
{
  if(m_Name.length() == 0)
    {
    return "";
    }
  if(m_IsOperator)
    {
    return "operator "+m_Name;
    }
  else
    {
    return m_Name;
    }
}


/**
 * Invokes a wrapped function.  This actually extracts the C++ objects
 * from the Tcl objects given as arguments and calls the function wrapper.
 */
void Function::Call(const Arguments& arguments) const
{
  // Call the function wrapper.
  m_WrappedFunctionInvoker(m_WrapperFacility, arguments);
}


} // namespace _cable_tcl_
