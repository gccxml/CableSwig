/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableConverter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableConverter_h
#define _cableConverter_h

#include "cableMethod.h"

namespace cable
{

/** Represent a type conversion operator.  */
class CABLE_PARSERS_EXPORT Converter: public Method
{
public:
  cableTypeMacro(Converter, Function);
  static Pointer New() { return new Self; }
  
  /** Get the function id for this Converter.  */
  FunctionIdType GetFunctionId() const;
  
  /** Print the function's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Converter();
  ~Converter();  
  
private:
  Converter(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
