/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrapFunctionWrapper.cxx
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
#include "wrapFunctionWrapper.h"
#include "wrapFunctionBase.h"
#include "wrapFunction.h"
#include "wrapFunctionSelector.h"
#include "wrapTypeInfo.h"
#include "wrapWrapperFacility.h"
#include "wrapException.h"


namespace _wrap_
{

/**
 * The constructor links this class to the given WrapperFacility.
 */
FunctionWrapper::FunctionWrapper(WrapperFacility* wrapperFacility,
                                 const String& wrappedFunctionName):
  m_WrapperFacility(wrapperFacility),
  m_WrappedFunctionName(wrappedFunctionName)
{
}


/**
 * Destructor frees wrapped functions.
 */
FunctionWrapper::~FunctionWrapper()
{
  // Free all the wrapped functions.
  for(Functions::const_iterator i = m_Functions.begin();
      i != m_Functions.end(); ++i)
    {
    delete (*i);
    }
}


/**
 * Get the wrapper facility to which this wrapper is attached.
 */
WrapperFacility* FunctionWrapper::GetWrapperFacility() const
{
  return m_WrapperFacility;
}


/**
 * Appends the list of functions provided by this wrapper class to the
 * current Tcl result.
 */
void FunctionWrapper::ListFunctions() const
{
  for(Functions::const_iterator f = m_Functions.begin();
      f != m_Functions.end(); ++f)
    {
    String prototype = (*f)->GetPrototype();
    Tcl_AppendResult(m_WrapperFacility->GetInterpreter(),
                     "  ", const_cast<char*>(prototype.c_str()), "\n", 0);
    }
}


/**
 * Get a pointer to the function to register with a Tcl interpreter
 * for the command for the wrapped type.
 */
FunctionWrapper::WrapperFunction FunctionWrapper::GetWrapperFunction() const
{
  return &FunctionWrapper::WrapperDispatchFunction;
}


/**
 * This is called to add a wrapped function.
 */
void FunctionWrapper::AddFunction(Function* function)
{
  // TODO: Uniquify overloads.
  m_Functions.push_back(function);
}


/**
 * When the overload resolution fails to find a match, this is called
 * to generate the error message.
 */
void
FunctionWrapper::UnknownOverload(const CvQualifiedTypes& argumentTypes) const
{
  String errorMessage = "No overload matches ";
  CvQualifiedTypes::const_iterator arg = argumentTypes.begin();
  errorMessage += m_WrappedFunctionName + "(";
  while(arg != argumentTypes.end())
    {
    errorMessage += arg->GetName();
    if(++arg != argumentTypes.end())
      { errorMessage += ", "; }
    }
  errorMessage += ")";
  
  // If there are any candidates, list them.
  if(!m_Functions.empty())
    {
    errorMessage += "\nCandidates are:";
    for(Functions::const_iterator i = m_Functions.begin();
        i != m_Functions.end(); ++i)
      {
      String candidate = (*i)->GetPrototype();
      errorMessage += "\n  "+candidate;
      }
    }
  
  m_WrapperFacility->ReportErrorMessage(errorMessage);
}


/**
 * Dispatch function to select a wrapped function called through a
 * command associated with the wrapped function name.
 */
int FunctionWrapper::WrapperDispatch(int objc, Tcl_Obj* CONST objv[]) const
{
  try{
  // We must select our own constructor.
  FunctionSelector functionSelector(m_WrapperFacility, objc, objv);
  
  for(Functions::const_iterator i = m_Functions.begin();
      i != m_Functions.end(); ++i)
    {
    functionSelector.AddCandidate(*i);
    }
    
  // See if any candidates match the given arguments.
  Function* function = functionSelector.Select();
  
  // Make sure we have a matching candidate.
  if(function)
    {
    function->Call(functionSelector.GetArguments());
    return TCL_OK;
    }
  else
    {
    this->UnknownOverload(functionSelector.GetArgumentTypes());
    return TCL_ERROR;
    }
  }
  // Catch any TclException that was thrown by this function.
  catch (TclException e)
    {
    m_WrapperFacility->ReportErrorMessage(e.GetExceptionMessage());
    return TCL_ERROR;
    }
  // We must catch any C++ exception to prevent it from unwinding the
  // call stack back through the Tcl interpreter's C code.
  catch (...)
    {
    m_WrapperFacility->ReportErrorMessage("Caught unknown exception!!");
    return TCL_ERROR;
    }
}


/**
 * The function called back from a Tcl interpreter when a command
 * referring to the wrapped function is invoked.
 *
 * This dispatches the call to the FunctionWrapper instance.
 */
int FunctionWrapper::WrapperDispatchFunction(ClientData clientData,
                                             Tcl_Interp* interp,
                                             int objc,
                                             Tcl_Obj* CONST objv[])
{  
  // The client data is the pointer to the instance of the
  // FunctionWrapper that registered the comand.
  return static_cast<FunctionWrapper*>(clientData)->WrapperDispatch(objc,
                                                                    objv);
}


} // namespace _wrap_
