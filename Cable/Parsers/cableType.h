/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableType_h
#define _cableType_h

#include "cableSourceObject.h"

namespace _cxx_ { class CvQualifiedType; class TypeSystem; }

namespace cable
{

/** Superclass for C++ type representation.  */
class CABLE_PARSERS_EXPORT Type: public SourceObject
{
public:
  cableTypeMacro(Type, SourceObject);
  
  enum TypeIdType { FundamentalTypeId, PointerTypeId, ReferenceTypeId,
                    ArrayTypeId, OffsetTypeId, MethodTypeId, FunctionTypeId,
                    ClassTypeId, EnumerationTypeId };
  
  /** Get the type id for this Type.  */
  virtual TypeIdType GetTypeId() const=0;
  
  /** Create the CxxTypes representation.  */
  virtual bool CreateCxxType(cxx::TypeSystem* ts)=0;
  
  /** Get the CxxTypes library's representation for this type.  */
  const cxx::CvQualifiedType& GetCxxType() const;
  
  /** Get/Set the const cv-qualifier.  */
  bool GetConst() const;
  void SetConst(bool c);
  
  /** Get/Set the volatile cv-qualifier.  */
  bool GetVolatile() const;
  void SetVolatile(bool v);
  
  /** Print the Type's representation.  */
  void Print(std::ostream& os, Indent indent) const;
protected:
  Type();
  ~Type();  
  
  // The type's top-level cv-qualifiers.
  bool m_Const;
  bool m_Volatile;
  
  // The type's CxxTypes library representation.
  cxx::CvQualifiedType& m_CxxType;
  
private:
  Type(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
