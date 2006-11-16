/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableFunctionType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableFunctionType_h
#define _cableFunctionType_h

#include "cableType.h"

namespace _cxx_ { class FunctionType; }

namespace cable
{

/** Represent a function type.  Ellipsis argument is not supported.  */
class CABLE_PARSERS_EXPORT FunctionType: public Type
{
public:
  cableTypeMacro(FunctionType, Type);
  static Pointer New() { return new Self; }
  
  /** Get the type id for this Type.  */
  TypeIdType GetTypeId() const;
  
  /** Create the CxxTypes representation.  */
  bool CreateCxxType(cxx::TypeSystem* ts);
  
  /** Get/Set the return Type of this FunctionType.  */
  Type* GetReturns() const;
  void SetReturns(Type* returns);
  
  /** Add an argument of the given type to this FunctionType.  */
  void AddArgument(Type* argument);
  
  /** Add an argument of the given type to this FunctionType.  */
  void AddArgument(Type* argument, bool hasDefault, const char *name);
  
  /** Get the number of arguments to the function.  */
  unsigned int GetNumberOfArguments() const;
  
  /** Get the number of arguments without default values.  */
  unsigned int GetNumberOfRequiredArguments() const;
  
  /** Get the argument with the given index.  */
  Type* GetArgument(unsigned int index) const;
  
  /** Get the name of the argument with the given index.  */
  const char* GetArgumentName(unsigned int index) const;

  /** Get the CxxTypes representation without cv-qualifiers.  */
  const cxx::FunctionType* GetCxxFunctionType(cxx::TypeSystem* ts) const;
  
protected:
  FunctionType();
  ~FunctionType();  
  
  class ArgumentTypeVector;
  class ArgumentNameVector;
  
  // The argument types of this FunctionType.
  ArgumentTypeVector& m_ArgumentTypeVector;
  ArgumentNameVector& m_ArgumentNameVector;
  
  // The return Type of this FunctionType.
  Type* m_Returns;
  
  // The number of arguments without default values.
  int m_RequiredArguments;
  
private:
  FunctionType(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
