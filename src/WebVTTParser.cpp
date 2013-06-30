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

#include <TimedText/WebVTTParser.h>

namespace TimedText
{

WebVTTParser::WebVTTParser(Buffer &buf, Client *_client)
  : buffer(buf), client(_client)
{
  state = Initial;
  status = Unfinished;
  headerStatus = InitialHeader;
  withBOM = BOMUnknown;
  currentStartTime = currentEndTime = MalformedTime;
}

WebVTTParser::~WebVTTParser()
{
}

bool
WebVTTParser::parseBOM()
{
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
  return true;
}

bool
WebVTTParser::parseHeaderTag()
{
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
      return true;
    } else {
      // If 'WEBVTT' is not the first non-BOM bytes,
      // then this cannot be a valid WebVTT document.
      status = Aborted;
      return false;
    }
  }
}

bool
WebVTTParser::parsePostHeaderTag()
{
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
      return true;
    } else {
      // If it's not a valid signature delimiter, it's a bad document.
      // Sorry :(
      status = Aborted;
      return false;
    }
  }
}

bool
WebVTTParser::parseHeaderComment()
{
  if(buffer.skipline()) {
    state = Header;
    // Once we've gotten to the HeaderComment state,
    // we will always have a Finished document on eof()
    if(buffer.eof())
      status = Finished;
    return true;
  } else {
    return false;
  }
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

WebVTTParser::ParseState
WebVTTParser::collectTimingsAndSettings(const String &line)
{
  int position = 0;
  currentStartTime = currentEndTime = MalformedTime;
  line.skipWhitespace(position);
  currentStartTime = collectTimeStamp(line, position);

  if(currentStartTime == MalformedTime)
    return BadCue;

  line.skipWhitespace(position);

  // Abort cue if we don't find '-->' following the start time
  // and optional whitespace
  if(line.indexOf("-->") != position)
    return BadCue;

  position += 3;
  line.skipWhitespace(position);

  currentEndTime = collectTimeStamp(line, position);

  if(currentEndTime == MalformedTime)
    return BadCue;

  line.skipWhitespace(position);

  currentSettings = line.substring(position);

  return CueText;
}

Timestamp
WebVTTParser::collectTimeStamp(const String &line, int &position)
{
  enum Mode {
    Minutes,
    Hours
  } mode = Minutes;
  int digits;
  if(position < 0 || position >= line.length() || !Char::isAsciiDigit(line[0]))
    return MalformedTime;

  int value1 = line.parseInt(position, &digits);
  // Only the hours field is flexible in terms of how many digits
  // are valid.
  if(digits != 2 || value1 > 59)
    mode = Hours;

  if(position >= line.length() || line[position++] != ':')
    return MalformedTime;

  if(position >= line.length() || !Char::isAsciiDigit(line[position]))
    return MalformedTime;

  int value2 = line.parseInt(position, &digits);
  // TODO: Be more flexible here, we don't necessarily want to die
  // just because of a marginally invalid time component
  if(digits != 2)
    return MalformedTime;

  int value3;
  if(mode == Hours || (position < line.length() && line[position] == ':')) {
    // If we don't have at least 3 components, or we have 3 but are expecting
    // 4, then die. TODO: As above, we can be more flexible here...
    if(position >= line.length() || line[position++] != ':')
      return MalformedTime;
    // If it's not a number, there isn't much we can do with it...
    if(position >= line.length() || !Char::isAsciiDigit(line[position]))
      return MalformedTime;
    value3 = line.parseInt(position, &digits);
    // TODO: Again, we should be more flexible here...
    if(digits != 2)
      return MalformedTime;
  } else {
    // If we are actually minutes, then shift everything over a bit.
    value3 = value2;
    value2 = value1;
    value1 = 0;
  }

  if(position >= line.length() || line[position++] != '.')
    return MalformedTime;
  // If it's not a number, there isn't much we can do with it...
  if(position >= line.length() || !Char::isAsciiDigit(line[position]))
    return MalformedTime;

  int value4 = line.parseInt(position, &digits);
  // TODO: same as above...
  if(digits != 3)
    return MalformedTime;

  Timestamp::Components components = { value1, value2, value3, value4 };
  return Timestamp::fromComponents(components);
}

void
WebVTTParser::dispatchCue()
{
  // TODO:
  // Create Cue object and send it off to the client
  currentSettings.clear();
  currentId.clear();
  currentEndTime = currentStartTime = MalformedTime;
  currentCueText.clear();
  state = Id;

  if(client)
    client->cuesAvailable();
}

void
WebVTTParser::dropCue()
{
  currentSettings.clear();
  currentId.clear();
  currentEndTime = currentStartTime = MalformedTime;
  currentCueText.clear();
  state = Id;
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
    status = Unfinished;
    if(*pstatus)
      *pstatus = status;
    return false;
  }
  switch(state) {
    case Initial:
      // Should never happen.
      break;
    case Header:
      if(line.isEmpty())
        state = Id;
      else {
        if(line.contains("-->")) {
          state = collectTimingsAndSettings(line);
          currentCueText.clear();
        } else {
          currentId = line;
          state = TimingsAndSettings;
        }
      }
      break;
    case Id:
      if(line.contains("-->")) {
        state = collectTimingsAndSettings(line);
        currentCueText.clear();
      } else if(!line.isEmpty()) {
        currentId = line;
        state = TimingsAndSettings;
      }
      break;
    case TimingsAndSettings:
      if(line.isEmpty()) {
        // 34. If line is the empty string, then discard cue and jump to the
        //     step labeled cue loop.
        state = Id;
      } else {
        state = collectTimingsAndSettings(line);
        currentCueText.clear();
      }
      break;
    case BadCue:
    case CueText:
      if(line.isEmpty() || line.contains("-->"))
        state == BadCue ? dropCue() : dispatchCue();
      else {
        // TODO: check return values, allocation error could theoretically
        // happyn
        if(!currentCueText.isEmpty())
          currentCueText.append('\n');
        currentCueText.append(line);
      }
      break;
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
