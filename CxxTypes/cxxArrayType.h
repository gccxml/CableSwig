/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxArrayType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxArrayType_h
#define _cxxArrayType_h

#include "cxxCvQualifiedType.h"

namespace _cxx_
{


/**
 * Represents a C-style array type.
 */
class _cxx_EXPORT ArrayType: public Type
{
public:
  typedef ArrayType Self;
  
  virtual RepresentationType GetRepresentationType() const;
  static ArrayType* SafeDownCast(Type*);
  static const ArrayType* SafeDownCast(const Type*);
  
  const CvQualifiedType& GetElementType() const
    { return m_ElementType; }
  
  unsigned long GetLength() const
    { return m_Length; }
  
  virtual String GenerateName(const String&, bool, bool) const;
  virtual String GenerateDeclaration(const String&, bool, bool) const;
protected:
  String GenerateLengthString() const;
  
  ArrayType(const CvQualifiedType&, unsigned long);
  ArrayType(const Self&): m_ElementType(NULL), m_Length(0) {}
  void operator=(const Self&) {}
  virtual ~ArrayType() {}
  
private:
  /**
   * The type of the array's elements.
   */
  CvQualifiedType m_ElementType;

  /**
   * The length of the array.
   */
  unsigned long m_Length;
  
  friend class TypeSystem;
};

} // namespace _cxx_


#endif
