#include <string>
#include "cableSystemTools.h"
#include "cableSourceRepresentation.h"
#include <fstream>
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
bool ParseName(const char* name, std::string& result)
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

bool GetGroupAndPackageInformation(Namespace* cns,
                                   std::string& groupName,
                                   std::vector<std::string>& groupsNames,
                                   std::string& packageName,
                                   std::string& packageVersion)
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

int DetermineClassesToWrap(std::ofstream& fout, const Namespace* cns, const char* group)
{
  const Namespace* wns = 0;
  Context::Iterator lower = cns->LowerBound("wrappers");
  Context::Iterator upper = cns->UpperBound("wrappers"); 
  if(lower != upper)
    {
    wns = Namespace::SafeDownCast(*lower);
    if(!wns)
      {
      return -1;
      }
    }
  if(!wns)
    {
    return -1;
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
        fout
          << "{" << c->GetQualifiedName() << "} {" 
          << td->GetName() << "} {" << group << "}\n";
        }
      }
    }
  return 0;
}

int CreateIndexFile(std::ofstream& fout, SourceRepresentation::Pointer sr)
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
      return -1;
      }
    }
  if(!cns)
    {
    return -1;
    }
  std::string group;
  std::vector<std::string> groups;
  std::string package;
  std::string packageVersion;
  GetGroupAndPackageInformation(cns, group, groups, package, packageVersion);
  if(group.size() == 0)
    {
    if(package.size() == 0)
      {
      std::cerr << "No group or package name specified. ";
      return -1;
      }
    } 
  // collect up all classes to be included or imported
  DetermineClassesToWrap(fout, cns, group.c_str());
  return 0;
}

  
int main(int ac, char* av[])
{
  if(ac < 2)
    {
    std::cerr << "Usage: " << av[0] << " input.xml output.idx";
    return 1;
    }
  std::string outFileName = av[2];
  std::string inFileName = av[1];
  std::ifstream inFile(inFileName.c_str());
  if(!inFile)
    {
    std::cerr << "Error opening xml input file: \""
              << inFileName.c_str() << "\"\n";
    return 1;
    }
  XMLSourceParser::Pointer parser = XMLSourceParser::New();  
  parser->SetStream(&inFile);
  if(!parser->Parse())
    {
    std::cerr << "Errors occurred during parsing of GCC-XML output: "
              << inFileName.c_str() << "\n";
    return 1;
    }
  parser->SetStream(0);
  inFile.close();
  SourceRepresentation::Pointer sr = parser->GetSourceRepresentation();
  if(!sr)
    {
    std::cerr << "Error getting SourceRepresentation from parser.\n";    
    return 1;
    }
  std::ofstream outFile(outFileName.c_str());
  if(!outFile)
    {
    std::cerr << "Error opening output File for write: \""
              << outFileName.c_str() << "\"\n";
    return 1;
    }
  return CreateIndexFile(outFile, sr);
}

  
