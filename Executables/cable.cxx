/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cable.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableVersion.h"
#include "cableXMLSourceParser.h"
#include "cableSourceRepresentation.h"
#include "cableTclGenerator.h"
#include "cableSystemTools.h"

using namespace cable;

String findSelfPath(const char* argv0);
String findGCC_XML(const char* argv0, const char* cableGCCXML);
String findXMLFileName(const char* argv0);

/** Program entry point.  */
int main(int argc, char* argv[])
{
  bool printVersion = false;
  bool printUsage = false;
  bool verbose = false;
  bool keepXML = false;
  const char* inFileName = 0;
  const char* tclOutName = 0;
  const char* xmlOutName = 0;
  const char* srcOutName = 0;
  const char* cableGCCXML = 0;
  String gccxmlOptions;
  
  for(unsigned int i=1;i < argc;++i)
    {
    String arg = argv[i];
    if(arg == "--version")
      { printVersion = true; }
    else if(arg == "--help")
      { printUsage = true; }
    else if(arg == "--verbose")
      { verbose = true; }
    else if((arg == "--gccxml") && ((i+1) < argc))
      { cableGCCXML = argv[i+1]; ++i; }
    else if(arg == "--keep-xml")
      { keepXML = true; }
    else if((arg == "-tcl") && ((i+1) < argc))
      { tclOutName = argv[i+1]; ++i; }
    else if((arg == "-xml") && ((i+1) < argc))
      { xmlOutName = argv[i+1]; ++i; }
    else if((arg == "-src") && ((i+1) < argc))
      { srcOutName = argv[i+1]; ++i; }
    else
      {
      if(!inFileName)
        {
        inFileName = argv[i];
        }
      else
        {
        gccxmlOptions += " ";
        gccxmlOptions += argv[i];
        }
      }
    }
  
  if(printVersion)
    {
    std::cout << "CABLE version " CABLE_VERSION_STRING "\n";
    return 0;
    }
  else if(printUsage)
    {
    std::cout << "CABLE version " CABLE_VERSION_STRING "\n";
    std::cout << "Usage:\n"
              << "  cable [options] input-file -language-name output-file [gccxml-options ...]\n"
              << "\n"
              << "where \"-language-name\" is one of:\n"
              << "  -tcl to generate Tcl wrappers.\n"
              << "\n"
              << "Supported options are:\n"
              << "  --verbose    = Verbose output during execution.\n"
              << "  --version    = Print the version string and exit.\n"
              << "  --help       = Print this message and exit.\n"
              << "  --gccxml xxx = Use \"xxx\" as GCC-XML program.\n"
              << "  --keep-xml   = Don't delete the GCC-XML output file.\n"
              << "\n"
              << "Unrecognized options will be passed on to GCC-XML.\n"
              << "If CABLE_GCCXML is set in the environment, it will be used\n"
              << "as the GCC-XML program.\n";
    return 0;
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
  String gccxmlExecutable = findGCC_XML(argv[0], cableGCCXML);
  
  // Get the GCC-XML output file name.
  String xmlFileName = findXMLFileName(inFileName);
  
  // Build the GCC-XML command.
  String gccxmlCommand =
    gccxmlExecutable+" "+inFileName+" -fxml=\""+xmlFileName+
    "\" -fxml-start=_cable_ -DCABLE_CONFIGURATION"+gccxmlOptions;
  
  if(verbose)
    {
    std::cout << "Running GCC-XML with command:\n"
              << gccxmlCommand.c_str() << "\n";
    }
  
  // Run the GCC-XML command.
  String output;
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
  
  // Print the parsed xml back out if requested.
  if(xmlOutName)
    {
    std::cout << "Writing XML to \"" << xmlOutName << "\"\n";
    std::ofstream xmlOutFile(xmlOutName);
    if(xmlOutFile)
      {
      parser->Print(xmlOutFile);
      }
    else
      {
      std::cerr << "Error opening xml output file.\n";
      return 1;
      }
    }
  
  // Get the parsed source representation.
  SourceRepresentation::Pointer sr = parser->GetSourceRepresentation();
  if(!sr)
    {
    std::cerr << "Error getting SourceRepresentation from parser.\n";    
    return 1;
    }
  
  // Print the parsed source representation back out if requested.
  if(srcOutName)
    {
    std::cout << "Writing source to \"" << srcOutName << "\"\n";
    std::ofstream srcOutFile(srcOutName);
    if(srcOutFile)
      {
      sr->Print(srcOutFile);
      }
    else
      {
      std::cerr << "Error opening src output file.\n";
      return 1;
      }
    }
  
  // Generate the Tcl wrappers if requested.
  if(tclOutName)
    {
    std::cout << "Writing Tcl wrappers to \"" << tclOutName << "\"\n";
    std::ofstream tclOutFile(tclOutName);
    if(tclOutFile)
      {
      TclGenerator::Pointer tg = TclGenerator::New();
      tg->SetSourceRepresentation(sr);
      tg->SetStream(&tclOutFile);
      tg->Generate();
      }
    else
      {
      std::cerr << "Error opening tcl output file.\n";
      return 1;
      }
    }
  
  return 0;
}

//----------------------------------------------------------------------------
String findSelfPath(const char* argv0)
{
  // Find our own executable's location.
  String av0 = argv0;
  SystemTools::ConvertToUnixSlashes(av0);
  String::size_type pos = av0.find_last_of("/");
  if(pos == std::string::npos)
    {
    av0 = SystemTools::FindProgram(argv0);
    pos = av0.find_last_of("/");
    }
  String selfPath;
  if(pos != String::npos)
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
String findGCC_XML(const char* argv0, const char* cableGCCXML)
{
  String gccxmlExecutable;
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
    String selfPath = findSelfPath(argv0);
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
String findXMLFileName(const char* inFileName)
{
  String ifn = inFileName;
  SystemTools::ConvertToUnixSlashes(ifn);
  String::size_type pos = ifn.rfind('/');
  String file;
  if(pos != String::npos)
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
