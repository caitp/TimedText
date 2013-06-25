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
  ::XML_Status status = ::XML_Parse(parser, buffer + pos, len, 1);
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
