/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableNamespace.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableNamespace_h
#define _cableNamespace_h

#include "cableContext.h"

namespace cable
{

/** Represent a namespace.  */
class CABLE_PARSERS_EXPORT Namespace: public Context
{
public:
  cableTypeMacro(Namespace, Context);
  static Pointer New() { return new Self; }
  
  /** Return whether this is the global namespace.  */
  bool IsGlobalNamespace() const;
  
  /** Print the Namespace's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Namespace();
  ~Namespace();
  
private:
  Namespace(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
