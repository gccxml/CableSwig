/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableContext.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableContext_h
#define _cableContext_h

#include "cableNamed.h"

namespace cable
{

/** Superclass for C++ entities containing other names.  */
class CABLE_PARSERS_EXPORT Context: public Named
{
public:
  cableTypeMacro(Context, Named);
  class Iterator;
  
  /** The name access right levels.  */
  enum Access { Public, Protected, Private };
  
  /** Add the given Named object to this Context.  */
  virtual void AddNamed(Named*, Access access);
  
  /** Get Begin/End iterator for the names in this context.  */
  Iterator Begin() const;
  Iterator End() const;
  
  /** Get Lower/Upper bound iterators for the range matching a name.  */
  Iterator LowerBound(const char* name) const;
  Iterator UpperBound(const char* name) const;
  
protected:
  Context();
  ~Context();
  
  class MemberMap;
  class MemberMapIterator;
  
  // Set of members nested in this context.
  MemberMap& m_MemberMap;

  /** Print the member representations.  */
  void PrintMembers(std::ostream& os, Indent indent) const;
  
  friend class Iterator;
  friend class MemberMapIterator;
private:
  Context(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

/** Iterate through the names in a context.  */
class CABLE_PARSERS_EXPORT Context::Iterator
{
public:
  /** Normal iterator interface.  */
  Iterator(const Iterator& r);
  ~Iterator();
  Iterator& operator=(const Iterator& r);
  Iterator& operator++();
  bool operator==(const Iterator& r) const;
  bool operator!=(const Iterator& r) const;
  Named* operator*() const;
  
  /** Get the access rights of the current entry.  */
  Context::Access GetAccess() const;
  
protected:
  Iterator(const MemberMapIterator& i);
  
  // The real map iterator.
  MemberMapIterator& m_MemberMapIterator;
  
  friend class Context;
};

} // namespace cable

#endif
