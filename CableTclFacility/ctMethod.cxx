/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctMethod.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctMethod.h"
#include "ctTypeInfo.h"

namespace _cable_tcl_
{

/**
 * The constructor passes the function name and pararmeter types down to
 * the FunctionBase.  It then adds the implicit object parameter to the
 * front of the parameter list.
 */
Method::Method(const WrapperFacility* wrapperFacility,
               MethodWrapper methodWrapper,
               const ClassType* wrappedTypeRepresentation,
               const String& name,
               bool isConst, bool isOperator,
               const CvQualifiedType& returnType,
               const ParameterTypes& parameterTypes):
  FunctionBase(name, parameterTypes),
  m_WrappedTypeRepresentation(wrappedTypeRepresentation),
  m_WrapperFacility(wrapperFacility),
  m_MethodWrapper(methodWrapper),
  m_IsOperator(isOperator)
{
  // Construct the function type associated with the method.  This does not
  // include the implicit object parameter.
  CvQualifiedTypes functionParameterTypes;  
  for(ParameterTypes::const_iterator arg = m_ParameterTypes.begin();
      arg != m_ParameterTypes.end(); ++arg)
    {
    functionParameterTypes.push_back((*arg)->GetCvQualifiedType(false, false));
    }
  m_FunctionType = TypeInfo::GetFunctionType(returnType, functionParameterTypes,
                                             isConst, false);

  // Add the implicit object parameter to the front of the parameter list.
  CvQualifiedType wrappedType = m_WrappedTypeRepresentation
    ->GetCvQualifiedType(isConst, false);
  const Type* implicit = TypeInfo::GetReferenceType(wrappedType).GetType();
  m_ParameterTypes.insert(m_ParameterTypes.begin(), implicit);

}

  
/**
 * Return whether the method is static.
 */
bool Method::IsStatic() const
{
  return false;
}

/**
 * Get a string representation of the method's function prototype.
 */
String Method::GetPrototype() const
{
  String name = m_WrappedTypeRepresentation->Name() + "::" + this->GetCallName();
  String prototype = m_FunctionType.GenerateDeclaration(name);
  if(this->IsStatic())
    {
    prototype = "static "+prototype;
    }
  return prototype;
}

String Method::GetInclassPrototype() const
{
  String prototype = m_FunctionType.GenerateDeclaration(this->GetCallName());
  if(this->IsStatic())
    {
    prototype = "static "+prototype;
    }
  return prototype;
}


/**
 * Get the name of the method as it would be called with the standard
 * obj.method() syntax.  This is used to add the operator keyword to
 * the name, if necessary.
 */
String Method::GetCallName() const
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
 * Invokes a wrapped method.  This actually extracts the C++ objects
 * from the Tcl objects given as arguments and calls the method wrapper.
 */
void Method::Call(const Arguments& arguments) const
{
  // Call the method wrapper.
  m_MethodWrapper(m_WrapperFacility, arguments);
}


} // namespace _cable_tcl_
