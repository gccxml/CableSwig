/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctInit.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctWrapperFacility.h"

// Make sure wrapper initialization function has external C-style linkage.
extern "C" { _cable_tcl_EXPORT int Cabletclfacility_Init(Tcl_Interp*); }

/**
 * When the WrapTclFacilitator library is loaded by a Tcl interpreter,
 * this is called to initialize it.  This just passes the call to the
 * wrapper facility class.
 */
_cable_tcl_EXPORT int Cabletclfacility_Init(Tcl_Interp* interp)
{
  _cable_tcl_::WrapperFacility::GetForInterpreter(interp);
  return TCL_OK;
}
