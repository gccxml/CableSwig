/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableSourceRepresentation.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableSourceRepresentation.h"
#include "cableSourceObject.h"
#include "cableNamespace.h"
#include "cableType.h"

#include "cxxTypeSystem.h"

#include <map>

namespace cable
{

typedef std::map<String, SourceObject::Pointer> SourceObjectMapBase;
class SourceRepresentation::SourceObjectMap: public SourceObjectMapBase
{
public:
  typedef SourceObjectMapBase::value_type value_type;
  typedef SourceObjectMapBase::iterator iterator;
  typedef SourceObjectMapBase::const_iterator const_iterator;  
  static SourceObjectMap* New() { return new SourceObjectMap; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
SourceRepresentation::SourceRepresentation():
  m_SourceObjectMap(*SourceObjectMap::New())
{
  m_TypeSystem = new cxx::TypeSystem;
}

//----------------------------------------------------------------------------
SourceRepresentation::~SourceRepresentation()
{
  m_SourceObjectMap.Delete();
  delete m_TypeSystem;
}

//----------------------------------------------------------------------------
SourceObject* SourceRepresentation::GetSourceObject(const char* id) const
{
  SourceObjectMap::const_iterator i = m_SourceObjectMap.find(id);
  if(i != m_SourceObjectMap.end())
    {
    return i->second;
    }
  return 0;
}

//----------------------------------------------------------------------------
void SourceRepresentation::SetSourceObject(const char* id,
                                           SourceObject* object)
{
  SourceObjectMap::iterator i = m_SourceObjectMap.find(id);
  
  // If the object is NULL, remove the id from the map.
  if(object)
    {
    if(i != m_SourceObjectMap.end())
      {
      i->second = object;
      }
    else
      {
      m_SourceObjectMap[id] = object;
      }
    }
  else
    {
    if(i != m_SourceObjectMap.end())
      {
      m_SourceObjectMap.erase(i);
      }
    }
}

//----------------------------------------------------------------------------
cxx::TypeSystem* SourceRepresentation::GetTypeSystem() const
{
  return m_TypeSystem;
}

//----------------------------------------------------------------------------
void SourceRepresentation::SetGlobalNamespace(const char* id)
{
  m_GlobalNamespaceId = id;
}

//----------------------------------------------------------------------------
const Namespace* SourceRepresentation::GetGlobalNamespace() const
{
  const char* id = m_GlobalNamespaceId.c_str();
  SourceObjectMap::const_iterator i = m_SourceObjectMap.find(id);
  if(i != m_SourceObjectMap.end())
    {
    return Namespace::SafeDownCast(i->second);
    }
  return 0;
}

//----------------------------------------------------------------------------
void SourceRepresentation::Print(std::ostream& os) const
{
  const Namespace* gns = this->GetGlobalNamespace();
  if(!gns) { return; }
  gns->Print(os, Indent());
}

//----------------------------------------------------------------------------
bool SourceRepresentation::CreateCxxTypes()
{
  for(SourceObjectMap::iterator i = m_SourceObjectMap.begin();
      i != m_SourceObjectMap.end(); ++i)
    {
    if(Type* t = Type::SafeDownCast(i->second))
      {
      if(!t->CreateCxxType(m_TypeSystem))
        {
        return false;
        }
      }
    }
  return true;
}

} // namespace cable
