/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctArgument.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctArgument_h
#define _ctArgument_h

#include "ctUtils.h"
#include "ctAnything.h"

#include <vector>

namespace _cable_tcl_
{

/**
 * Holds an argument after extraction from a Tcl object, but before
 * passing to the final conversion function.  This may hold a pointer
 * to the actual object, or an instance of a fundamental type.
 */
class _cable_tcl_EXPORT Argument
{
public:
  Argument();
  Argument(const Argument&);
  Argument& operator=(const Argument&);

  typedef Anything::ObjectType   ObjectType;
  typedef Anything::FunctionType FunctionType;
  
  Anything GetValue() const;
  const CvQualifiedType& GetType() const;
  void SetType(const CvQualifiedType&);
  void SetToObject(ObjectType object, const CvQualifiedType& type);
  void SetToBool(bool);
  void SetToInt(int);
  void SetToLong(long);
  void SetToDouble(double);
  void SetToPointer(ObjectType v, const CvQualifiedType& pointerType);
  void SetToFunction(FunctionType f,
                     const CvQualifiedType& functionPointerType);
private:
  enum ArgumentId { Uninitialized_id=0, Object_id, Pointer_id, Function_id,
                    bool_id, int_id, long_id, double_id };
  
  /**
   * The pointer to the actual object.
   */
  Anything m_Anything;

  /**
   * The type of the object.
   */
  CvQualifiedType m_Type;

  /**
   * Which type of Argument this is.
   */
  ArgumentId m_ArgumentId;
  
  /**
   * If a temporary is needed to hold the value extracted from the
   * Tcl object, this will hold it.
   */
  union
  {
    bool m_bool;
    int m_int;
    long m_long;
    double m_double;
  } m_Temp;
};

  
/**
 * Represent function arguments.
 */
typedef std::vector<Argument> Arguments;
  
} // namespace _cable_tcl_

#endif
