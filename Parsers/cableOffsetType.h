/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableOffsetType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableOffsetType_h
#define _cableOffsetType_h

#include "cableType.h"

namespace cable
{

class Class;

/** Represent the target of a pointer to member data.  */
class CABLE_PARSERS_EXPORT OffsetType: public Type
{
public:
  cableTypeMacro(OffsetType, Type);
  static Pointer New() { return new Self; }
  
  /** Get the type id for this Type.  */
  TypeIdType GetTypeId() const;
  
  /** Create the CxxTypes representation.  */
  bool CreateCxxType(cxx::TypeSystem* ts);
  
  /** Get/Set the type of the member data.  */
  Type* GetMemberType() const;
  void SetMemberType(Type* type);
  
  /** Get/Set the class containing the member data.  */
  Class* GetClass() const;
  void SetClass(Class* c);
  
protected:
  OffsetType();
  ~OffsetType();  
  
  // The type of the member data.
  Type* m_MemberType;
  
  // The class containing the member data.
  Class* m_Class;
  
private:
  OffsetType(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
