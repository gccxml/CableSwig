/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctFunctionSelector.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctFunctionSelector_h
#define _ctFunctionSelector_h

#include "ctArgument.h"

#include <vector>

namespace _cable_tcl_
{

class FunctionBase;
class Constructor;
class Method;
class Function;
class WrapperFacility;

/**
 * 
 */
class _cable_tcl_EXPORT FunctionSelectorBase
{
public:
  FunctionSelectorBase(const WrapperFacility*, int, Tcl_Obj*CONST[], unsigned int);
  virtual ~FunctionSelectorBase();
  
  CvQualifiedTypes GetArgumentTypes() const;
  
  const Arguments& GetArguments() const;
protected:  
  typedef std::vector<FunctionBase*> CandidateFunctions;
  void SetImplicitArgument(bool staticOnly);
  void AddCandidate(FunctionBase*);
  FunctionBase* ResolveOverload();
  FunctionBase* ResolveOverloadWithSeparateArguments();
  bool CandidateIsViable(unsigned int candidateIndex,
                         const Arguments& arguments);
  bool CxxConversionPossible(const CvQualifiedType& from,
                             const Type* to) const;
  bool TryMagic(int candidateIndex);
  bool TryMagic(int candidateIndex, int parameterIndex);  
  
  const WrapperFacility* m_WrapperFacility;
  int m_Objc;
  Tcl_Obj*CONST* m_Objv;
  unsigned int m_ArgumentCount;
  
  CandidateFunctions m_Candidates;
  std::vector< std::vector<bool> >  m_MatchedArguments;
  std::vector< Arguments >  m_CandidateArguments;
  Arguments m_Arguments;
  std::vector< Argument* > m_ArrayArguments;
};


class _cable_tcl_EXPORT ConstructorSelector: public FunctionSelectorBase
{
public:
  ConstructorSelector(const WrapperFacility*, int, Tcl_Obj*CONST[]);
  ~ConstructorSelector();
  
  void AddCandidate(Constructor*);
  Constructor* Select();
protected:
  void GuessArguments();  
};

class _cable_tcl_EXPORT MethodSelector: public FunctionSelectorBase
{
public:
  MethodSelector(const WrapperFacility*, int, Tcl_Obj*CONST[]);
  ~MethodSelector();
  
  void AddCandidate(Method*);
  Method* Select(bool);
protected:
  void GuessArguments();  
};


class _cable_tcl_EXPORT FunctionSelector: public FunctionSelectorBase
{
public:
  FunctionSelector(const WrapperFacility*, int, Tcl_Obj*CONST[]);
  ~FunctionSelector();
  
  void AddCandidate(Function*);
  Function* Select();
protected:
  void GuessArguments();  
};


} // namespace _cable_tcl_

#endif
