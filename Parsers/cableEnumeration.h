/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableEnumeration.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableEnumeration_h
#define _cableEnumeration_h

#include "cableNamed.h"

namespace _cxx_ { class EnumerationType; class TypeSystem; }

namespace cable
{

class EnumerationType;

/** Superclass for C++ entities containing other names.  */
class CABLE_PARSERS_EXPORT Enumeration: public Named
{
public:
  cableTypeMacro(Enumeration, Named);
  static Pointer New() { return new Self; }
  class Iterator;
  
  /** Add an enumeration name/value pair.  */
  void AddValue(const char* name, int value);
  
  /** Get Begin/End iterator for the enumeration name/value pairs.  */
  Iterator Begin() const;
  Iterator End() const;
  
  /** Get/Set the EnumerationType corresponding to this Enumeration.  */
  EnumerationType* GetEnumerationType() const;
  void SetEnumerationType(EnumerationType* type);
  
  /** Get the CxxTypes library representation for this enumeration's type.  */
  const cxx::EnumerationType* GetCxxEnumerationType(cxx::TypeSystem* ts) const;
  
  /** Print the enumeration's representation.  */
  void Print(std::ostream& os, Indent indent) const;
  
protected:
  Enumeration();
  ~Enumeration();
  
  class ValueMap;
  class ValueMapIterator;
  
  // Set of enumeration values.
  ValueMap& m_ValueMap;
  
  // The EnumerationType corresponding to this Enumeration.
  EnumerationType* m_EnumerationType;
  
  friend class Iterator;
  friend class ValueMapIterator;
private:
  Enumeration(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

/** Iterate through the enumeration name/value pairs.  */
class CABLE_PARSERS_EXPORT Enumeration::Iterator
{
public:
  /** Normal iterator interface.  */
  Iterator(const Iterator& r);
  ~Iterator();
  Iterator& operator=(const Iterator& r);
  Iterator& operator++();
  bool operator==(const Iterator& r) const;
  bool operator!=(const Iterator& r) const;  
  const char* operator*() const;
  
  /** Get the value of the current entry.  */
  int GetValue() const;
  
protected:
  Iterator(const ValueMapIterator& i);
  
  // The real map iterator.
  ValueMapIterator& m_ValueMapIterator;
  
  friend class Enumeration;
};

} // namespace cable

#endif
