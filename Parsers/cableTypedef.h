/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableTypedef.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableTypedef_h
#define _cableTypedef_h

#include "cableNamed.h"

namespace cable
{

class Type;

/** Represent a typedef.  */
class CABLE_PARSERS_EXPORT Typedef: public Named
{
public:
  cableTypeMacro(Typedef, Named);
  static Pointer New() { return new Self; }
  
  /** Get/Set the target type of the typedef.  */
  Type* GetType() const;
  void SetType(Type* type);
  
  /** Print the Typedef's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Typedef();
  ~Typedef();
  
  // The target of the typedef.
  Type* m_Type;
  
private:
  Typedef(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
