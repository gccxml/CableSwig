/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableOperatorMethod.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableOperatorMethod_h
#define _cableOperatorMethod_h

#include "cableMethod.h"

namespace cable
{

/** Represent an operator member of a class.  */
class CABLE_PARSERS_EXPORT OperatorMethod: public Method
{
public:
  cableTypeMacro(OperatorMethod, Function);
  static Pointer New() { return new Self; }
  
  /** Get the function id for this OperatorMethod.  */
  FunctionIdType GetFunctionId() const;
  
  /** Get the name of the operator as it is called.  */
  String GetCallName() const;
  
  /** Print the function's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  OperatorMethod();
  ~OperatorMethod();  
  
private:
  OperatorMethod(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
