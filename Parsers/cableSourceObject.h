/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableSourceObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableSourceObject_h
#define _cableSourceObject_h

#include "cableObject.h"

namespace cable
{

/** Superclass for C++ entity representation classes.  */
class CABLE_PARSERS_EXPORT SourceObject: public Object
{
public:
  cableTypeMacro(SourceObject, Object);
  
  /** Print the source object's representation.  */
  virtual void Print(std::ostream& os, Indent indent) const = 0;
  
protected:
  SourceObject();
  ~SourceObject();
  
private:
  SourceObject(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
