/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableFundamentalType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableFundamentalType_h
#define _cableFundamentalType_h

#include "cableType.h"

namespace cable
{

/** Represent a fundamental type.  */
class CABLE_PARSERS_EXPORT FundamentalType: public Type
{
public:
  cableTypeMacro(FundamentalType, Type);
  static Pointer New() { return new Self; }
  
  /** Get the type id for this Type.  */
  TypeIdType GetTypeId() const;
  
  /** Create the CxxTypes representation.  */
  bool CreateCxxType(cxx::TypeSystem* ts);

  /** Get/Set the fundamental type's name.  */
  const char* GetTypeName() const;
  void SetTypeName(const char* name);
  
protected:
  FundamentalType();
  ~FundamentalType();  
  
  // The name of the fundamental type.
  String m_TypeName;
  
private:
  FundamentalType(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
