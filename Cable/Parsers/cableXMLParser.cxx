/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableXMLParser.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableXMLParser.h"

#include "../Expat/expat.h"

namespace cable
{

//----------------------------------------------------------------------------
XMLParser::XMLParser()
{
  m_Stream = 0;
  m_Parser = 0;
}

//----------------------------------------------------------------------------
XMLParser::~XMLParser()
{
  this->SetStream(0);
}

//----------------------------------------------------------------------------
void XMLParser::SetStream(std::istream* is)
{
  m_Stream = is;
}

//----------------------------------------------------------------------------
std::istream* XMLParser::GetStream() const
{
  return m_Stream;
}

//----------------------------------------------------------------------------
bool XMLParser::Parse()
{
  // Make sure we have input.
  if(!m_Stream)
    {
    cableErrorMacro("Parse() called with no input stream.");
    return false;
    }
  
  // Create the expat XML parser.
  m_Parser = XML_ParserCreate(0);
  XML_SetElementHandler(m_Parser,
                        &XMLParser::StartElementFunction,
                        &XMLParser::EndElementFunction);
  XML_SetCharacterDataHandler(m_Parser,
                              &XMLParser::CharacterDataHandlerFunction);
  XML_SetUserData(m_Parser, this);
  
  // Parse the input stream.
  bool result = this->ParseStream();
  
  if(result)
    {
    // Tell the expat XML parser about the end-of-input.
    if(!XML_Parse(m_Parser, "", 0, 1))
      {
      this->ReportXmlParseError();
      result = false;
      }
    }
  
  // Clean up the parser.
  XML_ParserFree(m_Parser);
  m_Parser = 0;
  
  return result;
}

//----------------------------------------------------------------------------
bool XMLParser::ParseStream()
{
  // Default stream parser just reads a block at a time.
  std::istream& in = *(m_Stream);
  const int bufferSize = 4096;  
  char buffer[bufferSize];  
  
  // Read in the stream and send its contents to the XML parser.  This
  // read loop is very sensitive on certain platforms with slightly
  // broken stream libraries (like HPUX).  Normally, it is incorrect
  // to not check the error condition on the fin.read() before using
  // the data, but the fin.gcount() will be zero if an error occurred.
  // Therefore, the loop should be safe everywhere.
  while(!this->ParsingComplete() && in)
    {
    in.read(buffer, bufferSize);
    if(in.gcount())
      {
      if(!this->ParseBuffer(buffer, in.gcount()))
        {
        return false;
        }
      }
    }
  return true;
}

//----------------------------------------------------------------------------
bool XMLParser::ParsingComplete()
{
  // Default behavior is to parse to end of stream.
  return 0;
}

//----------------------------------------------------------------------------
void XMLParser::StartElement(const char* name, const char**)
{
  this->ReportUnknownElement(name);
}

//----------------------------------------------------------------------------
void XMLParser::EndElement(const char*)
{
}

//----------------------------------------------------------------------------
void XMLParser::CharacterDataHandler(const char*, int)
{
}

//----------------------------------------------------------------------------
void XMLParser::ReportStrayAttribute(const char* element, const char* attr,
                                     const char* value) const
{
  cableWarningMacro("Stray attribute in XML stream: Element " << element
                    << " has " << attr << "=\"" << value << "\"");
}

//----------------------------------------------------------------------------
void XMLParser::ReportMissingAttribute(const char* element,
                                       const char* attr) const
{
  cableErrorMacro("Missing attribute in XML stream: Element " << element
                  << " is missing " << attr);
}

//----------------------------------------------------------------------------
void XMLParser::ReportBadAttribute(const char* element, const char* attr,
                                   const char* value) const
{
  cableErrorMacro("Bad attribute value in XML stream: Element " << element
                  << " has " << attr << "=\"" << value << "\"");
}

//----------------------------------------------------------------------------
void XMLParser::ReportUnknownElement(const char* element) const
{
  cableErrorMacro("Unknown element in XML stream: " << element);
}

//----------------------------------------------------------------------------
void XMLParser::ReportXmlParseError() const
{
  cableErrorMacro("Error parsing XML in stream at line "
                  << XML_GetCurrentLineNumber(m_Parser)
                  << ": " << XML_ErrorString(XML_GetErrorCode(m_Parser)));
}

//----------------------------------------------------------------------------
unsigned long XMLParser::GetXMLByteIndex() const
{
  return XML_GetCurrentByteIndex(m_Parser);
}

//----------------------------------------------------------------------------
unsigned long XMLParser::GetXMLLineNumber() const
{
  return XML_GetCurrentLineNumber(m_Parser);
}

//----------------------------------------------------------------------------
unsigned long XMLParser::GetXMLColumnNumber() const
{
  return XML_GetCurrentColumnNumber(m_Parser);
}  

//----------------------------------------------------------------------------
bool XMLParser::ParseBuffer(const char* buffer, unsigned int count)
{
  // Pass the buffer to the expat XML parser.
  if(!XML_Parse(m_Parser, buffer, count, 0))
    {
    this->ReportXmlParseError();
    return false;
    }
  return true;
}

//----------------------------------------------------------------------------
bool XMLParser::ParseBuffer(const char* buffer)
{
  return this->ParseBuffer(buffer, strlen(buffer));
}

//----------------------------------------------------------------------------
void XMLParser::StartElementFunction(void* parser, const char *name,
                                     const char **atts)
{
  static_cast<XMLParser*>(parser)->StartElement(name, atts);
}

//----------------------------------------------------------------------------
void XMLParser::EndElementFunction(void* parser, const char *name)
{
  static_cast<XMLParser*>(parser)->EndElement(name);
}

//----------------------------------------------------------------------------
void XMLParser::CharacterDataHandlerFunction(void* parser, const char* data,
                                             int length)
{
  static_cast<XMLParser*>(parser)->CharacterDataHandler(data, length);
}

} // namespace cable
