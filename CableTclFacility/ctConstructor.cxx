/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctConstructor.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctConstructor.h"
#include "ctTypeInfo.h"


namespace _cable_tcl_
{


/**
 * The constructor passes the function name and pararmeter types down to
 * the FunctionBase.
 */
Constructor::Constructor(const WrapperFacility* wrapperFacility,
                         ConstructorWrapper constructorWrapper,
                         const ClassType* wrappedTypeRepresentation,
                         const String& name,
                         const ParameterTypes& parameterTypes):
  FunctionBase(name, parameterTypes),
  m_WrappedTypeRepresentation(wrappedTypeRepresentation),
  m_WrapperFacility(wrapperFacility),
  m_ConstructorWrapper(constructorWrapper)
{
}


/**
 * Get a string representation of the constructor's function prototype.
 */
String Constructor::GetPrototype() const
{
  String prototype = m_WrappedTypeRepresentation->Name() + "::" + m_Name + "(";
  ParameterTypes::const_iterator arg = m_ParameterTypes.begin();
  while(arg != m_ParameterTypes.end())
    {
    prototype += (*arg)->Name();
    if(++arg != m_ParameterTypes.end())
      { prototype += ", "; }
    }
  prototype += ")";
  
  return prototype;
}


/**
 * Invokes a wrapped constructor.  This actually extracts the C++ objects
 * from the Tcl objects given as arguments and calls the constructor wrapper.
 *
 * If construction succeeds, this also adds the object to the Wrapper's
 * instance table.
 */
void Constructor::Call(const Arguments& arguments) const
{
  // Call the constructor wrapper.
  m_ConstructorWrapper(m_WrapperFacility, arguments);
}


} // namespace _cable_tcl_
