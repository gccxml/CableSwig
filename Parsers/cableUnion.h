/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableUnion.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableUnion_h
#define _cableUnion_h

#include "cableClass.h"

namespace cable
{

/** Represent a union.  */
class CABLE_PARSERS_EXPORT Union: public Class
{
public:
  cableTypeMacro(Union, Context);
  static Pointer New() { return new Self; }
  
  /** Print the Union's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Union();
  ~Union();
  
private:
  Union(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
