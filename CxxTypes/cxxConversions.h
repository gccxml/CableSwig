/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxConversions.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxConversions_h
#define _cxxConversions_h

// Include all the type representations.
#include "cxxTypes.h"

namespace _cxx_
{

/**
 * A set of utilities for conversion tests.
 */
class _cxx_EXPORT Conversions
{
public:
  static bool ReferenceCanBindAsIdentity(const CvQualifiedType& from,
                                         const ReferenceType* to);
  static bool ReferenceCanBindAsDerivedToBase(const CvQualifiedType& from,
                                              const ReferenceType* to);
  static bool IsValidQualificationConversion(const PointerType* from,
                                             const PointerType* to);
  static bool CanConvert(const CvQualifiedType& from,
                         const CvQualifiedType& to);
private:
  static bool ReferenceBinding(const CvQualifiedType& from,
                               const CvQualifiedType& to);
};

} // namespace _cxx_

#endif
