// include swig stuff
#include "CableSwig.h"

//#include "swigmod.h"
#ifndef MACSWIG
#include "swigconfig.h"
#endif
extern "C" {
#include "preprocessor.h"
}
#include <ctype.h>

// include cable stuff
#include "cableVersion.h"
#include "cableXMLSourceParser.h"
#include "cableSourceRepresentation.h"
#include "cableTclGenerator.h"
#include "cableSystemTools.h"
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
#include "cxxCvQualifiedType.h"
#include "cxxPointerType.h"
#include "cxxReferenceType.h"
#include "cxxFunctionType.h"
#include "cxxClassType.h"
using namespace cable;



 
// copied from swig, with a modification
// that it does not look for = 0, but looks at 
// the "abstract" attribute
static List *pure_abstract(Node *n) {
   List *abs = 0;
   while (n) {
     if (Cmp(nodeType(n),"cdecl") == 0) {
       ::String *decl = Getattr(n,"decl");
       if (SwigType_isfunction(decl)) {
	 ::String *init = Getattr(n,"abstract");
	 if (Cmp(init,"1") == 0) {
	   if (!abs) {
	     abs = NewList();
	   }
	   Append(abs,n);
	 }
       }
     } else if (Cmp(nodeType(n),"destructor") == 0) {
       if (Cmp(Getattr(n,"abstract"),"1") == 0) {
	 if (!abs) {
	   abs = NewList();
	 }
	 Append(abs,n);
       }
     }
     n = nextSibling(n);
   }
   return abs;
}




void CableSwig::ReplaceString(std::string& source,
                              const char* replace,
                              const char* with)
{
  // get out quick if string is not found
  std::string::size_type start = source.find(replace);
  if(start ==  std::string::npos)
    {
    return;
    }
  std::string rest;
  std::string::size_type lengthReplace = strlen(replace);
  std::string::size_type lengthWith = strlen(with);
  while(start != std::string::npos)
    {
    rest = source.substr(start+lengthReplace);
    source = source.substr(0, start);
    source += with;
    source += rest;
    start = source.find(replace, start + lengthWith );
    }
}

// return the class name unless it is a templated class,
// in which case you return the typedef for the class
std::string CableSwig::GetClassName(const Class* c, const char* td)
{
  std::string name = c->GetName();
  if(name.find('<') == name.npos)
    {
    return name;
    }
  if(!td)
    {
    return this->TemplateName(c->GetName());
    }
  return std::string(td);
}

// convert to swig template names  <int> is <(int)>
std::string CableSwig::TemplateName(const char* s)
{
  std::string name = s;
  ReplaceString(name, "<", "<(");
  ReplaceString(name, ">", ")>");
  return name;
}


// convert from cable to swig type, recursive function
std::string CableSwig::ToSwigType(cxx::CvQualifiedType const&  t)
{
  switch(t.GetRepresentationType())
    {
    case cxx::ArrayType_id:
      break;
    case cxx::ClassType_id:
      break;
    case cxx::EnumerationType_id:
      break;
    case cxx::PointerType_id:
      {
      cxx::PointerType const* p
        = cxx::PointerType::SafeDownCast(t.GetType());
      std::string n = "p.";
      n += this->ToSwigType(p->GetPointedToType());
      return n;
      }
      break;
    case cxx::PointerToMemberType_id:
      break;
    case cxx::ReferenceType_id:
      {
      cxx::ReferenceType const* r 
        = cxx::ReferenceType::SafeDownCast(t.GetType());
      std::string n = "r.";
      n += this->ToSwigType(r->GetReferencedType());
      return n;
      break;
      }
    
    case cxx::FundamentalType_id:
      break;
    case cxx::FunctionType_id: 
      {
    
      cxx::FunctionType const* f 
        = cxx::FunctionType::SafeDownCast(t.GetType());
      std::string n = "f(";
      cxx::CvQualifiedTypes const& args = f->GetArgumentTypes();
      cxx::CvQualifiedTypes::const_iterator i = args.begin();
      if(i != args.end())
        {
        n += this->ToSwigType(*i);
        for(++i; i != args.end(); ++i)
          {
          n += ",";
          n += this->ToSwigType(*i);
          }
        }
      n += ").";
      n += this->ToSwigType(f->GetReturnType());
      return n;
      break;
      }
    case cxx::Undefined_id:
      break;
    }
  std::string ret;
  if(t.IsConst())
    {
    ret = "q(const).";
    }
  ret += this->TemplateName(t.GetType()->Name().c_str());
  return ret;
}





// Create a swig method and add it to the swig class Node sc
// sc must be a swig class node

void CableSwig::CreateSwigMethod(Method* mth, Node* sc, std::string& cname, 
                                 const char* access)
{
  m_CurrentFile = mth->GetFile();
  m_CurrentLine = mth->GetLine();
  Function::FunctionIdType fid = mth->GetFunctionId();
  FunctionType* ft = mth->GetFunctionType();
  const char* ftype = "cdecl";
  Node* m;
  if(fid == Function::ConverterId)
    {
    return;
    }
  const ClassType* rtype = ClassType::SafeDownCast(ft->GetReturns());
  if(rtype)
    {
    const Class* c = rtype->GetClass();
    Context* context = c->GetContext();
    if(context && Class::SafeDownCast(context))
      {
      std::cout << "Skipping method " 
                << mth->GetName() 
                << " because return type is the nested class: "
                << c->GetQualifiedName() << "\n";
      return;
      }
    }
  // do not wrap public destructors, as this
  // seems to confuse swig
  if(fid == Function::DestructorId)
    {
    if(strcmp(access, "public") == 0)
      {
      return;
      }
    }
  
  
  if(fid == Function::ConstructorId)
    {
    m = new_node("constructor");
    // for a constructor use the name of the class as the function name
    Setattr(m, "name", Getattr(sc, "name"));
    }
  else if(fid == Function::DestructorId)
    {
    m = new_node("destructor");
    // for a constructor use the name of the class as the function name
    std::string name = "~";
    name += cname;
    Setattr(m, "name", name.c_str());
    }
  else
    {
    m = new_node("cdecl");
    Setattr(m, "name", mth->GetCallName().c_str());
    if(mth->GetVirtual())
      {
      Setattr(m, "storage", "virtual");
      if(mth->GetPureVirtual())
        {
        Setattr(m, "abstract", "1");
        }
      }
    if(mth->GetStatic())
      { 
      Setattr(m, "storage", "static");
      }
    }
  if(access)
    {
    Setattr(m, "access", access);
    }
  
  ParmList* parms = 0;
  Parm* pp = 0;
  std::string allParams;
  for(unsigned int arg = 0; arg < ft->GetNumberOfArguments(); ++arg)
    {    
    std::string swigArg = this->ToSwigType(ft->GetArgument(arg)->GetCxxType());
    ::String* argName 
        = NewString(swigArg.c_str());
    if(allParams.size())
      {
      allParams += ", ";
      }
    allParams += Char(argName);
    Parm* p = NewParm(argName, 0); 
    Delete(argName);
    if(!parms)
      {
      parms = p;
      }
    else
      {
      set_nextSibling(pp, p);
      }
    pp = p;
    } 
  std::string decl;
  if(mth->GetConst())
    {
    decl += "q(const).";
    }
  decl += "f(";
  decl +=  allParams;
  decl += ").";
  // Set the declaration on the method
  Setattr(m, "decl", decl.c_str());
  // if there are parameters set them 
  if(parms)
    {
    Setattr(m, "parms", parms);
    }
  if(fid ==  Function::ConstructorId)
    {
    Setattr(m, "feature:new", "1");
    }
  else if(fid == Function::DestructorId)
    {
    }
  else
    {
    Setattr(m, "type", 
            this->ToSwigType(ft->GetReturns()->GetCxxType()).c_str());
    } 
  const ClassType* rt = ClassType::SafeDownCast(ft->GetReturns());
  // add the method to the class
  appendChild(sc, m);
  // add the member name to the class
  std::string symbolName;
  if(fid == Function::ConstructorId)
    {
    symbolName = cname;
    }
  else if(fid == Function::DestructorId)
    {
    symbolName = cname;
    }
  else
    {
    symbolName = mth->GetCallName();
    }
  // python has print as a keyword, so rename
  // it to Print
  if(symbolName == "print" && m_WrapLanguage == "python")
    {
    symbolName = "Print";
    }
  Swig_symbol_add((char*)symbolName.c_str(), m);
}

Node* CableSwig::CreateSwigClass(const Class* c, const char* td)
{
  std::string typedefName = this->GetClassName(c, td);
  std::string actualName = this->TemplateName(c->GetName()).c_str();
  

  Context* context = c->GetContext();
  if(context && Class::SafeDownCast(context))
    {
    typedefName = actualName = c->GetQualifiedName();
    }
  
  
  Node* sc = new_node("class");
  Setattr(sc, "name", actualName.c_str());
  Setattr(sc,"kind","class");
  Setattr(sc,"allows_typedef","1");
  // Create symbol table information
  // create a new scope 
  Symtab* sym = Swig_symbol_newscope();
  Swig_symbol_setscopename(typedefName.c_str());
  // set the symtab on the class
  Setattr(sc,"symtab",sym);
  // pop out of the new scope so we can add the class
  // to the outer scope
  Swig_symbol_popscope();
  // add the class to the current scope
  Swig_symbol_add((char*)typedefName.c_str(), sc);
  // Set the scope to the scope of the class for adding the methods
  Swig_symbol_setscope(sym);

  // first private methods
  Node* p = new_node("access");
  Setattr(p,"kind","private");
  // add the private declaration to the class
  appendChild(sc, p);
  for(Class::Iterator i = c->Begin(); i != c->End(); ++i)
    {
    Method* mth = Method::SafeDownCast(*i);
    if(mth && (i.GetAccess() == Class::Private))
      {
      CreateSwigMethod(mth, sc, typedefName, "private");
      }
    }
  
  // protected methods
  p = new_node("access");
  Setattr(p,"kind","protected");
  // add the protected declaration to the class
  appendChild(sc, p);
  for(Class::Iterator i = c->Begin(); i != c->End(); ++i)
    {
    Method* mth = Method::SafeDownCast(*i);
    if(mth && (i.GetAccess() == Class::Protected))
      {
      CreateSwigMethod(mth, sc, typedefName, "protected");
      }
    }

  // public methods
  p = new_node("access");
  Setattr(p,"kind","public");
  // add the public declaration to the class
  appendChild(sc, p);
  for(Class::Iterator i = c->Begin(); i != c->End(); ++i)
    {
    Method* mth = Method::SafeDownCast(*i);
    if(mth && (i.GetAccess() == Class::Public))
      {
      CreateSwigMethod(mth, sc, typedefName, "public");
      }
    }
  Setattr(sc,"abstract", pure_abstract(firstChild(sc)));
  SwigType_typedef_class((char*)this->TemplateName(c->GetName()).c_str());
  
  std::vector<Class*> parents;
  c->GetBaseClasses(parents); 
  List *bases = NewList();
  for( std::vector<Class*>::iterator i = parents.begin();
      i != parents.end(); ++i)
    {
    if(this->FindClassGroup((*i)->GetQualifiedName().c_str()))
      {    
      ::String* s=  NewStringf("%s", this->TemplateName((*i)->GetQualifiedName().c_str()).c_str());
      Append(bases,s);
      }
    }
  if(Len(bases))
    {
    Setattr(sc, "baselist", bases);
    }
  // if no methods force 
  if(c->Begin() == c->End())
    { 
    std::cerr << "force noassign for " << c->GetName() << "\n";
    Setattr(sc,"allocate:noassign", "1");
    }
  // leave the scope of the class
  Swig_symbol_popscope();
  return sc;
}


// Create a swig node for a class and a namespace
// and add them into the tree 
Node* CableSwig::CreateSwigClassInNamespace(const Class*c, const char* td,
                                            Node* top, const char* swigtype,
                                            Node* moduleIn)
{
  // if the class has already been included or imported
  // then do nothing and return
  if(m_ClassesIncludedOrImported.find(c) != m_ClassesIncludedOrImported.end())
    {
    return 0;
    }
  // add the class to the included or imported set
  m_ClassesIncludedOrImported.insert(c);
  // check to see if this is a nested class, and if so,
  // skip it because swig can not handle that case
  Context* context = c->GetContext();
  Namespace* classNameSpace = Namespace::SafeDownCast(context);
  // Create a new module node that copies the values from moduleIn
  Node* module = this->new_node("module");
  Setattr(module, "name", Getattr(moduleIn, "name"));
  Node* nameSpaceNode = 0;
  Node* priorScope =  Swig_symbol_current();  // save the swig scope
  // check to see if the class is in a namespace other than
  // the global one ::
  if(classNameSpace && 
     (strcmp(classNameSpace->GetName(), "::") != 0))
    {
    // create a new node for the namespace
    nameSpaceNode = new_node("namespace");
    Hash *h;
    // check for an existing namespace
    h = Swig_symbol_clookup((char*)classNameSpace->GetName(),0);
    if (h && (Strcmp(nodeType(h),"namespace") == 0)) 
      {
      if (Getattr(h,"alias")) 
        {
        h = Getattr(h,"namespace");
        }
      Swig_symbol_setscope(Getattr(h,"symtab"));
      }
    // this is a new namespace
    else
      {
      Swig_symbol_newscope();
      Swig_symbol_setscopename(classNameSpace->GetName());
      }
    }
  // Create the swig class node
  Node* swigClassNode = CreateSwigClass(c, td); 
  if(context && Class::SafeDownCast(context))
    {
    // ignore all nested classes
    Setattr(swigClassNode, "feature:ignore", "1");
    }
  
  // once the class has been created, we can finish
  // off the namespace creatation, and pop the scope
  if(nameSpaceNode)
    {
    set_nodeType(nameSpaceNode,"namespace");
    Setattr(nameSpaceNode,"name",classNameSpace->GetName());
    Setattr(nameSpaceNode,"symtab", Swig_symbol_popscope());
    Swig_symbol_setscope(priorScope);
    Swig_symbol_add((char*)classNameSpace->GetName(), nameSpaceNode);
    } 

  // at this point we have a nameSpaceNode and a swigClassNode
  // create include an include or import node for the class
  Node* importOrIncludeNode = new_node(swigtype);
  Setattr(importOrIncludeNode,"name", c->GetFile());
  Setattr(importOrIncludeNode,"module", Getattr(module, "name"));
  // add the module to the include or import node
  appendChild(importOrIncludeNode, module);

  if(nameSpaceNode)
    {
    // add the class to the namespace
    appendChild(nameSpaceNode, swigClassNode);
    // add the namespace to the import or include
    appendChild(importOrIncludeNode, nameSpaceNode);
    }
  else
    {
    // add the class to the import or include
    appendChild(importOrIncludeNode, swigClassNode); 
    }
  // add the importOrIncludeNode to the top node
  appendChild(top, importOrIncludeNode);
  return swigClassNode;
}

bool CableSwig::ParseName(const char* name, std::string& result)
{
  std::string group = name;
  std::string::size_type l = group.find('"');
  std::string::size_type r = group.rfind('"');
  if((l != std::string::npos) && (r != std::string::npos) && (r > l))
    {
    result = group.substr(l+1, r-l-1);
    return true;
    }
  return false;
}

bool CableSwig::GetGroupAndPackageInformation(Namespace* cns,
                                              std::string& groupName,
                                              std::vector<std::string>& groupsNames,
                                              std::string& packageName,
                                              std::string& packageVersion,
                                              std::string& configFile)
{
  const Variable* group = 0;
  Context::Iterator lower = cns->LowerBound("group");
  Context::Iterator upper = cns->UpperBound("group");
  if(lower != upper)
    {
    group = Variable::SafeDownCast(*lower);
    if(!group)
      {
      std::cerr << "Identifier _cable_::group is not a variable.\n";
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
      std::cerr << "Identifier _cable_::groups is not a variable.\n";
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
      std::cerr << "Identifier _cable_::package is not a variable.\n";
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
      std::cerr << "Identifier _cable_::package_version is not a variable.\n";
      return false;
      }
    }
  if(group)
    {
    if(!ParseName(group->GetInitializer(), groupName))
      {
      std::cerr << "Error parsing group name.\n";
      return false;
      }
    
    // Hold on to the name of the configuration file.
    configFile = group->GetFile();
    }
  if(package && !ParseName(package->GetInitializer(), packageName))
    {
    std::cerr << "Error parsing package name.\n";
    return false;
    }
  if(package_version &&
     !ParseName(package_version->GetInitializer(), packageVersion))
    {
    std::cerr << "Error parsing package version string.\n";
    return false;
    } 
  
  if(groups)
    {
    std::string gl = groups->GetInitializer();
    std::string::size_type lpos = gl.find('"');
    while((lpos != std::string::npos) && (lpos < gl.length()))
      {
      std::string::size_type rpos = gl.find_first_of(",}", lpos);
      std::string result;
      if((rpos == std::string::npos) ||
         !ParseName(gl.substr(lpos, rpos-lpos).c_str(), result))
        {
        std::cerr << "Error parsing groups list.\n";
        return false;
        }
      groupsNames.push_back(result);
      lpos = rpos+1;
      }
    }
  return true;
}

std::string CableSwig::GetTclName(const char* s)
{
  std::string tclName = s;
  std::string::iterator c = tclName.begin();
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
  return tclName;
}


// This is a copy of the file CableSwigModule.h 
// Currently, it supports PERL, TCL, and PYTHON
// Do not edit here, edit CableSwigModule.h, and
// then re-create this string from that file.
const char* SwigInitMacrosString = 
"/* --- DECLARE_MODULE_INIT and CALL_MODULE_INIT definitions */"
"\n#ifdef SWIGPERL\n"
"#ifndef PERL_OBJECT\n"
"#ifndef MULTIPLICITY\n"
"#define SWIG_INIT_ARGS CV* cv;\n"
"#define SWIG_INIT_CALL_ARGS cv\n"
"#else\n"
"#define SWIG_INIT_ARGS pTHXo_ CV* cv\n"
"#define SWIG_INIT_CALL_ARGS cv\n"
"#endif\n"
"#else\n"
"#define SWIG_INIT_ARGS CV *cv, CPerlObj *\n"
"#define SWIG_INIT_CALL_ARGS cv, 0\n"
"#endif\n"
"#define DECLARE_MODULE_INIT(name, Capitalname) \\\n"
"extern \"C\" int boot_##name( SWIG_INIT_ARGS );\n"
"\n"
"#define CALL_MODULE_INIT(name, Capitalname) \\\n"
"boot_##name( SWIG_INIT_CALL_ARGS );\n"
"#endif\n"
"\n"
"#ifdef SWIG_TCL_INT\n"
"#define DECLARE_MODULE_INIT(name, Capitalname) \\\n"
"extern \"C\" int Capitalname##_Init( Tcl_Interp* );\n"
"#define CALL_MODULE_INIT(name, Capitalname) \\\n"
"Capitalname##_Init( interp );\n"
"#endif\n"
"\n"
"#ifdef SWIGPYTHON\n"
"#define DECLARE_MODULE_INIT(name, Capitalname) \\\n"
"extern \"C\" int init_##name( );\n"
"#define CALL_MODULE_INIT(name, Capitalname) \\\n"
"init_##name( );\n"
"#endif\n"
"\n";

void CableSwig::AddClassToBeImported(cable::Class* c)
{
  m_ClassesToBeImported.insert(c);
}


// fill the following ivars:
// m_TypedefLookup -  map from class to typedef for included classes
// m_ClassesToBeIncluded  - set of classes to be included
// m_ClassesToBeImported  - set of classes to be imported
void CableSwig::DetermineClassesToWrap(const Namespace* cns)
{
  const Namespace* wns = 0;
  Context::Iterator lower = cns->LowerBound("wrappers");
  Context::Iterator upper = cns->UpperBound("wrappers"); 
  if(lower != upper)
    {
    wns = Namespace::SafeDownCast(*lower);
    if(!wns)
      {
      return;
      }
    }
  if(!wns)
    {
    return;
    } 
  for(Context::Iterator w = wns->Begin(); w != wns->End(); ++w)
    {
    Typedef* td = Typedef::SafeDownCast(*w);
    if(td)
      {
      const ClassType* ct = ClassType::SafeDownCast(td->GetType());
      if(ct)
        {
        const Class* c = ct->GetClass();
        // add this class and its typedef to the 
        // list of classes that will be included
        m_TypedefLookup.insert(std::pair<const Class*, cable::Typedef*>(c, td));
        m_ClassesToBeIncluded.insert(c);
        
        std::vector<Class*> parents;
        c->GetAllBaseClasses(parents);  
        // add all the parent classes of c to map of imported classes
        for( std::vector<Class*>::iterator i = parents.begin();
             i != parents.end(); ++i)
          {
          this->AddClassToBeImported(*i);
          }   
        // now look for return types
        for(Class::Iterator i = c->Begin(); i != c->End(); ++i)
          {
          Method* mth = Method::SafeDownCast(*i);
          if(mth)
            {
            FunctionType* ft = mth->GetFunctionType();
            const ClassType* rt = ClassType::SafeDownCast(ft->GetReturns());
            if(rt)
              {
              // add the parents of any return type into the list as well
              const Class* c = rt->GetClass();
              std::vector<Class*> parents;
              c->GetAllBaseClasses(parents);  
              for( std::vector<Class*>::iterator pi = parents.begin();
                   pi != parents.end(); ++pi)
                {
                this->AddClassToBeImported(*pi);
                }
              // add the class into the list
              this->AddClassToBeImported(rt->GetClass());
              }
            }
          }
        }
      }
    }
  // Now remove all the classes that will be included
  // from the list of imported classes
  for(std::set<const cable::Class*>::iterator i = m_ClassesToBeIncluded.begin();
      i != m_ClassesToBeIncluded.end(); ++i)
    {
    std::cerr << "include class " << (*i)->GetName() << "\n";
    m_ClassesToBeImported.erase(*i);
    }
  for(std::set<const cable::Class*>::iterator i = m_ClassesToBeImported.begin();
      i != m_ClassesToBeImported.end(); ++i)
    {
    std::cerr << "import class " << (*i)->GetName() << "\n";
    }
}


void CableSwig::ProcessImportedClasses(Node* top, Node* module)
{
  for(std::set<const cable::Class*>::iterator i = m_ClassesToBeImported.begin();
      i != m_ClassesToBeImported.end(); ++i)
    {
    const cable::Class* c = *i;
    std::cout << "processing class to be imported " << c->GetQualifiedName() << "\n";
    std::vector<Class*> parents;
    c->GetAllBaseClasses(parents);
    for(int j = parents.size()-1; j >=0; --j)
      {
      const cable::Class* p = parents[j];
      if(m_ClassesToBeIncluded.find(p) != m_ClassesToBeIncluded.end())
        {
        std::cout << "parent class of imported class is include " << p->GetQualifiedName() 
                  << "\n";
        std::map<const cable::Class*, cable::Typedef*>::iterator tdi = m_TypedefLookup.find(p);
        cable::Typedef* td = 0;
        if(tdi != m_TypedefLookup.end())
          {
          td = tdi->second; 
          this->CreateSwigClassInNamespace(p, td->GetName(), top, "include", module);
          }
        else
          {
          Swig_error(c->GetFile(), c->GetLine(), "no typedef found in map");
          std::cerr << "class skipped " << c->GetName() << "\n";
          }
        }
      else
        {
        std::cout << "import parent class\n";
        std::string group;
        if(this->FindClassGroup(p->GetQualifiedName().c_str(), &group))
          {
          std::cout << "found a group, so do real import " << p->GetQualifiedName() << "\n";
          Node* importModule = new_node("module");
          Setattr(importModule, "name", group.c_str());
          std::string cname;
          this->FindImportTypedef(p->GetQualifiedName().c_str(), &cname);
          this->CreateSwigClassInNamespace(p, cname.c_str(), top, "import", importModule);
          Delete(importModule);
          }
        else
          {
          std::cout << "no group, so do an ingnored include " << p->GetQualifiedName() << "\n";
          Node* includeModule = new_node("include");
          Setattr(includeModule, "name", p->GetFile());
          Node* sc = 
            this->CreateSwigClassInNamespace(p, p->GetName(), top, "include", includeModule);
          std::cout << "ignore class because we do not know what module it is in " << 
            p->GetQualifiedName() << "\n";
          if(sc)
            {
            Setattr(sc, "feature:ignore", "1");
            }
          Delete(includeModule);
          } 
        }
      }
    std::string group;
    if(!this->FindClassGroup(c->GetQualifiedName().c_str(), &group))
      {
      Node* includeModule = new_node("include");
      Setattr(includeModule, "name", c->GetFile());
      Node* sc = 
        this->CreateSwigClassInNamespace(c, c->GetName(), top, "include", includeModule);
      std::cout << "ignore class because we do not know what module it is in " << 
        c->GetQualifiedName() << "\n";
      if(sc)
        {
        Setattr(sc, "feature:ignore", "1");
        }
      Delete(includeModule);
      }
    else
      {
      std::cout << "import class from module " << c->GetQualifiedName() << "\n";
      Node* importModule = new_node("module");
      Setattr(importModule, "name", group.c_str());
      std::string cname;
      this->FindImportTypedef(c->GetQualifiedName().c_str(), &cname);
      std::cout << "Typedef Group name is " << cname << "\n";
      this->CreateSwigClassInNamespace(c, cname.c_str(), top, "import",importModule);
      Delete(importModule);
      }
    }
}


  
  
  
void CableSwig::ProcessIncludedClasses(Node* top, Node* module)
{
  for(std::set<const cable::Class*>::iterator i = m_ClassesToBeIncluded.begin();
      i != m_ClassesToBeIncluded.end(); ++i)
    {
    const cable::Class* c = *i;
    // include or import all parents in reverse order
    std::vector<Class*> parents;
    c->GetAllBaseClasses(parents);
    for(int j = parents.size()-1; j >=0; --j)
      {
      const cable::Class* p = parents[j];
      if(m_ClassesToBeIncluded.find(p) != m_ClassesToBeIncluded.end())
        { 
        std::map<const cable::Class*, cable::Typedef*>::iterator tdi = m_TypedefLookup.find(p);
        cable::Typedef* td = 0;
        if(tdi != m_TypedefLookup.end())
          {
          td = tdi->second;
          this->CreateSwigClassInNamespace(p, td->GetName(), top, "include", module);
          }
        else
          {
          Swig_error(c->GetFile(), c->GetLine(), "no typedef found in map");
          std::cerr << "class skipped " << c->GetName() << "\n";
          }
        }
      else
        {  
        std::string group;
        if(!this->FindClassGroup(p->GetQualifiedName().c_str(), &group))
          {
          Node* importModule = new_node("include");
          Setattr(importModule, "name", p->GetFile());
          Node* sc = 
            this->CreateSwigClassInNamespace(p, p->GetName(), top, "include", importModule);
          if(sc)
            {
            std::cout << "something odd... no class created by CreateSwigClassInNamespace\n";
            std::cout << p->GetName() << "\n";
            Setattr(sc, "feature:ignore", "1");
            }
          
          std::cout << "ignore class because we do not know what module it is in " << 
            p->GetQualifiedName() << "\n";
          Delete(importModule);
          }
        else
          {
          Node* importModule = new_node("module");
          Setattr(importModule, "name", group.c_str());
          std::string cname;
          this->FindImportTypedef(p->GetQualifiedName().c_str(), &cname);
          this->CreateSwigClassInNamespace(p, cname.c_str(), top, "import", importModule);
          Delete(importModule);
          }
        } 
      }
    std::map<const cable::Class*, cable::Typedef*>::iterator tdi = m_TypedefLookup.find(c);
    cable::Typedef* td = 0;
    if(tdi != m_TypedefLookup.end())
      {
      td = tdi->second;
      this->CreateSwigClassInNamespace(c, td->GetName(), top, "include", module);
      }
    else
      {
      Swig_error(c->GetFile(), c->GetLine(), "no typedef found in map");
      std::cerr << "class skipped " << c->GetName() << "\n";
      }
    }
}



bool CableSwig::ProcessSource(SourceRepresentation::Pointer sr, Node* top)
{ 
  const Namespace* gns = sr->GetGlobalNamespace();
  // Find the cable configuration namespace.
  Context::Iterator lower = gns->LowerBound("_cable_");
  Context::Iterator upper = gns->UpperBound("_cable_");
  Namespace* cns = 0;
  Namespace* wns = 0;
  if(lower != upper)
    {
    cns = Namespace::SafeDownCast(*lower);
    if(!cns)
      {
      return false;
      }
    }
  if(!cns)
    {
    return false;
    }
  std::string group;
  std::vector<std::string> groups;
  std::string package;
  std::string packageVersion;
  std::string configFile;
  this->GetGroupAndPackageInformation(cns, group, groups, package, packageVersion, configFile);
  if(group.size() == 0)
    {
    if(package.size() == 0)
      {
      std::cerr << "No group or package name specified. ";
      return false;
      }
    }
      // Include the cable configuration file
  if(configFile.size())
    {
    Node* n = new_node("insert");
    std::string headerCode = "\n#include <";
    headerCode += configFile;
    headerCode += ">\n";
    Setattr(n, "code", headerCode.c_str());
    Setattr(n, "section", "header");
    appendChild(top, n);
    }
  

  // Create a module node that has the group name
  Node* module = new_node("module");
  if(group.size() == 0)
    {
    Setattr(module, "name", package.c_str());
    }
  else
    {
    Setattr(module, "name", group.c_str());
    }
  bool pythonWrap = false;
  if(m_WrapLanguage == "python")
    {
    pythonWrap = true;
    }
  
  // Fix the case of the package name for Tcl package loader.
  if(package != group && groups.size() > 0)
    {
    std::cout << "create swig package "<< package << "\n";
    // start off with the definitions for DECLARE_MODULE_INIT and
    // CALL_MODULE_INIT
    std::string headerCode = SwigInitMacrosString;
    std::string initCode;
    std::string pythonCode;
    
    for(std::vector<std::string>::iterator i = groups.begin();
        i != groups.end(); ++i)
      {
      if(pythonWrap)
        {
        pythonCode += "from ";
        pythonCode += *i;
        pythonCode +=  " import *\n";
        }
      std::cout << "  init module: " << i->c_str() << "\n";
      std::string tname = GetTclName(i->c_str());
      headerCode += "\nDECLARE_MODULE_INIT(";
      headerCode += *i;
      headerCode += ", ";
      headerCode += tname;
      headerCode += ") \n";
      
      initCode += "\nCALL_MODULE_INIT(";
      initCode += *i;
      initCode += ", ";
      initCode += tname;
      initCode += ") \n";
      }
    if(pythonWrap)
      {
      Node* python = new_node("insert");
      Setattr(python, "code", pythonCode.c_str());
      Setattr(python, "section", "python");
      appendChild(top, python);
      }

    Node* header = new_node("insert");
    Setattr(header, "code", headerCode.c_str());
    Setattr(header, "section", "header");
    
    Node* init = new_node("insert");
    Setattr(init, "code", initCode.c_str());
    Setattr(init, "section", "init");
    appendChild(top, header);
    appendChild(top, init);
    }
  
  // collect up all classes to be included or imported
  this->DetermineClassesToWrap(cns);
  // first process imported classes 
  Node* importModule = new_node("module");
  Setattr(importModule, "name", "cableImport");
  this->ProcessImportedClasses(top, importModule);
  // next process included classes
  this->ProcessIncludedClasses(top, module);
  
  Setattr(top, "name", Getattr(module, "name"));
  Setattr(top, "module", module);

  //  printf("********************cable swig file******************\n");
  //  Swig_print_tree(top);
  //  printf("********************end cable swig file******************\n");

  return true;
}


void CableSwig::SetIncludeDirectories( char** includefiles, int includecount)
{
  m_IncludeFlags = "";
  for (int i = 0; i < includecount; i++) 
    {
    m_IncludeFlags += " -I";
    m_IncludeFlags += includefiles[i];
    }
  
}

// This is what is called by swig
int CableSwig::ParseFile(const char* input_file, Node* top, 
                         const char* wrapLang)
{
  m_WrapLanguage = wrapLang;
  bool verbose = false;
  bool keepXML = true;
  const char* inFileName = input_file;
  const char* tclOutName = 0;
  const char* xmlOutName = 0;
  const char* srcOutName = 0;
  const char* cableGCCXML = 0;
  std::string gccxmlOptions = m_IncludeFlags;

  if(!this->ReadMasterIndexFile())
    {
      return 1;
    }
   
  if(!inFileName)
    {
    std::cerr << "Must specify input file name (use --help for options).\n";
    return 1;
    }
  
  if(verbose)
    {
    std::cout << "Using input file \"" << inFileName << "\"\n";
    }
  // Open the XML input file produced by GCC-XML.
  std::ifstream inFile(inFileName);
  if(!inFile)
    {
    std::cerr << "Error opening GCC-XML output file: \""
              << inFileName << "\"\n";
    return 1;
    }
  
  // Parse the XML input file.
  XMLSourceParser::Pointer parser = XMLSourceParser::New();  
  parser->SetStream(&inFile);
  if(!parser->Parse())
    {
    std::cerr << "Errors occurred during parsing of GCC-XML output: "
              << inFileName << "\n";
    return 1;
    }
  parser->SetStream(0);
  inFile.close();

  // Get the parsed source representation.
  SourceRepresentation::Pointer sr = parser->GetSourceRepresentation();
  if(!sr)
    {
    std::cerr << "Error getting SourceRepresentation from parser.\n";    
    return 1;
    }     

  ProcessSource(sr, top);
  return 0;
}

//----------------------------------------------------------------------------
std::string CableSwig::findSelfPath(const char* argv0)
{
  // Find our own executable's location.
  cable::String av0 = argv0;
  SystemTools::ConvertToUnixSlashes(av0);
  std::string::size_type pos = av0.find_last_of("/");
  if(pos == std::string::npos)
    {
    av0 = SystemTools::FindProgram(argv0);
    pos = av0.find_last_of("/");
    }
  cable::String selfPath;
  if(pos != std::string::npos)
    {
    selfPath = SystemTools::CollapseDirectory(av0.substr(0, pos).c_str());
    }
  else
    {
    selfPath = SystemTools::CollapseDirectory(".");
    }
  SystemTools::ConvertToUnixSlashes(selfPath);
  return selfPath;
}

//----------------------------------------------------------------------------
std::string CableSwig::findGCC_XML(const char* argv0, const char* cableGCCXML)
{
  cable::String gccxmlExecutable;
  // Find the GCC-XML executable's location.
  if(cableGCCXML)
    {
    // It was given on the command line.
    gccxmlExecutable = cableGCCXML;
    SystemTools::ConvertToUnixSlashes(gccxmlExecutable);
    return gccxmlExecutable;
    }  
  else if(SystemTools::GetEnv("CABLE_GCCXML", gccxmlExecutable))
    {
    // There is an environment variable.
    SystemTools::ConvertToUnixSlashes(gccxmlExecutable);
    return gccxmlExecutable;
    }
  else
    {
    // Try to find it next to the cable executable.
    std::string selfPath = findSelfPath(".");
    gccxmlExecutable = selfPath+"gccxml"+SystemTools::GetExecutableExtension();
    SystemTools::ConvertToUnixSlashes(gccxmlExecutable);
    
    if(SystemTools::FileExists(gccxmlExecutable.c_str()))
      {
      return gccxmlExecutable;
      }
    }
  // Try to find a registry entry for GCC-XML.
  const char* gccxmlRegistry = "HKEY_CURRENT_USER\\Software\\Kitware\\GCC_XML;loc";
  if(SystemTools::ReadRegistryValue(gccxmlRegistry, gccxmlExecutable))
    {
    SystemTools::ConvertToUnixSlashes(gccxmlExecutable);
    gccxmlExecutable += "/gccxml";
    return gccxmlExecutable;
    }
  
  // Just hope it is in the path.
  return "gccxml";
}

//----------------------------------------------------------------------------
std::string CableSwig::findXMLFileName(const char* inFileName)
{
  cable::String ifn = inFileName;
  SystemTools::ConvertToUnixSlashes(ifn);
  std::string::size_type pos = ifn.rfind('/');
  std::string file;
  if(pos != std::string::npos)
    {
    file = ifn.substr(pos+1);
    }
  else
    {
    file = ifn;
    }
  file += ".xml";
  return file;
}


Node *CableSwig::new_node(const String_or_char *tag) 
{
  Node *n = NewHash();
  set_nodeType(n,tag);
  if(m_CurrentFile)
    {
    Setfile(n,(char*)m_CurrentFile);
    Setline(n,m_CurrentLine);
    }
  return n;
}

void CableSwig::AddMasterIndexFile(const char* s)
{
  if(s)
    {
    m_MasterIndexFiles.push_back(s);
    }
}

bool CableSwig::FindClassGroup(const char* className, std::string* group)
{
  std::map<csString, csString>::iterator i = m_ClassGroupLookup.find(className);
  if(i != m_ClassGroupLookup.end())
    {
    if(group)
      {
      *group =  (*i).second;
      }
    return true;
    }
  return false;
}




bool CableSwig::FindImportTypedef(const char* className, std::string* typeName)
{ 
  std::map<csString, csString>::iterator i = m_ImportTypedefLookup.find(className);
  if(i != m_ImportTypedefLookup.end())
    {
    if(typeName)
      {
      *typeName =  (*i).second;
      }
    return true;
    }
  if(typeName)
    {
    *typeName = className;
    }
  return false;
}


void CableSwig::ParseIndexInformation(const char* s)
{
  std::string packageInfo = s;
  std::string::size_type p1 = packageInfo.find("{");
  if(p1 == packageInfo.npos)
    {
    return;
    }
  std::string::size_type p2 = packageInfo.find("}");
  std::string qualifiedName = packageInfo.substr(p1+1, p2 - p1-1);
  p1 = packageInfo.find("{", p2);
  p2 = packageInfo.find("}", p1);
  std::string typeDef = packageInfo.substr(p1+1, p2 - p1-1);
  p1 = packageInfo.find("{", p2);
  p2 = packageInfo.find("}", p1);
  std::string package = packageInfo.substr(p1+1, p2 - p1-1);
  m_ClassGroupLookup[qualifiedName] = package;
  m_ImportTypedefLookup[qualifiedName] = typeDef;
}

bool CableSwig::ReadMasterIndexFile()
{
  for(std::vector<std::string>::iterator i = m_MasterIndexFiles.begin();
      i != m_MasterIndexFiles.end(); ++i)
    {
    std::ifstream fin(i->c_str());
    if(!fin)
      {
	std::cerr << "error can not open " << *i << "\n";
	return false;
      }
    char buffer[5000];
    while(fin)
      {
      fin.getline(buffer, 5000);
      if(strlen(buffer) == 0)
	{
	  continue;
	}
      std::ifstream idxIn(buffer);
      if(!idxIn)
	{
	  std::cerr << "error can not open " << buffer << '\n';
	  return false;
	}
      while(idxIn)
        {
        idxIn.getline(buffer, 5000);
        this->ParseIndexInformation(buffer);
        }
      }
    }
  return true;
}


