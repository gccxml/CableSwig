/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctFunctionWrapper.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctFunctionWrapper.h"
#include "ctFunctionBase.h"
#include "ctFunction.h"
#include "ctFunctionSelector.h"
#include "ctTypeInfo.h"
#include "ctWrapperFacility.h"
#include "ctException.h"

#include <set>

namespace _cable_tcl_
{

// Store a set of strings.
typedef std::set<String> FunctionCommandSetBase;
struct FunctionWrapper::FunctionCommandSet: public FunctionCommandSetBase
{
  typedef FunctionCommandSetBase::iterator iterator;
  typedef FunctionCommandSetBase::const_iterator const_iterator;
  typedef FunctionCommandSetBase::value_type value_type;
};


/**
 * The constructor links this class to the given WrapperFacility.
 */
FunctionWrapper::FunctionWrapper(WrapperFacility* wrapperFacility,
                                 const String& wrappedFunctionName):
  m_WrapperFacility(wrapperFacility),
  m_WrappedFunctionName(wrappedFunctionName),
  m_FunctionCommandSet(new FunctionCommandSet)
{
  this->AddInterpreterClassCommand(wrappedFunctionName);
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
  
  delete m_FunctionCommandSet;
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
 * Register a command with the Tcl interpreter to call back to the
 * WrapperDispatchFunction.
 */
void FunctionWrapper::AddInterpreterClassCommand(const String& command)
{
  // Check if this command was already added.
  FunctionCommandSet::const_iterator c = m_FunctionCommandSet->find(command);
  if(c != m_FunctionCommandSet->end()) { return; }
  
  // Add the command to the set of registered commands.
  m_FunctionCommandSet->insert(command);
  
  // Create the Tcl command.
  Tcl_CreateObjCommand(m_WrapperFacility->GetInterpreter(),
                       const_cast<char*>(command.c_str()),
                       this->GetWrapperFunction(), this, 0);
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
                                             Tcl_Interp*, int objc,
                                             Tcl_Obj* CONST objv[])
{  
  // The client data is the pointer to the instance of the
  // FunctionWrapper that registered the comand.
  return static_cast<FunctionWrapper*>(clientData)->WrapperDispatch(objc,
                                                                    objv);
}

} // namespace _cable_tcl_
