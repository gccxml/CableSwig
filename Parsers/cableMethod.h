/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableMethod.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableMethod_h
#define _cableMethod_h

#include "cableFunction.h"

namespace cable
{

/** Represent a method.  */
class CABLE_PARSERS_EXPORT Method: public Function
{
public:
  cableTypeMacro(Method, Function);
  static Pointer New() { return new Self; }
  
  /** Get the function id for this Method.  */
  FunctionIdType GetFunctionId() const;
  
  /** Get/Set whether the method is static.  */
  bool GetStatic() const;
  void SetStatic(bool s);

  /** Get/Set whether the method is virtual.  */
  bool GetVirtual() const;
  void SetVirtual(bool s);

    /** Get/Set whether the method is pure virtual.  */
  bool GetPureVirtual() const;
  void SetPureVirtual(bool s);

  /** Get/Set whether the method is const.  */
  bool GetConst() const;
  void SetConst(bool c);
  
  /** Print the function's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Method();
  ~Method();
  
  // Whether or not the method is pure virtual.
  bool m_PureVirtual;

  // Whether or not the method is virtual.
  bool m_Virtual;

  // Whether or not the method is static.
  bool m_Static;

  // Whether or not the method is const.
  bool m_Const;  
  
private:
  Method(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
