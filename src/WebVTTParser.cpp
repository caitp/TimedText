//
// Copyright (c) 2013 Caitlin Potter and Contributors
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
#include "WebVTTTokenizer.h"
namespace TimedText
{

WebVTTParser::WebVTTParser(Buffer &buf, Client *_client)
  : buffer(buf), client(_client)
{
  state = Initial;
  status = Unfinished;
  headerStatus = InitialHeader;
  withBOM = BOMUnknown;
  currentStartTime = currentEndTime = MalformedTimestamp;
}

WebVTTParser::~WebVTTParser()
{
  currentCues.clear();
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
  currentStartTime = currentEndTime = MalformedTimestamp;
  line.skipWhitespace(position);
  currentStartTime = collectTimestamp(line, position);

  if(currentStartTime.isMalformed())
    return BadCue;

  line.skipWhitespace(position);

  // Abort cue if we don't find '-->' following the start time
  // and optional whitespace
  if(line.indexOf("-->") != position)
    return BadCue;

  position += 3;
  line.skipWhitespace(position);

  currentEndTime = collectTimestamp(line, position);

  if(currentEndTime.isMalformed())
    return BadCue;

  line.skipWhitespace(position);

  currentSettings = line.substring(position);

  return CueText;
}

Timestamp
WebVTTParser::collectTimestamp(const String &line, int &position)
{
  enum Mode {
    Minutes,
    Hours
  } mode = Minutes;
  int digits;
  if(position < 0 || position >= line.length() || !Char::isAsciiDigit(line[0]))
    return MalformedTimestamp;

  unsigned value1 = line.parseUint(position, &digits);
  // Only the hours field is flexible in terms of how many digits
  // are valid.
  if(digits != 2 || value1 > 59)
    mode = Hours;

  if(position >= line.length() || line[position++] != ':')
    return MalformedTimestamp;

  if(position >= line.length() || !Char::isAsciiDigit(line[position]))
    return MalformedTimestamp;

  unsigned value2 = line.parseUint(position, &digits);
  // TODO: Be more flexible here, we don't necessarily want to die
  // just because of a marginally invalid time component
  if(digits != 2)
    return MalformedTimestamp;

  unsigned value3;
  if(mode == Hours || (position < line.length() && line[position] == ':')) {
    // If we don't have at least 3 components, or we have 3 but are expecting
    // 4, then die. TODO: As above, we can be more flexible here...
    if(position >= line.length() || line[position++] != ':')
      return MalformedTimestamp;
    // If it's not a number, there isn't much we can do with it...
    if(position >= line.length() || !Char::isAsciiDigit(line[position]))
      return MalformedTimestamp;
    value3 = line.parseUint(position, &digits);
    // TODO: Again, we should be more flexible here...
    if(digits != 2)
      return MalformedTimestamp;
  } else {
    // If we are actually minutes, then shift everything over a bit.
    value3 = value2;
    value2 = value1;
    value1 = 0;
  }

  if(position >= line.length() || line[position++] != '.')
    return MalformedTimestamp;
  // If it's not a number, there isn't much we can do with it...
  if(position >= line.length() || !Char::isAsciiDigit(line[position]))
    return MalformedTimestamp;

  unsigned value4 = line.parseUint(position, &digits);
  // TODO: same as above...
  if(digits != 3)
    return MalformedTimestamp;

  Timestamp::Components components = { value1, value2, value3, value4 };
  return Timestamp::fromComponents(components);
}

void
WebVTTParser::dispatchCue()
{
  String text;
  // If this allocation fails, we have no way of notifying the user,
  // currently!
  currentCueText.toString(text);
  Cue cue(WebVTTCue, currentStartTime, currentEndTime,
          currentId, text);
  cue.applySettings(currentSettings);
  currentCues.push(cue);

  // TODO:
  // Create Cue object and send it off to the client
  currentSettings.clear();
  currentId.clear();
  currentEndTime = currentStartTime = MalformedTimestamp;
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
  currentEndTime = currentStartTime = MalformedTimestamp;
  currentCueText.clear();
  state = Id;
}

bool
WebVTTParser::parse(Status *pstatus)
{
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
    if(pstatus)
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

        // If this was the last line in the buffer, we need to dispatch.
        if(buffer.eof())
          state == BadCue ? dropCue() : dispatchCue();
      }
      break;
  }
  line.clear();
  if(status != Aborted) {
    if(buffer.eof()) {
      status = Finished;
    } else {
      goto retry;
    }
  }
  if(pstatus)
    *pstatus = status;
  return status == Finished;
}

void
WebVTTParser::parsedCues(List<Cue> &result)
{
  result = currentCues;
  currentCues.clear();
}

// Translate CueText into tree of Node objects
bool
WebVTTParser::cuetextToNodes(const String &cuetext, Node &result)
{
  // 1. Let input be the string being parsed.
  // 2. Let position be a pointer into input, initially pointing at the start
  //    of the string
  // 3. Let result be a list of Node objects, initially empty.
  // 4. Let current be the WebVTT Internal Node Object result
  // 5. Let language stack be a stack of language codes, initially empty (NO)
  String input = cuetext;
  int position = 0;
  result = Node(InternalTextNode);
  if(input.isEmpty())
    return true;
  List<String> langStack;
  List<Node> nodeStack;
  Node current = result;
  nodeStack.push(current);
  WebVTTTokenizer tokenizer;
  WebVTTToken token;

  // 6. Loop: if position is past the end of input, return result and abort
  //    these steps
  while(position < input.length()) {
    if(tokenizer.next(input, position, token)) {
      // 7. Let token be the result of invoking the WebVTT cue text tokenizer.
      if(token.type() == WebVTTToken::Text) {
        // If token is a string
        //   1. Create a WebVTT Text Object whose value is the value of the
        //      string token token.
        Node newTextNode(TextNode);
        String text;
        token.data(text);
        newTextNode.setText(text);
        //   2. Append the newly created WebVTT Text Object to current
        current.push(newTextNode);
      } else if(token.type() == WebVTTToken::StartTag) {
        // If token is a start tag
        String name;
        token.data(name);
        Node newNode;
        List<String> classes = token.classes();
        String annotation = token.annotation();
        if(!::strcmp("c",name))
          // If the tag is "c"
          //   Attach a WebVTT Class Object.
          newNode = Node(ClassNode);
        else if(!::strcmp("i", name))
          // If the tag name is "i"
          //   Attach a WebVTT Italic Object.
          newNode = Node(ItalicNode);
        else if(!::strcmp("b", name))
          // If the tag name is "b"
          //   Attach a WebVTT Bold Object.
          newNode = Node(BoldNode);
        else if(!::strcmp("u", name))
          // If the tag name is "u"
          //   Attach a WebVTT Underline Object.
          newNode = Node(UnderlineNode);
        else if(!::strcmp("ruby", name))
          // If the tag name is "ruby"
          //   Attach a WebVTT Ruby Object.
          newNode = Node(RubyNode);
        else if(!::strcmp("rt", name)) {
          // If the tag name is "rt"
          //   If current is a WebVTT Ruby Object, then attach a WebVTT Ruby
          //   Text Object.
          if(current.element() == RubyNode)
            newNode = Node(RubyTextNode);
        } else if(!::strcmp("lang", name)) {
          langStack.push(annotation);
          newNode = Node(LangNode);
        }

        if(newNode.type() == InternalNode) {
          // When the steps above say to attach a WebVTT Internal Node Object
          // of a particular concrete class, the user agent must run the
          // following steps:
          // 2. Set the new object's list of applicable classes to the list of
          //    classes in the token, excluding any classes that are the empty
          //    string.
          newNode.setApplicableClasses(classes);
          // 3. Set the new object's applicable language to the top entry on
          //    the language stack, if it is not empty.
          String lang;
          if(langStack.lastItem(lang))
            newNode.setLang(lang);
          // 4. Append the newly created node object to current.
          current.push(newNode);
          nodeStack.push(current);
          current = newNode;
        }
      } else if(token.type() == WebVTTToken::EndTag) {
        // If token is an end tag
        //   If any of the following conditions is true, then let current be
        //   the parent node of current
        String name;
        token.data(name);
        #define MatchingTag(tag,type) \
        (!::strcmp(tag,name) && current.element() == type)
        if(MatchingTag("c", ClassNode)
           || MatchingTag("i", ItalicNode)
           || MatchingTag("b", BoldNode)
           || MatchingTag("u", UnderlineNode)
           || MatchingTag("ruby", RubyNode)
           || MatchingTag("rt", RubyTextNode)
           || MatchingTag("v", VoiceNode)) {
          nodeStack.pop(current);
        } else if(MatchingTag("lang", LangNode)) {
          // Otherwise, if the tag name of the end tag token token is "lang",
          // and current is a WebVTT Language Object, then let current be the
          // parent node of current, and pop the top value from the language
          // stack.
          String unused;
          langStack.pop(unused);
          nodeStack.pop(current);
        } else if(MatchingTag("ruby", RubyTextNode)) {
          // Otherwise, if the tag name of the end tag token token is "ruby"
          // and current is a WebVTT Ruby Text Object, then let current be
          // the parent node of the parent node of current.
          nodeStack.pop(current);
          nodeStack.pop(current);
        }
        #undef MatchingTag
      } else if(token.type() == WebVTTToken::TimestampTag) {
        // If token is a timestamp tag
        //   1. Let input be the tag value.
        //   2. Let position be a pointer into input, initially pointing at the
        //      start of the string.
        //   3. Collect a WebVTT timestamp
        //   4. If the algorithm does not fail, and if position now points at
        //      the end of input (i.e. there are no trailing characters after
        //      the timestamp), then create a WebVTT Timestamp Object whose
        //      value is the collected time, then append it to current.
        Timestamp time = token.timestamp();
        if(time != MalformedTimestamp) {
          // The WebVTTToken will return a MalformedTimestamp if the time is
          // not valid, and so it can be safely ignored in that case.
          Node newNode(TimestampNode);
          newNode.setTimestamp(time);
          current.push(newNode);
        }
      }

      // Reset the tokenizer...
      tokenizer.reset();
    }
  }

  return true;
}

} // TimedText
