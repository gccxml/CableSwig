/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxReferenceType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxReferenceType_h
#define _cxxReferenceType_h

#include "cxxCvQualifiedType.h"

namespace _cxx_
{


/**
 * Represents a C++ reference type.
 */
class _cxx_EXPORT ReferenceType: public Type
{
public:
  typedef ReferenceType Self;
  
  virtual RepresentationType GetRepresentationType() const;
  static ReferenceType* SafeDownCast(Type*);
  static const ReferenceType* SafeDownCast(const Type*);
  
  virtual String GenerateName(const String&, bool, bool) const;
  
  const CvQualifiedType& GetReferencedType() const;

protected:  
  ReferenceType(const CvQualifiedType&);
  ReferenceType(const Self&): m_ReferencedType(NULL) {}
  void operator=(const Self&) {}
  virtual ~ReferenceType() {}
  
private:
  /**
   * The type to which this type refers.
   */
  CvQualifiedType m_ReferencedType;
  
  friend class TypeSystem;
};


} // namespace _cxx_


#endif
