/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableClass.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableClass_h
#define _cableClass_h

#include "cableContext.h"

namespace _cxx_ { class ClassType; class TypeSystem; }

namespace cable
{

class ClassType;
class Method;

/** Represent a class.  */
class CABLE_PARSERS_EXPORT Class: public Context
{
public:
  cableTypeMacro(Class, Context);
  static Pointer New() { return new Self; }
  
  /** Get/Set the ClassType corresponding to the Class.  */
  ClassType* GetClassType() const;
  void SetClassType(ClassType* type);
  
  /** Get/Set whether the Class is abstract.  */
  bool GetAbstract() const;
  void SetAbstract(bool abstract);
  
  /** Add a public base class.  */
  void AddBaseClass(Class* c);
  
  /** Get the CxxTypes library representation for this class's type.  */
  const cxx::ClassType* GetCxxClassType(cxx::TypeSystem* ts) const;
  
  /** Print the Class's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
  /** Test whether the class and all superclasses have a public copy
      constructor.  */
  bool HasPublicCopyConstructor() const;
  
  /** Test whether the given method is a copy constructor for this
      class.  */
  bool IsCopyConstructor(const Method* m) const;
  
protected:
  Class();
  ~Class();
  
  class BaseClassVector;
  
  // Vector of public base classes.
  BaseClassVector& m_BaseClassVector;
  
  // The ClassType corresponding to this Class.
  ClassType* m_ClassType;
  
  // Flag marking class abstract.
  bool m_Abstract;
  
private:
  Class(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
