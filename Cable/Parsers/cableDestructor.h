/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableDestructor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableDestructor_h
#define _cableDestructor_h

#include "cableMethod.h"

namespace cable
{

/** Represent a destructor.  */
class CABLE_PARSERS_EXPORT Destructor: public Method
{
public:
  cableTypeMacro(Destructor, Method);
  static Pointer New() { return new Self; }
  
  /** Get the function id for this Destructor.  */
  FunctionIdType GetFunctionId() const;  
  
  /** Print the function's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Destructor();
  ~Destructor();
  
private:
  Destructor(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
