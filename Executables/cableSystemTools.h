/*=========================================================================

  Program:   GCC-XML
  Module:    cableSystemTools.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableSystemTools_h
#define _cableSystemTools_h

#include "cableUtils.h"

#include <iostream>
#include <fstream>
#include <string>

namespace cable
{

/** Utility methods to hide platform differences.  */
class SystemTools
{
public:
  /** Read a registry value with the given key.  Return whether it was
      found.  */
  static bool ReadRegistryValue(const char *key, String& result);
  
  /** Copy a file named by "source" to the file named by
      "destination". */
  static bool FileCopy(const char* source, const char* destination);
  
  /** Get a lower case string.  */
  static String LowerCase(const String& s);
  
  /** Read an environment variable.  */
  static const char* GetEnv(const char* key);
  static bool GetEnv(const char* key, String& result);
  
  /** Check if a file exists.  */
  static bool FileExists(const char* filename);
  
  /** Check if a file exists and is a directory.  */
  static bool FileIsDirectory(const char* name);
  
  /** Given a path to a directory, convert it to a full path.  This
   * collapses away relative paths.  The full path is returned.  */
  static String CollapseDirectory(const char* in_dir);
  
  /** Get the current working directory.  */
  static String GetCWD();
  
  /** Convert the given path to one with forward slashes.  */
  static void ConvertToUnixSlashes(String& path);

  /** Run the given command and get back the output and the result value.  */
  static bool RunCommand(const char* command, String& output,
                         int &retVal);
  
  /** Delete the file with the given name.  */
  static bool RemoveFile(const char* source);
  
  /** Convert a file path to a suitable output format.  */
  static String ConvertToOutputPath(const char* path);
  static String ConvertToWindowsOutputPath(const char* path);
  static String ConvertToUnixOutputPath(const char* path);
  
  /** Get the file extension for an executable.  */
  static String GetExecutableExtension();
  
  /** Find a program in the system search path.  */
  static String FindProgram(const char* name);
};

} // namespace cable

#endif
