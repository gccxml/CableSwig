/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctFunctionWrapper.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctFunctionWrapper_h
#define _ctFunctionWrapper_h

#include "ctUtils.h"
#include "ctArgument.h"

#include <vector>

namespace _cable_tcl_
{

class FunctionBase;
class Function;
class WrapperFacility;

/**
 * Dispatch class for all registered functions of the same name.
 */
class _cable_tcl_EXPORT FunctionWrapper
{
public:
  FunctionWrapper(WrapperFacility*, const String&);
  ~FunctionWrapper();
  
  Tcl_Interp* GetInterpreter() const;
  WrapperFacility* GetWrapperFacility() const;

  void ListFunctions() const;
  
  ///! The type of a wrapper function for a Tcl interpreter call-back.
  typedef int (*WrapperFunction)(ClientData, Tcl_Interp*, int, Tcl_Obj* CONST[]);
  WrapperFunction GetWrapperFunction() const;
  
  void AddInterpreterClassCommand(const String&);
  
  void AddFunction(Function*);
  
protected:
  typedef std::vector<FunctionBase*> CandidateFunctions;
  void UnknownOverload(const CvQualifiedTypes& argumentTypes) const;
  int WrapperDispatch(int, Tcl_Obj* CONST[]) const;
  
  static int WrapperDispatchFunction(ClientData, Tcl_Interp*,
                                     int, Tcl_Obj*CONST[]);
protected:
  ///! The wrapper facility for this wrapper's interpreter.
  WrapperFacility* m_WrapperFacility;

  ///! The name of the wrapped function.
  const String m_WrappedFunctionName;
  
  typedef std::vector<Function*> Functions;
  
  ///! The overloads available to this function wrapper.
  Functions m_Functions;

  struct FunctionCommandSet;
  /**
   * The set of commands registered with the Tcl interpreter that will
   * invoke the WrapperDispatchFunction callback.
   */
  FunctionCommandSet* m_FunctionCommandSet;
};

} // namespace _cable_tcl_

#endif

