/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableClassType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableClassType_h
#define _cableClassType_h

#include "cableType.h"

namespace cable
{

class Class;

/** Represent a Class type.  */
class CABLE_PARSERS_EXPORT ClassType: public Type
{
public:
  cableTypeMacro(ClassType, Type);
  static Pointer New() { return new Self; }
  
  /** Get the type id for this Type.  */
  TypeIdType GetTypeId() const;
  
  /** Create the CxxTypes representation.  */
  bool CreateCxxType(cxx::TypeSystem* ts);
  
  /** Get/Set the Class corresponding to this ClassType.  */
  Class* GetClass() const;
  void SetClass(Class* c);
  
protected:
  ClassType();
  ~ClassType();  
  
  // The Class that corresponds to this ClassType.
  Class* m_Class;
  
private:
  ClassType(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
