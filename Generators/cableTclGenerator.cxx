/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableTclGenerator.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableTclGenerator.h"
#include "cableSourceRepresentation.h"

#include "cableVersion.h"

#include "cableNamespace.h"
#include "cableVariable.h"
#include "cableTypedef.h"
#include "cableClass.h"
#include "cableEnumeration.h"
#include "cableType.h"
#include "cableClassType.h"
#include "cableFunction.h"
#include "cableMethod.h"
#include "cableFunctionType.h"
#include "cableEnumerationType.h"
#include "cableReferenceType.h"

#include "cxxTypeSystem.h"

#include <vector>
#include <map>
#include <set>

namespace cable
{

typedef std::vector<String> StringVectorBase;
class TclGenerator::StringVector: public StringVectorBase
{
public:
  typedef StringVectorBase::iterator iterator;
  typedef StringVectorBase::const_iterator const_iterator;
  typedef StringVectorBase::value_type value_type;
  static StringVector* New() { return new StringVector; }
  void Delete() { delete this; }
};
  
//----------------------------------------------------------------------------
class MethodEntry
{
public:
  MethodEntry(Method* method, unsigned int argc):
    m_Method(method), m_ArgumentCount(argc) {}
  unsigned int GetArgumentCount() const { return m_ArgumentCount; }
  Method* GetMethod() const { return m_Method; }
private:
  Method* m_Method;
  unsigned int m_ArgumentCount;
};

typedef std::vector<MethodEntry> MethodVectorBase;
class TclGenerator::MethodVector: public MethodVectorBase
{
public:
  typedef MethodVectorBase::iterator iterator;
  typedef MethodVectorBase::const_iterator const_iterator;
  typedef MethodVectorBase::value_type value_type;
};
  
//----------------------------------------------------------------------------
class TclGenerator::ClassWrapperMap:
    public std::map<const Class*, StringVector>
{
public:
  typedef std::map<const Class*, StringVector> ClassWrapperMapBase;
  typedef ClassWrapperMapBase::value_type value_type;
  typedef ClassWrapperMapBase::iterator iterator;
  typedef ClassWrapperMapBase::const_iterator const_iterator;  
  static ClassWrapperMap* New() { return new ClassWrapperMap; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
typedef std::set<const cxx::ClassType*> ClassTypeSetBase;
class TclGenerator::ClassTypeSet: public ClassTypeSetBase
{
public:
  typedef ClassTypeSetBase::value_type value_type;
  typedef ClassTypeSetBase::iterator iterator;
  typedef ClassTypeSetBase::const_iterator const_iterator;  
  static ClassTypeSet* New() { return new ClassTypeSet; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
typedef std::set<const Enumeration*> EnumerationSetBase;
class TclGenerator::EnumerationSet: public EnumerationSetBase
{
public:
  typedef EnumerationSetBase::value_type value_type;
  typedef EnumerationSetBase::iterator iterator;
  typedef EnumerationSetBase::const_iterator const_iterator;  
  static EnumerationSet* New() { return new EnumerationSet; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
TclGenerator::TclGenerator():
  m_ClassWrapperMap(*ClassWrapperMap::New()),
  m_GroupVector(*GroupVector::New()),
  m_WrapperList(*WrapperList::New()),
  m_ClassesForDerivedToBase(*ClassesForDerivedToBase::New()),
  m_ClassesThatNeedDestructor(*ClassesThatNeedDestructor::New()),
  m_EnumerationsThatNeedValues(*EnumerationsThatNeedValues::New()),
  m_EnumerationsThatNeedReturn(*EnumerationsThatNeedReturn::New())
{
  m_WriteGroup = false;
  m_WritePackage = false;
}

//----------------------------------------------------------------------------
TclGenerator::~TclGenerator()
{
  m_GroupVector.Delete();
  m_ClassWrapperMap.Delete();
  m_ClassesForDerivedToBase.Delete();
  m_ClassesThatNeedDestructor.Delete();
  m_EnumerationsThatNeedValues.Delete();
  m_EnumerationsThatNeedReturn.Delete();
}

//----------------------------------------------------------------------------
bool TclGenerator::GenerateWrappers()
{  
  // Find the configuration settings.
  if(!this->FindConfiguration()) { return false; }
  
  // Find the types that will be used by the wrappers.
  this->FindCvTypes();
  
  // Write the file header.
  this->WriteHeaders();
  
  // Write the wrapper code.
  if(m_WriteGroup)
    {
    this->WriteWrappers();
    }
  
  // Write package initialization code if requested.
  if(m_WritePackage)
    {
    this->WritePackageInitialization();
    }
  
  return true;
}

//----------------------------------------------------------------------------
bool TclGenerator::FindConfiguration()
{
  const Namespace* gns = m_SourceRepresentation->GetGlobalNamespace();
  const Namespace* cns = 0;
  
  // Find the cable configuration namespace.
  Context::Iterator lower = gns->LowerBound("_cable_");
  Context::Iterator upper = gns->UpperBound("_cable_");
  if(lower != upper)
    {
    cns = Namespace::SafeDownCast(*lower);
    if(!cns)
      {
      cableErrorMacro("Identifier _cable_ is not a namespace.");
      return false;
      }
    }
  if(!cns)
    {
    cableErrorMacro("Could not find _cable_ namespace in input.\n");
    return false;
    }
  
  // Find the group name, if any.
  const Variable* group = 0;
  lower = cns->LowerBound("group");
  upper = cns->UpperBound("group");
  if(lower != upper)
    {
    group = Variable::SafeDownCast(*lower);
    if(!group)
      {
      cableErrorMacro("Identifier _cable_::group is not a variable.");
      return false;
      }
    }
  
  // Find the groups list, if any.
  const Variable* groups = 0;
  lower = cns->LowerBound("groups");
  upper = cns->UpperBound("groups");
  if(lower != upper)
    {
    groups = Variable::SafeDownCast(*lower);
    if(!groups)
      {
      cableErrorMacro("Identifier _cable_::groups is not a variable.");
      return false;
      }
    }
  
  // Find the package name, if any.
  const Variable* package = 0;
  lower = cns->LowerBound("package");
  upper = cns->UpperBound("package");
  if(lower != upper)
    {
    package = Variable::SafeDownCast(*lower);
    if(!package)
      {
      cableErrorMacro("Identifier _cable_::package is not a variable.");
      return false;
      }
    }
  
  // Find the package version, if any.
  const Variable* package_version = 0;
  lower = cns->LowerBound("package_version");
  upper = cns->UpperBound("package_version");
  if(lower != upper)
    {
    package_version = Variable::SafeDownCast(*lower);
    if(!package_version)
      {
      cableErrorMacro("Identifier _cable_::package_version is not a variable.");
      return false;
      }
    }
  
  if(group)
    {
    if(!this->ParseName(group->GetInitializer(), m_Group))
      {
      cableErrorMacro("Error parsing group name.");
      return false;
      }
    
    // Hold on to the name of the configuration file.
    m_ConfigFileName = group->GetFile();
    }
  
  if(package && !this->ParseName(package->GetInitializer(), m_Package))
    {
    cableErrorMacro("Error parsing package name.");
    return false;
    }
  
  if(package_version &&
     !this->ParseName(package_version->GetInitializer(), m_PackageVersion))
    {
    cableErrorMacro("Error parsing package version string.");
    return false;
    }
  
  if(groups)
    {
    String gl = groups->GetInitializer();
    String::size_type lpos = gl.find('"');
    while((lpos != String::npos) && (lpos < gl.length()))
      {
      String::size_type rpos = gl.find_first_of(",}", lpos);
      String result;
      if((rpos == String::npos) ||
         !this->ParseName(gl.substr(lpos, rpos-lpos).c_str(), result))
        {
        cableErrorMacro("Error parsing groups list.");
        return false;
        }
      m_GroupVector.push_back(result);
      lpos = rpos+1;
      }
    }
  
  if(groups && !package)
    {
    cableErrorMacro("Groups listed with no package.");
    return false;
    }
  
  if(package && !groups)
    {
    cableErrorMacro("Package requested with no groups listed.");
    return false;
    }
  
  if(package_version && !package)
    {
    cableErrorMacro("Package version specified with no package.");
    return false;
    }
  
  if(!group && !package && !groups)
    {
    cableErrorMacro("No configuration given.");
    return false;
    }
  
  m_WriteGroup = false;
  m_WritePackage = false;
  if(group) { m_WriteGroup = true; }
  if(package) { m_WritePackage = true; }
  
  if(group && !this->FindWrappers(cns)) { return false; }
  
  return true;
}

//----------------------------------------------------------------------------
bool TclGenerator::ParseName(const char* name, String& result)
{
  String group = name;
  String::size_type l = group.find('"');
  String::size_type r = group.rfind('"');
  if((l != String::npos) && (r != String::npos) && (r > l))
    {
    result = group.substr(l+1, r-l-1);
    return true;
    }
  return false;
}

//----------------------------------------------------------------------------
bool TclGenerator::FindWrappers(const Namespace* cns)
{
  const Namespace* wns = 0;
  
  // Find the wrapper list namespace.
  Context::Iterator lower = cns->LowerBound("wrappers");
  Context::Iterator upper = cns->UpperBound("wrappers");
  if(lower != upper)
    {
    wns = Namespace::SafeDownCast(*lower);
    if(!wns)
      {
      cableErrorMacro("Identifier _cable_::wrappers is not a namespace.");
      return false;
      }
    }
  if(!wns)
    {
    cableErrorMacro("Could not find _cable_::wrappers namespace in input.\n");
    return false;
    }
  
  // Look for entries in the wrappers namespace.
  return this->FindWrappers(wns, "");
}

//----------------------------------------------------------------------------
bool TclGenerator::FindWrappers(const Namespace* wns, const char* prefix)
{
  // Look for entries in this namespace.
  for(Context::Iterator w = wns->Begin(); w != wns->End(); ++w)
    {
    Namespace* ns = Namespace::SafeDownCast(*w);
    Typedef* td = Typedef::SafeDownCast(*w);
    if(td)
      {
      const ClassType* ct = ClassType::SafeDownCast(td->GetType());
      if(ct)
        {
        const Class* c = ct->GetClass();
        String name = prefix;
        name += td->GetName();
        m_ClassWrapperMap[c].push_back(name.c_str());
        }
      else
        {
        cableErrorMacro("Entry \"" << td->GetName()
                        << "\" in wrappers namespace is not a class.");
        return false;
        }
      }
    else if(ns)
      {
      String newPrefix = prefix;
      newPrefix += ns->GetName();
      newPrefix += "::";
      if(!this->FindWrappers(ns, newPrefix.c_str()))
        {
        return false;
        }
      }
    else
      {
      cableWarningMacro("Ignoring entry \"" << prefix
                        << (*w)->GetName() << "\" in wrappers namespace.");
      }
    }
  
  return true;
}

//----------------------------------------------------------------------------
void TclGenerator::PrintConfiguration(std::ostream& os) const
{
  os << "Group: \"" << m_Group.c_str() << "\"\n";
  os << "Package: \"" << m_Package.c_str() << "\"\n";
  os << "Groups: \n";
  for(GroupVector::const_iterator i = m_GroupVector.begin();
      i != m_GroupVector.end(); ++i)
    {
    os << "  \"" << (*i).c_str() << "\"\n";
    }
  os << "Wrappers: \n";
  for(ClassWrapperMap::const_iterator i = m_ClassWrapperMap.begin();
      i != m_ClassWrapperMap.end(); ++i)
    {
    os << "  \"" << (*i).first->GetQualifiedName().c_str() << "\"\n";
    }
}

//----------------------------------------------------------------------------
void TclGenerator::FindCvTypes()
{
  m_CvTypeWriter = CvTypeWriter::New();
  for(ClassWrapperMap::const_iterator i = m_ClassWrapperMap.begin();
      i != m_ClassWrapperMap.end(); ++i)
    {
    this->FindClassCvTypes(i->first);
    }
}

//----------------------------------------------------------------------------
void TclGenerator::FindClassCvTypes(const Class* c)
{
  // Add the implict argument types for method calls.
  cxx::CvQualifiedType cvClass = c->GetClassType()->GetCxxType();
  cxx::CvQualifiedType ccvClass = cvClass.GetMoreQualifiedType(true, false);
  this->AddCvReferenceType(cvClass);
  this->AddCvReferenceType(ccvClass);
  this->AddCvPointerType(cvClass);
  this->AddCvPointerType(ccvClass);
  
  // Add the superclass information.
  const cxx::ClassType* ct = cxx::ClassType::SafeDownCast(cvClass.GetType());
  this->AddSuperclassCvTypes(ct);
  m_ClassesForDerivedToBase.insert(ct);
  
  // Add the types used by the methods.
  for(Context::Iterator i = c->Begin(); i != c->End(); ++i)
    {
    if(i.GetAccess() == Context::Public)
      {
      const Function* f = Function::SafeDownCast(*i);
      if(f)
        {
        this->FindFunctionCvTypes(f);
        }
      }
    }
}

//----------------------------------------------------------------------------
void TclGenerator::FindFunctionCvTypes(const Function* f)
{
  FunctionType* ft = f->GetFunctionType();
  Type* returns = ft->GetReturns();
  
  // Add the return type of the function.
  if(returns)
    {
    this->FindReturnCvTypes(returns);
    }
  
  // Add the argument types of the function.  
  for(unsigned int i=0; i < ft->GetNumberOfArguments(); ++i)
    {
    Type* argument = ft->GetArgument(i);
    if(argument)
      {
      this->FindArgumentCvTypes(argument);
      }
    }  
}

//----------------------------------------------------------------------------
void TclGenerator::FindReturnCvTypes(const Type* returns)
{
  // Add the return type.
  cxx::CvQualifiedType returnType = returns->GetCxxType();
  m_CvTypeWriter->Add(returnType);
  
  // If the return type is an enumeration type, we need the
  // enumeration values.
  const EnumerationType* et = EnumerationType::SafeDownCast(returns);
  if(et)
    {
    m_EnumerationsThatNeedReturn.insert(et->GetEnumeration());
    m_EnumerationsThatNeedValues.insert(et->GetEnumeration());
    }
  
  // If the return type is a class type or pointer or reference to
  // class type, we must add supperclass information so that
  // derived-to-base conversions can be registered.
  const cxx::Type* rtp = returnType.GetType();
  cxx::CvQualifiedType cvType;
  if(returnType.IsReferenceType())
    {
    cvType = cxx::ReferenceType::SafeDownCast(rtp)->GetReferencedType();
    }
  else if(returnType.IsPointerType())
    {
    cvType = cxx::PointerType::SafeDownCast(rtp)->GetPointedToType();
    }
  else
    {
    cvType = returnType;
    }
  if(cvType.IsClassType())
    {
    const cxx::ClassType* classType =
      cxx::ClassType::SafeDownCast(cvType.GetType());
    this->AddSuperclassCvTypes(classType);
    m_ClassesForDerivedToBase.insert(classType);
    }
}

//----------------------------------------------------------------------------
void TclGenerator::FindArgumentCvTypes(const Type* argument)
{
  // Add the argument's type.
  cxx::CvQualifiedType argType = argument->GetCxxType();
  m_CvTypeWriter->Add(argType);
  
  // If the argument type is an enumeration type or a reference to
  // const enumeration type, we want to make sure the enumeration
  // value objects are provided.
  const Type* type = argument;
  const EnumerationType* et = EnumerationType::SafeDownCast(argument);
  const ReferenceType* rt = ReferenceType::SafeDownCast(argument);
  if(rt)
    {
    et = EnumerationType::SafeDownCast(rt->GetTarget());
    }
  if(et)
    {
    m_EnumerationsThatNeedValues.insert(et->GetEnumeration());
    }
}

//----------------------------------------------------------------------------
void TclGenerator::AddCvReferenceType(const cxx::CvQualifiedType& target)
{
  cxx::TypeSystem* ts = m_SourceRepresentation->GetTypeSystem();
  const cxx::ReferenceType* rt = ts->GetReferenceType(target);
  m_CvTypeWriter->Add(rt->GetCvQualifiedType(false, false));
}

//----------------------------------------------------------------------------
void TclGenerator::AddCvPointerType(const cxx::CvQualifiedType& target)
{
  cxx::TypeSystem* ts = m_SourceRepresentation->GetTypeSystem();
  const cxx::PointerType* pt = ts->GetPointerType(target);
  m_CvTypeWriter->Add(pt->GetCvQualifiedType(false, false));
}

//----------------------------------------------------------------------------
void TclGenerator::AddSuperclassCvTypes(const cxx::ClassType* classType)
{
  // Add given class's pointer and reference types.
  cxx::CvQualifiedType self = classType->GetCvQualifiedType(false, false);
  cxx::CvQualifiedType constSelf = classType->GetCvQualifiedType(true, false);
  this->AddCvReferenceType(self);
  this->AddCvReferenceType(constSelf);
  this->AddCvPointerType(self);
  this->AddCvPointerType(constSelf);
  
  // Now add the parent class types.
  for(cxx::ClassTypes::const_iterator p = classType->ParentsBegin();
      p != classType->ParentsEnd(); ++p)
    {
    this->AddSuperclassCvTypes(*p);
    }
}

//----------------------------------------------------------------------------
void TclGenerator::WriteHeaders() const
{
  std::ostream& os = *m_Stream;
  os <<
    "/**\n"
    " * Automatically generated by CABLE.  Do not edit!\n"
    " */\n"
    "\n";
  
  if(m_ConfigFileName.length() > 0)
    {
    os <<
      "// Include the code that is getting wrapped.\n"
      "#include \"" << m_ConfigFileName.c_str() << "\"\n"
      "\n";
    }
  
  os <<
    "// Include wrapper facility headers.\n"
    "#include \"CableTclFacility/ctCalls.h\"\n"
    "#include \"CableTclFacility/ctConverters.h\"\n"
    "#include \"CableTclFacility/ctClassWrapper.h\"\n"
    "#include \"CableTclFacility/ctMethod.h\"\n"
    "#include \"CableTclFacility/ctStaticMethod.h\"\n"
    "#include \"CableTclFacility/ctConstructor.h\"\n"
    "#include \"CableTclFacility/ctFunctionWrapper.h\"\n"
    "#include \"CableTclFacility/ctFunction.h\"\n"
    "\n";
}

//----------------------------------------------------------------------------
void TclGenerator::WritePackageInitialization() const
{
  std::ostream& os = *m_Stream;
  
  // Fix the case of the package name for Tcl package loader.
  String tclName = m_Package;
  String::iterator c = tclName.begin();
  if(c != tclName.end())
    {
    char ch = *c;
    if((ch >= 'a') && (ch <= 'z'))
      {
      *c = (ch+('A'-'a'));
      }
    for(++c; c != tclName.end(); ++c)
      {
      char ch = *c;
      if((ch >= 'A') && (ch <= 'Z'))
        {
        *c = (ch+('a'-'A'));
        }
      }
    }
  
  os <<
    "namespace _cable_tcl_\n"
    "{\n";
  for(GroupVector::const_iterator i = m_GroupVector.begin();
      i != m_GroupVector.end(); ++i)
    {
    os <<
      "extern void Wrapper_" << i->c_str() << "_Initialize(Tcl_Interp*);\n";
    }
    
  os <<
    "} // namespace _cable_tcl_\n"
    "\n";
  
  os <<
    "extern \"C\" { CABLE_TCL_WRAPPER_EXPORT int " << tclName.c_str() <<
    "_Init(Tcl_Interp* interp); }\n"
    "\n"
    "int " << tclName.c_str() << "_Init(Tcl_Interp* interp)\n"
    "{\n"
    "  // Initialize the WrapperFacility for this interpreter.\n"
    "  _cable_tcl_::WrapperFacility::GetForInterpreter(interp);\n"
    "\n"  
    "  // Make sure the WrapperFacility is the correct version.\n"
    "  if(!Tcl_PkgRequire(interp, \"cable\", \""
    CABLE_VERSION_STRING "\", 1))\n"
    "    {\n"
    "    return TCL_ERROR;\n"
    "    }\n"
    "  \n"
    "  // Initialize the wrappers in this package.\n";
  
  for(GroupVector::const_iterator i = m_GroupVector.begin();
      i != m_GroupVector.end(); ++i)
    {
    os <<
      "  _cable_tcl_::Wrapper_" << i->c_str() << "_Initialize(interp);\n";
    }
  
  // If there is a version number, write the Tcl_PkgProvide call.
  if(m_PackageVersion.length() > 0)
    {
    os <<
      "\n"
      "  Tcl_PkgProvide(interp, \"" << m_Package.c_str() <<
      "\", \"" << m_PackageVersion.c_str() << "\");\n";
    }
  
  os <<
    "  \n"
    "  return TCL_OK;\n"
    "}\n"
    "\n";
}

//----------------------------------------------------------------------------
void TclGenerator::WriteWrappers() const
{
  std::ostream& os = *m_Stream;
  
  // Hide all the code in an anonymous namespace inside _cable_tcl_.
  os <<
    "\n"
    "namespace _cable_tcl_\n"
    "{\n"
    "// Hide everything in an anonymous namespace.\n"
    "namespace\n"
    "{\n"
    "// Specializations will encapsulate wrappers.\n"
    "template <typename> struct Wrapper;\n"
    "\n";

  // Write out the block of CvType specializations for all needed types.
  os <<
    "// Define a CvType specialization for each type used in this wrapper"
    " group.\n";
  m_CvTypeWriter->WriteClasses(os);
  os <<
    "\n";
  m_CvTypeWriter->WriteDataDeclarations(os);
  this->WriteReturnEnumClasses();
  os <<
    "\n";
  
  // Generate the class wrappers.
  this->WriteClassWrappers();

  // Write out the block of CvType initializations.
  os <<
    "void InitializeGroupTypeRepresentations()\n"
    "{\n";
  m_CvTypeWriter->WriteInitializations(os);
  os <<
    "}\n"
    "void InitializeGroupTypeHandlers(WrapperFacility* wrapperFacility)\n"
    "{\n";
  this->WriteGroupTypeHandlers();
  os <<
    "}\n"
    "} // anonymous namespace\n"
    "\n"
    "void Wrapper_" << m_Group.c_str() << "_Initialize(Tcl_Interp* interp)\n"
    "{\n"
    "  // Get the WrapperFacility for this interpreter.\n"
    "  WrapperFacility* wrapperFacility = WrapperFacility::GetForInterpreter(interp);\n"
    "\n"    
    "  // Make sure the global group stuff has been initialized.\n"
    "  static bool initialized = false;\n"
    "  if(!initialized)\n"
    "    {\n"
    "    InitializeGroupTypeRepresentations();\n"
    "    initialized = true;\n"
    "    }\n"
    "\n"
    "  // Initialize interpreter-specific stuff for this group.\n"
    "  InitializeGroupTypeHandlers(wrapperFacility);\n"
    "\n"
    "  // Register this group's wrappers with the WrapperFacility.\n";
  for(WrapperList::const_iterator w = m_WrapperList.begin();
      w != m_WrapperList.end(); ++w)
    {
    os <<
      "  Wrapper< " << w->c_str() <<
      " >::RegisterWithWrapperFacility(wrapperFacility);\n";
    }
  os <<
    "}\n"
    "\n"
    "} // namespace _cable_tcl_\n";    
}

//----------------------------------------------------------------------------
void TclGenerator::WriteReturnEnumClasses() const
{
  // For all enumeration types that are returned from a method, write
  // the corresponding ReturnEnum<T> class specialization code.
  std::ostream& os = *m_Stream;
  for(EnumerationsThatNeedReturn::const_iterator
        e = m_EnumerationsThatNeedReturn.begin();
      e != m_EnumerationsThatNeedReturn.end(); ++e)
    {
    const Enumeration* enumeration = *e;
    String typeName = enumeration->GetQualifiedName();
    String prefix = enumeration->GetContext()->GetQualifiedName();
    os <<
      "\n"
      "template <>\n"
      "struct ReturnEnum< "<< typeName.c_str() << " >\n"
      "{\n"
      "  static void From(const " << typeName.c_str() << "& result,\n"
      "                   const WrapperFacility* wrapperFacility)\n"
      "    {\n"
      "    const char* name=0;\n"
      "    switch (result)\n"
      "      {\n";
    for(Enumeration::Iterator v = enumeration->Begin();
        v != enumeration->End(); ++v)
      {
      String value = prefix+"::"+*v;
      os <<
        "      case " << value.c_str() <<
        ": name = \"" << value.c_str() << "\"; break;\n";
      }
    os <<
      "      }\n"
      "    Tcl_SetObjResult(wrapperFacility->GetInterpreter(),\n"
      "                     Tcl_NewStringObj(const_cast<char*>(name), -1));\n"
      "    }\n"
      "};\n";    
    }
}

//----------------------------------------------------------------------------
void TclGenerator::WriteEnumValueRegistration() const
{
  // Write code that generates enumeration object instances for each
  // value of the enumeration types used by the wrappers.
  std::ostream& os = *m_Stream;
  for(EnumerationsThatNeedValues::const_iterator
        e = m_EnumerationsThatNeedValues.begin();
      e != m_EnumerationsThatNeedValues.end(); ++e)
    {
    const Enumeration* enumeration = *e;
    String typeName = enumeration->GetQualifiedName();
    String prefix = enumeration->GetContext()->GetQualifiedName();
    if(prefix != "::") { prefix += "::"; }
    for(Enumeration::Iterator v = enumeration->Begin();
        v != enumeration->End(); ++v)
      {
      String value = prefix+*v;
      os <<
        "  wrapperFacility->SetEnumerationConstant(\n" <<
        "    \"" << value.c_str() << "\", new " << typeName.c_str() <<
        "(" << value.c_str() << "),\n"
        "    CvType< " << typeName.c_str() << " >::type.GetType());\n";
      }
    }
}

//----------------------------------------------------------------------------
void TclGenerator::WriteGroupTypeHandlers() const
{
  // Write out the code to register conversion functions for the wrappers.
  std::ostream& os = *m_Stream;
  for(ClassesForDerivedToBase::const_iterator
        c = m_ClassesForDerivedToBase.begin();
      c != m_ClassesForDerivedToBase.end(); ++c)
    {
    // Write out derived-to-base conversions.
    const cxx::ClassType* derived = *c;
    cxx::ClassTypes superclasses;
    derived->GetAllSuperclasses(superclasses);
    for(cxx::ClassTypes::const_iterator b = superclasses.begin();
        b != superclasses.end(); ++b)
      {
      const cxx::ClassType* base = *b;
      cxx::CvQualifiedType dt = derived->GetCvQualifiedType(false, false);
      cxx::CvQualifiedType dtc = derived->GetCvQualifiedType(true, false);
      cxx::CvQualifiedType bt = base->GetCvQualifiedType(false, false);
      cxx::CvQualifiedType btc = base->GetCvQualifiedType(true, false);
      this->WriteReferenceDerivedToBase(dt, bt);
      this->WriteReferenceDerivedToBase(dtc, btc);
      this->WritePointerDerivedToBase(dt, bt);
      this->WritePointerDerivedToBase(dtc, btc);
      }
    }
  for(ClassesThatNeedDestructor::const_iterator
        c = m_ClassesThatNeedDestructor.begin();
      c != m_ClassesThatNeedDestructor.end(); ++c)
    {
    const cxx::ClassType* curClass = *c;    
    os <<
      "  wrapperFacility->SetDeleteFunction(CvType< " <<
      curClass->Name().c_str() <<
      " >::type.GetType(), &OldObjectOf< " << curClass->Name().c_str() <<
      " >::Delete);\n";
    }
  for(EnumerationsThatNeedValues::const_iterator
        e = m_EnumerationsThatNeedValues.begin();
      e != m_EnumerationsThatNeedValues.end(); ++e)
    {
    const Enumeration* enumeration = *e;
    String typeName = enumeration->GetQualifiedName();
    os <<
      "  wrapperFacility->SetDeleteFunction(CvType< " << typeName.c_str() <<
      " >::type.GetType(), &OldObjectOf< " << typeName.c_str() <<
      " >::Delete);\n";
    }
  
  // Write code to register all the enumeration values.
  this->WriteEnumValueRegistration();  
}

//----------------------------------------------------------------------------
void
TclGenerator
::WriteReferenceDerivedToBase(const cxx::CvQualifiedType& derivedType,
                              const cxx::CvQualifiedType& baseType) const
{
  std::ostream& os = *m_Stream;
  cxx::TypeSystem* ts = m_SourceRepresentation->GetTypeSystem();
  const cxx::ReferenceType* brt = ts->GetReferenceType(baseType);
  cxx::CvQualifiedType baseRef = brt->GetCvQualifiedType(false, false);
  os <<
    "  wrapperFacility->SetConversion(\n"
    "    CvType< " << derivedType.GetName().c_str() << " >::type,\n"
    "    CvType< " << baseRef.GetName().c_str() << " >::type.GetType(),\n"
    "    Converter::ReferenceDerivedToBase< " <<
    derivedType.GetName().c_str() << ", \n" <<
    "                                       " <<
    baseType.GetName().c_str() << " >::GetConversionFunction());\n";
}

//----------------------------------------------------------------------------
void
TclGenerator
::WritePointerDerivedToBase(const cxx::CvQualifiedType& derivedType,
                            const cxx::CvQualifiedType& baseType) const
{
  std::ostream& os = *m_Stream;
  cxx::TypeSystem* ts = m_SourceRepresentation->GetTypeSystem();
  const cxx::PointerType* dpt = ts->GetPointerType(derivedType);
  const cxx::PointerType* bpt = ts->GetPointerType(baseType);  
  cxx::CvQualifiedType derivedPtr = dpt->GetCvQualifiedType(false, false);
  cxx::CvQualifiedType basePtr = bpt->GetCvQualifiedType(false, false);
  os <<
    "  wrapperFacility->SetConversion(\n"
    "    CvType< " << derivedPtr.GetName().c_str() << " >::type,\n"
    "    CvType< " << basePtr.GetName().c_str() << " >::type.GetType(),\n"
    "    Converter::PointerDerivedToBase< " <<
    derivedType.GetName().c_str() << ", \n" <<
    "                                     " <<
    baseType.GetName().c_str() << " >::GetConversionFunction());\n";
}

//----------------------------------------------------------------------------
void TclGenerator::WriteClassWrappers() const
{
  // Write out each class wrapper.
  for(ClassWrapperMap::const_iterator i = m_ClassWrapperMap.begin();
      i != m_ClassWrapperMap.end(); ++i)
    {
    this->WriteClassWrapper(i->first, i->second);
    }
}

//----------------------------------------------------------------------------
void TclGenerator::WriteClassWrapper(const Class* c,
                                     const StringVector& names) const
{
  std::ostream& os = *m_Stream;
  String cName = c->GetQualifiedName();
  
  // List this class wrapper.
  m_WrapperList.push_back(cName);
  
  // Find the methods to wrap.
  MethodVector methods;
  MethodVector converters;
  this->FindMethods(c, methods, converters);
  
  // Write the wrapper class definition.
  this->WriteClassWrapperDefinition(c, names, methods);
  
  // Write the method wrapper definitions.
  for(unsigned int index = 0; index < methods.size(); ++index)
    {
    this->WriteMethodWrapper(c, methods, index);
    }
  
  // Write the wrapper registration function.
  this->WriteClassWrapperRegistration(c, names, methods, converters);
}

//----------------------------------------------------------------------------
void TclGenerator::FindMethods(const Class* c, MethodVector& methods,
                               MethodVector& converters) const
{
  // Check whether the copy constructor will be valid.
  bool copyValid = c->HasPublicCopyConstructor();
  const ClassType* ct = c->GetClassType();
  bool hasPublicDestructor = true;
  for(Context::Iterator i = c->Begin(); i != c->End(); ++i)
    {
    // We want only members that are public methods.
    Method* m = Method::SafeDownCast(*i);
    if(m && (i.GetAccess() == Context::Public) &&
       (copyValid || (!c->IsCopyConstructor(m))))
      {
      Function::FunctionIdType fid = m->GetFunctionId();
      FunctionType* ft = m->GetFunctionType();
      if((fid == Function::MethodId) || (fid == Function::OperatorMethodId) ||
         (fid == Function::ConstructorId && !c->GetAbstract()))
        {
        // Add a method entry for each allowed number of arguments.
        unsigned int ac = ft->GetNumberOfArguments();
        unsigned int rac = ft->GetNumberOfRequiredArguments();
        for(unsigned int i=rac; i <= ac; ++i)
          {
          methods.push_back(MethodEntry(m, i));
          }
        }
      else if(fid == Function::ConverterId)
        {
        converters.push_back(MethodEntry(m, 0));
        }
      // If the return type is a class type (not a pointer or
      // reference to it), then it will be a temporary and must have a
      // public destructor.
      const ClassType* rt = ClassType::SafeDownCast(ft->GetReturns());
      if(rt)
        {
        const cxx::Type* t = rt->GetCxxType().GetType();
        m_ClassesThatNeedDestructor.insert(cxx::ClassType::SafeDownCast(t));
        }      
      }
    else if(m && (i.GetAccess() != Context::Public) &&
            (m->GetFunctionId() == Function::DestructorId))
      {
      // Found a non-public destructor.
      hasPublicDestructor = false;
      }
    }
  
  // If there is a public destructor available, we want to register it.
  if(hasPublicDestructor)
    {
    cxx::CvQualifiedType classCvType = ct->GetCxxType();
    const cxx::Type* t = classCvType.GetType();
    m_ClassesThatNeedDestructor.insert(cxx::ClassType::SafeDownCast(t));
    }
}

//----------------------------------------------------------------------------
void
TclGenerator::WriteClassWrapperDefinition(const Class* c,
                                          const StringVector& names,
                                          const MethodVector& methods) const
{
  std::ostream& os = *m_Stream;
  String cName = c->GetQualifiedName();
  // Write comment and beginning of class definition.
  os <<
    "/**\n"
    " * Class wrapper definition for\n"
    " *   " << cName.c_str() << "\n"
    " *\n"
    " * Wrapped names are:\n";
  for(StringVector::const_iterator i = names.begin(); i != names.end(); ++i)
    {
    os << " *   " << i->c_str() << "\n";
    }
  os <<
    " */\n"
    "template <>\n"
    "struct Wrapper< " << cName.c_str() << " >\n"
    "{\n"
    "  // Prototypes for method wrappers.\n";
  for(unsigned int index = 0; index < methods.size(); ++index)
    {
    const Method* m = methods[index].GetMethod();
    os << "  static void ";
    this->WriteMethodName(m, index);
    os << "(const WrapperFacility*, const Arguments&);\n";    
    }
  // Write end of class definition.
  os <<
    "\n"
    "  // Function to register these wrappers with a WrapperFacility.\n"
    "  static void RegisterWithWrapperFacility(WrapperFacility*);\n"
    "};\n"
    "\n";
}

//----------------------------------------------------------------------------
void TclGenerator::WriteMethodName(const Method* m, unsigned int index) const
{
  // Write the name of the method wrapper corresponding to the given
  // method.
  std::ostream& os = *m_Stream;
  Function::FunctionIdType fid = m->GetFunctionId();
  String stName = "";
  String mtName = "Method";
  String fName = m->GetName();
  if(m->GetStatic()) { stName = "Static"; }
  if(fid == Function::ConstructorId) { mtName = "Constructor"; }
  else if(fid == Function::OperatorMethodId)
    {
    mtName = "Operator";
    fName = this->GetOperatorName(fName.c_str());
    }
  os << stName.c_str() << mtName.c_str() << "_" << index << "_" 
     << fName.c_str();
}

//----------------------------------------------------------------------------
void TclGenerator::WriteMethodWrapper(const Class* c,
                                      const MethodVector& methods,
                                      unsigned int index) const
{
  std::ostream& os = *m_Stream;
  const Method* m = methods[index].GetMethod();
  String cName = c->GetQualifiedName();
  unsigned int numberOfArguments = methods[index].GetArgumentCount();
  Function::FunctionIdType fid = m->GetFunctionId();
  bool isStatic = m->GetStatic();
  
  this->WriteMethodComment(c, m);

  os <<
    "void\n"
    "Wrapper< " << cName.c_str() << " >\n"
    "::";
  this->WriteMethodName(m, index);
  os <<
    "(const WrapperFacility* wrapperFacility, const Arguments& arguments)\n"
    "{\n";
  
  if(fid == Function::ConstructorId)
    {
    os << "  Return< " << cName.c_str() << " >::FromConstructor(new "
       << cName.c_str() << "(";
    this->WriteArgumentList(m->GetFunctionType(), 0, numberOfArguments);
    os << "), wrapperFacility);\n";
    }
  else
    {
    if(isStatic)
      {
      this->WriteReturnBegin(m);
      os << "  " << cName.c_str() << "::";
      }
    else
      {
      this->WriteImplicitArgument(c, m);
      this->WriteReturnBegin(m);
      os << "  instance.";
      }    
    os << m->GetCallName() << "(";
    this->WriteArgumentList(m->GetFunctionType(), 1, numberOfArguments);
    this->WriteReturnEnd(m);
    os << ");\n";
    }
  
  os <<
    "}\n"
    "\n";
}

//----------------------------------------------------------------------------
bool TclGenerator::ReturnsVoid(const Function* f) const
{
  const FunctionType* ft = f->GetFunctionType();
  const Type* rt = ft->GetReturns();
  if(!rt) { return true; }
  const cxx::Type* t = rt->GetCxxType().GetType();
  if(!t->IsFundamentalType()) { return false; }
  if(cxx::FundamentalType::SafeDownCast(t)->IsVoid()) { return true; }
  return false;
}

//----------------------------------------------------------------------------
String TclGenerator::GetReturnTypeName(const Function* f) const
{
  String returnTypeName = "void";
  if(!this->ReturnsVoid(f))
    {
    cxx::CvQualifiedType rt = f->GetFunctionType()->GetReturns()->GetCxxType();
    returnTypeName = rt.GetName();
    }
  return returnTypeName;
}

//----------------------------------------------------------------------------
void TclGenerator::WriteMethodComment(const Class* c, const Method* m) const
{
  // Write the comment for a method wrapper describing the prototype
  // of the wrapped method.
  std::ostream& os = *m_Stream;
  String mName = c->GetName();
  mName += "::";
  mName += m->GetCallName();
  const FunctionType* ft = m->GetFunctionType();
  cxx::CvQualifiedType cvft = ft->GetCxxType();
  String prototype = cvft.GenerateDeclaration(mName.c_str(), m->GetConst());
  
  // For a constructor, remove the "void " from the prototype.
  if(m->GetFunctionId() == Function::ConstructorId)
    {
    prototype = prototype.substr(5);
    }
  
  os <<
    "/**\n"
    " * " << prototype.c_str() << "\n"
    " */\n";
}

//----------------------------------------------------------------------------
void TclGenerator::WriteImplicitArgument(const Class* c, const Method* m) const
{
  std::ostream& os = *m_Stream;
  String implicit = c->GetQualifiedName();
  if(m->GetConst())
    {
    implicit = "const "+implicit;
    }
  os << "  " << implicit.c_str() << "& instance = ArgumentAsReferenceTo< "
     << implicit.c_str() << " >(wrapperFacility)(arguments[0]);\n";
}

//----------------------------------------------------------------------------
void TclGenerator::WriteReturnBegin(const Function* f) const
{
  std::ostream& os = *m_Stream;
  if(this->ReturnsVoid(f)) { return; }  
  const FunctionType* ft = f->GetFunctionType();
  const Type* rt = ft->GetReturns();
  const cxx::CvQualifiedType cvType = rt->GetCxxType();
  if(cvType.IsPointerType())
    {
    const cxx::CvQualifiedType targetType =
      cxx::PointerType::SafeDownCast(cvType.GetType())->GetPointedToType();
    os << "  ReturnPointerTo< "
       << targetType.GetName().c_str() << " >::From(\n";
    }
  else if(cvType.IsReferenceType())
    {
    const cxx::CvQualifiedType targetType =
      cxx::ReferenceType::SafeDownCast(cvType.GetType())->GetReferencedType();
    os << "  ReturnReferenceTo< "
       << targetType.GetName().c_str() << " >::From(\n";
    }
  else if(cvType.IsEnumerationType())
    {
    os << "  ReturnEnum< " << cvType.GetName().c_str() << " >::From(\n";
    }
  else
    {
    os << "  Return< " << cvType.GetName().c_str() << " >::From(\n";
    }
}

//----------------------------------------------------------------------------
void TclGenerator::WriteReturnEnd(const Function* f) const
{      
  std::ostream& os = *m_Stream;
  if(this->ReturnsVoid(f))
    {
    os << ");\n"
       << "  Return<void>::From(wrapperFacility";
    }
  else
    {
    os << "), wrapperFacility";
    }
}

//----------------------------------------------------------------------------
void TclGenerator::WriteArgumentList(const FunctionType* ft,
                                     unsigned int offset,
                                     unsigned int count) const
{
  // Write the code to convert and pass a list of arguments to wrapped
  // method in its method wrapper's actual call to it.
  std::ostream& os = *m_Stream;
  for(unsigned int i=0; i < count; ++i)
    {
    cxx::CvQualifiedType cvType = ft->GetArgument(i)->GetCxxType();
    if(i > 0) { os << ","; }
    os << "\n"
       << "    CvType< " << cvType.GetName()
       << " >::ArgumentFor(wrapperFacility)(arguments[" << (i+offset)
       << "])";
    }  
}

//----------------------------------------------------------------------------
void
TclGenerator
::WriteClassWrapperRegistration(const Class* c,
                                const StringVector& names,
                                const MethodVector& methods,
                                const MethodVector& converters) const
{
  // Write the wrapper registration function.
  std::ostream& os = *m_Stream;
  String cName = c->GetQualifiedName();
  os <<
    "void\n"
    "Wrapper< " << cName.c_str() << " >\n"
    "::RegisterWithWrapperFacility(WrapperFacility* wrapperFacility)\n"
    "{\n"
    "  // Get the ClassType representation for the wrapped class.\n"
    "  const ClassType* wrappedTypeRepresentation =\n"
    "    ClassType::SafeDownCast(CvType< " << cName.c_str() <<
    " >::type.GetType());\n"
    "\n"
    "  // Ask the WrapperFacility to create a ClassWrapper for this type.\n"
    "  ClassWrapper* wrapper =\n"
    "    wrapperFacility->CreateClassWrapper(wrappedTypeRepresentation);\n"
    "\n"
    "  // If no ClassWrapper is given, then a wrapper for this class has\n"
    "  // already been registered.\n"
    "  if(!wrapper) { return; }\n"
    "\n";
  
  // Register the class's command names.
  os << "  // Register commands for the class.\n";
  for(StringVector::const_iterator i = names.begin(); i != names.end(); ++i)
    {
    os << "  wrapper->AddInterpreterClassCommand(\"" << i->c_str() << "\");\n";
    }
  os << "\n";
  
  // Write code to register all the method wrappers.
  os <<
    "  // Register constructor, method, conversion,"
    " and enumeration wrappers.\n";
  for(unsigned int index = 0 ; index < methods.size() ; ++index)
    {
    this->WriteMethodRegistration(c, methods, index);
    }
  
  // Write code to register all the conversions.
  for(unsigned int index = 0 ; index < converters.size() ; ++index)
    {
    this->WriteConverterRegistration(c, converters[index].GetMethod());
    }
  
  os << "}\n"
     << "\n";
}

//----------------------------------------------------------------------------
void TclGenerator::WriteMethodRegistration(const Class* c,
                                           const MethodVector& methods,
                                           unsigned int index) const
{
  std::ostream& os = *m_Stream;
  const Method* m = methods[index].GetMethod();
  const FunctionType* ft = m->GetFunctionType();
  unsigned int numberOfArguments = methods[index].GetArgumentCount();
  Function::FunctionIdType fid = m->GetFunctionId();
  bool isStatic = m->GetStatic();
  bool isOperator = (fid == Function::OperatorMethodId);
  
  if(numberOfArguments > 0)
    {
    os << "  {\n"
       << "  Method::ParameterTypes parameterTypes;\n";
    for(unsigned int i=0; i < numberOfArguments; ++i)
      {
      os << "  parameterTypes.push_back(CvType< "
         << ft->GetArgument(i)->GetCxxType().GetName().c_str()
         << " >::type.GetType());\n";
      }
    }
  
  os << "  wrapper->AddFunction(\n";
  
  if(fid == Function::ConstructorId)
    {  
    os << "    new Constructor(wrapperFacility, &Wrapper::";
    this->WriteMethodName(m, index);
    os << ",\n"
       << "                    wrappedTypeRepresentation, "
       << "\"" << m->GetName() << "\"";
    }
  else
    {
    String returnTypeName = this->GetReturnTypeName(m);
    String wft = "Method";
    String indent = "";
    if(isStatic)
      {
      wft = "StaticMethod";
      indent = "      ";
      }
    os << "    new " << wft.c_str() << "(wrapperFacility, &Wrapper::";
    this->WriteMethodName(m, index);
    os << ",\n";
    os << "               " << indent.c_str()
       << "wrappedTypeRepresentation, \"" << m->GetName() << "\", ";
    if(!isStatic)
      {
      os << (m->GetConst()? "true":"false") << ", ";
      }
    os << (isOperator? "true":"false") << ",\n"
       << "               " << indent.c_str()
       << "CvType< " << returnTypeName.c_str() << " >::type";
    }  
  
  if(numberOfArguments > 0)
    {
    os << ",\n"
       << "      parameterTypes));\n"
       << "  }\n";
    }
  else
    {
    os << "));\n";
    }
}

//----------------------------------------------------------------------------
void TclGenerator::WriteConverterRegistration(const Class* c,
                                              const Method* m) const
{
  std::ostream& os = *m_Stream;
  String sourceType = c->GetQualifiedName();
  String returnTypeName = this->GetReturnTypeName(m);
  if(m->GetConst())
    {
    sourceType = "const "+sourceType;
    }
  
  os << "  wrapperFacility->SetConversion(\n"
     << "    CvType< " << sourceType.c_str() << " >::type,\n"
     << "    CvType< " << returnTypeName.c_str() << " >::type.GetType(),\n"
     << "    Converter::ConversionOperator< " << sourceType.c_str() << ", \n"
     << "                                   "
     << returnTypeName.c_str() << " >::GetConversionFunction());\n";
}

} // namespace cable
