/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableFunction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableFunction_h
#define _cableFunction_h

#include "cableNamed.h"

namespace cable
{

class FunctionType;

/** Represent a function.  Ellipsis argument is not supported.  */
class CABLE_PARSERS_EXPORT Function: public Named
{
public:
  cableTypeMacro(Function, Named);
  static Pointer New() { return new Self; }
  
  enum FunctionIdType { FunctionId, MethodId, ConstructorId, DestructorId,
                        ConverterId, OperatorMethodId, OperatorFunctionId };
  
  /** Get the function id for this Function.  */
  virtual FunctionIdType GetFunctionId() const;
  
  /** Get the name of the function as it is called.  */
  virtual String GetCallName() const;
  
  /** Get/Set the function type of this Function.  */
  FunctionType* GetFunctionType() const;
  void SetFunctionType(FunctionType* type);
  
  /** Print the function's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Function();
  ~Function();  
  
  // The function type of this Function.
  FunctionType* m_FunctionType;
  
private:
  Function(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
