/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableStruct.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableStruct_h
#define _cableStruct_h

#include "cableClass.h"

namespace cable
{

/** Represent a struct.  */
class CABLE_PARSERS_EXPORT Struct: public Class
{
public:
  cableTypeMacro(Struct, Context);
  static Pointer New() { return new Self; }
  
  /** Print the Struct's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Struct();
  ~Struct();
  
private:
  Struct(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
