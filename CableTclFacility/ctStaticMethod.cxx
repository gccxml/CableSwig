/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctStaticMethod.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctStaticMethod.h"
#include "ctTypeInfo.h"

namespace _cable_tcl_
{

/**
 * The constructor passes the function name and pararmeter types down to
 * the FunctionBase.  It then adds the implicit object parameter to the
 * front of the parameter list.  This implicit object parameter for
 * a static method wrapper is of a special type that matches any object.
 */
StaticMethod::StaticMethod(const WrapperFacility* wrapperFacility,
                           MethodWrapper methodWrapper,
                           const ClassType* wrappedTypeRepresentation,
                           const String& name, bool isOperator,
                           const CvQualifiedType& returnType,
                           const ParameterTypes& parameterTypes):
  Method(wrapperFacility, methodWrapper, wrappedTypeRepresentation, name,
         false, isOperator, returnType, parameterTypes)
{
  // Replace the implicit object parameter with a dummy.
  const Type* implicit =
    TypeInfo::GetFundamentalType(FundamentalType::Void, false, false).GetType();
  m_ParameterTypes[0] = implicit;
}


/**
 * Return whether the method is static.
 */
bool StaticMethod::IsStatic() const
{
  return true;
}


} // namespace _cable_tcl_
