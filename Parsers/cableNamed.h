/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableNamed.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableNamed_h
#define _cableNamed_h

#include "cableSourceObject.h"

namespace cable
{

class Context;

/** Superclass for C++ entities that have a name.  */
class CABLE_PARSERS_EXPORT Named: public SourceObject
{
public:
  cableTypeMacro(Named, SourceObject);
  
  /** Get/Set the name of the source entity.  */
  const char* GetName() const;
  void SetName(const char* name);
  
  /** Get/Set the filename of the source entity's declaration.  */
  const char* GetFile() const;
  void SetFile(const char* file);
  
  /** Get/Set the line number of the source entity's declaration.  */
  unsigned long GetLine() const;
  void SetLine(unsigned long line);
  
  /** Get/Set the context of the source entity's declaration.  */
  Context* GetContext() const;
  void SetContext(Context* context);
  
  /** Get the fully qualified name of this Named entity.  */
  String GetQualifiedName() const;
  
protected:
  Named();
  ~Named();
  
  String m_Name;
  String m_File;
  unsigned long m_Line;
  Context* m_Context;
  
private:
  Named(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
