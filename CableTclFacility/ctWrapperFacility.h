/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctWrapperFacility.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctWrapperFacility_h
#define _ctWrapperFacility_h

#include "ctUtils.h"
#include "ctArgument.h"

namespace _cable_tcl_
{

class ConversionTable;
class ClassWrapper;
class FunctionWrapper;
class CxxObject;

/**
 * An instance of this class is associated with each interpreter.  It
 * provides the basic wrapper facility commands in the "wrap::"
 * namespace.  It keeps track of the ConversionTable and set of
 * wrapper classes that have been setup for the interpreter.
 */
class _cable_tcl_EXPORT WrapperFacility
{
public:
  typedef WrapperFacility Self;
  static WrapperFacility* GetForInterpreter(Tcl_Interp*);

  Tcl_Interp* GetInterpreter() const;
  ConversionTable* GetConversionTable() const;
  void ReportErrorMessage(const String& errorMessage) const;
  
  ClassWrapper* CreateClassWrapper(const ClassType*);
  ClassWrapper* GetClassWrapper(const ClassType*) const;
  FunctionWrapper* CreateFunctionWrapper(const String&);
  FunctionWrapper* GetFunctionWrapper(const String&) const;
  
  CvQualifiedType GetObjectType(Tcl_Obj* obj) const;
  Argument GetObjectArgument(Tcl_Obj* obj) const;
  ConversionFunction GetConversionFunction(const CvQualifiedType& from,
                                           const Type* to) const;
  
  void SetEnumerationConstant(const String&, void*, const Type*);
  
  ///! The type of a function that deletes an object.
  typedef void (*DeleteFunction)(const void*);
  void SetDeleteFunction(const Type*, DeleteFunction);
  void DeleteObject(const void*, const Type*) const;
  
  CxxObject* GetCxxObjectFor(const Anything&, const Type*) const;
  void DeleteCxxObjectFor(const Anything&, const Type*) const;
  bool CxxObjectExists(const CxxObject*) const;
  void SetConversion(const CvQualifiedType&, const Type*, ConversionFunction) const;
  ConversionFunction GetConversion(const CvQualifiedType&, const Type*) const;
  bool DebugIsOn() const;
  void OutputDebugText(const char*) const;
  
private:
  WrapperFacility(Tcl_Interp*);
  ~WrapperFacility();

  void InitializeForInterpreter();
  static void InterpreterFreeCallback(ClientData, Tcl_Interp*);
  int ListMethodsCommand(int, Tcl_Obj*CONST[]) const;
  int ListClassesCommand(int, Tcl_Obj*CONST[]) const;
  int TypeOfCommand(int, Tcl_Obj*CONST[]) const;
  int InterpreterCommand(int, Tcl_Obj*CONST[]) const;
  int DebugOnCommand(int, Tcl_Obj*CONST[]);
  int DebugOffCommand(int, Tcl_Obj*CONST[]);
  static int ListMethodsCommandFunction(ClientData, Tcl_Interp*,
                                        int, Tcl_Obj*CONST[]);
  static int ListClassesCommandFunction(ClientData, Tcl_Interp*,
                                        int, Tcl_Obj*CONST[]);
  static int TypeOfCommandFunction(ClientData, Tcl_Interp*,
                                   int, Tcl_Obj*CONST[]);
  static int InterpreterCommandFunction(ClientData, Tcl_Interp*,
                                        int, Tcl_Obj*CONST[]);
  static int DebugOnCommandFunction(ClientData, Tcl_Interp*,
                                    int, Tcl_Obj*CONST[]);
  static int DebugOffCommandFunction(ClientData, Tcl_Interp*,
                                     int, Tcl_Obj*CONST[]);

  ///! The Tcl interpreter to which this facility is attached.
  Tcl_Interp* m_Interpreter;

  struct EnumMap;
  ///! The table of enumeration values that have been registered.
  EnumMap* m_EnumMap;

  struct ClassWrapperMap;
  ///! Map from class type to wrapper instance.
  ClassWrapperMap* m_ClassWrapperMap;

  struct FunctionWrapperMap;
  ///! Map from function name to wrapper instance.
  FunctionWrapperMap* m_FunctionWrapperMap;

  struct DeleteFunctionMap;
  ///! Table of registered delete functions.
  DeleteFunctionMap* m_DeleteFunctionMap;  

  struct CxxObjectMap;
  ///! Map from object location and type to the CxxObject instance for it.
  CxxObjectMap* m_CxxObjectMap;

  struct CxxFunctionMap;
  ///! Map from function pointer value to CxxObject instance for it.
  CxxFunctionMap* m_CxxFunctionMap;
  
  struct CxxObjectSet;
  ///! A set of all CxxObject instances known by this WrapperFacility.
  CxxObjectSet* m_CxxObjectSet;
  
  struct ConversionMap;
  ///! Map from conversion from/to pair to a conversion function.
  ConversionMap* m_ConversionMap;

  ///! Debug flag.  For consistency, included whether debug support is built.
  bool m_Debug;
public:
  void InitializePredefinedConversions() const;
  static void ClassInitialize();
};


/**
 * A function to delete an object of any type.  Instantiations of this
 * can be registered as delete functions with a WrapperFacility.
 */
template <typename T>
struct OldObjectOf
{
  static void Delete(const void* object)
    {
    delete const_cast<T*>(static_cast<const T*>(object));
    }
};


} // namespace _cable_tcl_

#endif
