/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableEnumeration.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableEnumeration.h"
#include "cxxTypeSystem.h"

#include <map>

namespace cable
{

typedef std::map<String, int> ValueMapBase;
class Enumeration::ValueMap: public ValueMapBase
{
public:
  typedef ValueMapBase::value_type value_type;
  typedef ValueMapBase::iterator iterator;
  typedef ValueMapBase::const_iterator const_iterator;  
  static ValueMap* New() { return new ValueMap; }
  void Delete() { delete this; }
};
  
//----------------------------------------------------------------------------
class Enumeration::ValueMapIterator:
    public Enumeration::ValueMap::const_iterator
{
public:
  typedef Enumeration::ValueMap::const_iterator ValueMapIteratorBase;
  ValueMapIterator() {}
  ValueMapIterator(const ValueMapIteratorBase& r):
    ValueMapIteratorBase(r) {}
  static ValueMapIterator* New() { return new ValueMapIterator; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
Enumeration::Enumeration():
  m_ValueMap(*ValueMap::New())
{
  m_EnumerationType = 0;
}

//----------------------------------------------------------------------------
Enumeration::~Enumeration()
{
  m_ValueMap.Delete();
}

//----------------------------------------------------------------------------
void Enumeration::AddValue(const char* name, int value)
{
  m_ValueMap[name] = value;
}
  
//----------------------------------------------------------------------------
Enumeration::Iterator Enumeration::Begin() const
{
  return Iterator(ValueMapIterator(m_ValueMap.begin()));
}

//----------------------------------------------------------------------------
Enumeration::Iterator Enumeration::End() const
{
  return Iterator(ValueMapIterator(m_ValueMap.end()));
}

//----------------------------------------------------------------------------
EnumerationType* Enumeration::GetEnumerationType() const
{
  return m_EnumerationType;
}

//----------------------------------------------------------------------------
void Enumeration::SetEnumerationType(EnumerationType* type)
{
  m_EnumerationType = type;
}

//----------------------------------------------------------------------------
const cxx::EnumerationType*
Enumeration::GetCxxEnumerationType(cxx::TypeSystem* ts) const
{
  return ts->GetEnumerationType(this->GetQualifiedName());
}

//----------------------------------------------------------------------------
void Enumeration::Print(std::ostream& os, Indent indent) const
{
  if(m_Name != "")
    {
    os << indent << "enum " << m_Name.c_str() << "\n";
    }
  else
    {
    os << indent << "enum\n";
    }
  Indent nextIndent = indent.GetNextIndent();
  os << nextIndent << "{\n";
  
  if(!m_ValueMap.empty())
    {
    ValueMap::const_iterator i = m_ValueMap.begin();
    os << nextIndent << i->first.c_str() << " = " << i->second;
    while(++i != m_ValueMap.end())
      {
      os << ",\n"
         << nextIndent << i->first.c_str() << " = " << i->second;
      }
    os << "\n";
    }
  os << nextIndent << "};\n";
}

//----------------------------------------------------------------------------
Enumeration::Iterator::Iterator(const ValueMapIterator& i):
  m_ValueMapIterator(*ValueMapIterator::New())
{
  m_ValueMapIterator = i;
}
  
//----------------------------------------------------------------------------
Enumeration::Iterator::Iterator(const Iterator& r):
  m_ValueMapIterator(*ValueMapIterator::New())
{
  m_ValueMapIterator = r.m_ValueMapIterator;
}

//----------------------------------------------------------------------------
Enumeration::Iterator::~Iterator()
{
  m_ValueMapIterator.Delete();
}

//----------------------------------------------------------------------------
Enumeration::Iterator& Enumeration::Iterator::operator=(const Iterator& r)
{
  m_ValueMapIterator = r.m_ValueMapIterator;
  return *this;
}

//----------------------------------------------------------------------------
Enumeration::Iterator& Enumeration::Iterator::operator++()
{
  ++m_ValueMapIterator;
  return *this;
}

//----------------------------------------------------------------------------
bool Enumeration::Iterator::operator==(const Iterator& r) const
{
  return (m_ValueMapIterator == r.m_ValueMapIterator);
}

//----------------------------------------------------------------------------
bool Enumeration::Iterator::operator!=(const Iterator& r) const
{
  return (m_ValueMapIterator != r.m_ValueMapIterator);
}

//----------------------------------------------------------------------------
const char* Enumeration::Iterator::operator*() const
{
  return m_ValueMapIterator->first.c_str();
}

//----------------------------------------------------------------------------
int Enumeration::Iterator::GetValue() const
{
  return m_ValueMapIterator->second;
}

} // namespace cable
