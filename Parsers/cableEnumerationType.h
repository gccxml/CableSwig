/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableEnumerationType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableEnumerationType_h
#define _cableEnumerationType_h

#include "cableType.h"

namespace cable
{

class Enumeration;

/** Represent an enumeration type.  */
class CABLE_PARSERS_EXPORT EnumerationType: public Type
{
public:
  cableTypeMacro(EnumerationType, Type);
  static Pointer New() { return new Self; }
  
  /** Get the type id for this Type.  */
  TypeIdType GetTypeId() const;
  
  /** Create the CxxTypes representation.  */
  bool CreateCxxType(cxx::TypeSystem* ts);
  
  /** Get/Set the Enumeration corresponding to this EnumerationType.  */
  Enumeration* GetEnumeration() const;
  void SetEnumeration(Enumeration* enumeration);
  
protected:
  EnumerationType();
  ~EnumerationType();  
  
  // The Enumeration corresponding to this EnumerationType.
  Enumeration* m_Enumeration;
  
private:
  EnumerationType(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
