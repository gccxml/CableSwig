/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrapFunctionWrapper.h
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
#ifndef _wrapFunctionWrapper_h
#define _wrapFunctionWrapper_h

#include "wrapUtils.h"
#include "wrapArgument.h"

#include <vector>

namespace _wrap_
{

class FunctionBase;
class Function;
class WrapperFacility;

/**
 * Dispatch class for all registered functions of the same name.
 */
class _wrap_EXPORT FunctionWrapper
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

} // namespace _wrap_

#endif

