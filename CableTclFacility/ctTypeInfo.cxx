/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctTypeInfo.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctTypeInfo.h"

namespace _cable_tcl_
{

// A macro to generate all cv-qualifier combinations for defining a
// fundamental type's CvPredefinedType<> specialization.
#define _cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(T) \
CvQualifiedType CvPredefinedType<T>::type; \
CvQualifiedType CvPredefinedType<const T>::type; \
CvQualifiedType CvPredefinedType<volatile T>::type; \
CvQualifiedType CvPredefinedType<const volatile T>::type

/*@{
 * The actual CvQualifiedType instance for this fundamental type.
 */
#ifdef _cable_tcl_NO_CV_VOID
CvQualifiedType CvPredefinedType<void>::type;
#else
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(void);
#endif
#ifndef _cable_tcl_NO_WCHAR_T
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(wchar_t);
#endif
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(bool);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(char);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(signed char);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(unsigned char);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(short);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(unsigned short);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(int);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(unsigned int);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(long);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(unsigned long);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(float);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(double);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(long double);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(char*);

_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(bool&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(char&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(signed char&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(unsigned char&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(short&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(unsigned short&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(int&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(unsigned int&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(long&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(unsigned long&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(float&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(double&);
_cable_tcl_DEFINE_FUNDAMENTAL_CVTYPES(long double&);
//@}

/*@{
 * The actual CvQualifiedType instance for this Tcl-related type.
 */
CvQualifiedType CvPredefinedType<Tcl_Interp>::type;
CvQualifiedType CvPredefinedType<Tcl_Interp*>::type;
//@}
  

/**
 * Register an ArrayType having the given element type and size.
 */
CvQualifiedType
TypeInfo::GetArrayType(const CvQualifiedType& elementType,
                       unsigned long size)
{
  return typeSystem->GetArrayType(elementType, size)
    ->GetCvQualifiedType(false, false);
}


/**
 * Register a ClassType having the given name, cv-qualifiers, and
 * (optionally) parents.
 */
CvQualifiedType
TypeInfo::GetClassType(const String& name,
                       bool isConst, bool isVolatile,
                       bool isAbstract, const ClassTypes& parents)
{
  return typeSystem->GetClassType(name, isAbstract, parents)
    ->GetCvQualifiedType(isConst, isVolatile);
}


/**
 * Register an EnumerationType having the given name and cv-qualifiers.
 */
CvQualifiedType
TypeInfo::GetEnumerationType(const String& name,
                             bool isConst, bool isVolatile)
{
  return typeSystem->GetEnumerationType(name)
    ->GetCvQualifiedType(isConst, isVolatile);
}


/**
 * Register a FunctionType having the given return type, argument types,
 * and cv-qualifiers.
 */
CvQualifiedType
TypeInfo::GetFunctionType(const CvQualifiedType& returnType,
                          const CvQualifiedTypes& argumentTypes,
                          bool isConst, bool isVolatile)
{
  return typeSystem->GetFunctionType(returnType, argumentTypes)
    ->GetCvQualifiedType(isConst, isVolatile);
}


/**
 * Register a FundamentalType having the given type id and cv-qualifiers.
 */
CvQualifiedType
TypeInfo::GetFundamentalType(FundamentalType::Id id,
                                bool isConst, bool isVolatile)
{
#ifdef _cable_tcl_NO_WCHAR_T
  if(id == FundamentalType::WChar_t) { id = FundamentalType::UnsignedShortInt; }
#endif
  return typeSystem->GetFundamentalType(id)
    ->GetCvQualifiedType(isConst, isVolatile);
}


/**
 * Register a PointerType pointing to the given type and having the
 * given cv-qualifiers.
 */
CvQualifiedType
TypeInfo::GetPointerType(const CvQualifiedType& referencedType,
                         bool isConst, bool isVolatile)
{
  return typeSystem->GetPointerType(referencedType)
    ->GetCvQualifiedType(isConst, isVolatile);
}


/**
 * Register a PointerToMemberType pointing to the given type inside the
 * given ClassType and having the given cv-qualifiers.
 */
CvQualifiedType
TypeInfo::GetPointerToMemberType(const CvQualifiedType& referencedType,
                                 const ClassType* classScope,
                                 bool isConst, bool isVolatile)
{
  return typeSystem->GetPointerToMemberType(referencedType, classScope)
    ->GetCvQualifiedType(isConst, isVolatile);
}


/**
 * Register a ReferenceType referencing the given type.
 */
CvQualifiedType
TypeInfo::GetReferenceType(const CvQualifiedType& referencedType)
{
  return typeSystem->GetReferenceType(referencedType)
    ->GetCvQualifiedType(false, false);
}


/**
 * There is exactly one TypeSystem in the wrapper facility.
 * This is it.
 */
TypeSystem* TypeInfo::typeSystem = 0;

  
// A macro to generate all cv-qualifier combinations for generating a
// fundamental type's CvPredefinedType<> specialization.
#define _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(T, ID) \
CvPredefinedType<T>::type = GetFundamentalType(FundamentalType::ID, false, false); \
CvPredefinedType<const T>::type = GetFundamentalType(FundamentalType::ID, true, false); \
CvPredefinedType<volatile T>::type = GetFundamentalType(FundamentalType::ID, false, true); \
CvPredefinedType<const volatile T>::type = GetFundamentalType(FundamentalType::ID, true, true)

#define _cable_tcl_GENERATE_REFERENCE_CVTYPES(T) \
CvPredefinedType<T&>::type = GetReferenceType(CvPredefinedType<T>::type); \
CvPredefinedType<const T&>::type = GetReferenceType(CvPredefinedType<const T>::type); \
CvPredefinedType<volatile T&>::type = GetReferenceType(CvPredefinedType<volatile T>::type); \
CvPredefinedType<const volatile T&>::type = GetReferenceType(CvPredefinedType<const volatile T>::type);

/**
 * Initialization function for TypeInfo class.
 * This will be called exactly once by WrapperFacility::ClassInitialize().
 */
void TypeInfo::ClassInitialize()
{
  Tcl_CreateExitHandler(TypeInfo::ClassExitHanlder, 0);
  TypeInfo::typeSystem = new TypeSystem;
#ifdef _cable_tcl_NO_CV_VOID
  CvPredefinedType<void>::type = GetFundamentalType(FundamentalType::Void, false, false);
#else
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(void, Void);
#endif
#ifndef _cable_tcl_NO_WCHAR_T
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(wchar_t, WChar_t);
#endif
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(bool, Bool);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(char, Char);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(signed char, SignedChar);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(unsigned char, UnsignedChar);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(short, ShortInt);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(unsigned short, UnsignedShortInt);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(int, Int);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(unsigned int, UnsignedInt);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(long, LongInt);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(unsigned long, UnsignedLongInt);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(float, Float);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(double, Double);
  _cable_tcl_GENERATE_FUNDAMENTAL_CVTYPES(long double, LongDouble);

  _cable_tcl_GENERATE_REFERENCE_CVTYPES(bool);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(char);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(signed char);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(unsigned char);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(short);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(unsigned short);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(int);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(unsigned int);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(long);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(unsigned long);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(float);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(double);
  _cable_tcl_GENERATE_REFERENCE_CVTYPES(long double);
  
  CvPredefinedType<Tcl_Interp>::type  = GetClassType("Tcl_Interp", false, false);
  CvPredefinedType<Tcl_Interp*>::type  = GetPointerType(CvPredefinedType<Tcl_Interp>::type, false, false);
  CvPredefinedType<char*>::type  = GetPointerType(CvPredefinedType<char>::type, false, false);
  CvPredefinedType<const char*>::type  = GetPointerType(CvPredefinedType<const char>::type, false, false);
  CvPredefinedType<volatile char*>::type  = GetPointerType(CvPredefinedType<volatile char>::type, false, false);
  CvPredefinedType<const volatile char*>::type  = GetPointerType(CvPredefinedType<const volatile char>::type, false, false);
}


/**
 * Registered with Tcl to be called just before exit.
 */
void TypeInfo::ClassExitHanlder(ClientData)
{
  if(TypeInfo::typeSystem)
    {
    delete TypeInfo::typeSystem;
    }
}

} // namespace _cable_tcl_
