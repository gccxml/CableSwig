/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxFunctionType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxFunctionType_h
#define _cxxFunctionType_h

#include "cxxCvQualifiedType.h"

#include <vector>

namespace _cxx_
{


/**
 * Represent a C++ function type.  This consists of the return type and
 * argument types.
 */
class _cxx_EXPORT FunctionType: public Type
{
public:
  typedef FunctionType Self;
  
  virtual RepresentationType GetRepresentationType() const;
  static FunctionType* SafeDownCast(Type*);
  static const FunctionType* SafeDownCast(const Type*);
  
  const CvQualifiedType& GetReturnType() const
    { return m_ReturnType; }
  const CvQualifiedTypes& GetArgumentTypes() const
    { return m_Arguments; }
  
  virtual String GenerateName(const String&, bool, bool) const;
  virtual String GenerateDeclaration(const String&, bool, bool) const;
protected:
  String GenerateArgumentString() const;
  
  FunctionType(const CvQualifiedType& returnType,
               const CvQualifiedTypes& arguments);
  FunctionType(const Self&): Type(), m_ReturnType(NULL) {}
  void operator=(const Self&) {}
  virtual ~FunctionType() {}
  
private:
  /**
   * The function's return type.
   */
  CvQualifiedType m_ReturnType;
  
  /**
   * The function's argument types.
   */
  CvQualifiedTypes m_Arguments;
  
  friend class TypeSystem;
};

} // namespace _cxx_


#endif
