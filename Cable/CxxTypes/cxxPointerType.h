/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxPointerType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxPointerType_h
#define _cxxPointerType_h

#include "cxxCvQualifiedType.h"

namespace _cxx_
{


/**
 * Represent a C++ pointer type.
 */
class _cxx_EXPORT PointerType: public Type
{
public:
  typedef PointerType Self;

  virtual RepresentationType GetRepresentationType() const;
  static PointerType* SafeDownCast(Type*);
  static const PointerType* SafeDownCast(const Type*);
  
  virtual String GenerateName(const String&, bool, bool) const;
  
  const CvQualifiedType& GetPointedToType() const;
  
protected:
  PointerType(const CvQualifiedType&);
  PointerType(const Self&): Type(), m_PointedToType(NULL) {}
  void operator=(const Self&) {}
  virtual ~PointerType() {}
  
protected:
  /**
   * The type to which this type refers.
   */
  CvQualifiedType m_PointedToType;
  
  friend class TypeSystem;
};

} // namespace _cxx_


#endif
