/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableFunctionType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableFunctionType.h"
#include "cxxTypeSystem.h"

#include <vector>

namespace cable
{

typedef std::vector<Type*> ArgumentTypeVectorBase;
class FunctionType::ArgumentTypeVector: public ArgumentTypeVectorBase
{
public:
  typedef ArgumentTypeVectorBase::iterator iterator;
  typedef ArgumentTypeVectorBase::const_iterator const_iterator;
  typedef ArgumentTypeVectorBase::value_type value_type;
  static ArgumentTypeVector* New() { return new ArgumentTypeVector; }
  void Delete() { delete this; }
};

typedef std::vector<std::string> ArgumentNameVectorBase;
class FunctionType::ArgumentNameVector: public ArgumentNameVectorBase
{
public:
  typedef ArgumentNameVectorBase::iterator iterator;
  typedef ArgumentNameVectorBase::const_iterator const_iterator;
  typedef ArgumentNameVectorBase::value_type value_type;
  static ArgumentNameVector* New() { return new ArgumentNameVector; }
  void Delete() { delete this; }
};

typedef std::vector<std::string> ArgumentAttsVectorBase;
class FunctionType::ArgumentAttsVector: public ArgumentAttsVectorBase
{
public:
  typedef ArgumentAttsVectorBase::iterator iterator;
  typedef ArgumentAttsVectorBase::const_iterator const_iterator;
  typedef ArgumentAttsVectorBase::value_type value_type;
  static ArgumentAttsVector* New() { return new ArgumentAttsVector; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
FunctionType::FunctionType():
  m_ArgumentTypeVector(*ArgumentTypeVector::New()),
  m_ArgumentNameVector(*ArgumentNameVector::New()),
  m_ArgumentAttsVector(*ArgumentAttsVector::New())
{
  m_Returns = 0;
  m_RequiredArguments = 0;
}

//----------------------------------------------------------------------------
FunctionType::~FunctionType()
{
  m_ArgumentTypeVector.Delete();
  m_ArgumentNameVector.Delete();
  m_ArgumentAttsVector.Delete();
}

//----------------------------------------------------------------------------
Type::TypeIdType FunctionType::GetTypeId() const
{
  return FunctionTypeId;
}

//----------------------------------------------------------------------------
bool FunctionType::CreateCxxType(cxx::TypeSystem* ts)
{
  // Make sure we haven't already created the type.
  if(m_CxxType.GetType())
    {
    return true;
    }
  
  const cxx::FunctionType* t = this->GetCxxFunctionType(ts);
  if(t)
    {
    m_CxxType = t->GetCvQualifiedType(false, false);
    return true;
    }
  cableErrorMacro("Couldn't create cxx::FunctionType.");
  return false;
}

//----------------------------------------------------------------------------
Type* FunctionType::GetReturns() const
{
  return m_Returns;
}

//----------------------------------------------------------------------------
void FunctionType::SetReturns(Type* returns)
{
  m_Returns = returns;
}

//----------------------------------------------------------------------------
void FunctionType::AddArgument(Type* argument)
{
  this->AddArgument(argument, false, 0, 0);
}

//----------------------------------------------------------------------------
void FunctionType::AddArgument(Type* argument, bool hasDefault, const char *name, const char *attributes)
{
  std::string argname = name ? name : "";
  std::string argatts = attributes ? attributes : "";
  m_ArgumentTypeVector.push_back(argument);
  m_ArgumentNameVector.push_back(argname);
  m_ArgumentAttsVector.push_back(argatts);
  if(!hasDefault)
    {
    ++m_RequiredArguments;
    }
}

//----------------------------------------------------------------------------
unsigned int FunctionType::GetNumberOfArguments() const
{
  return static_cast<unsigned int>(m_ArgumentTypeVector.size());
}

//----------------------------------------------------------------------------
unsigned int FunctionType::GetNumberOfRequiredArguments() const
{
  return m_RequiredArguments;
}

//----------------------------------------------------------------------------
Type* FunctionType::GetArgument(unsigned int index) const
{
  if(index >= m_ArgumentTypeVector.size())
    {
    return 0;
    }
  return *(m_ArgumentTypeVector.begin() + index);
}

//----------------------------------------------------------------------------
const char* FunctionType::GetArgumentName(unsigned int index) const
{
  if(index >= m_ArgumentNameVector.size())
    {
    return 0;
    }
  // WARNING: Return value only valid for the lifetime of this object.
  return (*(m_ArgumentNameVector.begin() + index)).c_str();
}

//----------------------------------------------------------------------------
const char* FunctionType::GetArgumentAttributes(unsigned int index) const
{
  if(index >= m_ArgumentAttsVector.size())
    {
    return 0;
    }
  // WARNING: Return value only valid for the lifetime of this object.
  return (*(m_ArgumentAttsVector.begin() + index)).c_str();
}

//----------------------------------------------------------------------------
const cxx::FunctionType*
FunctionType::GetCxxFunctionType(cxx::TypeSystem* ts) const
{
  if(!m_Returns || !m_Returns->CreateCxxType(ts))
    {
    cableErrorMacro("No return type set.");
    return 0;
    }
  cxx::CvQualifiedType returns = m_Returns->GetCxxType();
  cxx::CvQualifiedTypes arguments;
  for(ArgumentTypeVector::const_iterator i = m_ArgumentTypeVector.begin();
      i != m_ArgumentTypeVector.end(); ++i)
    {
    if(!(*i)->CreateCxxType(ts))
      {
      cableErrorMacro("Invalid argument type.");
      return 0;
      }
    arguments.push_back((*i)->GetCxxType());
    }
  
  return ts->GetFunctionType(returns, arguments);
}

} // namespace cable
