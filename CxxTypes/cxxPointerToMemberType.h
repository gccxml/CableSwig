/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxPointerToMemberType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxPointerToMemberType_h
#define _cxxPointerToMemberType_h

#include "cxxPointerType.h"

namespace _cxx_
{

class ClassType;

/**
 * Represents a C++ pointer-to-member type.
 */
class _cxx_EXPORT PointerToMemberType: public PointerType
{
public:
  typedef PointerToMemberType Self;
  
  virtual RepresentationType GetRepresentationType() const;
  static PointerToMemberType* SafeDownCast(Type*);
  static const PointerToMemberType* SafeDownCast(const Type*);
  
  virtual String GenerateName(const String&, bool, bool) const;
  
  const ClassType* GetClassType() const;
protected:
  PointerToMemberType(const CvQualifiedType&, const ClassType*);  
  PointerToMemberType(const Self& s): PointerType(s), m_ClassType(NULL) {}
  void operator=(const Self&) {}
  virtual ~PointerToMemberType() {}
  
private:
  /**
   * The class type holding the member.
   */
  const ClassType* m_ClassType;
  
  friend class TypeSystem;
};

} // namespace _cxx_


#endif
