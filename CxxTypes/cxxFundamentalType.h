/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxFundamentalType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxFundamentalType_h
#define _cxxFundamentalType_h

#include "cxxCvQualifiedType.h"

namespace _cxx_
{


/**
 * Represent a C++ fundamental type.  These are defined in 3.9.1.
 */
class _cxx_EXPORT FundamentalType: public Type
{
public:
  typedef FundamentalType Self;
  
  /**
   * Enumerate the fundamental types.
   */
  enum Id { UnsignedChar, UnsignedShortInt, UnsignedInt, UnsignedLongInt,
            UnsignedLongLongInt, SignedChar, Char, ShortInt, Int, LongInt,
            LongLongInt, WChar_t, Bool, Float, Double, LongDouble,
            ComplexFloat, ComplexDouble, ComplexLongDouble, Void,
            NumberOfTypes };
  
  virtual RepresentationType GetRepresentationType() const;
  static FundamentalType* SafeDownCast(Type*);
  static const FundamentalType* SafeDownCast(const Type*);
  
  virtual String GenerateName(const String&, bool, bool) const;

  const Id& GetId() const { return m_Id; }
  
 /*@{
   * Test the id of this FundamentalType.
   */
  bool IsUnsignedChar() const        { return (m_Id == UnsignedChar); }
  bool IsUnsignedShortInt() const    { return (m_Id == UnsignedShortInt); }
  bool IsUnsignedInt() const         { return (m_Id == UnsignedInt); }
  bool IsUnsignedLongInt() const     { return (m_Id == UnsignedLongInt); }
  bool IsUnsignedLongLongInt() const { return (m_Id == UnsignedLongLongInt); }
  bool IsSignedChar() const          { return (m_Id == SignedChar); }
  bool IsChar() const                { return (m_Id == Char); }
  bool IsShortInt() const            { return (m_Id == ShortInt); }
  bool IsInt() const                 { return (m_Id == Int); }
  bool IsLongInt() const             { return (m_Id == LongInt); }
  bool IsLongLongInt() const         { return (m_Id == LongLongInt); }
  bool IsWChar_t() const             { return (m_Id == WChar_t); }
  bool IsBool() const                { return (m_Id == Bool); }
  bool IsFloat() const               { return (m_Id == Float); }
  bool IsDouble() const              { return (m_Id == Double); }
  bool IsLongDouble() const          { return (m_Id == LongDouble); }
  bool IsComplexFloat() const        { return (m_Id == ComplexFloat); }
  bool IsComplexDouble() const       { return (m_Id == ComplexDouble); }
  bool IsComplexLongDouble() const   { return (m_Id == ComplexLongDouble); }
  bool IsVoid() const                { return (m_Id == Void); }
  //@}
  
protected:
  FundamentalType(Id);
  FundamentalType(const Self&) {}
  void operator=(const Self&) {}
  virtual ~FundamentalType() {}
  
private:
  /**
   * Store which integral type this is.
   */
  Id m_Id;
  
  friend class TypeSystem;
  
  static const char* fundamentalTypeNames[NumberOfTypes];
};


} // namespace _cxx_


#endif
