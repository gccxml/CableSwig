#ifndef CableSwig_h
#define CableSwig_h


#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning ( disable : 4786 )
#endif

// include swig stuff
#ifndef MACSWIG
#include "swigconfig.h"
#endif

#include "swigwarn.h"

#include "cparse.h"
extern "C" {
#include "preprocessor.h"
}
#undef Iterator
#include <string>
#include "cableClass.h"
#include "cxxCvQualifiedType.h"
#include "cxxPointerType.h"
#include "cxxReferenceType.h"
#include "cxxFunctionType.h"
#include "cableTypedef.h"
#include "cableClass.h"
#include "cableMethod.h"
#include "cableNamespace.h"
#include "cableSourceRepresentation.h"
#include "cableXMLSourceParser.h"
#include <set>
#include <map>


// use this class to shrink the size of symbols in .o files
// std::string is really basic_string<....lots of stuff....>
// when combined with a map or set, the symbols can be > 2000 chars!
struct csString : public std::string
{
  typedef std::string StdString;
  typedef StdString::value_type             value_type;
  typedef StdString::pointer                pointer;
  typedef StdString::reference              reference;
  typedef StdString::const_reference        const_reference;
  typedef StdString::size_type              size_type;
  typedef StdString::difference_type        difference_type;
  typedef StdString::iterator               iterator;
  typedef StdString::const_iterator         const_iterator;
  typedef StdString::reverse_iterator       reverse_iterator;
  typedef StdString::const_reverse_iterator const_reverse_iterator;
  
  csString(): StdString() {}
  csString(const value_type* s): StdString(s) {}
  csString(const value_type* s, size_type n): StdString(s, n) {}
  csString(const StdString& s, size_type pos=0, size_type n=npos):
    StdString(s, pos, n) {}
};


class CableSwig
{
public:
  CableSwig() 
    {
      m_CurrentLine = 0;
      m_CurrentFile = 0;
    }
  void SetDependOutput(const char* s)
    {
      m_DependFile = s;
    }
  int ParseFile(const char* input_file, Node* top, const char* wrapLang);
  void AddMasterIndexFile(const char* m);
private:
  bool DumpCMakeDependInformation(cable::XMLSourceParser::Pointer sr, const char* f);
  void ParseIndexInformation(const char* s);
  void ReduceCommaSpace(std::string& source);
  void ReplaceString(std::string& source,
                     const char* replace,
                     const char* with);
  std::string GetClassName(const cable::Class* c, const char* typeDefName);
  std::string TemplateName(const char* s);
  std::string ToSwigType(cxx::CvQualifiedType const&  t);
  void CreateSwigMethod(cable::Method* mth, Node* sc, std::string& cname, const char* access);
  Node* CreateSwigClass(const cable::Class* c, const char* typeDefName);
  Node* CreateSwigClassInNamespace(const cable::Class* c, const char* typeDefName,
                                   Node* top, const char* swigtype, Node* module);
  bool ParseName(const char* name, std::string& result);
  bool GetGroupAndPackageInformation(cable::Namespace* cns,
                                     std::string& groupName,
                                     std::vector<std::string>& groupsNames,
                                     std::string& packageName,
                                     std::string& packageVersion,
                                     std::string& configFile);
  std::string GetTclName(const char* s);
  bool ProcessSource(cable::SourceRepresentation::Pointer sr, Node* top);
  Node* new_node(const String_or_char *tag);
  
  void DetermineClassesToWrap(const cable::Namespace* cns);
  void ProcessImportedClasses(Node* top, Node* module);
  void ProcessIncludedClasses(Node* top, Node* module);
  bool FindClassGroup(const char* className, std::string* group);
  bool FindImportTypedef(const char* className, std::string* typeName);
  bool ReadMasterIndexFile();
  void AddClassToBeImported(cable::Class const*);
  void AddImportReturnTypesAndArguments(cable::Class const* );
  void FindWrappedBases(List* bases, const cable::Class*, bool skiped = false, const cable::Class* original = 0 );
  void AddImportClass( const cable::Type* rt);
private:
  std::string m_DependFile;
  std::string m_WrapLanguage;
  const char* m_CurrentFile;
  int         m_CurrentLine;
  std::string m_NativeStringName;
  std::set<std::string> m_NamespacesToUse;
  std::vector<std::string> m_MasterIndexFiles;
  std::vector<std::string> m_Loaders;
  std::map<csString, csString> m_ClassGroupLookup;   // GetQualifiedName to cable group
  std::map<csString, csString> m_ImportTypedefLookup;  // GetQualifiedName to typedef
  std::map<const cable::Class*, cable::Typedef*> m_TypedefLookup;
  std::set<const cable::Class*> m_ClassesToBeIncluded;  // set of classes that will be wrapped
  std::set<const cable::Class*> m_ClassesToBeImported;  // set of classes that will be imported 
  std::set<const cable::Class*> m_ClassesIncludedOrImported; // set of processed classes
  
};

#endif
