/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxType.cxx
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
 * Given cv-qualifiers, construct the CvQualifiedType referring to
 * this type.
 */
CvQualifiedType
Type::GetCvQualifiedType(bool isConst, bool isVolatile) const
{
  return CvQualifiedType(this, isConst, isVolatile);
}


/**
 * Get the name of the type.
 */
String
Type::Name() const
{
  return this->CvName(false, false);
}


/**
 * Get the name of the type with the given cv-qualifiers.
 */
String
Type::CvName(bool isConst, bool isVolatile) const
{
  return this->GetCvQualifiedType(isConst, isVolatile).GenerateDeclaration("");
}


/**
 * Get the name of the type as it would be used in a declaration with the
 * given name.
 */
String
Type::GenerateDeclaration(const String& name) const
{
  return this->GenerateDeclaration(name, false, false);
}


/**
 * Get the name of the type as it would be used in a declaration with the
 * given name.
 */
String
Type::GenerateDeclaration(const String& name,
                          bool isConst, bool isVolatile) const
{
  return this->GenerateName(name, isConst, isVolatile);
}
  

/**
 * Get a cv-qualifier string that can be conactenated on the left end
 * of a string.
 */
String Type::GetLeftCvString(bool isConst, bool isVolatile) const
{
  if(isConst && isVolatile)
    {
    return "const volatile ";
    }
  else if(isConst)
    {
    return "const ";
    }
  else if(isVolatile)
    {
    return "volatile ";
    }
  else
    {
    return "";
    }
}


/**
 * Get a cv-qualifier string that can be conactenated on the right end
 * of a string.
 */
String Type::GetRightCvString(bool isConst, bool isVolatile) const
{
  if(isConst && isVolatile)
    {
    return " const volatile";
    }
  else if(isConst)
    {
    return " const";
    }
  else if(isVolatile)
    {
    return " volatile";
    }
  else
    {
    return "";
    }
}


/**
 * Prepare a string holding the "outer" type for concatenation to the right
 * of a named type.  This basically adds a space if the first character
 * is alphanumeric or a colon.
 */
String Type::PrepareOuterStringForPostfix(const String& outer) const
{
  if(outer.length() > 0)
    {
    char first = outer[0];
    if(((first >= 'A') && (first <= 'Z'))
       || ((first >= 'a') && (first <= 'z'))
       || ((first >= '0') && (first <= '9'))
       || (first == '_')
       || (first == ':'))
      {
      return " "+outer;
      }
    }
  return outer;
}


/**
 * Constructor that takes a RepresentationType for both the "from" and
 * "to" types of the failed cast.
 */
TypeDownCastException::TypeDownCastException(RepresentationType from,
                                             RepresentationType to):
  m_From(from),
  m_To(to)
{
}


/**
 * Constructor that automatically pulls RepresentationType out of the
 * "from" type of the failed cast.
 */
TypeDownCastException::TypeDownCastException(const Type* from,
                                             RepresentationType to):
  m_From(from? from->GetRepresentationType():Undefined_id),
  m_To(to)
{
}


/**
 * Get the exceptions text message.
 */
String TypeDownCastException::GetMessage() const
{
  static const char* representationTypeNames[] =
    {"undefined", "ArrayType", "ClassType", "PointerType",
     "PointerToMemberType", "ReferenceType", "FundamentalType",
     "FunctionType"};
  
  String from = representationTypeNames[m_From];
  String to = representationTypeNames[m_To];
  return "Attempt to cast object of type "+from+" to "+to+".";
}


} // namespace _cxx_
