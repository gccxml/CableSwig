/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctFunctionBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctFunctionBase_h
#define _ctFunctionBase_h

#include "ctUtils.h"

namespace _cable_tcl_
{

/**
 * Base class for all method wrappers.
 */
class _cable_tcl_EXPORT FunctionBase
{
public:
  typedef std::vector<const Type*> ParameterTypes;
  FunctionBase(const String& name,
               const ParameterTypes& parameterTypes);
  virtual ~FunctionBase();

  const String& GetName() const;
  virtual String GetPrototype() const =0;
  unsigned long GetNumberOfParameters() const;
  const ParameterTypes& GetParameterTypes() const;
  ParameterTypes::const_iterator ParametersBegin() const;
  ParameterTypes::const_iterator ParametersEnd() const;
protected:
  /**
   * The name of the method.
   */
  String m_Name;
  
  /**
   * The parameter types of the method.  These may be needed for
   * overload resolution.
   */
  ParameterTypes m_ParameterTypes;
};

} // namespace _cable_tcl_

#endif
