/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableXMLSourceParser.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableXMLSourceParser.h"
#include "cableXMLSourceElement.h"
#include "cableSourceRepresentation.h"

#include "cableNamespace.h"
#include "cableNamespaceAlias.h"
#include "cableTypedef.h"
#include "cableClass.h"
#include "cableStruct.h"
#include "cableUnion.h"
#include "cableEnumeration.h"
#include "cableFunction.h"
#include "cableMethod.h"
#include "cableConstructor.h"
#include "cableDestructor.h"
#include "cableOperatorFunction.h"
#include "cableOperatorMethod.h"
#include "cableConverter.h"
#include "cableVariable.h"
#include "cableField.h"

#include "cableFundamentalType.h"
#include "cablePointerType.h"
#include "cableReferenceType.h"
#include "cableArrayType.h"
#include "cableClassType.h"
#include "cableFunctionType.h"
#include "cableMethodType.h"
#include "cableOffsetType.h"
#include "cableEnumerationType.h"

#include <sstream>
#include <map>

namespace cable
{

namespace
{
  template <typename From, typename To>
  bool StreamConvert(const From& from, To& to)
  {
    std::stringstream str;
    str << from;
    str >> to;
    return !!str;
  }
}

typedef std::map<String, String> FileMapBase;
class XMLSourceParser::FileMap: public FileMapBase
{
public:
  typedef FileMapBase::value_type value_type;
  typedef FileMapBase::iterator iterator;
  typedef FileMapBase::const_iterator const_iterator;  
  static FileMap* New() { return new FileMap; }
  void Delete() { delete this; }
};

typedef std::map<String, XMLSourceElement::Pointer> ElementMapBase;
class XMLSourceParser::ElementMap: public ElementMapBase
{
public:
  typedef ElementMapBase::value_type value_type;
  typedef ElementMapBase::iterator iterator;
  typedef ElementMapBase::const_iterator const_iterator;  
  static ElementMap* New() { return new ElementMap; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
XMLSourceParser::XMLSourceParser():
  m_FileMap(*FileMap::New()),
  m_ElementMap(*ElementMap::New())
{
  m_CurrentElement = 0;
  m_SourceParseError = false;
  m_SourceRepresentation = 0;
}

//----------------------------------------------------------------------------
XMLSourceParser::~XMLSourceParser()
{
  this->FreeSourceRepresentation();
  m_ElementMap.Delete();
  m_FileMap.Delete();
}

//----------------------------------------------------------------------------
void XMLSourceParser::Print(std::ostream& os) const
{
  os << "<GCC_XML>\n";
  Indent indent;
  for(ElementMap::const_iterator i = m_ElementMap.begin();
      i != m_ElementMap.end(); ++i)
    {
    i->second->Print(os, indent.GetNextIndent());
    }
  os << "</GCC_XML>\n";
}

//----------------------------------------------------------------------------
const char* XMLSourceParser::GetSourceFile(const char* fid) const
{
  FileMap::const_iterator i = m_FileMap.find(fid);
  if(i != m_FileMap.end())
    {
    return i->second.c_str();
    }
  return 0;
}

//----------------------------------------------------------------------------
XMLSourceElement* XMLSourceParser::GetSourceElement(const char* id) const
{
  ElementMap::const_iterator i = m_ElementMap.find(id);
  if(i != m_ElementMap.end())
    {
    return i->second;
    }
  return 0;
}

//----------------------------------------------------------------------------
SourceRepresentation* XMLSourceParser::GetSourceRepresentation()
{
  if(m_SourceParseError) { return 0; }
  if(m_ElementMap.empty()) { return 0; }
  if(!m_SourceRepresentation)
    {
    if(!this->ConstructSourceRepresentation())
      {
      cableErrorMacro("Error creating source representation.");
      this->FreeSourceRepresentation();
      return 0;
      }
    }
  return m_SourceRepresentation;
}

//----------------------------------------------------------------------------
String XMLSourceParser::GetErrorPrefix() const
{
  std::ostringstream prefix;
  prefix << "Input line " << this->GetXMLLineNumber()
         << ", column " << this->GetXMLColumnNumber()
         << ":\n";
  return prefix.str().c_str();
}

//----------------------------------------------------------------------------
bool XMLSourceParser::ParseStream()
{
  m_SourceParseError = false;
  m_UnknownElementNesting = 0;
  if(!this->Superclass::ParseStream())
    {
    return false;
    }
  return !m_SourceParseError;
}

//----------------------------------------------------------------------------
void XMLSourceParser::StartElement(const char *name, const char **atts)
{
  // If inside an unknown element, ignore everything.
  if(m_UnknownElementNesting)
    {
    // Increase nesting count inside unknown element.
    ++m_UnknownElementNesting;
    cableWarningMacro(this->GetErrorPrefix().c_str()
                      << "Ignoring " << name << " inside unknown element.");
    return;
    }
  
  // Dispatch element handler.
  String n = name;  
  if(n == "GCC_XML")               { this->Start_GCC_XML(atts); }
  else if(n == "Argument")         { this->Start_Argument(atts); }
  else if(n == "Ellipsis")         { this->Start_Ellipsis(atts); }
  else if(n == "EnumValue")        { this->Start_EnumValue(atts); }
  else if(m_CurrentElement)
    {
    // Cannot start a new top-level element while one is open.
    ++m_UnknownElementNesting;
    m_SourceParseError = true;
    cableErrorMacro(this->GetErrorPrefix().c_str()
                    << "Found " << name << " inside another element.");
    return;
    }
  else if(n == "File")             { this->Start_File(atts); }
  else if(n == "Unimplemented")    { this->Start_Unimplemented(atts); }
  else if(n == "Namespace")        { this->StartOther(name, atts); }
  else if(n == "NamespaceAlias")   { this->StartOther(name, atts); }
  else if(n == "FundamentalType")  { this->StartOther(name, atts); }
  else if(n == "PointerType")      { this->StartOther(name, atts); }
  else if(n == "ReferenceType")    { this->StartOther(name, atts); }
  else if(n == "ArrayType")        { this->StartOther(name, atts); }
  else if(n == "FunctionType")     { this->StartOther(name, atts); }
  else if(n == "MethodType")       { this->StartOther(name, atts); }
  else if(n == "OffsetType")       { this->StartOther(name, atts); }
  else if(n == "Enumeration")      { this->StartOther(name, atts); }
  else if(n == "Class")            { this->StartOther(name, atts); }
  else if(n == "Struct")           { this->StartOther(name, atts); }
  else if(n == "Union")            { this->StartOther(name, atts); }
  else if(n == "Typedef")          { this->StartOther(name, atts); }
  else if(n == "Variable")         { this->StartOther(name, atts); }
  else if(n == "Field")            { this->StartOther(name, atts); }
  else if(n == "Function")         { this->StartOther(name, atts); }
  else if(n == "Method")           { this->StartOther(name, atts); }
  else if(n == "Constructor")      { this->StartOther(name, atts); }
  else if(n == "Destructor")       { this->StartOther(name, atts); }
  else if(n == "OperatorFunction") { this->StartOther(name, atts); }
  else if(n == "OperatorMethod")   { this->StartOther(name, atts); }
  else if(n == "Converter")        { this->StartOther(name, atts); }
  else
    {
    // We have encountered an unknown element.  Ignore everything
    // until the element ends.
    ++m_UnknownElementNesting;
    m_SourceParseError = true;
    this->ReportUnknownElement(name);
    }
}

//----------------------------------------------------------------------------
void XMLSourceParser::EndElement(const char *name)
{
  // If inside an unknown element, ignore everything.
  if(m_UnknownElementNesting)
    {
    // Decrease nesting count inside unknown element.
    if(--m_UnknownElementNesting == 0)
      {
      cableWarningMacro(this->GetErrorPrefix().c_str()
                        << "End of bad element " << name);
      }
    return;
    }
  
  // Dispatch element handler.
  String n = name;  
  if(n == "GCC_XML")               { this->End_GCC_XML(); }
  else if(n == "Argument")         { this->End_Argument(); }
  else if(n == "Ellipsis")         { this->End_Ellipsis(); }
  else if(n == "EnumValue")        { this->End_EnumValue(); }
  else if(n == "File")             { this->End_File(); }
  else if(n == "Unimplemented")    { this->End_Unimplemented(); }
  else if(n == "Namespace")        { this->EndOther(); }
  else if(n == "NamespaceAlias")   { this->EndOther(); }
  else if(n == "FundamentalType")  { this->EndOther(); }
  else if(n == "PointerType")      { this->EndOther(); }
  else if(n == "ReferenceType")    { this->EndOther(); }
  else if(n == "ArrayType")        { this->EndOther(); }
  else if(n == "FunctionType")     { this->EndOther(); }
  else if(n == "MethodType")       { this->EndOther(); }
  else if(n == "OffsetType")       { this->EndOther(); }
  else if(n == "Enumeration")      { this->EndOther(); }
  else if(n == "Class")            { this->EndOther(); }
  else if(n == "Struct")           { this->EndOther(); }
  else if(n == "Union")            { this->EndOther(); }
  else if(n == "Typedef")          { this->EndOther(); }
  else if(n == "Variable")         { this->EndOther(); }
  else if(n == "Field")            { this->EndOther(); }
  else if(n == "Function")         { this->EndOther(); }
  else if(n == "Method")           { this->EndOther(); }
  else if(n == "Constructor")      { this->EndOther(); }
  else if(n == "Destructor")       { this->EndOther(); }
  else if(n == "OperatorFunction") { this->EndOther(); }
  else if(n == "OperatorMethod")   { this->EndOther(); }
  else if(n == "Converter")        { this->EndOther(); }
  else { /* Unknown element reported by matching StartElement. */ }
}

//----------------------------------------------------------------------------
void XMLSourceParser::Start_GCC_XML(const char**)
{
}

//----------------------------------------------------------------------------
void XMLSourceParser::End_GCC_XML()
{
}

//----------------------------------------------------------------------------
void XMLSourceParser::Start_Argument(const char** atts)
{
  if(!m_CurrentElement)
    {
    m_SourceParseError = true;
    cableErrorMacro(this->GetErrorPrefix().c_str()
                    << "Argument element not nested in function-style element.");
    return;
    }
  
  XMLSourceElement::Pointer element = XMLSourceElement::New();
  element->SetName("Argument");
  element->SetAttributes(atts);
  element->SetXMLSourceParser(this);
  m_CurrentElement->AddNestedElement(element);
}

//----------------------------------------------------------------------------
void XMLSourceParser::End_Argument()
{
}

//----------------------------------------------------------------------------
void XMLSourceParser::Start_Ellipsis(const char** atts)
{
  if(!m_CurrentElement)
    {
    m_SourceParseError = true;
    cableErrorMacro(this->GetErrorPrefix().c_str()
                    << "Ellipsis element not nested in function-style element.");
    return;
    }
  
  XMLSourceElement::Pointer element = XMLSourceElement::New();
  element->SetName("Ellipsis");
  element->SetAttributes(atts);
  element->SetXMLSourceParser(this);
  m_CurrentElement->AddNestedElement(element);
}

//----------------------------------------------------------------------------
void XMLSourceParser::End_Ellipsis()
{
}

//----------------------------------------------------------------------------
void XMLSourceParser::Start_EnumValue(const char** atts)
{
  if(!m_CurrentElement ||
     (String(m_CurrentElement->GetName()) != "Enumeration"))
    {
    m_SourceParseError = true;
    cableErrorMacro(this->GetErrorPrefix().c_str()
                    << "EnumValue element not nested Enumeration element.");
    return;
    }
  
  XMLSourceElement::Pointer element = XMLSourceElement::New();
  element->SetName("EnumValue");
  element->SetAttributes(atts);
  element->SetXMLSourceParser(this);
  m_CurrentElement->AddNestedElement(element);
}

//----------------------------------------------------------------------------
void XMLSourceParser::End_EnumValue()
{
}

//----------------------------------------------------------------------------
void XMLSourceParser::Start_File(const char** atts)
{
  XMLSourceElement::Pointer element = XMLSourceElement::New();
  element->SetAttributes(atts);
  const char* id = element->GetAttribute("id");
  const char* name = element->GetAttribute("name");
  if(id && name)
    {
    m_FileMap[id] = name;
    }
}

//----------------------------------------------------------------------------
void XMLSourceParser::End_File()
{
}

//----------------------------------------------------------------------------
void XMLSourceParser::Start_Unimplemented(const char**)
{
}

//----------------------------------------------------------------------------
void XMLSourceParser::End_Unimplemented()
{
}

//----------------------------------------------------------------------------
void XMLSourceParser::StartOther(const char* name, const char** atts)
{
  XMLSourceElement::Pointer element = XMLSourceElement::New();
  element->SetName(name);
  element->SetAttributes(atts);
  element->SetXMLSourceParser(this);
  m_CurrentElement = element;
  
  // Add an extra reference since m_CurrentElement is not a smart
  // pointer.
  m_CurrentElement->Register();
}

//----------------------------------------------------------------------------
void XMLSourceParser::EndOther()
{
  // Clean up the extra reference since m_CurrentElement is not a
  // smart pointer.
  XMLSourceElement::Pointer element = m_CurrentElement;
  m_CurrentElement->Unregister();
  m_CurrentElement = 0;
  
  const char* id = element->GetId();
  if(!id)
    {
    m_SourceParseError = true;
    cableErrorMacro(this->GetErrorPrefix().c_str()
                    << "Element " << element->GetName() << " has no id.");
    return;
    }
  
  m_ElementMap[id] = element;
}

//----------------------------------------------------------------------------
bool XMLSourceParser::ConstructSourceRepresentation()
{
  {
  SourceRepresentation::Pointer sr = SourceRepresentation::New();
  m_SourceRepresentation = sr;
  // Add an extra reference since m_SourceRepresentation is not a
  // smart pointer.
  m_SourceRepresentation->Register();
  }
  
  // Add all elements into the representation.
  for(ElementMap::const_iterator i = m_ElementMap.begin();
      i != m_ElementMap.end(); ++i)
    {
    if(!this->AddElementRepresentation(i->second))
      {
      cableErrorMacro("Error adding element " << i->second->GetName()
                      << " " << i->second->GetId());
      return false;
      }
    }
  
  return m_SourceRepresentation->CreateCxxTypes();
}

//----------------------------------------------------------------------------
void XMLSourceParser::FreeSourceRepresentation()
{
  if(m_SourceRepresentation)
    {
    // Clean up the extra reference since m_SourceRepresentation is
    // not a smart pointer.
    m_SourceRepresentation->Unregister();
    m_SourceRepresentation = 0;
    }
}

//----------------------------------------------------------------------------
SourceObject*
XMLSourceParser::AddElementRepresentation(XMLSourceElement* element)
{
  // If the element has already been added, return its existing
  // representation.
  const char* id = element->GetId();
  SourceObject* o = m_SourceRepresentation->GetSourceObject(id);
  if(o) { return o; }

  // Need to construct representation of this element.
  String n = element->GetName();
  if(n == "Namespace")             { o=this->AddNamespace(element); }
  else if(n == "NamespaceAlias")   { o=this->AddNamespaceAlias(element); }
  else if(n == "FundamentalType")  { o=this->AddFundamentalType(element); }
  else if(n == "PointerType")      { o=this->AddPointerType(element); }
  else if(n == "ReferenceType")    { o=this->AddReferenceType(element); }
  else if(n == "ArrayType")        { o=this->AddArrayType(element); }
  else if(n == "FunctionType")     { o=this->AddFunctionType(element); }
  else if(n == "MethodType")       { o=this->AddMethodType(element); }
  else if(n == "OffsetType")       { o=this->AddOffsetType(element); }
  else if(n == "Enumeration")      { o=this->AddEnumeration(element); }
  else if(n == "Class")            { o=this->AddClass(element); }
  else if(n == "Struct")           { o=this->AddStruct(element); }
  else if(n == "Union")            { o=this->AddUnion(element); }
  else if(n == "Typedef")          { o=this->AddTypedef(element); }
  else if(n == "Variable")         { o=this->AddVariable(element); }
  else if(n == "Field")            { o=this->AddField(element); }
  else if(n == "Function")         { o=this->AddFunction(element); }
  else if(n == "Method")           { o=this->AddMethod(element); }
  else if(n == "Constructor")      { o=this->AddConstructor(element); }
  else if(n == "Destructor")       { o=this->AddDestructor(element); }
  else if(n == "OperatorFunction") { o=this->AddOperatorFunction(element); }
  else if(n == "OperatorMethod")   { o=this->AddOperatorMethod(element); }
  else if(n == "Converter")        { o=this->AddConverter(element); }
  return o;
}

//----------------------------------------------------------------------------
void XMLSourceParser::SetSourceObject(XMLSourceElement* element,
                                      SourceObject* object)
{  
  const char* id = element->GetId();
  m_SourceRepresentation->SetSourceObject(id, object);
}

//----------------------------------------------------------------------------
bool XMLSourceParser::SetupNamed(XMLSourceElement* element, Named* named)
{  
  // Set the name.
  const char* name = element->GetAttribute("name");
  if(name)
    {
    named->SetName(name);
    }
  else
    {
    cableErrorMacro("No name on Named object " << named->GetNameOfClass());
    return false;
    }
  
  // Set the location.
  const char* location = element->GetAttribute("location");
  if(location)
    {
    // Need to parse "fid:line" from location.
    bool ok = true;
    String loc = location;
    String::size_type pos = loc.find(':');
    
    if(pos != String::npos)
      {
      String fid = loc.substr(0, pos);
      String lineStr = loc.substr(pos+1);
      unsigned long line;
      named->SetFile(this->GetSourceFile(fid.c_str()));
      if(StreamConvert(lineStr.c_str(), line)) { named->SetLine(line); }
      else { ok = false; }
      }
    else { ok = false; }
    if(!ok)
      {
      cableWarningMacro("Invalid location entry " << location << " in "
                        << element->GetNameOfClass() << " "
                        << element->GetId());
      }
    }
  
  Context::Access access = Context::Public;
  const char* accessStr = element->GetAttribute("access");
  if(accessStr)
    {
    String a = accessStr;
    if(a == "public") { access = Context::Public; }
    else if(a == "protected") { access = Context::Protected; }
    else if(a == "private") { access = Context::Private; }
    else
      {
      cableErrorMacro("Invalid access " << accessStr << " in "
                      << element->GetNameOfClass() << " "
                      << element->GetId());
      return false;
      }
    }
  
  // Set the context.
  XMLSourceElement* contextElement = element->GetElement("context");
  if(contextElement)
    {
    SourceObject* co = this->AddElementRepresentation(contextElement);
    Context* context = Context::SafeDownCast(co);
    // If there is a context, add this Named object to it.
    if(context)
      {
      named->SetContext(context);
      context->AddNamed(named, access);
      }
    }
  
  return true;
}

//----------------------------------------------------------------------------
bool XMLSourceParser::SetupFunctionType(XMLSourceElement* element,
                                        FunctionType* type)
{
  const char* returnsId = element->GetAttribute("returns");
  if(returnsId)
    {
    Type* returns = this->GetTypeFromId(returnsId);
    if(returns)
      {
      type->SetReturns(returns);
      }
    else
      {
      cableErrorMacro("Invalid return type \"" << returnsId << "\" on "
                      << element->GetName() << " " << element->GetId());
      return false;
      }
    }
  else if((String(element->GetName()) == "Constructor") ||
          (String(element->GetName()) == "Destructor"))
    {
    Type* returns = this->GetConstructorReturnType();
    if(!returns) { return false; }
    type->SetReturns(returns);
    }
  else
    {
    cableErrorMacro("No returns attribute on " << element->GetName() << " "
                    << element->GetId());
    }
  
  for(unsigned int i=0;i < element->GetNumberOfNestedElements(); ++i)
    {
    XMLSourceElement* argElement = element->GetNestedElement(i);
    if(String(argElement->GetName()) == "Argument")
      {
      const char* typeId = argElement->GetAttribute("type");
      if(!typeId)
        {
        cableErrorMacro("No type attribute on Argument " << i << " in "
                        << element->GetName() << " " << element->GetId());
        return false;
        }
      Type* argType = this->GetTypeFromId(typeId);
      if(!argType)
        {
        cableErrorMacro("Invalid type \"" << typeId << "\" on Argument " << i
                        << " in " << element->GetName() << " "
                        << element->GetId());
        return false;
        }
      const char* defaultArg = argElement->GetAttribute("default");      
      type->AddArgument(argType, defaultArg != 0);
      }
    else if(String(argElement->GetName()) == "Ellipsis")
      {
      // Ellipsis argument not supported.  Pretend it is not there.
      }
    else
      {
      cableErrorMacro("Found " << element->GetName() << " nested in "
                      << element->GetName() << " " << element->GetId()
                      << ".  It is not an Argument or Ellipsis.");
      return false;
      }
    }
  return true;
}

//----------------------------------------------------------------------------
bool XMLSourceParser::SetupClass(XMLSourceElement* element, Class* c)
{
  if(!this->SetupNamed(element, c)) { return false; }
  ClassType::Pointer ct = ClassType::New();
  ct->SetClass(c);
  c->SetClassType(ct);
  
  const char* abstract = element->GetAttribute("abstract");
  if(abstract && String(abstract) == "1") { c->SetAbstract(true); }
  
  const char* bases = element->GetAttribute("bases");
  if(bases)
    {
    String id;
    std::stringstream baseStream;
    baseStream << bases;
    while(baseStream >> id)
      {
      // Base class is public if there is no ':'.
      String::size_type pos = id.find(':');
      if(pos == String::npos)
        {
        XMLSourceElement* belement = this->GetSourceElement(id.c_str());
        SourceObject* bobj = this->AddElementRepresentation(belement);
        Class* base = Class::SafeDownCast(bobj);
        if(base)
          {
          c->AddBaseClass(base);
          }
        else
          {
          cableErrorMacro("Error creating base class " << id.c_str());
          return false;
          }
        }
      }
    }
  
  // Add the ClassType element with a dummy id.
  String cid = element->GetId();
  cid += "_t";
  m_SourceRepresentation->SetSourceObject(cid.c_str(), ct);
  
  // Add the Class element itself.
  this->SetSourceObject(element, c);
  return true;
}

//----------------------------------------------------------------------------
bool XMLSourceParser::SetupVariable(XMLSourceElement* element, Variable* v)
{
  if(!this->SetupNamed(element, v)) { return false; }
  
  // Get the target type.
  const char* tid = element->GetAttribute("type");
  if(!tid)
    {
    cableErrorMacro("No type given for " << element->GetName() << " "
                    << element->GetId());
    return false;
    }
  Type* type = this->GetTypeFromId(tid);
  if(!type)
    {
    cableErrorMacro("Cannot find type " << tid << " for "
                    << element->GetName() << " " << element->GetId());
    return false;
    }
  
  v->SetType(type);
  this->SetSourceObject(element, v);
  return true;
}

//----------------------------------------------------------------------------
Type* XMLSourceParser::GetConstructorReturnType()
{
  SourceObject* tobj = m_SourceRepresentation->GetSourceObject("crt");
  if(tobj) { return Type::SafeDownCast(tobj); }
  FundamentalType::Pointer type = FundamentalType::New();
  type->SetTypeName("void");
  m_SourceRepresentation->SetSourceObject("crt", type);
  return type;
}

//----------------------------------------------------------------------------
Type* XMLSourceParser::GetTypeFromId(const char* tid)
{
  // Use existing type if it is there.
  SourceObject* tobj = m_SourceRepresentation->GetSourceObject(tid);
  if(tobj)
    {
    // If the id refers to a Class or Enumeration, get the
    // corresponding ClassType or EnumerationType.  This can only
    // happen for a non-cv-qualified class or enumeration type because
    // its type id matches its element id.
    Class* c = Class::SafeDownCast(tobj);
    if(c) { return c->GetClassType(); }
    Enumeration* e = Enumeration::SafeDownCast(tobj);
    if(e) { return e->GetEnumerationType(); }
    else { return Type::SafeDownCast(tobj); }
    }
  
  // Parse the type id to separate any cv-qualifiers.
  String typeStr = tid;
  String::size_type cvPosition = typeStr.find_first_of("cvr");
  String id = typeStr.substr(0, cvPosition);
  
  // Get the non-cv-qualified version of the type.
  XMLSourceElement* typeElement = this->GetSourceElement(id.c_str());
  if(!typeElement) { return 0; }
  if(!this->AddElementRepresentation(typeElement)) { return 0; }
  Type* type = this->GetTypeFromId(id.c_str());
  
  // If the desired result type is not cv-qualified, we are done.
  if(cvPosition == String::npos) { return type; }  
  
  // Get the desired cv qualifiers.
  bool isConst = false;
  bool isVolatile = false;
  String cv = typeStr.substr(cvPosition);
  if(cv.find('c') != String::npos) { isConst = true; }
  if(cv.find('v') != String::npos) { isVolatile = true; }
  
  // Add the type with these cv qualifiers.
  Type::Pointer newType;
  switch (type->GetTypeId())
    {
    case Type::FundamentalTypeId:
      {
      FundamentalType::Pointer ft = FundamentalType::New();
      ft->SetTypeName(FundamentalType::SafeDownCast(type)->GetTypeName());
      ft->SetConst(isConst);
      ft->SetVolatile(isVolatile);
      newType = ft;
      }; break;
    case Type::PointerTypeId:
      {
      PointerType::Pointer pt = PointerType::New();
      pt->SetTarget(PointerType::SafeDownCast(type)->GetTarget());
      pt->SetConst(isConst);
      pt->SetVolatile(isVolatile);
      newType = pt;
      }; break;
    case Type::ArrayTypeId:
      {
      ArrayType::Pointer at = ArrayType::New();
      ArrayType* sat = ArrayType::SafeDownCast(type);
      at->SetTarget(sat->GetTarget());
      at->SetLength(sat->GetLength());
      at->SetConst(isConst);
      at->SetVolatile(isVolatile);
      newType = at;
      }; break;
    case Type::ClassTypeId:
      {
      ClassType::Pointer ct = ClassType::New();
      ct->SetClass(ClassType::SafeDownCast(type)->GetClass());
      ct->SetConst(isConst);
      ct->SetVolatile(isVolatile);
      newType = ct;
      }; break;
    case Type::OffsetTypeId:
      {
      OffsetType::Pointer ot = OffsetType::New();
      OffsetType* sot = OffsetType::SafeDownCast(type);
      ot->SetClass(sot->GetClass());
      ot->SetMemberType(sot->GetMemberType());
      ot->SetConst(isConst);
      ot->SetVolatile(isVolatile);
      newType = ot;
      }; break;
    case Type::EnumerationTypeId:
      {
      EnumerationType::Pointer et = EnumerationType::New();
      const EnumerationType* etp = EnumerationType::SafeDownCast(type);
      et->SetEnumeration(etp->GetEnumeration());
      et->SetConst(isConst);
      et->SetVolatile(isVolatile);
      newType = et;
      }; break;
    case Type::FunctionTypeId:
      {
      cableErrorMacro("Cannot add cv-qualifiers to FunctionType.");
      return 0;
      };
    case Type::MethodTypeId:
      {
      cableErrorMacro("Cannot add cv-qualifiers to MethodType.");
      return 0;
      };
    case Type::ReferenceTypeId:
      {
      cableErrorMacro("Cannot add cv-qualifiers to ReferenceType.");
      return 0;
      }
    }  
  if(!newType)
    {
    cableErrorMacro("Could not add cv-qualifiers to "
                    << type->GetNameOfClass() << ".");
    return 0;
    }
  
  m_SourceRepresentation->SetSourceObject(tid, newType);  
  return newType;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddNamespace(XMLSourceElement* element)
{
  Namespace::Pointer ns = Namespace::New();
  if(!this->SetupNamed(element, ns)) { return 0; }
  this->SetSourceObject(element, ns);
  
  // If this is the global namespace, tell the SourceRepresentation.
  if(ns->IsGlobalNamespace())
    {
    m_SourceRepresentation->SetGlobalNamespace(element->GetId());
    }
  
  return ns;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddNamespaceAlias(XMLSourceElement* element)
{
  NamespaceAlias::Pointer na = NamespaceAlias::New();
  if(!this->SetupNamed(element, na)) { return 0; }
  bool ok = false;
  XMLSourceElement* nsElement = element->GetElement("namespace");
  if(nsElement)
    {
    SourceObject* nso = this->AddElementRepresentation(nsElement);
    Namespace* ns = Namespace::SafeDownCast(nso);
    if(ns)
      {
      na->SetNamespace(ns);
      ok = true;
      }
    }
  if(!ok)
    {
    cableErrorMacro("Couldn't get real namespace for NamespaceAlias "
                    << element->GetId());
    return 0;
    }
  this->SetSourceObject(element, na);
  return na;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddFundamentalType(XMLSourceElement* element)
{
  const char* name = element->GetAttribute("name");
  if(!name)
    {
    cableErrorMacro("No name attribute on FundamentalType "
                    << element->GetId());
    return 0;
    }
  
  FundamentalType::Pointer type = FundamentalType::New();
  type->SetTypeName(name);
  this->SetSourceObject(element, type);
  return type;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddPointerType(XMLSourceElement* element)
{
  const char* targetId = element->GetAttribute("type");
  if(!targetId)
    {
    cableErrorMacro("No type attribute on PointerType " << element->GetId());
    return 0;
    }
  Type* target = this->GetTypeFromId(targetId);
  if(!target)
    {
    cableErrorMacro("Invalid type \"" << targetId << "\" on PointerType "
                    << element->GetId());
    return 0;
    }
  
  PointerType::Pointer type = PointerType::New();
  type->SetTarget(target);
  this->SetSourceObject(element, type);
  return type;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddReferenceType(XMLSourceElement* element)
{
  const char* targetId = element->GetAttribute("type");
  if(!targetId)
    {
    cableErrorMacro("No type attribute on ReferenceType " << element->GetId());
    return 0;
    }
  Type* target = this->GetTypeFromId(targetId);
  if(!target)
    {
    cableErrorMacro("Invalid type \"" << targetId << "\" on ReferenceType "
                    << element->GetId());
    return 0;
    }
  
  ReferenceType::Pointer type = ReferenceType::New();
  type->SetTarget(target);
  this->SetSourceObject(element, type);
  return type;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddArrayType(XMLSourceElement* element)
{
  const char* targetId = element->GetAttribute("type");
  if(!targetId)
    {
    cableErrorMacro("No type attribute on ArrayType " << element->GetId());
    return 0;
    }
  Type* target = this->GetTypeFromId(targetId);
  if(!target)
    {
    cableErrorMacro("Invalid type \"" << targetId << "\" on ArrayType "
                    << element->GetId());
    }
  const char* maxStr = element->GetAttribute("max");
  if(!maxStr)
    {
    cableErrorMacro("No max attribute on ArrayType " << element->GetId());
    return 0;
    }
  long max = 0;
  if(String(maxStr).length() > 0)
    {
    if(!StreamConvert(maxStr, max))
      {
      cableErrorMacro("Cannot convert max=\"" << maxStr << "\" to integer "
                      "on ArrayType " << element->GetId());
      return 0;
      }
    }
  else
    {
    max = -1;
    }
  
  ArrayType::Pointer type = ArrayType::New();
  type->SetTarget(target);
  type->SetLength(max+1);
  this->SetSourceObject(element, type);
  return type;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddFunctionType(XMLSourceElement* element)
{
  FunctionType::Pointer type = FunctionType::New();
  if(!this->SetupFunctionType(element, type)) { return 0; }
  this->SetSourceObject(element, type);
  return type;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddMethodType(XMLSourceElement* element)
{  
  XMLSourceElement* bte = element->GetElement("basetype");
  if(!bte)
    {
    cableErrorMacro("No basetype on MethodType " << element->GetId());
    return 0;
    }  
  Class* c = Class::SafeDownCast(this->AddElementRepresentation(bte));
  if(!c)
    {
    cableErrorMacro("Error getting Class from basetype on MethodType "
                    << element->GetId());
    return 0;
    }
  
  MethodType::Pointer type = MethodType::New();
  if(!this->SetupFunctionType(element, type)) { return 0; }
  type->SetClass(c);
  
  this->SetSourceObject(element, type);
  return type;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddOffsetType(XMLSourceElement* element)
{  
  XMLSourceElement* bte = element->GetElement("basetype");
  if(!bte)
    {
    cableErrorMacro("No basetype on OffsetType " << element->GetId());
    return 0;
    }
  Class* c = Class::SafeDownCast(this->AddElementRepresentation(bte));
  if(!c)
    {
    cableErrorMacro("Error getting Class from basetype on OffsetType "
                    << element->GetId());
    return 0;
    }  
  const char* mtypeId = element->GetAttribute("type");
  if(!mtypeId)
    {
    cableErrorMacro("No attribute type on OffsetType " << element->GetId());
    return 0;
    }
  Type* mtype = this->GetTypeFromId(mtypeId);
  if(!mtype)
    {
    cableErrorMacro("Error getting Type from " << mtypeId << " on OffsetType "
                    << element->GetId());
    return 0;
    }
  
  OffsetType::Pointer type = OffsetType::New();
  type->SetClass(c);
  type->SetMemberType(mtype);
  
  this->SetSourceObject(element, type);
  return type;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddEnumeration(XMLSourceElement* element)
{
  Enumeration::Pointer e = Enumeration::New();
  if(!this->SetupNamed(element, e)) { return 0; }
  EnumerationType::Pointer et = EnumerationType::New();
  et->SetEnumeration(e);
  e->SetEnumerationType(et);
  
  // If the enum is anonymous, leave the name empty.
  if(e->GetName()[0] == '.') { e->SetName(""); }
  
  for(unsigned int i=0;i < element->GetNumberOfNestedElements(); ++i)
    {
    XMLSourceElement* valueElement = element->GetNestedElement(i);
    if(String(valueElement->GetName()) == "EnumValue")
      {
      const char* name = valueElement->GetAttribute("name");
      if(!name)
        {
        cableErrorMacro("No name attribute on EnumValue " << i
                        << " in Enumeration " << element->GetId());
        return 0;
        }
      const char* initStr = valueElement->GetAttribute("init");
      if(!initStr)
        {
        cableErrorMacro("No init attribute on EnumValue " << i
                        << " in Enumeration " << element->GetId());
        return 0;
        }
      int init;
      if(!StreamConvert(initStr, init))
        {
        cableErrorMacro("Invalid init " << initStr << " on EnumValue " << i
                        << " in Enumeration " << element->GetId());
        return 0;
        }
      e->AddValue(name, init);
      }
    else
      {
      cableErrorMacro("Found " << element->GetName()
                      << " nested in Enumeration " << element->GetId()
                      << ".  It is not an EnumValue.");
      return false;
      }
    }  
  
  // Add the EnumerationType element with a dummy id.
  String eid = element->GetId();
  eid += "_t";
  m_SourceRepresentation->SetSourceObject(eid.c_str(), et);
  
  // Add the Enumeration element itself.
  this->SetSourceObject(element, e);
  return e;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddClass(XMLSourceElement* element)
{
  Class::Pointer c = Class::New();
  if(!this->SetupClass(element, c)) { return 0; }
  return c;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddStruct(XMLSourceElement* element)
{
  Struct::Pointer s = Struct::New();
  if(!this->SetupClass(element, s)) { return 0; }
  return s;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddUnion(XMLSourceElement* element)
{
  Union::Pointer u = Union::New();
  if(!this->SetupClass(element, u)) { return 0; }
  return u;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddTypedef(XMLSourceElement* element)
{
  Typedef::Pointer td = Typedef::New();
  if(!this->SetupNamed(element, td)) { return 0; }
  
  // Get the target type.
  const char* tid = element->GetAttribute("type");
  if(!tid)
    {
    cableErrorMacro("No type given for Typedef " << element->GetId());
    return 0;
    }
  Type* type = this->GetTypeFromId(tid);
  if(!type)
    {
    cableErrorMacro("Cannot find type " << tid << " for Typedef "
                    << element->GetId());
    return 0;
    }
  
  td->SetType(type);
  this->SetSourceObject(element, td);
  return td;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddVariable(XMLSourceElement* element)
{
  Variable::Pointer v = Variable::New();
  const char* init = element->GetAttribute("init");
  if(init) { v->SetInitializer(init); }
  if(!this->SetupVariable(element, v)) { return 0; }
  return v;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddField(XMLSourceElement* element)
{
  Field::Pointer f = Field::New();
  if(!this->SetupVariable(element, f)) { return 0; }
  return f;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddFunction(XMLSourceElement* element)
{
  Function::Pointer f = Function::New();
  if(!this->SetupNamed(element, f)) { return 0; }
  FunctionType::Pointer ft = FunctionType::New();
  if(!this->SetupFunctionType(element, ft)) { return 0; }  
  f->SetFunctionType(ft);
  
  // Add the FunctionType element with a dummy id.
  String fid = element->GetId();
  fid += "_t";
  m_SourceRepresentation->SetSourceObject(fid.c_str(), ft);
  
  // Add the Function element itself.
  this->SetSourceObject(element, f);
  return f;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddMethod(XMLSourceElement* element)
{
  bool isConst = false;
  bool isStatic = false;
  const char* constAttr = element->GetAttribute("const");
  if(constAttr && (String(constAttr) == "1")) { isConst = true; }
  const char* staticAttr = element->GetAttribute("static");
  if(staticAttr && (String(staticAttr) == "1")) { isStatic = true; }  
  if(isConst && isStatic)
    {
    cableErrorMacro("Method " << element->GetId()
                    << " is both const and static.");
    return 0;
    }
  
  // Create the Method.
  Method::Pointer m = Method::New();
  if(!this->SetupNamed(element, m)) { return 0; }
  FunctionType::Pointer ft = FunctionType::New();
  if(!this->SetupFunctionType(element, ft)) { return 0; }  
  m->SetFunctionType(ft);
  m->SetConst(isConst);
  m->SetStatic(isStatic);
  
  // Add the FunctionType element with a dummy id.
  String fid = element->GetId();
  fid += "_t";
  m_SourceRepresentation->SetSourceObject(fid.c_str(), ft);
  
  // Add the Method element itself.
  this->SetSourceObject(element, m);
  return m;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddConstructor(XMLSourceElement* element)
{
  // Create the Constructor.
  Constructor::Pointer m = Constructor::New();
  if(!this->SetupNamed(element, m)) { return 0; }
  FunctionType::Pointer ft = FunctionType::New();
  if(!this->SetupFunctionType(element, ft)) { return 0; }  
  m->SetFunctionType(ft);
  
  // Add the FunctionType element with a dummy id.
  String fid = element->GetId();
  fid += "_t";
  m_SourceRepresentation->SetSourceObject(fid.c_str(), ft);
  
  // Add the Constructor element itself.
  this->SetSourceObject(element, m);
  return m;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddDestructor(XMLSourceElement* element)
{
  // Create the Destructor.
  Destructor::Pointer m = Destructor::New();
  if(!this->SetupNamed(element, m)) { return 0; }
  FunctionType::Pointer ft = FunctionType::New();
  if(!this->SetupFunctionType(element, ft)) { return 0; }  
  m->SetFunctionType(ft);
  
  // Add the FunctionType element with a dummy id.
  String fid = element->GetId();
  fid += "_t";
  m_SourceRepresentation->SetSourceObject(fid.c_str(), ft);
  
  // Add the Destructor element itself.
  this->SetSourceObject(element, m);
  return m;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddOperatorFunction(XMLSourceElement* element)
{
  // Create the OperatorFunction.
  OperatorFunction::Pointer f = OperatorFunction::New();
  if(!this->SetupNamed(element, f)) { return 0; }
  FunctionType::Pointer ft = FunctionType::New();
  if(!this->SetupFunctionType(element, ft)) { return 0; }  
  f->SetFunctionType(ft);
  
  // Add the FunctionType element with a dummy id.
  String fid = element->GetId();
  fid += "_t";
  m_SourceRepresentation->SetSourceObject(fid.c_str(), ft);
  
  // Add the OperatorFunction element itself.
  this->SetSourceObject(element, f);
  return f;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddOperatorMethod(XMLSourceElement* element)
{
  bool isConst = false;
  bool isStatic = false;
  const char* constAttr = element->GetAttribute("const");
  if(constAttr && (String(constAttr) == "1")) { isConst = true; }
  const char* staticAttr = element->GetAttribute("static");
  if(staticAttr && (String(staticAttr) == "1")) { isStatic = true; }  
  if(isConst && isStatic)
    {
    cableErrorMacro("Method " << element->GetId()
                    << " is both const and static.");
    return 0;
    }
  
  // Create the OperatorMethod.
  OperatorMethod::Pointer m = OperatorMethod::New();
  if(!this->SetupNamed(element, m)) { return 0; }
  FunctionType::Pointer ft = FunctionType::New();
  if(!this->SetupFunctionType(element, ft)) { return 0; }  
  m->SetFunctionType(ft);
  m->SetConst(isConst);
  m->SetStatic(isStatic);
  
  // Add the FunctionType element with a dummy id.
  String fid = element->GetId();
  fid += "_t";
  m_SourceRepresentation->SetSourceObject(fid.c_str(), ft);
  
  // Add the OperatorMethod element itself.
  this->SetSourceObject(element, m);
  return m;
}

//----------------------------------------------------------------------------
SourceObject* XMLSourceParser::AddConverter(XMLSourceElement* element)
{
  bool isConst = false;
  const char* constAttr = element->GetAttribute("const");
  if(constAttr && (String(constAttr) == "1")) { isConst = true; }
  
  // Create the Converter.
  Converter::Pointer m = Converter::New();
  if(!this->SetupNamed(element, m)) { return 0; }
  FunctionType::Pointer ft = FunctionType::New();
  if(!this->SetupFunctionType(element, ft)) { return 0; }  
  m->SetFunctionType(ft);
  m->SetConst(isConst);
  
  // Add the FunctionType element with a dummy id.
  String fid = element->GetId();
  fid += "_t";
  m_SourceRepresentation->SetSourceObject(fid.c_str(), ft);
  
  // Add the Converter element itself.
  this->SetSourceObject(element, m);
  return m;
}

} // namespace cable
