/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxEnumerationType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxEnumerationType_h
#define _cxxEnumerationType_h

#include "cxxCvQualifiedType.h"

namespace _cxx_
{

/**
 * Represents a C++ enumeration type.
 */
class _cxx_EXPORT EnumerationType: public Type
{
public:
  typedef EnumerationType Self;
  
  virtual RepresentationType GetRepresentationType() const;
  static EnumerationType* SafeDownCast(Type*);
  static const EnumerationType* SafeDownCast(const Type*);

  String GetName() const;
  virtual String GenerateName(const String&, bool, bool) const;
protected:
  EnumerationType(const String&);
  EnumerationType(const Self&): Type() {}
  void operator=(const Self&) {}
  virtual ~EnumerationType() {}
  
private:  
  /**
   * The name of the class.
   */
  String m_Name;
  
  friend class TypeSystem;
};


} // namespace _cxx_


#endif
