/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableXMLSourceElement.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableXMLSourceElement.h"
#include "cableXMLSourceParser.h"

#include <map>
#include <vector>

namespace cable
{

typedef std::map<String, String> AttributeMapBase;
class XMLSourceElement::AttributeMap: public AttributeMapBase
{
public:
  typedef AttributeMapBase::value_type value_type;
  typedef AttributeMapBase::iterator iterator;
  typedef AttributeMapBase::const_iterator const_iterator;  
  static AttributeMap* New() { return new AttributeMap; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
typedef std::vector<XMLSourceElement::Pointer> NestedElementVectorBase;
class XMLSourceElement::NestedElementVector: public NestedElementVectorBase
{
public:
  typedef NestedElementVectorBase::value_type value_type;
  typedef NestedElementVectorBase::iterator iterator;
  typedef NestedElementVectorBase::const_iterator const_iterator;  
  static NestedElementVector* New() { return new NestedElementVector; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
XMLSourceElement::XMLSourceElement():
  m_AttributeMap(*AttributeMap::New()),
  m_NestedElementVector(*NestedElementVector::New())
{
}

//----------------------------------------------------------------------------
XMLSourceElement::~XMLSourceElement()
{
  m_AttributeMap.Delete();
  m_NestedElementVector.Delete();
}

//----------------------------------------------------------------------------
const char* XMLSourceElement::GetName() const
{
  return m_Name.c_str();
}

//----------------------------------------------------------------------------
const char* XMLSourceElement::GetId() const
{
  return this->GetAttribute("id");
}

//----------------------------------------------------------------------------
const char* XMLSourceElement::GetAttribute(const char* name) const
{
  AttributeMap::const_iterator i = m_AttributeMap.find(name);
  if(i != m_AttributeMap.end())
    {
    return i->second.c_str();
    }
  return 0;  
}

//----------------------------------------------------------------------------
XMLSourceElement* XMLSourceElement::GetElement(const char* name) const
{
  const char* id = this->GetAttribute(name);
  if(!id) { return 0; }
  return m_XMLSourceParser->GetSourceElement(id);
}

//----------------------------------------------------------------------------
unsigned int XMLSourceElement::GetNumberOfNestedElements() const
{
  return m_NestedElementVector.size();
}
  
//----------------------------------------------------------------------------
XMLSourceElement* XMLSourceElement::GetNestedElement(unsigned int index) const
{
  if(index >= m_NestedElementVector.size()) { return 0; }
  return m_NestedElementVector[index];
}

//----------------------------------------------------------------------------
void XMLSourceElement::Print(std::ostream& os, Indent indent)
{
  os << indent << "<" << m_Name.c_str();
  for(AttributeMap::const_iterator i = m_AttributeMap.begin();
      i != m_AttributeMap.end(); ++i)
    {
    os << " " << i->first.c_str() << "=\"" << i->second.c_str() << "\"";
    }
  if(m_NestedElementVector.empty())
    {
    os << "/>\n";
    }
  else
    {
    os << ">\n";
    for(NestedElementVector::const_iterator i = m_NestedElementVector.begin();
        i != m_NestedElementVector.end(); ++i)
      {
      (*i)->Print(os, indent.GetNextIndent());
      }
    os << indent << "</" << m_Name.c_str() << ">\n";
    }
}

//----------------------------------------------------------------------------
void XMLSourceElement::SetName(const char* name)
{
  m_Name = name;
}

//----------------------------------------------------------------------------
void XMLSourceElement::SetAttributes(const char** atts)
{
  for(unsigned int i=0; atts[i] && atts[i+1]; i+=2)
    {
    m_AttributeMap[atts[i]] = atts[i+1];
    }  
}

//----------------------------------------------------------------------------
void XMLSourceElement::SetXMLSourceParser(XMLSourceParser* parser)
{
  m_XMLSourceParser = parser;
}

//----------------------------------------------------------------------------
void XMLSourceElement::AddNestedElement(XMLSourceElement* element)
{
  m_NestedElementVector.push_back(element);
}

} // namespace cable
