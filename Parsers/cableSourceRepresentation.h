/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableSourceRepresentation.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableSourceRepresentation_h
#define _cableSourceRepresentation_h

#include "cableObject.h"

namespace _cxx_ { class TypeSystem; }

namespace cable
{

class SourceObject;
class Namespace;

/** Primary source representation class.  */
class CABLE_PARSERS_EXPORT SourceRepresentation: public Object
{
public:
  cableTypeMacro(SourceRepresentation, Object);
  static Pointer New() { return new Self; }
  
  /** Get/Set the source object corresponding to an identifier.  */
  SourceObject* GetSourceObject(const char* id) const;
  void SetSourceObject(const char* id, SourceObject* object);
  
  /** Get the CxxTypes TypeSystem representing types.  */
  cxx::TypeSystem* GetTypeSystem() const;
  
  /** Set the id used to identify the global namespace object.  */
  void SetGlobalNamespace(const char* id);
  
  /** Get the global namespace of the representation.  */
  const Namespace* GetGlobalNamespace() const;
  
  /** Print the representation.  */
  void Print(std::ostream& os) const;
  
protected:
  SourceRepresentation();
  ~SourceRepresentation();
  
  class SourceObjectMap;
  
  // The complete set of objects involved in the representation.
  SourceObjectMap& m_SourceObjectMap;
  
  // The id corresponding to the global namespace.
  String m_GlobalNamespaceId;
  
  // The CxxTypes TypeSystem representing types for us.
  cxx::TypeSystem* m_TypeSystem;
  
private:
  SourceRepresentation(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
