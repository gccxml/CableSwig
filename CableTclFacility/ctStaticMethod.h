/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctStaticMethod.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctStaticMethod_h
#define _ctStaticMethod_h

#include "ctMethod.h"

namespace _cable_tcl_
{

/**
 * The subclass of FunctionBase which is used for static
 * method wrappers.
 */
class _cable_tcl_EXPORT StaticMethod: public Method
{
public:
  // Pull a typedef out of the superclass.
  typedef FunctionBase::ParameterTypes ParameterTypes;
  
  StaticMethod(const WrapperFacility* wrapperFacility,
               MethodWrapper methodWrapper,
               const ClassType* wrappedTypeRepresentation,
               const String& name, bool isOperator,
               const CvQualifiedType& returnType,
               const ParameterTypes& parameterTypes = ParameterTypes());
  virtual bool IsStatic() const;
};


} // namespace _cable_tcl_

#endif
