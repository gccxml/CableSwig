/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableGenerator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableGenerator_h
#define _cableGenerator_h

#include "cableObject.h"
#include "cableSourceRepresentation.h"

namespace cable
{

/** Superclass for wrapper generation classes.  */
class CABLE_PARSERS_EXPORT Generator: public Object
{
public:
  cableTypeMacro(Generator, Object);
  
  /** Get/Set the input source representation.  */
  SourceRepresentation* GetSourceRepresentation() const;
  void SetSourceRepresentation(SourceRepresentation* sr);
  
  /** Get/Set the output stream.  */
  std::ostream* GetStream() const;
  void SetStream(std::ostream* stream);
  
  /** Generate the wrappers.  */
  bool Generate();
  
protected:
  Generator();
  ~Generator();
  
  // Wrapper generation method to be defined by subclass.
  virtual bool GenerateWrappers()=0;
  
  // Get an identifier used to refer to an operator of the given name.
  const char* GetOperatorName(const char* name) const;
  
  // The source representation from which to generate wrappers.
  SourceRepresentation::Pointer m_SourceRepresentation;
  
  // The output stream to which the subclass will write wrapper code.
  std::ostream* m_Stream;
  
private:
  Generator(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
