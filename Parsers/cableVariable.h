/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableVariable.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableVariable_h
#define _cableVariable_h

#include "cableNamed.h"

namespace cable
{

class Type;

/** Represent a variable.  */
class CABLE_PARSERS_EXPORT Variable: public Named
{
public:
  cableTypeMacro(Variable, Named);
  static Pointer New() { return new Self; }
  
  /** Get/Set the type of the variable.  */
  Type* GetType() const;
  void SetType(Type* type);
  
  /** Get/Set the initializer string of the variable.  */
  const char* GetInitializer() const;
  void SetInitializer(const char* init);
  
  /** Print the Variable's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Variable();
  ~Variable();
  
  // The type of the variable.
  Type* m_Type;
  
  // The initializer string, if any.
  String m_Initializer;
  
private:
  Variable(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
