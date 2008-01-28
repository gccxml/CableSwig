/*=========================================================================

  Program:   GCC-XML
  Module:    cableSystemTools.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableSystemTools.h"

#include <vector>

#include <ctype.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#if !defined(_WIN32) || defined(__CYGWIN__)
#include <limits.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/wait.h>
#endif

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include <direct.h>
inline const char* Getcwd(char* buf, unsigned int len)
{
  return _getcwd(buf, len);
}
inline int Chdir(const char* dir)
{
  #if defined(__BORLANDC__)
  return chdir(dir);
  #else
  return _chdir(dir);
  #endif
}
#else
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
inline const char* Getcwd(char* buf, unsigned int len)
{
  return getcwd(buf, len);
}
inline int Chdir(const char* dir)
{
  return chdir(dir);
}
#endif

namespace cable
{

#if defined(_WIN32) && !defined(__CYGWIN__)
//----------------------------------------------------------------------------
bool SystemTools::ReadRegistryValue(const char* key, String& result)
{
  // Adapted from CMake's SystemTools.h: ReadAValue
  // Get the data of key value.
  // Example : 
  //      HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\2.1\InstallPath
  //      =>  will return the data of the "default" value of the key
  //      HKEY_LOCAL_MACHINE\SOFTWARE\Scriptics\Tcl\8.4;Root
  //      =>  will return the data of the "Root" value of the key
  
  // find the primary key
  String primary = key;
  String second;
  String valuename;
 
  size_t start = primary.find("\\");
  if(start == String::npos)
    {
    return false;
    }
  size_t valuenamepos = primary.find(";");
  if (valuenamepos != String::npos)
    {
    valuename = primary.substr(valuenamepos+1);
    }
  second = primary.substr(start+1, valuenamepos-start-1);
  primary = primary.substr(0, start);
  
  HKEY primaryKey;
  if(primary == "HKEY_CURRENT_USER")
    {
    primaryKey = HKEY_CURRENT_USER;
    }
  else if(primary == "HKEY_CURRENT_CONFIG")
    {
    primaryKey = HKEY_CURRENT_CONFIG;
    }
  else if(primary == "HKEY_CLASSES_ROOT")
    {
    primaryKey = HKEY_CLASSES_ROOT;
    }
  else if(primary == "HKEY_LOCAL_MACHINE")
    {
    primaryKey = HKEY_LOCAL_MACHINE;
    }
  else if(primary == "HKEY_USERS")
    {
    primaryKey = HKEY_USERS;
    }
  
  HKEY hKey;
  if(RegOpenKeyEx(primaryKey, second.c_str(),
                  0, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
    return false;
    }
  else
    {
    DWORD dwType, dwSize;
    dwSize = 1023;
    char data[1024];
    if(RegQueryValueEx(hKey, (LPTSTR)valuename.c_str(), NULL, &dwType, 
                       (BYTE *)data, &dwSize) == ERROR_SUCCESS)
      {
      if (dwType == REG_SZ)
        {
        result = data;
        return true;
        }
      }
    }
  return false;
}
#else
//----------------------------------------------------------------------------
bool SystemTools::ReadRegistryValue(const char*, String&)
{
  return false;
}  
#endif

//----------------------------------------------------------------------------
bool SystemTools::FileCopy(const char* source, const char* destination)
{
  const int bufferSize = 4096;
  char buffer[bufferSize];
  std::ifstream fin(source,
#ifdef _WIN32
                    std::ios::binary |
#endif
                    std::ios::in);
  if(!fin) { return false; }
  std::ofstream fout(destination,
#ifdef _WIN32
                     std::ios::binary |
#endif
                     std::ios::out | std::ios::trunc);
  if(!fout) { return false; }
  
  // This copy loop is very sensitive on certain platforms with
  // slightly broken stream libraries (like HPUX).  Normally, it is
  // incorrect to not check the error condition on the fin.read()
  // before using the data, but the fin.gcount() will be zero if an
  // error occurred.  Therefore, the loop should be safe everywhere.
  while(fin)
    {
    fin.read(buffer, bufferSize);
    if(fin.gcount())
      {
      fout.write(buffer, fin.gcount());
      }
    }
  
  return true;
}

//----------------------------------------------------------------------------
String SystemTools::LowerCase(const String& s)
{
  String n;
  n.resize(s.size());
  for (size_t i = 0; i < s.size(); i++)
    {
    n[i] = tolower(s[i]);
    }
  return n;
}

//----------------------------------------------------------------------------
const char* SystemTools::GetEnv(const char* key)
{
  return getenv(key);
}

//----------------------------------------------------------------------------
bool SystemTools::GetEnv(const char* key, String& result)
{
  const char* v = getenv(key);
  if(v)
    {
    result = v;
    return true;
    }
  else
    {
    return false;
    }
}

//----------------------------------------------------------------------------
bool SystemTools::FileExists(const char* filename)
{
  struct stat fs;
  if(stat(filename, &fs) != 0) 
    {
    return false;
    }
  else
    {
    return true;
    }
}

//----------------------------------------------------------------------------
bool SystemTools::FileIsDirectory(const char* name)
{  
  struct stat fs;
  if(stat(name, &fs) == 0)
    {
#if _WIN32
    return ((fs.st_mode & _S_IFDIR) != 0);
#else
    return S_ISDIR(fs.st_mode);
#endif
    }
  else
    {
    return false;
    }
}

//----------------------------------------------------------------------------
String SystemTools::CollapseDirectory(const char* in_dir)
{
  String dir = in_dir;
#ifdef _WIN32
  // Ultra-hack warning:
  // This changes to the target directory, saves the working directory,
  // and then changes back to the original working directory.
  String cwd = SystemTools::GetCWD();
  if(dir != "") { Chdir(dir.c_str()); }
  String newDir = SystemTools::GetCWD();
  Chdir(cwd.c_str());
  return newDir;
#else
# ifdef MAXPATHLEN
  char resolved_name[MAXPATHLEN];
# else
#  ifdef PATH_MAX
  char resolved_name[PATH_MAX];
#  else
  char resolved_name[5024];
#  endif
# endif
  if(dir != "")
    {
    realpath(dir.c_str(), resolved_name);
    dir = resolved_name;
    }
  else
    {
    dir = SystemTools::GetCWD();
    }
  return dir;
#endif
}

//----------------------------------------------------------------------------
void SystemTools::ConvertToUnixSlashes(String& path)
{
  String::size_type pos = 0;
  while((pos = path.find('\\', pos)) != String::npos)
    {
    path[pos] = '/';
    pos++;
    }
  // remove any trailing slash
  if(path.size() && path[path.size()-1] == '/')
    {
    path = path.substr(0, path.size()-1);
    }

  // if there is a tilda ~ then replace it with HOME
  if(path.find("~") == 0)
    {
    String home;
    if(SystemTools::GetEnv("HOME", home))
      {
      path = home + path.substr(1);
      }
    }
}

//----------------------------------------------------------------------------
String SystemTools::GetCWD()
{
  char buf[2048];
  String path = Getcwd(buf, 2048);
  return path;
}

//----------------------------------------------------------------------------
bool SystemTools::RunCommand(const char* command,  String& output,
                               int &retVal)
{
  const int BUFFER_SIZE = 4096;
  char buffer[BUFFER_SIZE];
#if defined(WIN32) && !defined(__CYGWIN__)
  String commandToFile = command;
  commandToFile += " > ";
  String tempFile;
  tempFile += _tempnam(0, "gccxml");
  
  commandToFile += tempFile;
  retVal = system(commandToFile.c_str());
  std::ifstream fin(tempFile.c_str());
  if(!fin)
    {
    fin.close();
    SystemTools::RemoveFile(tempFile.c_str());
    return false;
    }
  while(fin)
    {
    fin.getline(buffer, BUFFER_SIZE);
    output += buffer;
    }
  fin.close();
  SystemTools::RemoveFile(tempFile.c_str());
  return (retVal == 0);
#else
  fflush(stdout);
  fflush(stderr);
  FILE* cpipe = popen(command, "r");
  if(!cpipe)
    {
    return false;
    }
  fgets(buffer, BUFFER_SIZE, cpipe);
  while(!feof(cpipe))
    {
    output += buffer;
    fgets(buffer, BUFFER_SIZE, cpipe);
    }

  retVal = pclose(cpipe);
  retVal = WEXITSTATUS(retVal);
  return (retVal == 0);
#endif
}

//----------------------------------------------------------------------------
bool SystemTools::RemoveFile(const char* source)
{
  return (unlink(source) != 0) ? false : true;
}

//----------------------------------------------------------------------------
String SystemTools::ConvertToOutputPath(const char* path)
{
#if defined(_WIN32) && !defined(__CYGWIN__)
  return SystemTools::ConvertToWindowsOutputPath(path);
#else
  return SystemTools::ConvertToUnixOutputPath(path);
#endif
}

//----------------------------------------------------------------------------
String SystemTools::ConvertToWindowsOutputPath(const char* path)
{  
  // remove double slashes not at the start
  String ret = path;
  String::size_type pos = 0;
  // first convert all of the slashes
  while((pos = ret.find('/', pos)) != String::npos)
    {
    ret[pos] = '\\';
    pos++;
    }
  // check for really small paths
  if(ret.size() < 2)
    {
    return ret;
    }
  // now clean up a bit and remove double slashes
  // Only if it is not the first position in the path which is a network
  // path on windows
  pos = 1; // start at position 1
  while((pos = ret.find("\\\\", pos)) != String::npos)
    {
    ret.erase(pos, 1);
    }
  // now double quote the path if it has spaces in it
  // and is not already double quoted
  if(ret.find(" ") != String::npos
     && ret[0] != '\"')
    {
    String result;
    result = "\"" + ret + "\"";
    ret = result;
    }
  return ret;
}

//----------------------------------------------------------------------------
String SystemTools::ConvertToUnixOutputPath(const char* path)
{
  // change // to /, and escape any spaces in the path
  String ret = path;
  
  // remove // except at the beginning might be a cygwin drive
  String::size_type pos = 1;
  while((pos = ret.find("//", pos)) != String::npos)
    {
    ret.erase(pos, 1);
    }
  // now escape spaces if there is a space in the path
  if(ret.find(" ") != String::npos)
    {
    String result = "";
    char lastch = 1;
    for(const char* ch = ret.c_str(); *ch != '\0'; ++ch)
      {
      // if it is already escaped then don't try to escape it again
      if(*ch == ' ' && lastch != '\\')
        {
        result += '\\';
        }
      result += *ch;
      lastch = *ch;
      }
    ret = result;
    }
  return ret;
}

//----------------------------------------------------------------------------
String SystemTools::GetExecutableExtension()
{
#ifdef _WIN32
  return ".exe";
#else
  return "";
#endif
}

//----------------------------------------------------------------------------
void cableSystemToolsGetPath(std::vector<String>& path)
{
  // adds the elements of the env variable path to the arg passed in
#if defined(_WIN32) && !defined(__CYGWIN__)
  const char* pathSep = ";";
#else
  const char* pathSep = ":";
#endif
  String pathEnv;
  if(!SystemTools::GetEnv("PATH", pathEnv)) { return; }
  // A hack to make the below algorithm work.  
  if(pathEnv[pathEnv.length()-1] != pathSep[0])
    {
    pathEnv += pathSep;
    }
  String::size_type start =0;
  bool done = false;
  while(!done)
    {
    String::size_type endpos = pathEnv.find(pathSep, start);
    if(endpos != String::npos)
      {
      path.push_back(pathEnv.substr(start, endpos-start));
      start = endpos+1;
      }
    else
      {
      done = true;
      }
    }
  for(std::vector<String>::iterator i = path.begin();
      i != path.end(); ++i)
    {
    SystemTools::ConvertToUnixSlashes(*i);
    }
}

//----------------------------------------------------------------------------
String SystemTools::FindProgram(const char* name)
{
  // Find the executable with the given name.  Searches the system
  // path.  Returns the full path to the executable if it is found.
  // Otherwise, the empty string is returned.
  
  // See if the executable exists as written.
  if(SystemTools::FileExists(name) && !SystemTools::FileIsDirectory(name))
    {
    return name;
    }
  
  String tryPath = name;
#ifdef _WIN32
  tryPath += ".exe";
  if(SystemTools::FileExists(tryPath.c_str()) &&
     !SystemTools::FileIsDirectory(tryPath.c_str()))
    {
    return tryPath.c_str();
    }
#endif

  // Get the system search path.
  std::vector<String> path;
  cableSystemToolsGetPath(path);
  
  for(std::vector<String>::const_iterator p = path.begin();
      p != path.end(); ++p)
    {
    tryPath = *p;
    tryPath += "/";
    tryPath += name;
    if(SystemTools::FileExists(tryPath.c_str()) &&
      !SystemTools::FileIsDirectory(tryPath.c_str()))
      {
      return tryPath.c_str();
      }
#ifdef _WIN32
    tryPath += ".exe";
    if(SystemTools::FileExists(tryPath.c_str()) &&
       !SystemTools::FileIsDirectory(tryPath.c_str()))
      {
      return tryPath.c_str();
      }
#endif
    }

  // Couldn't find the program.
  return "";
}

/**
 * Copy a file named by "source" to the file named by "destination".
 */
void SystemTools::cmCopyFile(const char* source,
                               const char* destination)
{
  const int bufferSize = 4096;
  char buffer[bufferSize];

  // Open files

#if defined(_WIN32) || defined(__CYGWIN__)
  std::ifstream fin(source, 
                    std::ios::binary | std::ios::in);
#else
  std::ifstream fin(source);
#endif
  if(!fin)
    {
    int e = errno;
    std::string m = "CopyFile failed to open source file \"";
    m += source;
    m += "\"";
    m += " System Error: ";
    m += strerror(e);
    std::cerr << m.c_str() << "\n";
    return;
    }

#if defined(_WIN32) || defined(__CYGWIN__)
  std::ofstream fout(destination, 
                     std::ios::binary | std::ios::out | std::ios::trunc);
#else
  std::ofstream fout(destination, 
                     std::ios::out | std::ios::trunc);
#endif
  if(!fout)
    {
    int e = errno;
    std::string m = "CopyFile failed to open destination file \"";
    m += destination;
    m += "\"";
    m += " System Error: ";
    m += strerror(e);
    std::cerr << m.c_str() << "\n";
    return;
    }
  
  // This copy loop is very sensitive on certain platforms with
  // slightly broken stream libraries (like HPUX).  Normally, it is
  // incorrect to not check the error condition on the fin.read()
  // before using the data, but the fin.gcount() will be zero if an
  // error occurred.  Therefore, the loop should be safe everywhere.
  while(fin)
    {
    fin.read(buffer, bufferSize);
    if(fin.gcount())
      {
      fout.write(buffer, fin.gcount());
      }
    }
  
  // Make sure the operating system has finished writing the file
  // before closing it.  This will ensure the file is finished before
  // the check below.
  fout.flush();
  
  fin.close();
  fout.close();

  // More checks.
  struct stat statSource, statDestination;
  statSource.st_size = 12345;
  statDestination.st_size = 12345;
  if(stat(source, &statSource) != 0)
    {
    int e = errno;
    std::string m = "CopyFile failed to stat source file \"";
    m += source;
    m += "\"";
    m += " System Error: ";
    m += strerror(e);
    std::cerr << m.c_str() << "\n";
    }
  else if(stat(destination, &statDestination) != 0)
    {
    int e = errno;
    std::string m = "CopyFile failed to stat destination file \"";
    m += source;
    m += "\"";
    m += " System Error: ";
    m += strerror(e);
    std::cerr << m.c_str() << "\n";
    }
  else if(statSource.st_size != statDestination.st_size)
    {
    std::cerr << "copy failed " << "\n";
    }
}

bool SystemTools::FilesDiffer(const char* source,
                              const char* destination)
{
  struct stat statSource;
  if (stat(source, &statSource) != 0) 
    {
    return true;
    }

  struct stat statDestination;
  if (stat(destination, &statDestination) != 0) 
    {
    return true;
    }

  if(statSource.st_size != statDestination.st_size)
    {
    return true;
    }

  if(statSource.st_size == 0)
    {
    return false;
    }

#if defined(_WIN32) || defined(__CYGWIN__)
  std::ifstream finSource(source, 
                          std::ios::binary | std::ios::in);
  std::ifstream finDestination(destination, 
                               std::ios::binary | std::ios::in);
#else
  std::ifstream finSource(source);
  std::ifstream finDestination(destination);
#endif
  if(!finSource || !finDestination)
    {
    return true;
    }

  char* source_buf = new char[statSource.st_size];
  char* dest_buf = new char[statSource.st_size];

  finSource.read(source_buf, statSource.st_size);
  finDestination.read(dest_buf, statSource.st_size);

  if(statSource.st_size != static_cast<long>(finSource.gcount()) ||
     statSource.st_size != static_cast<long>(finDestination.gcount()))
    {
    std::cerr << "copy failed\n";
    delete [] source_buf;
    delete [] dest_buf;
    return false;
    }
  int ret = memcmp((const void*)source_buf, 
                   (const void*)dest_buf, 
                   statSource.st_size);

  delete [] dest_buf;
  delete [] source_buf;

  return ret != 0;
}

bool SystemTools::CopyFileIfDifferent(const char* source,
                                        const char* destination)
{
  if(SystemTools::FilesDiffer(source, destination))
    {
    SystemTools::cmCopyFile(source, destination);
    return true;
    }
  return false;
}


} // namespace cable
