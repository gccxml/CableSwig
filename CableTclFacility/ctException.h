/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctException.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctException_h
#define _ctException_h

#include "ctUtils.h"

namespace _cable_tcl_
{

class TclException
{
public:
  TclException(const String& message): m_ExceptionMessage(message) {}
  TclException(const String& message, const String& target):
    m_ExceptionMessage(message+": "+target) {}
  const String& GetExceptionMessage() { return m_ExceptionMessage; }
private:
  String m_ExceptionMessage;
};

#define _cable_tcl_UnknownConversionException(x, y) \
  TclException("Don't know how to convert "+String(x)+" to "+String(y))

} // namespace _cable_tcl_

#endif
