/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableXMLParser.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _cableXMLParser_h
#define _cableXMLParser_h

#include "cableObject.h"

namespace cable
{

/** General-purpose XML parser class.  */
class CABLE_PARSERS_EXPORT XMLParser: public Object
{
public:
  cableTypeMacro(XMLParser, Object);
  static Pointer New() { return new Self; }
  
  /** Set the input stream.  */
  virtual void SetStream(std::istream* is);

  /** Get the input stream.  */
  virtual std::istream* GetStream() const;
  
  /** Parse the input stream.  */
  bool Parse();
  
protected:
  XMLParser();
  ~XMLParser();
  
  // Input stream.  Set by user.
  std::istream* m_Stream;
  
  // Expat parser structure.  Exists only during call to Parse().
  void* m_Parser;
  
  // Called by Parse() to read the stream and call ParseBuffer.  Can
  // be replaced by subclasses to change how the stream is read.
  virtual bool ParseStream();
  
  // Called before each block of input is read from the stream to
  // check if parsing is complete.  Can be replaced by subclasses to
  // change the terminating condition for parsing.  Parsing always
  // stops when the end of file is reached in the stream.
  virtual bool ParsingComplete();
  
  // Called when a new element is opened in the XML source.  Should be
  // replaced by subclasses to handle each element.
  //  name = Name of new element.
  //  atts = Null-terminated array of attribute name/value pairs.
  //         Even indices are attribute names, and odd indices are values.
  virtual void StartElement(const char* name, const char** atts);
  
  // Called at the end of an element in the XML source opened when
  // StartElement was called.
  virtual void EndElement(const char* name);
  
  // Called when there is character data to handle.
  virtual void CharacterDataHandler(const char* data, int length);  
  
  // Called by begin handlers to report any stray attribute values.
  virtual void ReportStrayAttribute(const char* element, const char* attr,
                                    const char* value) const;
  
  // Called by begin handlers to report any missing attribute values.
  virtual void ReportMissingAttribute(const char* element,
                                      const char* attr) const;
  
  // Called by begin handlers to report bad attribute values.
  virtual void ReportBadAttribute(const char* element, const char* attr,
                                  const char* value) const;
  
  // Called by StartElement to report unknown element type.
  virtual void ReportUnknownElement(const char* element) const;
  
  // Called by Parse to report an XML syntax error.
  virtual void ReportXmlParseError() const;  
  
  // Get the current byte index from the beginning of the XML stream.
  unsigned long GetXMLByteIndex() const;
  
  // Get the current line number in the XML stream.
  unsigned long GetXMLLineNumber() const;
  
  // Get the current column number in the XML stream.
  unsigned long GetXMLColumnNumber() const;
  
  // Send the given buffer to the XML parser.
  bool ParseBuffer(const char* buffer, unsigned int count);
  
  // Send the given c-style string to the XML parser.
  bool ParseBuffer(const char* buffer);
  
  // Begin element handler that is registered with the XML_Parser.
  // This just casts the user data to a XMLParser and calls
  // StartElement.
  static void StartElementFunction(void* parser, const char* name,
                                   const char** atts);
  
  // End element handler that is registered with the XML_Parser.  This
  // just casts the user data to a XMLParser and calls EndElement.
  static void EndElementFunction(void* parser, const char* name);

  // Character data handler that is registered with the XML_Parser.
  // This just casts the user data to a XMLParser and calls
  // CharacterDataHandler.
  static void CharacterDataHandlerFunction(void* parser, const char* data,
                                           int length);
private:
  XMLParser(const Self&);  // Not implemented.
  void operator=(const Self&);  // Not implemented.
};

} // namespace cable

#endif
