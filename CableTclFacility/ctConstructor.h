/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctConstructor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctConstructor_h
#define _ctConstructor_h

#include "ctFunctionBase.h"
#include "ctArgument.h"

namespace _cable_tcl_
{

class WrapperFacility;

/**
 * Pointer to a function that implements a wrapped constructor.
 */
typedef void (*ConstructorWrapper)(const WrapperFacility*, const Arguments&);

/**
 * The subclass of FunctionBase which is used for constructor
 * wrappers.
 */
class _cable_tcl_EXPORT Constructor: public FunctionBase
{
public:
  // Pull a typedef out of the superclass.
  typedef FunctionBase::ParameterTypes ParameterTypes;
  
  Constructor(const WrapperFacility* wrapperFacility,
              ConstructorWrapper constructorWrapper,
              const ClassType* wrappedTypeRepresentation,
              const String& name,
              const ParameterTypes& parameterTypes = ParameterTypes());
  virtual String GetPrototype() const;
  void Call(const Arguments&) const;
private:
  const ClassType* m_WrappedTypeRepresentation;
  const WrapperFacility* m_WrapperFacility;
  ConstructorWrapper m_ConstructorWrapper;
};

} // namespace _cable_tcl_

#endif
