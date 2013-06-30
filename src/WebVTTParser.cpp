#include <TimedText/WebVTTParser.h>

namespace TimedText
{

WebVTTParser::WebVTTParser(Buffer &buf)
  : buffer(buf)
{
  state = Initial;
  status = Unfinished;
  headerStatus = InitialHeader;
  withBOM = BOMUnknown;
}

WebVTTParser::~WebVTTParser()
{
}

bool
WebVTTParser::parseBOM()
{
  if(headerStatus == InitialHeader && withBOM == BOMUnknown) {
retry:
    if(buffer.remaining() < 3) {
      if(!buffer.isFinal()) {
        if(buffer.isAsynchronous()) {
          buffer.sleep();
          goto retry;
        } else {
          status = Unfinished;
          return false;
        }
      } else {
        status = Aborted;
        return false;
      }
    } else {
      if(!::memcmp(buffer.curr(), "\xEF\xBB\xBF", 3)) {
        buffer.seek(3);
        withBOM = WithBOM;
      } else if(!::memcmp(buffer.curr(), "WEB", 3)) {
        withBOM = WithoutBOM;
      } else {
        // In any other situation, we have an invalid header
        // and need to abort parsing
        status = Aborted;
        return false;
      }
    }
    headerStatus = TagHeader;
  }
  return true;
}

bool
WebVTTParser::parseHeaderTag()
{
  if(headerStatus == TagHeader) {
retry:
    if(buffer.remaining() < 6) {
      if(!buffer.isFinal()) {
        if(buffer.isAsynchronous()) {
          buffer.sleep();
          goto retry;
        } else {
          status = Unfinished;
          return false;
        }
      } else {
        status = Aborted;
        return false;
      }
    } else {
      if(!::memcmp(buffer.curr(), "WEBVTT", 6)) {
        buffer.seek(6);
        headerStatus = PostTagHeader;
      } else {
        // If 'WEBVTT' is not the first non-BOM bytes,
        // then this cannot be a valid WebVTT document.
        status = Aborted;
        return false;
      }
    }
  }
  return true;
}

bool
WebVTTParser::parsePostHeaderTag()
{
  if(headerStatus == PostTagHeader) {
retry:
    if(buffer.remaining() < 1) {
      if(!buffer.isFinal()) {
        if(buffer.isAsynchronous()) {
          buffer.sleep();
          goto retry;
        } else {
          status = Unfinished;
          return false;
        }
      } else {
        // If this branch is reached, we are in the final
        // block of the document and there are no bytes left.
        // If this state is correct, then we have correctly
        // parsed an empty document.
        status = Finished;
        return true;
      }
    } else {
      // If we do have a byte to read, it needs to be an acceptable
      // byte.
      char c;
      buffer.next(c);
      if(isValidSignatureDelimiter(c)) {
        if(Char::isNewlineChar(c))
          buffer.seek(-1);
        headerStatus = CommentHeader;
      } else {
        // If it's not a valid signature delimiter, it's a bad document.
        // Sorry :(
        status = Aborted;
        return false;
      }
    }
  }
  return true;
}

bool
WebVTTParser::parseHeaderComment()
{
  if(headerStatus == CommentHeader) {
    if(buffer.skipline()) {
      state = Id;
      // Once we've gotten to the HeaderComment state,
      // we will always have a Finished document on eof()
      if(buffer.eof())
        status = Finished;
    } else {
      return false;
    }
  }
  return true;
}

bool
WebVTTParser::parseHeader()
{
  if(state != Initial)
    return false;
  if(headerStatus == InitialHeader)
    if(!parseBOM())
      return false;
  if(headerStatus == TagHeader)
    if(!parseHeaderTag())
      return false;
  if(headerStatus == PostTagHeader)
    if(!parsePostHeaderTag())
      return false;
  if(headerStatus == CommentHeader)
    if(!parseHeaderComment())
      return false;
  return true;
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
