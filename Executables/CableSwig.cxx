



#include "swigmod.h"
#ifndef MACSWIG
#include "swigconfig.h"
#endif
extern "C" {
#include "preprocessor.h"
}
#include <ctype.h>


static Node *new_node(const String_or_char *tag) {
  Node *n = NewHash();
  set_nodeType(n,tag);
//  Setfile(n,cparse_file);
//  Setline(n,cparse_line);
  return n;
}

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
using namespace cable;

void ReplaceString(std::string& source,
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


cable::String findSelfPath(const char* argv0);
cable::String findGCC_XML(const char* argv0, const char* cableGCCXML);
cable::String findXMLFileName(const char* argv0);

cable::String GetClassName(const Class* c, const Typedef* td)
{
  cable::String name = c->GetName();
  if(name.find('<') == name.npos)
    {
    return name;
    }
  return std::string(td->GetName());
}

cable::String TemplateName(const char* s)
{
  cable::String name = s;
  ReplaceString(name, "<", "<(");
  ReplaceString(name, ">", ")>");
  return name;
}

cable::String ToSwigType(cxx::CvQualifiedType const&  t)
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
      cable::String n = "p.";
      n += ToSwigType(p->GetPointedToType());
      return n;
      }
      break;
    case cxx::PointerToMemberType_id:
      break;
    case cxx::ReferenceType_id:
      {
      cxx::ReferenceType const* r 
        = cxx::ReferenceType::SafeDownCast(t.GetType());
      cable::String n = "r.";
      n += ToSwigType(r->GetReferencedType());
      return n;
      break;
      }
    
    case cxx::FundamentalType_id:
      break;
    case cxx::FunctionType_id: 
      {
    
      cxx::FunctionType const* f 
        = cxx::FunctionType::SafeDownCast(t.GetType());
      cable::String n = "f(";
      cxx::CvQualifiedTypes const& args = f->GetArgumentTypes();
      cxx::CvQualifiedTypes::const_iterator i = args.begin();
      if(i != args.end())
        {
        n += ToSwigType(*i);
        for(++i; i != args.end(); ++i)
          {
          n += ",";
          n += ToSwigType(*i);
          }
        }
      n += ").";
      n += ToSwigType(f->GetReturnType());
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
  ret += TemplateName(t.GetType()->Name().c_str());
  return ret;
}


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
void CreateSwigMethod(Method* mth, Node* sc, cable::String& cname)
{
  Function::FunctionIdType fid = mth->GetFunctionId();
  FunctionType* ft = mth->GetFunctionType();
  const char* ftype = "cdecl";
  Node* m;
  if(fid == Function::ConstructorId)
    {
    m = new_node("constructor");
    // for a constructor use the name of the class as the function name
    Setattr(m, "sym:name",  cname.c_str());
    Setattr(m, "name", Getattr(sc, "name"));
    }
  else
    {
    m = new_node("cdecl");
    Setattr(m, "sym:name", mth->GetCallName().c_str());
    Setattr(m, "name", mth->GetCallName().c_str());
    if(mth->GetVirtual())
      {
      Setattr(m, "storage", "virtual");
      if(mth->GetPureVirtual())
        {
        Setattr(m, "abstract", "1");
        }
      }
    }
  ParmList* parms = 0;
  Parm* pp = 0;
  std::string allParams;
  for(unsigned int arg = 0; arg < ft->GetNumberOfArguments(); ++arg)
    {    
    std::string swigArg = ToSwigType(ft->GetArgument(arg)->GetCxxType());
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
  cable::String decl;
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
  if(fid !=  Function::ConstructorId)
    {
    // set the return type
    Setattr(m, "type", 
            ToSwigType(ft->GetReturns()->GetCxxType()).c_str());
    }
  else
    {
    Setattr(m, "feature:new", "1");
    }  
  // add the method to the class
  appendChild(sc, m);
  // add the member name to the class
  if(fid == Function::ConstructorId)
    {
    Swig_symbol_add(Getattr(sc, "sym:name"), m);
    }
  else
    {
    Swig_symbol_add((char*)mth->GetCallName().c_str(), m);
    }
}

Node* CreateSwigClass(const Class* c, const Typedef* td)
{
  cable::String cname = GetClassName(c, td);
  Node* sc = new_node("class");
  Setattr(sc, "sym:name", cname.c_str());
  Setattr(sc, "name", TemplateName(c->GetName()).c_str());
  Setattr(sc,"kind","class");
  Setattr(sc,"allows_typedef","1");
  // Create symbol table information
  // create a new scope 
  Node* sym = Swig_symbol_newscope();
  Swig_symbol_setscopename("Bill");
  // set the symtab on the class
  Setattr(sc,"symtab",Swig_symbol_popscope());
  // set the name of the scope the the class name
  // add the class name to class
  Swig_symbol_add((char*)cname.c_str(), sc);
  // set the scope to the scope of the class
  Swig_symbol_setscope(sym);
  
  // first private methods
  Node* p = new_node("access");
  Setattr(p,"kind","private");
  // add the public declaration to the class
  appendChild(sc, p);
  for(Class::Iterator i = c->Begin(); i != c->End(); ++i)
    {
    Method* mth = Method::SafeDownCast(*i);
    if(mth && (i.GetAccess() == Class::Private))
      {
      Function::FunctionIdType fid = mth->GetFunctionId();
      FunctionType* ft = mth->GetFunctionType();
      if((fid == Function::ConstructorId) || 
         (fid == Function::MethodId) || 
         (fid == Function::OperatorMethodId))
        {
        CreateSwigMethod(mth, sc, cname);
        }
      }
    }
  
  // protected methods
  p = new_node("access");
  Setattr(p,"kind","protected");
  // add the public declaration to the class
  appendChild(sc, p);
  for(Class::Iterator i = c->Begin(); i != c->End(); ++i)
    {
    Method* mth = Method::SafeDownCast(*i);
    if(mth && (i.GetAccess() == Class::Protected))
      {
      Function::FunctionIdType fid = mth->GetFunctionId();
      FunctionType* ft = mth->GetFunctionType();
      if((fid == Function::ConstructorId) || 
         (fid == Function::MethodId) || 
         (fid == Function::OperatorMethodId))
        {
        CreateSwigMethod(mth, sc, cname);
        }
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
      Function::FunctionIdType fid = mth->GetFunctionId();
      FunctionType* ft = mth->GetFunctionType();
      if((fid == Function::ConstructorId) || 
         (fid == Function::MethodId) || 
         (fid == Function::OperatorMethodId))
        {
        CreateSwigMethod(mth, sc, cname);
        }
      }
    }
      
  Setattr(sc,"abstract", pure_abstract(firstChild(sc)));
  return sc;
}

bool ProcessSource(SourceRepresentation::Pointer sr, Node* top)
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
  lower = cns->LowerBound("wrappers");
  upper = cns->UpperBound("wrappers");  
  if(lower != upper)
    {
    wns = Namespace::SafeDownCast(*lower);
    if(!wns)
      {
      return false;
      }
    }
  if(!wns)
    {
    return false;
    }
  
  
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
        Node* sc = CreateSwigClass(c, td);
        Node* n = new_node("include");
        Setattr(n,"name", c->GetFile());
        // add the class to the include file
        appendChild(n, sc); 
        // add the include directive to the tree
        Node* in = new_node("insert");
        std::string code = "\n#include \"" + std::string(c->GetFile()) + "\"\n";
        Setattr(in, "code", code.c_str());
        appendChild(n, in);
        
        // Hack to use the itk namespace *****
        in = new_node("insert");
        code = "\nusing namespace itk;\n";
        Setattr(in, "code", code.c_str());
        appendChild(n, in);

        // add the include file to the parse tree
        appendChild(top, n); 
        }
      
      else
        {
        return false;
        }
      }
    }
  printf("********************cable swig file******************\n");
  Swig_print_tree(top);
  printf("********************end cable swig file******************\n");

  return true;
}


/** Program entry point.  */
int cable_parse_file(const char* input_file, Node* top, char** includefiles, int includecount)
{
  bool verbose = true;
  bool keepXML = true;
  const char* inFileName = input_file;
  const char* tclOutName = 0;
  const char* xmlOutName = 0;
  const char* srcOutName = 0;
  const char* cableGCCXML = 0;
  cable::String gccxmlOptions;
   for (int i = 0; i < includecount; i++) 
     {
     gccxmlOptions += " -I";
     gccxmlOptions += includefiles[i];
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
  
  // Find the GCC-XML executable name.
  cable::String gccxmlExecutable = findGCC_XML(".", cableGCCXML);
  
  // Get the GCC-XML output file name.
  cable::String xmlFileName = findXMLFileName(inFileName);
  
  // Build the GCC-XML command.
  cable::String gccxmlCommand =
    gccxmlExecutable+" "+inFileName+" -fxml=\""+xmlFileName+
    "\" -fxml-start=_cable_ -DCABLE_CONFIGURATION"+gccxmlOptions;
  
  if(verbose)
    {
    std::cout << "Running GCC-XML with command:\n"
              << gccxmlCommand.c_str() << "\n";
    }
  
  // Run the GCC-XML command.
  cable::String output;
  int ret;
  if(!SystemTools::RunCommand(gccxmlCommand.c_str(), output, ret) || ret)
    {
    std::cerr << "Error running GCC-XML with command:\n"
              << gccxmlCommand.c_str() << "\n";
    return 1;
    }
  
  // Open the XML input file produced by GCC-XML.
  std::ifstream inFile(xmlFileName.c_str());
  if(!inFile)
    {
    std::cerr << "Error opening GCC-XML output file: \""
              << xmlFileName.c_str() << "\"\n";
    return 1;
    }
  
  // Parse the XML input file.
  XMLSourceParser::Pointer parser = XMLSourceParser::New();  
  parser->SetStream(&inFile);
  if(!parser->Parse())
    {
    std::cerr << "Errors occurred during parsing of GCC-XML output: "
              << xmlFileName.c_str() << "\n";
    return 1;
    }
  parser->SetStream(0);
  inFile.close();
  
  if(!keepXML)
    {
    // Delete the XML file.
    SystemTools::RemoveFile(xmlFileName.c_str());
    }
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
cable::String findSelfPath(const char* argv0)
{
  // Find our own executable's location.
  cable::String av0 = argv0;
  SystemTools::ConvertToUnixSlashes(av0);
  cable::String::size_type pos = av0.find_last_of("/");
  if(pos == std::string::npos)
    {
    av0 = SystemTools::FindProgram(argv0);
    pos = av0.find_last_of("/");
    }
  cable::String selfPath;
  if(pos != cable::String::npos)
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
cable::String findGCC_XML(const char* argv0, const char* cableGCCXML)
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
    cable::String selfPath = findSelfPath(".");
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
cable::String findXMLFileName(const char* inFileName)
{
  cable::String ifn = inFileName;
  SystemTools::ConvertToUnixSlashes(ifn);
  cable::String::size_type pos = ifn.rfind('/');
  cable::String file;
  if(pos != cable::String::npos)
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


