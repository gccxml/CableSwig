/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableFunctionType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

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

//----------------------------------------------------------------------------
FunctionType::FunctionType():
  m_ArgumentTypeVector(*ArgumentTypeVector::New())
{
  m_Returns = 0;
  m_RequiredArguments = 0;
}

//----------------------------------------------------------------------------
FunctionType::~FunctionType()
{
  m_ArgumentTypeVector.Delete();
}

//----------------------------------------------------------------------------
Type::TypeIdType FunctionType::GetTypeId() const
{
  return FunctionTypeId;
}

//----------------------------------------------------------------------------
bool FunctionType::CreateCxxType(cxx::TypeSystem* ts)
{
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
  this->AddArgument(argument, false);
}

//----------------------------------------------------------------------------
void FunctionType::AddArgument(Type* argument, bool hasDefault)
{
  m_ArgumentTypeVector.push_back(argument);
  if(!hasDefault)
    {
    ++m_RequiredArguments;
    }
}

//----------------------------------------------------------------------------
unsigned int FunctionType::GetNumberOfArguments() const
{
  return m_ArgumentTypeVector.size();
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
const cxx::FunctionType*
FunctionType::GetCxxFunctionType(cxx::TypeSystem* ts) const
{
  if(!m_Returns)
    {
    cableErrorMacro("No return type set.");
    return 0;
    }
  cxx::CvQualifiedType returns = m_Returns->GetCxxType();
  cxx::CvQualifiedTypes arguments;
  for(ArgumentTypeVector::const_iterator i = m_ArgumentTypeVector.begin();
      i != m_ArgumentTypeVector.end(); ++i)
    {
    arguments.push_back((*i)->GetCxxType());
    }
  
  return ts->GetFunctionType(returns, arguments);
}

} // namespace cable
