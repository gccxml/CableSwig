/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableTclGenerator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableTclGenerator_h
#define _cableTclGenerator_h

#include "cableGenerator.h"
#include "cableCvTypeWriter.h"

namespace _cxx_ { class CvQualifiedType; class ClassType; }

namespace cable
{

class Namespace;
class Class;
class Function;
class Method;
class Type;
class FunctionType;

/** Tcl wrapper generation class.  */
class CABLE_PARSERS_EXPORT TclGenerator: public Generator
{
public:
  cableTypeMacro(TclGenerator, Generator);
  static Pointer New() { return new Self; }
  
  /** Print the configuration settings used to generate wrappers.  */
  void PrintConfiguration(std::ostream& os) const;
  
protected:
  TclGenerator();
  ~TclGenerator();
  
  class ClassWrapperMap;
  class StringVector;
  class MethodVector;
  
  // Wrapper generation method called by Generator::Generate().
  bool GenerateWrappers();  
  
  // Find configuration settings from the source representation.
  bool FindConfiguration();
  bool FindWrappers(const Namespace* cns);
  
  // Methods to walk the wrappers and collect the types used.
  void FindCvTypes();
  void FindClassCvTypes(const Class* c);
  void FindFunctionCvTypes(const Function* f);
  void FindReturnCvTypes(const Type* returns);
  void FindArgumentCvTypes(const Type* argument);
  void AddCvReferenceType(const cxx::CvQualifiedType& target);
  void AddCvPointerType(const cxx::CvQualifiedType& target);
  void AddSuperclassCvTypes(const cxx::ClassType* classType);
  
  // Methods to write output code.
  void WriteHeaders() const;
  void WritePackageInitialization() const;
  void WriteWrappers() const;
  void WriteReturnEnumClasses() const;
  void WriteEnumValueRegistration() const;
  void WriteGroupTypeHandlers() const;
  void WriteReferenceDerivedToBase(const cxx::CvQualifiedType& derivedType,
                                   const cxx::CvQualifiedType& baseType) const;
  void WritePointerDerivedToBase(const cxx::CvQualifiedType& derivedType,
                                 const cxx::CvQualifiedType& baseType) const;
  void WriteClassWrappers() const;
  void WriteClassWrapper(const Class* c, const StringVector& names) const;
  void FindMethods(const Class* c, MethodVector& methods,
                   MethodVector& converters) const;
  void WriteClassWrapperDefinition(const Class* c, const StringVector& names,
                                   const MethodVector& methods) const;
  void WriteMethodName(const Method* m, unsigned int index) const;
  void WriteMethodWrapper(const Class* c, const MethodVector& methods,
                          unsigned int index) const;
  bool ReturnsVoid(const Function* f) const;
  String GetReturnTypeName(const Function* f) const;
  void WriteMethodComment(const Class* c, const Method* m) const;
  void WriteImplicitArgument(const Class* c, const Method* m) const;
  void WriteReturnBegin(const Function* f) const;
  void WriteReturnEnd(const Function* f) const;
  void WriteArgumentList(const FunctionType* ft, unsigned int offset,
                         unsigned int count) const;
  void WriteClassWrapperRegistration(const Class* c,
                                     const StringVector& names,
                                     const MethodVector& methods,
                                     const MethodVector& converters) const;
  void WriteMethodRegistration(const Class* c, const MethodVector& methods,
                               unsigned int index) const;
  void WriteConverterRegistration(const Class* c, const Method* m) const;
  
  typedef StringVector GroupVector;
  typedef StringVector WrapperList;
  
  // The set of classes to be wrapped.
  ClassWrapperMap& m_ClassWrapperMap;
  
  // The list of groups combined in the package to be generated.
  GroupVector& m_GroupVector;
  
  // The name of the group of wrappers to be generated.
  String m_Group;
  
  // The name of the package to be generated.
  String m_Package;
  
  // Whether a group of wrappers will be generated.
  bool m_WriteGroup;
  
  // Whether package initialization code will be generated.
  bool m_WritePackage;
  
  // The name of the cable configuration file.
  String m_ConfigFileName;
  
  // Utility to write the CxxTypes library code in the output.
  CvTypeWriter::Pointer m_CvTypeWriter;
  
  // The list of wrapper classes that have been generated.
  WrapperList& m_WrapperList;
  
  class ClassTypeSet;
  class EnumerationSet;
  typedef ClassTypeSet ClassesForDerivedToBase;
  typedef ClassTypeSet ClassesThatNeedDestructor;
  typedef EnumerationSet EnumerationsThatNeedReturn;
  typedef EnumerationSet EnumerationsThatNeedValues;
  
  // Keep track of entities that need special consideration.
  ClassesForDerivedToBase& m_ClassesForDerivedToBase;
  ClassesThatNeedDestructor& m_ClassesThatNeedDestructor;
  EnumerationsThatNeedReturn& m_EnumerationsThatNeedReturn;  
  EnumerationsThatNeedValues& m_EnumerationsThatNeedValues;

  // Internal utility methods.
  bool ParseName(const char* name, String& result);
private:
  TclGenerator(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
