/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableCvTypeWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableCvTypeWriter_h
#define _cableCvTypeWriter_h

#include "cableObject.h"

namespace _cxx_ { class CvQualifiedType; class FunctionType; }

namespace cable
{

/** Class to write CxxTypes generator code.  */
class CABLE_PARSERS_EXPORT CvTypeWriter: public Object
{
public:
  cableTypeMacro(CvTypeWriter, Object);  
  static Pointer New() { return new Self; }
  
  /** Add a type to be written.  */
  void Add(const cxx::CvQualifiedType& cvType);
  
  /** Write the CvType class definitions.  */
  void WriteClasses(std::ostream& os) const;
  
  /** Write the CvType static data member definitions.  */
  void WriteDataDeclarations(std::ostream& os) const;
  
  /** Write the CvType static data member initializations.  */
  void WriteInitializations(std::ostream& os) const;
  
protected:
  CvTypeWriter();
  ~CvTypeWriter();  
  
  // Internal utility methods.
  void AddFunctionTypes(const cxx::FunctionType* ft);
  void WriteInitialization(std::ostream& os,
                           const cxx::CvQualifiedType& cvType) const;
  void WriteArgumentAs(std::ostream& os,
                       const cxx::CvQualifiedType& cvType) const;
  
  class TypeOrdering;
  class TypesAdded;
  
  // The ordering of types as they will be written.
  TypeOrdering& m_TypeOrdering;
  
  // The set of types that have been added.
  TypesAdded& m_TypesAdded;

private:
  CvTypeWriter(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
