/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableConstructor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableConstructor_h
#define _cableConstructor_h

#include "cableMethod.h"

namespace cable
{

/** Represent a constructor.  */
class CABLE_PARSERS_EXPORT Constructor: public Method
{
public:
  cableTypeMacro(Constructor, Method);
  static Pointer New() { return new Self; }
  
  /** Get the function id for this Constructor.  */
  FunctionIdType GetFunctionId() const;  
  
  /** Print the function's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Constructor();
  ~Constructor();
  
private:
  Constructor(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
