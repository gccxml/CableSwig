/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxType_h
#define _cxxType_h

#include "cxxUtils.h"

namespace _cxx_
{

/**
 * Enumeration of identifiers for representation types.
 */
enum RepresentationType
{
  Undefined_id=0,
  
  ArrayType_id, ClassType_id, EnumerationType_id, PointerType_id,
  PointerToMemberType_id, ReferenceType_id, FundamentalType_id,
  FunctionType_id
};

class CvQualifiedType;
class TypeSystem;

/**
 * Abstract interface to a C++ type representation.
 */
class _cxx_EXPORT Type
{
public:
  /**
   * Retrieve what kind of Type this is.
   */
  virtual RepresentationType GetRepresentationType() const = 0;
  
  /*@{
   * Quick type representation test.
   */     
  bool IsArrayType() const           { return this->GetRepresentationType() == ArrayType_id; }
  bool IsClassType() const           { return this->GetRepresentationType() == ClassType_id; } 
  bool IsEnumerationType() const     { return this->GetRepresentationType() == EnumerationType_id; }
  bool IsFunctionType() const        { return this->GetRepresentationType() == FunctionType_id; }
  bool IsFundamentalType() const     { return this->GetRepresentationType() == FundamentalType_id; }
  bool IsPointerType() const         { return this->GetRepresentationType() == PointerType_id; }
  bool IsPointerToMemberType() const { return this->GetRepresentationType() == PointerToMemberType_id; }
  bool IsReferenceType() const       { return this->GetRepresentationType() == ReferenceType_id; }
  bool IsEitherPointerType() const   { return (this->IsPointerType() || this->IsPointerToMemberType()); }
  //@}
  
  virtual CvQualifiedType GetCvQualifiedType(bool, bool) const;

  String Name() const;
  String CvName(bool isConst, bool isVolatile) const;

  virtual String GenerateName(const String& outerType,
                              bool isConst, bool isVolatile) const =0;

  String GenerateDeclaration(const String& name) const;  
  virtual String GenerateDeclaration(const String& name,
                                     bool isConst, bool isVolatile) const;
  
  ///! Compare two types for equality.
  static bool Equal(const Type* l, const Type* r) { return l == r; }

  ///! Provide an ordering function for types.
  static bool Less(const Type* l, const Type* r) { return l < r; }
protected:
  Type() {}
  virtual ~Type() {}
  String GetLeftCvString(bool isConst, bool isVolatile) const;
  String GetRightCvString(bool isConst, bool isVolatile) const;
  String PrepareOuterStringForPostfix(const String&) const;
};


/**
 * An exception of this type is thrown when a representation's
 * SafeDownCast fails.
 */
class TypeDownCastException
{
public:
  TypeDownCastException(RepresentationType from, RepresentationType to);
  TypeDownCastException(const Type* from, RepresentationType to);
  
  String GetMessage() const;
private:
  RepresentationType m_From;
  RepresentationType m_To;
};

  
} // namespace _cxx_

#endif
