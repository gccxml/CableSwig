/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxTypeSystem.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxTypeSystem_h
#define _cxxTypeSystem_h

// Include all the type representations.
#include "cxxTypes.h"

namespace _cxx_
{

/**
 * A complete system of types.  This class must be used to generate all
 * type representations.
 */
class _cxx_EXPORT TypeSystem
{
public:

  const ArrayType*           GetArrayType(const CvQualifiedType&,
                                          unsigned long);
  ClassType*                 GetClassType(const String&,
                                          bool isCopyable = false,
                                          const ClassTypes& = ClassTypes());
  const EnumerationType*     GetEnumerationType(const String&);
  const FunctionType*        GetFunctionType(const CvQualifiedType&,
                                             const CvQualifiedTypes&);
  const FundamentalType*     GetFundamentalType(FundamentalType::Id);
  const PointerType*         GetPointerType(const CvQualifiedType&);
  const PointerToMemberType* GetPointerToMemberType(const CvQualifiedType&,
                                              const ClassType*);
  const ReferenceType*       GetReferenceType(const CvQualifiedType&);
  
  TypeSystem();
  ~TypeSystem();
  
private:
  // Forward declare the map types used in the implementation of this
  // class.  They need only be defined in the .cxx file.
  struct ArrayTypeMap;
  struct ClassTypeMap;
  struct EnumerationTypeMap;
  struct FunctionTypeMap;
  struct FundamentalTypeMap;
  struct PointerTypeMap;
  struct PointerToMemberTypeMap;
  struct ReferenceTypeMap;
  
  ///! Store all the ArrayType instances that have been allocated.
  ArrayTypeMap* m_ArrayTypeMap;
  
  ///! Store all the ClassType instances that have been allocated.
  ClassTypeMap* m_ClassTypeMap;
  
  ///! Store all the EnumerationType instances that have been allocated.
  EnumerationTypeMap* m_EnumerationTypeMap;

  ///! Store all the FunctionType instances that have been allocated.
  FunctionTypeMap* m_FunctionTypeMap;
  
  ///! Store all the FundamentalType instances that have been allocated.
  FundamentalTypeMap* m_FundamentalTypeMap;

  ///! Store all the PointerType instances that have been allocated.
  PointerTypeMap* m_PointerTypeMap;

  ///! Store all the PointerToMemberType instances that have been allocated.
  PointerToMemberTypeMap* m_PointerToMemberTypeMap;  

  ///! Store all the ReferenceType instances that have been allocated.
  ReferenceTypeMap* m_ReferenceTypeMap;  
};

} // namespace _cxx_

#endif

