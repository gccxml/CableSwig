/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxCvQualifiedType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxCvQualifiedType.h"

namespace _cxx_
{


/**
 * Default constructor.  This only exists to allow CvQualifiedType instances
 * to be stored in STL containers.
 */
CvQualifiedType::CvQualifiedType():
  m_Type(NULL),
  m_Const(false),
  m_Volatile(false)
{
}


/**
 * Don't call this.  CvQualifiedType instances should be obtained from
 * Type::GetCvQualifiedType().
 * Constructor takes a pointer to the type to which these qualifiers
 * refer.  It defaults all qualifier flags to false.
 */
CvQualifiedType::CvQualifiedType(const Type* in_type):
  m_Type(in_type),
  m_Const(false),
  m_Volatile(false)
{
}


/**
 * Don't call this.  CvQualifiedType instances should be obtained from
 * Type::GetCvQualifiedType().
 * This constructor takes a pointer to the type to which these qualifiers
 * refer, and settings for the const and volatile cv-qualifier flags.
 */
CvQualifiedType::CvQualifiedType(const Type* in_type,
                                 bool in_const, bool in_volatile):
  m_Type(in_type),
  m_Const(in_const),
  m_Volatile(in_volatile)
{
}


/**
 * Copy constructor.
 */
CvQualifiedType::CvQualifiedType(const Self& r):
  m_Type(r.m_Type),
  m_Const(r.m_Const),
  m_Volatile(r.m_Volatile)
{
}


/**
 * Construct a CvQualifiedType with the given cv-qualifiers added.
 */
CvQualifiedType CvQualifiedType::GetMoreQualifiedType(bool isConst,
                                                      bool isVolatile) const
{
  return CvQualifiedType(m_Type, m_Const || isConst, m_Volatile || isVolatile);
}


/**
 * Return whether this CvQualifiedType is equally or more cv-qualified at
 * the top level.
 */
bool
CvQualifiedType
::IsEquallyOrMoreCvQualifiedThan(const CvQualifiedType& cvType) const
{
  return !((cvType.IsConst() && !m_Const)
           || (cvType.IsVolatile() && !m_Volatile));
}


/**
 * Return the type name with its cv-qualifiers.
 */
String CvQualifiedType::GenerateName(const String& outerType,
                                     bool isConst, bool isVolatile) const
{
  bool isc = isConst || m_Const;
  bool isv = isVolatile || m_Volatile;
  
  return m_Type->GenerateName(outerType, isc, isv);
}


/**
 * Get the name of the type as it would be used in a declaration with the
 * given name.
 *
 */
String CvQualifiedType::GenerateDeclaration(const String& name,
                                            bool isConst, bool isVolatile) const
{
  bool isc = isConst || m_Const;
  bool isv = isVolatile || m_Volatile;
  
  return m_Type->GenerateDeclaration(name, isc, isv);
}


/**
 * CvQualifiedTypes compare equal iff they refer to the same Type, and
 * have the same cv-qualifiers.
 */
bool CvQualifiedType::operator== (const Self& r) const
{
  return ((Type::Equal(m_Type, r.m_Type))
          && (m_Const == r.m_Const)
          && (m_Volatile == r.m_Volatile));
}


/**
 * Uniquely orders CvQualifiedType instances.  Useful for using them as
 * map keys.
 */
bool CvQualifiedType::operator< (const Self& r) const
{
  // First, compare the Type pointers.  Here we take advantage of the
  // fact that types are generated uniquely from the TypeSystem's factory,
  // so that the same type always has the same pointer value.
  if(Type::Less(m_Type, r.m_Type))
    {
    return true;
    }
  else if(Type::Less(r.m_Type, m_Type))
    {
    return false;
    }
  else // if(Type::Equal(m_Type, r.m_Type))
    {
    // The base type is the same.  Compare the cv-qualifiers.
    int lhs = (((m_Const?1:0) << 1) | (m_Volatile?1:0));
    int rhs = (((r.m_Const?1:0) << 1) | (r.m_Volatile?1:0));
    return (lhs < rhs);
    }
}


/**
 * Retrieve the kind of Type to which the cv-qualifiers are applied.
 */
RepresentationType CvQualifiedType::GetRepresentationType() const
{
  return m_Type->GetRepresentationType();
}



} // namespace _cxx_
