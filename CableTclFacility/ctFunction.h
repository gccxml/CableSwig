/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctFunction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctFunction_h
#define _ctFunction_h

#include "ctFunctionBase.h"
#include "ctArgument.h"

namespace _cable_tcl_
{

class WrapperFacility;

/**
 * Pointer to a function that implements a wrapped function.
 */
typedef void (*WrappedFunctionInvoker)(const WrapperFacility*,
                                       const Arguments&);

/**
 * The subclass of FunctionBase which is used for function
 * wrappers.
 */
class _cable_tcl_EXPORT Function: public FunctionBase
{
public:
  // Pull a typedef out of the superclass.
  typedef FunctionBase::ParameterTypes ParameterTypes;
  
  Function(const WrapperFacility* wrapper,
           WrappedFunctionInvoker wrappedFunctionInvoker,
           const String& name, bool isOperator,
           const CvQualifiedType& returnType,
           const ParameterTypes& parameterTypes = ParameterTypes());
  virtual String GetPrototype() const;
  String GetCallName() const;
  void Call(const Arguments&) const;
private:
  const WrapperFacility* m_WrapperFacility;
  WrappedFunctionInvoker m_WrappedFunctionInvoker;
  CvQualifiedType m_FunctionType;
  bool m_IsOperator;
};


} // namespace _cable_tcl_

#endif
