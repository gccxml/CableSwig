/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctClassWrapper.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctClassWrapper.h"
#include "ctFunctionBase.h"
#include "ctConstructor.h"
#include "ctMethod.h"
#include "ctStaticMethod.h"
#include "ctFunctionSelector.h"
#include "ctTypeInfo.h"
#include "ctTclCxxObject.h"
#include "ctWrapperFacility.h"
#include "ctException.h"

#include <set>
#include <map>
#include <queue>

namespace _cable_tcl_
{

// Map from method name to its registered method wrapper.
typedef std::multimap<String, Method*> MethodMapBase;
struct ClassWrapper::MethodMap: public MethodMapBase
{
  typedef MethodMapBase::iterator iterator;
  typedef MethodMapBase::const_iterator const_iterator;
  typedef MethodMapBase::value_type value_type;
};

// Store a set of strings.
typedef std::set<String> ClassCommandSetBase;
struct ClassWrapper::ClassCommandSet: public ClassCommandSetBase
{
  typedef ClassCommandSetBase::iterator iterator;
  typedef ClassCommandSetBase::const_iterator const_iterator;
  typedef ClassCommandSetBase::value_type value_type;
};


/**
 * The constructor links this wrapper to the given WrapperFacility.
 */
ClassWrapper::ClassWrapper(WrapperFacility* wrapperFacility,
                           const ClassType* wrappedTypeRepresentation):
  m_WrapperFacility(wrapperFacility),
  m_WrappedTypeRepresentation(wrappedTypeRepresentation),
  m_MethodMap(new MethodMap),
  m_ClassCommandSet(new ClassCommandSet)
{
  String wrappedTypeName = m_WrappedTypeRepresentation->Name();
  String noTemplate =
    wrappedTypeName.substr(0, wrappedTypeName.find_first_of("<"));
  m_ConstructorName = noTemplate.substr(noTemplate.find_last_of(":") + 1);
  
  this->AddInterpreterClassCommand(wrappedTypeName);
}


/**
 * Destructor deletes all registered constructor and method wrappers.
 */
ClassWrapper::~ClassWrapper()
{
  // Free all the wrapped constructors.
  for(Constructors::const_iterator i = m_Constructors.begin();
      i != m_Constructors.end(); ++i)
    {
    delete (*i);
    }
  
  // Free all the wrapped functions.
  for(MethodMap::const_iterator i = m_MethodMap->begin();
      i != m_MethodMap->end(); ++i)
    {
    delete i->second;
    }
  delete m_MethodMap;
  delete m_ClassCommandSet;
}


/**
 * Get the representation of the type that is wrapped by this instance.
 */
const ClassType* ClassWrapper::GetWrappedTypeRepresentation() const
{
  return m_WrappedTypeRepresentation;
}


/**
 * Get the wrapper facility to which this wrapper is attached.
 */
WrapperFacility* ClassWrapper::GetWrapperFacility() const
{
  return m_WrapperFacility;
}


/**
 * Appends the list of methods provided by this wrapper's class to the
 * current Tcl result.
 */
void ClassWrapper::ListMethods() const
{
  String className = m_WrappedTypeRepresentation->Name();  
  Tcl_AppendResult(m_WrapperFacility->GetInterpreter(),
                   "Methods provided by class ",
                   const_cast<char*>(className.c_str()),
                   ":\n",
                   NULL);
  for(MethodMap::const_iterator m = m_MethodMap->begin();
      m != m_MethodMap->end(); ++m)
    {
    String prototype = m->second->GetInclassPrototype();
    Tcl_AppendResult(m_WrapperFacility->GetInterpreter(),
                     "  ", const_cast<char*>(prototype.c_str()), "\n", NULL);
    }
}


/**
 * Get a pointer to the function to register with a Tcl interpreter
 * for the class-level command for the wrapped type.
 */
ClassWrapper::WrapperFunction ClassWrapper::GetClassWrapperFunction() const
{
  return &ClassWrapper::ClassWrapperDispatchFunction;
}


/**
 * Get a pointer to the function to register with a Tcl interpreter
 * for the instance-level command for the wrapped type.
 */
ClassWrapper::WrapperFunction ClassWrapper::GetObjectWrapperFunction() const
{
  return &ClassWrapper::ObjectWrapperDispatchFunction;
}


/**
 * Register a command with the Tcl interpreter to call back to the
 * ClassWrapperDispatchFunction.
 */
void ClassWrapper::AddInterpreterClassCommand(const String& command)
{
  // Check if this command was already added.
  ClassCommandSet::const_iterator c = m_ClassCommandSet->find(command);
  if(c != m_ClassCommandSet->end()) { return; }
  
  // Add the command to the set of registered commands.
  m_ClassCommandSet->insert(command);
  
  // Create the Tcl command.
  Tcl_CreateObjCommand(m_WrapperFacility->GetInterpreter(),
                       const_cast<char*>(command.c_str()),
                       this->GetClassWrapperFunction(), this, 0);
}


/**
 * A subclass calls this to add a wrapped function.
 */
void ClassWrapper::AddFunction(Method* method)
{
  m_MethodMap->insert(MethodMap::value_type(method->GetName(), method));
}

/**
 * A subclass calls this to add a wrapped constructor.
 */
void ClassWrapper::AddFunction(Constructor* constructor)
{
  m_Constructors.push_back(constructor);
}


/**
 * When an object name is given as a command with no arguments, no method
 * name has been specified.  This is called to generate the error message.
 */
void ClassWrapper::NoMethodSpecified() const
{
  String wrappedTypeName = m_WrappedTypeRepresentation->Name();
  String errorMessage =
    "No method specified: "+wrappedTypeName+"::???";
  m_WrapperFacility->ReportErrorMessage(errorMessage);
}


/**
 * When an unknown constructor is encountered by the wrapper, this is called
 * to generate the error message.
 */
void ClassWrapper::UnknownConstructor(const CvQualifiedTypes& argumentTypes) const
{
  String wrappedTypeName = m_WrappedTypeRepresentation->Name();
  String errorMessage = "No constructor matches ";
  errorMessage += wrappedTypeName + "::" + m_ConstructorName + "(";
  CvQualifiedTypes::const_iterator arg = argumentTypes.begin();
  while(arg != argumentTypes.end())
    {
    errorMessage += arg->GetName();
    if(++arg != argumentTypes.end())
      { errorMessage += ", "; }
    }
  errorMessage += ")";
  
  // If there are any candidates, list them.
  if(!m_Constructors.empty())
    {
    errorMessage += "\nCandidates are:";
    for(Constructors::const_iterator i = m_Constructors.begin();
        i != m_Constructors.end(); ++i)
      {
      String candidate = (*i)->GetPrototype();
      errorMessage += "\n  "+candidate;
      }
    }
  
  m_WrapperFacility->ReportErrorMessage(errorMessage);
}


/**
 * When an unknown method is encountered by the wrapper, this is called
 * to generate the error message.
 */
void ClassWrapper::UnknownMethod(const String& methodName,
                                 const CvQualifiedTypes& argumentTypes,
                                 const CandidateFunctions& candidates) const
{
  String errorMessage = "No method matches ";
  CvQualifiedTypes::const_iterator arg = argumentTypes.begin();
  CvQualifiedType objectType = (*arg++);
  if(objectType.GetType()->IsReferenceType())
    {
    const ReferenceType* objectReference =
      ReferenceType::SafeDownCast(objectType.GetType());
    objectType = objectReference->GetReferencedType();
    }
  errorMessage += objectType.GetType()->Name() + "::" + methodName + "(";
  while(arg != argumentTypes.end())
    {
    errorMessage += arg->GetName();
    if(++arg != argumentTypes.end())
      { errorMessage += ", "; }
    }
  errorMessage += ")";
  
  if(objectType.IsConst())
    {
    errorMessage += " const";
    }
  
  // If there are any candidates, list them.
  if(!candidates.empty())
    {
    errorMessage += "\nCandidates are:";
    for(CandidateFunctions::const_iterator i = candidates.begin();
        i != candidates.end(); ++i)
      {
      String candidate = (*i)->GetPrototype();
      errorMessage += "\n  "+candidate;
      }
    }
  
  m_WrapperFacility->ReportErrorMessage(errorMessage);
}


/**
 * Dispatch function to select a wrapped constructor called through
 * a command associated with the wrapped class name.
 */
int
ClassWrapper
::ClassWrapperDispatch(int objc, Tcl_Obj* CONST objv[]) const
{
  try{
  // See if any wrapper in our class hierarchy knows about a static
  // method with this name.
  if(objc > 1)
    {
    String methodName = Tcl_GetStringFromObj(objv[1], NULL);
    const ClassWrapper* wrapper = this->FindMethodWrapper(methodName);
    if(wrapper)
      {
      // We have found a wrapper that knows about the method.  Call it.
      return wrapper->CallWrappedFunction(objc, objv, true);
      }
    }
  
  // We must select our own constructor.
  ConstructorSelector constructorSelector(m_WrapperFacility, objc, objv);
  
  for(Constructors::const_iterator i = m_Constructors.begin();
      i != m_Constructors.end(); ++i)
    {
    constructorSelector.AddCandidate(*i);
    }
    
  // See if any candidates match the given arguments.
  Constructor* constructor = constructorSelector.Select();
  
  // Make sure we have a matching candidate.
  if(constructor)
    {
    constructor->Call(constructorSelector.GetArguments());
    return TCL_OK;
    }
  else
    {
    this->UnknownConstructor(constructorSelector.GetArgumentTypes());
    return TCL_ERROR;
    }
  }
  // Catch any TclException that was thrown this method.
  catch (TclException e)
    {
    m_WrapperFacility->ReportErrorMessage(e.GetExceptionMessage());
    return TCL_ERROR;
    }
  catch (const std::exception& e)
    {
    m_WrapperFacility->ReportErrorMessage(e.what());
    return TCL_ERROR;
    }
  // We must catch any C++ exception to prevent it from unwinding the
  // call stack back through the Tcl interpreter's C code.
  catch (...)
    {
    m_WrapperFacility->ReportErrorMessage(
      "CABLE Tcl Wrapping Facility Error:\n"
      "Caught unknown exception before returning control to Tcl.");
    return TCL_ERROR;
    }
}



/**
 * Dispatch function to find a wrapper that knows about the method called.
 */
int ClassWrapper::ObjectWrapperDispatch(int objc, Tcl_Obj* CONST objv[]) const
{
  try{
  // Make sure we have a method name.
  if(objc < 2)
    {
    this->NoMethodSpecified();
    return TCL_ERROR;
    }  
  
  // Get the method name.
  String methodName = Tcl_GetStringFromObj(objv[1], NULL);
  
  // See if any wrapper in our class hierarchy knows about a method
  // with this name.
  const ClassWrapper* wrapper = this->FindMethodWrapper(methodName);
  if(wrapper)
    {
    // We have found a wrapper that knows about the method.  Call it.
    return wrapper->CallWrappedFunction(objc, objv, false);
    }
  else
    {
    // We don't know about the method.  Determine the argument types
    // of the method call to report the error.
    CvQualifiedTypes argumentTypes;
    // Add the implicit object argument.
    argumentTypes.push_back(m_WrapperFacility->GetObjectType(objv[0]));
    // Add the normal arguments.
    for(int i=2; i < objc; ++i)
      {
      argumentTypes.push_back(m_WrapperFacility->GetObjectType(objv[i]));
      }
    // Report that the method is unknown.
    this->UnknownMethod(methodName, argumentTypes);
    return TCL_ERROR;
    }
  }
  // Catch any TclException that was thrown this method.
  catch (TclException e)
    {
    m_WrapperFacility->ReportErrorMessage(e.GetExceptionMessage());
    return TCL_ERROR;
    }
  catch (const std::exception& e)
    {
    m_WrapperFacility->ReportErrorMessage(e.what());
    return TCL_ERROR;
    }
  // We must catch any C++ exception to prevent it from unwinding the
  // call stack back through the Tcl interpreter's C code.
  catch (...)
    {
    m_WrapperFacility->ReportErrorMessage(
      "CABLE Tcl Wrapping Facility Error:\n"
      "Caught unknown exception before returning control to Tcl.");
    return TCL_ERROR;
    }
}


/**
 * Find a Wrapper that knows about a method with the given name.  This
 * may involve chaining up the class hierarchy.  If no wrapper knows about
 * the method, NULL is returned.
 */
const ClassWrapper* ClassWrapper::FindMethodWrapper(const String& name) const
{  
  // A queue to do a BFS of this class and its parents.
  std::queue<const ClassType*> classQueue;
  
  // Start with the search at this class.
  classQueue.push(m_WrappedTypeRepresentation);
  while(!classQueue.empty())
    {
    // Get the next class off the queue.
    const ClassType* curClass = classQueue.front(); classQueue.pop();
    
    // If the class has a wrapper, see if it knows about the method.
    const ClassWrapper* wrapper = m_WrapperFacility->GetClassWrapper(curClass);
    if(wrapper && wrapper->HasMethod(name))
      {
      return wrapper;
      }
    
    // The class did not know about the method.  Add its parents to the queue.
    for(ClassTypes::const_iterator parent = curClass->ParentsBegin();
        parent != curClass->ParentsEnd(); ++parent)
      {
      classQueue.push(*parent);
      }
    }
  
  // We didn't find a wrapper that knows about the method.
  return NULL;
}


/**
 * Return whether this Wrapper knows about a method with the given name.
 */
bool ClassWrapper::HasMethod(const String& name) const
{
  return (m_MethodMap->count(name) > 0);
}


/**
 * This is called when this wrapper has been selected as knowing about
 * the method being invoked.  Here we use a FunctionSelector to get
 * the correct method wrapper, and call it.
 */
int ClassWrapper::CallWrappedFunction(int objc, Tcl_Obj* CONST objv[],
                                      bool staticOnly) const
{  
  MethodSelector methodSelector(m_WrapperFacility, objc, objv);
  
  // Get the method name.
  String methodName = Tcl_GetStringFromObj(objv[1], NULL);

  // Prepare the set of candidate functions.
  CandidateFunctions candidates;    
  MethodMap::const_iterator first = m_MethodMap->lower_bound(methodName);
  MethodMap::const_iterator last = m_MethodMap->upper_bound(methodName);
  for(MethodMap::const_iterator i = first; i != last; ++i)
    {
    methodSelector.AddCandidate(i->second);
    candidates.push_back(i->second);
    }
  
  // See if any candidates match the given arguments.
  Method* method = methodSelector.Select(staticOnly);
  
  // Make sure we have a matching candidate.  If not, we do not chain
  // up the class hierarchy because of name hiding.
  if(!method)
    {
    this->UnknownMethod(methodName, methodSelector.GetArgumentTypes(),
                        candidates);
    return TCL_ERROR;
    }
  
  // Try to call the wrapped method.
  method->Call(methodSelector.GetArguments());
  
  return TCL_OK;  
}


/**
 * The function called back from a Tcl interpreter when a command
 * referring to the wrapped type is invoked.
 *
 * This dispatches the call to the Wrapper of this type for the interpreter
 * that made the call-back.
 */
int ClassWrapper::ClassWrapperDispatchFunction(ClientData clientData,
                                               Tcl_Interp*, int objc,
                                               Tcl_Obj* CONST objv[])
{  
  // The client data is the pointer to the instance of the ClassWrapper
  // subclass that registered the comand.
  return static_cast<ClassWrapper*>(clientData)->ClassWrapperDispatch(objc,
                                                                      objv);
}


/**
 * The function called back from a Tcl interpreter when a command
 * referring to an instance of or pointer to the wrapped type is invoked.
 *
 * This dispatches the call to the ClassWrapper of this type for the
 * interpreter that made the call-back.
 */
int ClassWrapper::ObjectWrapperDispatchFunction(ClientData clientData,
                                                Tcl_Interp*, int objc,
                                                Tcl_Obj* CONST objv[])
{
  // The client data is the pointer to the instance of the
  // ClassWrapper that registered the comand.
  return static_cast<ClassWrapper*>(clientData)->ObjectWrapperDispatch(objc,
                                                                       objv);
}


} // namespace _cable_tcl_
