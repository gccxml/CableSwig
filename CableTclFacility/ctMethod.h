/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctMethod.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctMethod_h
#define _ctMethod_h

#include "ctFunctionBase.h"
#include "ctArgument.h"

namespace _cable_tcl_
{

class WrapperFacility;

/**
 * Pointer to a function that implements a wrapped method.
 */
typedef void (*MethodWrapper)(const WrapperFacility*, const Arguments&);
  
/**
 * The subclass of FunctionBase which is used for method
 * wrappers.
 */
class _cable_tcl_EXPORT Method: public FunctionBase
{
public:
  // Pull a typedef out of the superclass.
  typedef FunctionBase::ParameterTypes ParameterTypes;
  
  Method(const WrapperFacility* wrapperFacility,
         MethodWrapper methodWrapper,
         const ClassType* wrappedTypeRepresentation,
         const String& name,
         bool isConst, bool isOperator,
         const CvQualifiedType& returnType,
         const ParameterTypes& parameterTypes = ParameterTypes());
  virtual bool IsStatic() const;
  virtual String GetPrototype() const;
  String GetInclassPrototype() const;
  String GetCallName() const;
  void Call(const Arguments&) const;
private:
  const ClassType* m_WrappedTypeRepresentation;
  const WrapperFacility* m_WrapperFacility;
  MethodWrapper m_MethodWrapper;
  CvQualifiedType m_FunctionType;
  bool m_IsOperator;
};


} // namespace _cable_tcl_

#endif
