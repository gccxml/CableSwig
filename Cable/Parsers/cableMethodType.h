/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableMethodType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableMethodType_h
#define _cableMethodType_h

#include "cableFunctionType.h"

namespace cable
{

class Class;

/** Represent a method type.  Ellipsis argument is not supported.  */
class CABLE_PARSERS_EXPORT MethodType: public FunctionType
{
public:
  cableTypeMacro(MethodType, FunctionType);
  static Pointer New() { return new Self; }
  
  /** Get the type id for this Type.  */
  TypeIdType GetTypeId() const;
  
  /** Create the CxxTypes representation.  */
  bool CreateCxxType(cxx::TypeSystem* ts);
  
  /** Get/Set the Class of which the method is a member.  */
  Class* GetClass() const;
  void SetClass(Class* c);
  
protected:
  MethodType();
  ~MethodType();  
  
  // The class of which the method is a member.
  Class* m_Class;
  
private:
  MethodType(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
