/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctClassWrapper.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctClassWrapper_h
#define _ctClassWrapper_h

#include "ctUtils.h"
#include "ctArgument.h"

#include <vector>

namespace _cable_tcl_
{

class FunctionBase;
class Constructor;
class Method;
class StaticMethod;
class WrapperFacility;

/**
 * Dispatch class for a particular class's wrapped methods.
 */
class _cable_tcl_EXPORT ClassWrapper
{
public:
  ClassWrapper(WrapperFacility*, const ClassType*);
  virtual ~ClassWrapper();
  
  const ClassType* GetWrappedTypeRepresentation() const;
  
  WrapperFacility* GetWrapperFacility() const;
  
  void ListMethods() const;
  
  ///! The type of a wrapper function for a Tcl interpreter call-back.
  typedef int (*WrapperFunction)(ClientData, Tcl_Interp*, int, Tcl_Obj* CONST[]);
  WrapperFunction GetClassWrapperFunction() const;
  WrapperFunction GetObjectWrapperFunction() const;
  
  void AddInterpreterClassCommand(const String&);
  
  void AddFunction(Method*);
  void AddFunction(Constructor*);
  
protected:
  typedef std::vector<FunctionBase*> CandidateFunctions;
  void NoMethodSpecified() const;
  void UnknownConstructor(const CvQualifiedTypes& argumentTypes) const;
  void UnknownMethod(const String& methodName,
                     const CvQualifiedTypes& argumentTypes,
                     const CandidateFunctions& = CandidateFunctions()) const;
  void ReportErrorMessage(const String& errorMessage) const;
  int ObjectWrapperDispatch(int, Tcl_Obj* CONST[]) const;
  const ClassWrapper* FindMethodWrapper(const String& name) const;
  bool HasMethod(const String& name) const;
  int ClassWrapperDispatch(int, Tcl_Obj* CONST[]) const;
  int CallWrappedFunction(int, Tcl_Obj* CONST[], bool) const;
  
  static int ClassWrapperDispatchFunction(ClientData, Tcl_Interp*,
                                          int, Tcl_Obj*CONST[]);  
  static int ObjectWrapperDispatchFunction(ClientData, Tcl_Interp*,
                                           int, Tcl_Obj*CONST[]);
protected:
  ///! The wrapper facility for this wrapper's interpreter.
  WrapperFacility* m_WrapperFacility;
  
  ///! The TypeSystem's representation for this wrapped type.
  const ClassType* m_WrappedTypeRepresentation;
  
  /**
   * The name of a constructor of the type.  This is the type name
   * without template arguments or namespace qualification.
   */
  String m_ConstructorName;
  
  typedef std::vector<Constructor*> Constructors;
  
  ///! The constructors available to this wrapper.
  Constructors m_Constructors;
  
  struct MethodMap;
  /**
   * Map from method name to method wrapper.  Duplicate names are
   * allowed.  The method dispatch function needs to know about all
   * possible methods.  This is defined here, but must be filled in by
   * calls from a subclass to AddFunction.
   */
  MethodMap* m_MethodMap;
  
  struct ClassCommandSet;
  /**
   * The set of commands registered with the Tcl interpreter that will
   * invoke the ClassWrapperDispatchFunction callback.
   */
  ClassCommandSet* m_ClassCommandSet;
};

} // namespace _cable_tcl_

#endif

