/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableNamespaceAlias.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableNamespaceAlias_h
#define _cableNamespaceAlias_h

#include "cableContext.h"

namespace cable
{

class Namespace;

/** Represent a namespace alias.  */
class CABLE_PARSERS_EXPORT NamespaceAlias: public Context
{
public:
  cableTypeMacro(NamespaceAlias, Context);
  static Pointer New() { return new Self; }
  
  /** Print the NamespaceAlias's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
  /** Get/Set the real namespace referecned by this alias.  */
  Namespace* GetNamespace() const;
  void SetNamespace(Namespace* ns);
  
  /** Override Context::AddNamed to forward addition to real namespace.  */
  void AddNamed(Named* named, Access access);
  
protected:
  NamespaceAlias();
  ~NamespaceAlias();
  
  // The real namespace that is aliased.
  Namespace* m_Namespace;
  
private:
  NamespaceAlias(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
