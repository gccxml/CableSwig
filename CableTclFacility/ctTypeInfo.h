/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctTypeInfo.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctTypeInfo_h
#define _ctTypeInfo_h

#include "ctUtils.h"

namespace _cable_tcl_
{

class WrapperFacility;

/**
 * A class to maintain a single instance of TypeSystem.
 */
class _cable_tcl_EXPORT TypeInfo
{
public:
  static CvQualifiedType GetArrayType(const CvQualifiedType& elementType,
                                      unsigned long size);
  static CvQualifiedType GetClassType(const String& name,
                                      bool isConst, bool isVolatile,
                                      bool isAbstract = false,
                                      const ClassTypes& parents = ClassTypes());
  static CvQualifiedType GetEnumerationType(const String& name,
                                            bool isConst, bool isVolatile);
  static CvQualifiedType GetFunctionType(const CvQualifiedType& returnType,
                                         const CvQualifiedTypes& argumentTypes,
                                         bool isConst, bool isVolatile);
  static CvQualifiedType GetFundamentalType(FundamentalType::Id,
                                            bool isConst, bool isVolatile);
  static CvQualifiedType GetPointerType(const CvQualifiedType& referencedType,
                                        bool isConst, bool isVolatile);
  static CvQualifiedType GetPointerToMemberType(const CvQualifiedType& referencedType,
                                                const ClassType* classScope,
                                                bool isConst, bool isVolatile);
  static CvQualifiedType GetReferenceType(const CvQualifiedType& referencedType);
private:
  static TypeSystem* typeSystem;
  static void ClassInitialize();
  static void ClassExitHanlder(ClientData);
  friend class WrapperFacility;
};

/**
 * The wrapper facility uses specializations of this class with the member
 * static CvQualifiedType name;
 * for each type it needs to use.
 */
template <class T>
struct CvPredefinedType;

// A macro to generate all cv-qualifier combinations for declaring a
// fundamental type's CvPredefinedType<> specialization.
#define _cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(T) \
template <> struct _cable_tcl_EXPORT CvPredefinedType<T> { static CvQualifiedType type; }; \
template <> struct _cable_tcl_EXPORT CvPredefinedType<const T> { static CvQualifiedType type; }; \
template <> struct _cable_tcl_EXPORT CvPredefinedType<volatile T> { static CvQualifiedType type; }; \
template <> struct _cable_tcl_EXPORT CvPredefinedType<const volatile T> { static CvQualifiedType type; }

/*@{
 * A specialization for a fundamental type.
 */
  
// These require some special handling for some compilers.
#ifdef _cable_tcl_NO_CV_VOID
template <> struct _cable_tcl_EXPORT CvPredefinedType<void> { static CvQualifiedType type; };
#else  
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(void);
#endif
#ifndef _cable_tcl_NO_WCHAR_T
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(wchar_t);
#endif
  
// Normal fundamental type definitions.
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(bool);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(char);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(signed char);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(unsigned char);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(short);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(unsigned short);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(int);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(unsigned int);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(long);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(unsigned long);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(float);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(double);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(long double);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(char*);
  
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(bool&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(char&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(signed char&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(unsigned char&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(short&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(unsigned short&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(int&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(unsigned int&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(long&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(unsigned long&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(float&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(double&);
_cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES(long double&);
//@}  

// We are done with this macro.  Don't let it leak out of this header.
#undef _cable_tcl_DECLARE_FUNDAMENTAL_CVTYPES  

/*@{
 * A specialization for a Tcl-related type.
 */
template <> struct _cable_tcl_EXPORT CvPredefinedType<Tcl_Interp> { static CvQualifiedType type; };
template <> struct _cable_tcl_EXPORT CvPredefinedType<Tcl_Interp*> { static CvQualifiedType type; };
//@}
  
} // namespace _cable_tcl_

#endif
