//
// Copyright (c) 2014 Caitlin Potter and Contributors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//  * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <TimedText/Format.h>
#include <cstring>

#include <expat.h>

#if (XML_MAJOR_VERSION < 2) \
 && (XML_MINOR_VERSION < 96) \
 && (XML_MICRO_VERSION < 9)
#  error "Sniffing depends on feature introduced in expat 1.95.8"
#endif

namespace TimedText
{

static bool
skipBOM(const char *buffer, int &pos, int &len)
{
  const char utf8BOM[] = { 0xEF, 0xBB, 0xBF };
  // In any case, skip the UTF-8 BOM if we see one.
  if(pos == 0 && len >= 3
     && !::memcmp(buffer + pos, utf8BOM, sizeof(utf8BOM))) {
    pos += 3, len -= 3;
    return true;
  }
  return false;
}

bool
sniffBufferForWebVTT(const char *buffer, int &pos, int len)
{
  if(!buffer)
    return false;
  if(pos > 0)
    len -= pos;
  if(len < 0)
    len = ::strlen(buffer);
  if(len == 0)
    return false;

  // Skip UTF-8 BOM if we're in position 0
  skipBOM(buffer, pos, len);

  // If we're not in the first byte of text, it's much harder to tell
  // if we're a WebVTT document or not, so simply abort parsing in this case.
  if(pos != 0 && pos != 3)
    return false;

  // This is probably a WebVTT document, let the WebVTT parser determine if
  // it is valid or not.
  if(len >= 6 && !::memcmp(buffer + pos, "WEBVTT", 6))
    return true;

  return false;
}

struct TTMLSniffer {
  ::XML_Parser parser;
  bool isTTML;  
};

void XMLCALL
ttmlNamespaceDecl(void *userData, const XML_Char *prefix, const XML_Char *uri)
{
  const char *ttmlns[] = {
    // Known TTML namespace URIs
    "http://www.w3.org/ns/ttml",
    "http://www.w3.org/ns/ttml#parameter",
    "http://www.w3.org/ns/ttml#styling",
    "http://www.w3.org/ns/ttml#metadata"
  };
  TTMLSniffer &self = *static_cast<TTMLSniffer *>(userData);
  for(int i=0; i<sizeof(ttmlns)/sizeof(*ttmlns); ++i) {
    const char *ns = ttmlns[i];
    if(!::strcmp(ns,uri)) {
      self.isTTML = true;
      ::XML_StopParser(self.parser, 1);
      return;
    }
  }
}

bool
sniffBufferForTTML(const char *buffer, int &pos, int len)
{
  if(!buffer)
    return false;
  if(pos > 0)
    len -= pos;
  if(len < 0)
    len = ::strlen(buffer);
  if(len == 0)
    return false;

  // Create an expat sniffer
  ::XML_Parser parser = ::XML_ParserCreateNS(NULL, '\n');
  if(!parser)
    return false;
  TTMLSniffer sniffer = { parser, false };
  ::XML_SetUserData(parser,&sniffer);
  ::XML_SetStartNamespaceDeclHandler(parser, &ttmlNamespaceDecl);
  // Ignore XML parser result
  ::XML_Parse(parser, buffer + pos, len, 1);
  ::XML_ParserFree(parser);

  return sniffer.isTTML;
}

Format
detectFormat(const char *buffer, int len)
{
  int pos = 0;
  const char utf8BOM[] = { 0xEF, 0xBB, 0xBF };
	if(!buffer)
		return FormatUnknown;
	if(len < 0)
		len = ::strlen(buffer);
	if(len == 0)
		return FormatUnknown;

  if(sniffBufferForWebVTT(buffer, pos, len))
    return FormatWebVTT;
  else if(sniffBufferForTTML(buffer, pos, len))
    return FormatTTML;
  return FormatUnknown;
}

} // TimedText
