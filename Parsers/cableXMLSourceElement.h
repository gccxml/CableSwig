/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableXMLSourceElement.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableXMLSourceElement_h
#define _cableXMLSourceElement_h

#include "cableObject.h"

namespace cable
{

class XMLSourceParser;

/** Element for input file generated by GCC-XML.  */
class CABLE_PARSERS_EXPORT XMLSourceElement: public Object
{
public:
  cableTypeMacro(XMLSourceElement, Object);
  static Pointer New() { return new Self; }  
  
  /** Get the name of the attribute read from the file.  */
  const char* GetName() const;
  
  /** Get the identifier of the element from the file.  */
  const char* GetId() const;
  
  /** Get the string value of the attribute with the given name.  */
  const char* GetAttribute(const char* name) const;
  
  /** Get the element referenced by the attribute with the given name.  */
  XMLSourceElement* GetElement(const char* name) const;
  
  /** Get the number of nested elements.  */
  unsigned int GetNumberOfNestedElements() const;
  
  /** Get the nested element at given index.  */
  XMLSourceElement* GetNestedElement(unsigned int index) const;
  
  /** Print the element.  */
  void Print(std::ostream&, Indent);
  
protected:
  XMLSourceElement();
  ~XMLSourceElement();  
  
  class AttributeMap;
  class NestedElementVector;
  
  // The attributes attached to the element from the file.
  AttributeMap& m_AttributeMap;
  
  // The list of nested elements.
  NestedElementVector& m_NestedElementVector;
  
  // The name of the element from the file.
  String m_Name;
  
  // The XMLSourceParser that created this element.
  XMLSourceParser* m_XMLSourceParser;
  
  // Methods used by XMLSourceParser to setup the element.
  friend class XMLSourceParser;
  
  // Set the name of the attribute read from the file.
  void SetName(const char*);
  void SetAttributes(const char**);
  void SetXMLSourceParser(XMLSourceParser*);
  void AddNestedElement(XMLSourceElement*);
  
private:
  XMLSourceElement(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
