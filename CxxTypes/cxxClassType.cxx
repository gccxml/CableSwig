/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cxxClassType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cxxClassType.h"

#include <queue>
#include <set>

namespace _cxx_
{

  
/**
 * Retrieve what kind of Type this is.
 */
RepresentationType ClassType::GetRepresentationType() const
{
  return ClassType_id;
}


/**
 * Try to cast the given Type to an ClassType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
ClassType* ClassType::SafeDownCast(Type* t)
{
  ClassType* result = dynamic_cast<ClassType*>(t);
  if(!result) { throw TypeDownCastException(t, ClassType_id); }
  return result;
}


/**
 * Try to cast the given Type to an ClassType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
const ClassType* ClassType::SafeDownCast(const Type* t)
{
  const ClassType* result = dynamic_cast<const ClassType*>(t);
  if(!result) { throw TypeDownCastException(t, ClassType_id); }
  return result;
}


/**
 * Return whether the class is copyable.
 */
bool ClassType::IsCopyable() const
{
  return m_Copyable;
}


/**
 * Merge the class information given with that already known.  This
 * includes the copyable flag, and parent classes.
 */
void ClassType::MergeClassInformation(bool isCopyable,
                                      const ClassTypes& parents)
{
  m_Copyable = isCopyable | m_Copyable;
  if(parents.size() > m_Parents.size())
    {
    m_Parents = parents;
    }
}


/**
 * Get the name of the class.
 */
String ClassType::GetName() const
{
  return m_Name;
}


/**
 * Get a begin iterator to the set of immediate parent classes.
 */
ClassTypes::const_iterator ClassType::ParentsBegin() const
{
  return m_Parents.begin();
}


/**
 * Get an end iterator to the set of immediate parent classes.
 */
ClassTypes::const_iterator ClassType::ParentsEnd() const
{
  return m_Parents.end();
}


/**
 * Find all the superclasses through any inheritance chain.
 * This overly complicated implementation is used to get around problems
 * with DLLs and MSVC's implementation of STL.
 */
void ClassType::GetAllSuperclasses(ClassTypes& result) const
{
  // A queue to do a BFS of this class and its parents.
  std::queue<const ClassType*> classQueue;

  // Record queued classes so that we don't repeat any.
  std::set<const ClassType*> queued;
  
  // Start with the search at this class.
  classQueue.push(this);
  queued.insert(this);
  while(!classQueue.empty())
    {
    // Get the next class off the queue.
    const ClassType* curClass = classQueue.front(); classQueue.pop();
    
    // Add the class's parents to the queue.
    for(ClassTypes::const_iterator parent = curClass->ParentsBegin();
        parent != curClass->ParentsEnd(); ++parent)
      {
      // Only queue the parent class if it has not yet been queued.
      // This prevents duplication in the case of diamond inheritance.
      if(queued.find(*parent) == queued.end())
        {
        classQueue.push(*parent);
        queued.insert(*parent);
        
        // Every class but the initial one should be added to the
        // result.
        result.push_back(*parent);
        }
      }
    }
}


/**
 * Determine whether this ClassType is a subclass of the given ClassType
 * through any chain of inheritance.
 */
bool ClassType::IsSubclassOf(const ClassType* superclass) const
{
  // See if any of our immediate parents are the superclass.
  for(ClassTypes::const_iterator parent = m_Parents.begin();
      parent != m_Parents.end(); ++parent)
    {
    if(*parent == superclass)
      {
      return true;
      }
    }
  
  // Didn't find an immediate parent.  Ask each parent recursively.
  for(ClassTypes::const_iterator parent = m_Parents.begin();
      parent != m_Parents.end(); ++parent)
    {
    if((*parent)->IsSubclassOf(superclass))
      {
      return true;
      }
    }
  
  // We are not a subclass of the given class.
  return false;
}


String ClassType::GenerateName(const String& outerType,
                               bool isConst, bool isVolatile) const
{
  String cv = this->GetLeftCvString(isConst, isVolatile);
  return cv+m_Name+this->PrepareOuterStringForPostfix(outerType);
}


/**
 * Constructor for ClassType just takes the name of the class.  This name
 * may include template parameters of an instantiation.
 */
ClassType::ClassType(const String& name, bool isCopyable,
                     const ClassTypes& parents):
  m_Name(name),
  m_Copyable(isCopyable),
  m_Parents(parents)
{
}


} // namespace _cxx_
