/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableSourceObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

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

  /** Get/Set attributes.  */
  const char* GetAttributes() const;
  void SetAttributes(const char* atts);

  /** Print the source object's representation.  */
  virtual void Print(std::ostream& os, Indent indent) const = 0;
  
protected:
  SourceObject();
  ~SourceObject();

  String m_Attributes;

private:
  SourceObject(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
