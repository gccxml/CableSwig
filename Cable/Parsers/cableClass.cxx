/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableClass.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableClass.h"
#include "cableConstructor.h"
#include "cableFunctionType.h"
#include "cableReferenceType.h"
#include "cableClassType.h"
#include "cxxTypeSystem.h"


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
  
  bool isCopyable = !m_Abstract && this->HasPublicCopyConstructor();
  return ts->GetClassType(this->GetQualifiedName(), isCopyable, bases);
}

//----------------------------------------------------------------------------
bool Class::HasPublicCopyConstructor() const
{
  // Look for our own copy constructor.
  bool found = false;
  for(Context::Iterator i = this->Begin(); i != this->End(); ++i)
    {
    Constructor* c = Constructor::SafeDownCast(*i);
    if(c && (i.GetAccess() == Context::Public))
      {
      if(this->IsCopyConstructor(c))
        {
        found = true;
        break;
        }
      }
    }
  if(!found) { return false; }
  
  // Now make sure all superclasses' copy constructors are valid.
  for(BaseClassVector::const_iterator i = m_BaseClassVector.begin();
      i != m_BaseClassVector.end(); ++i)
    {
    if(!(*i)->HasPublicCopyConstructor()) { return false; }
    }
  return true;
}

//----------------------------------------------------------------------------
bool Class::IsCopyConstructor(const Method* m) const
{
  // Must be a constructor accepting a reference to this class.
  if(m->GetFunctionId() != Function::ConstructorId) { return false; }
  FunctionType* ft = m->GetFunctionType();
  if(ft->GetNumberOfArguments() != 1) { return false; }
  ReferenceType* rt = ReferenceType::SafeDownCast(ft->GetArgument(0));
  if(!rt) { return false; }
  if(m->GetContext() != static_cast<const Context*>(this)) { return false; }
  ClassType* ct = ClassType::SafeDownCast(rt->GetTarget());
  if(!ct) { return false; }
  if(ct->GetClass() != this) { return false; }  
  return true;
}

void Class::GetBaseClasses(std::vector<Class*>& b) const
{
  for(BaseClassVector::const_iterator i = m_BaseClassVector.begin();
      i != m_BaseClassVector.end(); ++i)
    {
    b.push_back(*i);
    }
}
void Class::GetAllBaseClasses(std::vector<Class*>& b)  const
{
  this->GetBaseClasses(b);
  for(BaseClassVector::const_iterator i = m_BaseClassVector.begin();
      i != m_BaseClassVector.end(); ++i)
    {
    (*i)->GetAllBaseClasses(b);
    }
}

} // namespace cable
