/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableClass.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableClass.h"
#include "cxxTypeSystem.h"

#include <vector>

namespace cable
{

typedef std::vector<Class*> BaseClassVectorBase;
class Class::BaseClassVector: public BaseClassVectorBase
{
public:
  typedef BaseClassVectorBase::iterator iterator;
  typedef BaseClassVectorBase::const_iterator const_iterator;
  typedef BaseClassVectorBase::value_type value_type;
  static BaseClassVector* New() { return new BaseClassVector; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
Class::Class():
  m_BaseClassVector(*BaseClassVector::New())
{
  m_Abstract = false;
}

//----------------------------------------------------------------------------
Class::~Class()
{
  m_BaseClassVector.Delete();
}

//----------------------------------------------------------------------------
void Class::Print(std::ostream& os, Indent indent) const
{
  os << indent << "class " << this->GetName() << "\n";
  Indent nextIndent = indent.GetNextIndent();
  os << nextIndent << "{\n";
  this->PrintMembers(os, nextIndent);
  os << nextIndent << "};\n";
}

//----------------------------------------------------------------------------
ClassType* Class::GetClassType() const
{
  return m_ClassType;
}

//----------------------------------------------------------------------------
void Class::SetClassType(ClassType* type)
{
  m_ClassType = type;
}

//----------------------------------------------------------------------------
bool Class::GetAbstract() const
{
  return m_Abstract;
}

//----------------------------------------------------------------------------
void Class::SetAbstract(bool abstract)
{
  m_Abstract = abstract;
}

//----------------------------------------------------------------------------
void Class::AddBaseClass(Class* c)
{
  m_BaseClassVector.push_back(c);
}

//----------------------------------------------------------------------------
const cxx::ClassType* Class::GetCxxClassType(cxx::TypeSystem* ts) const
{
  cxx::ClassTypes bases;
  for(BaseClassVector::const_iterator i = m_BaseClassVector.begin();
      i != m_BaseClassVector.end(); ++i)
    {
    bases.push_back((*i)->GetCxxClassType(ts));
    }
  return ts->GetClassType(this->GetQualifiedName(), m_Abstract, bases);
}


} // namespace cable
