/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableContext.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableContext.h"

#include <map>

namespace cable
{

namespace
{
// An entry in the set of names in a context.
class Entry
{
public:
  Entry(const Entry& e): m_Named(e.m_Named), m_Access(e.m_Access) {}
  Entry(Named* n, Context::Access a): m_Named(n), m_Access(a) {}
  Entry& operator=(const Entry& e)
    { m_Named = e.m_Named; m_Access = e.m_Access; return *this; }
  Named* GetNamed() const { return m_Named; }
  Context::Access GetAccess() const { return m_Access; }  
private:
  Named* m_Named;
  Context::Access m_Access;
};
} // anonymous namespace

typedef std::multimap<String, Entry> MemberMapBase;
class Context::MemberMap: public MemberMapBase
{
public:
  typedef MemberMapBase::value_type value_type;
  typedef MemberMapBase::iterator iterator;
  typedef MemberMapBase::const_iterator const_iterator;  
  static MemberMap* New() { return new MemberMap; }
  void Delete() { delete this; }
};

class Context::MemberMapIterator: public Context::MemberMap::const_iterator
{
public:
  typedef Context::MemberMap::const_iterator MemberMapIteratorBase;
  MemberMapIterator() {}
  MemberMapIterator(const MemberMapIteratorBase& r):
    MemberMapIteratorBase(r) {}
  static MemberMapIterator* New() { return new MemberMapIterator; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
Context::Context():
  m_MemberMap(*MemberMap::New())
{
}

//----------------------------------------------------------------------------
Context::~Context()
{
  m_MemberMap.Delete();
}

//----------------------------------------------------------------------------
void Context::AddNamed(Named* named, Access access)
{
  const char* name = named->GetName();
  MemberMap::value_type entry(name, Entry(named, access));
  m_MemberMap.insert(entry);
}

//----------------------------------------------------------------------------
Context::Iterator Context::Begin() const
{
  return Iterator(MemberMapIterator(m_MemberMap.begin()));
}

//----------------------------------------------------------------------------
Context::Iterator Context::End() const
{
  return Iterator(MemberMapIterator(m_MemberMap.end()));
}

//----------------------------------------------------------------------------
Context::Iterator Context::LowerBound(const char* name) const
{
  return Iterator(MemberMapIterator(m_MemberMap.lower_bound(name)));
}

//----------------------------------------------------------------------------
Context::Iterator Context::UpperBound(const char* name) const
{
  return Iterator(MemberMapIterator(m_MemberMap.upper_bound(name)));
}

//----------------------------------------------------------------------------
void Context::PrintMembers(std::ostream& os, Indent indent) const
{
  String n = this->GetNameOfClass();
  if(n == "Namespace")
    {
    for(Iterator i = this->Begin(); i != this->End(); ++i)
      {
      if(i.GetAccess() == Public)
        {
        (*i)->Print(os, indent);
        }
      }
    }
  else
    {
    os << indent << "public:\n";
    for(Iterator i = this->Begin(); i != this->End(); ++i)
      {
      if(i.GetAccess() == Public)
        {
        (*i)->Print(os, indent.GetNextIndent());
        }
      }
    os << indent << "protected:\n";
    for(Iterator i = this->Begin(); i != this->End(); ++i)
      {
      if(i.GetAccess() == Protected)
        {
        (*i)->Print(os, indent.GetNextIndent());
        }
      }
    os << indent << "private:\n";
    for(Iterator i = this->Begin(); i != this->End(); ++i)
      {
      if(i.GetAccess() == Private)
        {
        (*i)->Print(os, indent.GetNextIndent());
        }
      }
    }
}

//----------------------------------------------------------------------------
Context::Iterator::Iterator(const MemberMapIterator& i):
  m_MemberMapIterator(*MemberMapIterator::New())
{
  m_MemberMapIterator = i;
}
  
//----------------------------------------------------------------------------
Context::Iterator::Iterator(const Iterator& r):
  m_MemberMapIterator(*MemberMapIterator::New())
{
  m_MemberMapIterator = r.m_MemberMapIterator;
}

//----------------------------------------------------------------------------
Context::Iterator::~Iterator()
{
  m_MemberMapIterator.Delete();
}

//----------------------------------------------------------------------------
Context::Iterator& Context::Iterator::operator=(const Iterator& r)
{
  m_MemberMapIterator = r.m_MemberMapIterator;
  return *this;
}

//----------------------------------------------------------------------------
Context::Iterator& Context::Iterator::operator++()
{
  ++m_MemberMapIterator;
  return *this;
}

//----------------------------------------------------------------------------
bool Context::Iterator::operator==(const Iterator& r) const
{
  return (m_MemberMapIterator == r.m_MemberMapIterator);
}

//----------------------------------------------------------------------------
bool Context::Iterator::operator!=(const Iterator& r) const
{
  return (m_MemberMapIterator != r.m_MemberMapIterator);
}

//----------------------------------------------------------------------------
Context::Access Context::Iterator::GetAccess() const
{
  return m_MemberMapIterator->second.GetAccess();
}

//----------------------------------------------------------------------------
Named* Context::Iterator::operator*() const
{
  return m_MemberMapIterator->second.GetNamed();
}

} // namespace cable
