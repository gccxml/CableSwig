/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableXMLSourceParser.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableXMLSourceParser_h
#define _cableXMLSourceParser_h

#include "cableXMLParser.h"
#include <vector>
#include <string>
namespace _cxx_ { class CvQualifiedType; }

namespace cable
{

class XMLSourceElement;
class SourceRepresentation;
class SourceObject;
class Named;
class Type;
class FunctionType;
class Class;
class Variable;

/** Parser for input file generated by GCC-XML.  */
class CABLE_PARSERS_EXPORT XMLSourceParser: public XMLParser
{
public:
  cableTypeMacro(XMLSourceParser, XMLParser);
  static Pointer New() { return new Self; }  
  
  /** Print the set of elements parsed.  */
  void Print(std::ostream&) const;
  
  /** Get the source file name with the given id.  */
  const char* GetSourceFile(const char* fid) const;
  
  /** Get the source element with the given id.  */
  XMLSourceElement* GetSourceElement(const char* id) const;

  /** Get the SourceRepresentation constructed from parsing.  */
  SourceRepresentation* GetSourceRepresentation();
  
  /** Get a vector of file names used by this source */
  void GetFileNames(std::vector<std::string>& );
protected:
  XMLSourceParser();
  ~XMLSourceParser();
  
  class FileMap;
  class ElementMap;
  
  // Map from file id to file name parsed from the input.
  FileMap& m_FileMap;
  
  // Map from element id to the element parsed from the input.
  ElementMap& m_ElementMap;
  
  // Nesting count of elements encountered inside a unknown element.
  unsigned int m_UnknownElementNesting;
  
  // Whether any error has occurred during parsing.
  bool m_SourceParseError;
  
  // The top-level element currently being parsed.
  XMLSourceElement* m_CurrentElement;
  
  // The source representation constructed from parsing.
  SourceRepresentation* m_SourceRepresentation;
  
  // Internal utility methods.
  String GetErrorPrefix() const;
  
  // Start handlers for each element type.
  void Start_GCC_XML(const char** atts);
  void Start_Argument(const char** atts);
  void Start_Ellipsis(const char** atts);
  void Start_EnumValue(const char** atts);
  void Start_File(const char** atts);
  void Start_Unimplemented(const char** atts);
  void StartOther(const char* name, const char** atts);
  
  // End handlers for each element type.
  void End_GCC_XML();
  void End_Argument();
  void End_Ellipsis();
  void End_EnumValue();
  void End_File();
  void End_Unimplemented();
  void EndOther();
  
  // Override XMLParser methods.
  void StartElement(const char* name, const char** atts);
  void EndElement(const char* name);
  bool ParseStream();
  
  // SourceRepresentation construction methods.
  bool ConstructSourceRepresentation();
  void FreeSourceRepresentation();
  SourceObject* AddElementRepresentation(XMLSourceElement* element);
  void SetSourceObject(XMLSourceElement* element, SourceObject* object);
  bool SetupNamed(XMLSourceElement* element, Named* named);
  bool SetupFunctionType(XMLSourceElement* element, FunctionType* type);
  bool SetupClass(XMLSourceElement* element, Class* c);
  bool SetupVariable(XMLSourceElement* element, Variable* v);
  Type* GetConstructorReturnType();
  Type* GetTypeFromId(const char* tid);
  
  SourceObject* AddNamespace(XMLSourceElement* element);
  SourceObject* AddNamespaceAlias(XMLSourceElement* element);
  SourceObject* AddFundamentalType(XMLSourceElement* element);  
  SourceObject* AddPointerType(XMLSourceElement* element);  
  SourceObject* AddReferenceType(XMLSourceElement* element);  
  SourceObject* AddArrayType(XMLSourceElement* element);  
  SourceObject* AddFunctionType(XMLSourceElement* element);  
  SourceObject* AddMethodType(XMLSourceElement* element);  
  SourceObject* AddOffsetType(XMLSourceElement* element);  
  SourceObject* AddEnumeration(XMLSourceElement* element);
  SourceObject* AddClass(XMLSourceElement* element);
  SourceObject* AddStruct(XMLSourceElement* element);
  SourceObject* AddUnion(XMLSourceElement* element);
  SourceObject* AddTypedef(XMLSourceElement* element);
  SourceObject* AddVariable(XMLSourceElement* element);
  SourceObject* AddField(XMLSourceElement* element);
  SourceObject* AddFunction(XMLSourceElement* element);
  SourceObject* AddMethod(XMLSourceElement* element);
  SourceObject* AddConstructor(XMLSourceElement* element);
  SourceObject* AddDestructor(XMLSourceElement* element);
  SourceObject* AddOperatorFunction(XMLSourceElement* element);
  SourceObject* AddOperatorMethod(XMLSourceElement* element);
  SourceObject* AddConverter(XMLSourceElement* element);
private:
  XMLSourceParser(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
