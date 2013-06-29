#include <TimedText/WebVTTParser.h>

namespace TimedText
{

WebVTTParser::WebVTTParser(Buffer &buf)
  : buffer(buf)
{
  state = Initial;
  status = Unfinished;
}

WebVTTParser::~WebVTTParser()
{
}

bool
WebVTTParser::parseHeader()
{
  const char webvtt[] = "WEBVTT";
  int n;
  if(buffer.pos() == 0) {
retryBOM:
    if(buffer.size() < 3) {
      status = Unfinished;
      if(!buffer.isFinal() && buffer.isAsynchronous()) {
        buffer.sleep();
        goto retry;
      }
      return false;
    }
    char bom[3] = "";
    const char utf8BOM[] = { 0xEF, 0xBB, 0xBF };
    n = buffer.read(bom, sizeof(bom));
    if(!::memcmp(utf8BOM, bom, 3)) {
      buffer.seek(3, true);
    } else if(!::memcmp(webvtt, bom, 3)) {
      // Don't seek
      goto retry;
    } else {
      // In all other cases, we abort. (This can't possibly
      // have a valid header)
      status = Aborted;
      return false;
    }
  }
  if(buffer.pos() == 0 || buffer.pos() == 3) {
retry:
    if(buffer.size() - buffer.pos() < 6) {
      status = Unfinished;
      if(!buffer.isFinal() && buffer.isAsynchronous()) {
        buffer.sleep();
        goto retry;
      }
      return false;
    }
    char header[6] = "";
    int n = buffer.read(header, sizeof(header));
    if(::memcmp(webvtt,header,6)) {
      status = Aborted;
      return false;
    }
    char c;
    buffer.seek(6, false);
    bool ok;
retry_delimiter:
    if(!(ok = buffer.next(c)) || !isValidSignatureDelimiter(c)) {
      if(!ok && buffer.eof()) {
        // This is a special case:
        // 8. If position is past the end of input, then abort these steps.
        //    The file was successfully processed, but it contains no useful
        //    data and so no text track cues where added to output.
        status = Finished;
        return true;
      } else if(ok) {
        buffer.seek(-1);
        status = Aborted;
        return false;
      } else if(buffer.isAsynchronous()) {
        buffer.sleep();
        goto retry_delimiter;
      } else {
        status = Unfinished;
        return false;
      }
    }
    state = Header;
    return true;
  }
  return false;
}

bool
WebVTTParser::parse(Status *pstatus)
{
  int n;
  if(status == Aborted)
    return false;
  if(state == Initial)
    if(!parseHeader()) {
      if(*pstatus)
        *pstatus = status;
      return false;
    }
retry:
  if(!buffer.getline(line)) {
    if(*pstatus)
      *pstatus = Unfinished;
    return false;
  }
  switch(state) {
  }
  line.clear();
  if(status != Aborted)
    if(buffer.eof())
      status = Finished;
    else
      goto retry;

  if(pstatus)
    *pstatus = status;
  return status == Finished;
}

} // TimedText
