/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableField.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableField_h
#define _cableField_h

#include "cableVariable.h"

namespace cable
{

/** Represent a variable.  */
class CABLE_PARSERS_EXPORT Field: public Variable
{
public:
  cableTypeMacro(Field, Variable);
  static Pointer New() { return new Self; }

  /** Parse the Field's offset from string input.  */
  void ParseOffset(const char* offset);

  /** Get the Field's offset.  */
  int GetOffset(void) const;

  /** Set the Field's offset.  */
  void SetOffset(int offset);

  /** Print the Field's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Field();
  ~Field();

  int m_Offset;

private:
  Field(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
