/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableIndent.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableIndent_h
#define _cableIndnet_h

#include "cableUtils.h"

namespace cable
{

/** Indentation printing class.  */
class CABLE_PARSERS_EXPORT Indent
{
public:
  Indent();
  Indent(unsigned int indent);
  Indent GetNextIndent() const;
  void Print(std::ostream& os) const;
protected:
  unsigned int m_Indent;
};

std::ostream& operator<<(std::ostream& os, const Indent& indent);

} // namespace cable

#endif
