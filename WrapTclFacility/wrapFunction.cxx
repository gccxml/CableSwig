/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrapFunction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "wrapFunction.h"
#include "wrapTypeInfo.h"

namespace _wrap_
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


} // namespace _wrap_
