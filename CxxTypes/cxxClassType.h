/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxClassType.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cxxClassType_h
#define _cxxClassType_h

#include "cxxCvQualifiedType.h"

#include <vector>

namespace _cxx_
{
class _cxx_EXPORT ClassType;

/**
 * A vector of ClassType pointers.
 */
typedef std::vector<const ClassType*> ClassTypes;

/**
 * Represents a C++ class type.  This could have been produced by a
 * class, struct, union, template full specialization, or template
 * instantiation.
 */
class _cxx_EXPORT ClassType: public Type
{
public:
  typedef ClassType Self;
  
  virtual RepresentationType GetRepresentationType() const;
  static ClassType* SafeDownCast(Type*);
  static const ClassType* SafeDownCast(const Type*);

  bool IsCopyable() const;

  void MergeClassInformation(bool, const ClassTypes&);
  
  String GetName() const;
  ClassTypes::const_iterator ParentsBegin() const;
  ClassTypes::const_iterator ParentsEnd() const;
  void GetAllSuperclasses(ClassTypes&) const;
  bool IsSubclassOf(const ClassType*) const;
  
  virtual String GenerateName(const String&, bool, bool) const;
protected:
  ClassType(const String&, bool isCopyable, const ClassTypes&);
  ClassType(const Self&) {}
  void operator=(const Self&) {}
  virtual ~ClassType() {}
  
private:  
  /**
   * The name of the class.
   */
  String m_Name;
  
  /**
   * Flag for whether the class is copyable.
   */
  bool m_Copyable;
  
  /**
   * The immediate public superclasses of this class.
   * A pointer or reference ot this class can be cast up to these
   * types.
   */
  ClassTypes m_Parents;
  
  /**
   * The list of types from which this class can construct.
   */
  CvQualifiedTypes m_ConversionByConstructor;
  
  /**
   * The list of types to which this class can convert by type conversion
   * operator.
   */
  CvQualifiedTypes m_ConversionOperators;
  
  friend class TypeSystem;
};


} // namespace _cxx_


#endif
