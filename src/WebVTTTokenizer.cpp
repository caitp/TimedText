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

#include <TimedText/Types.h>
#include "WebVTTTokenizer.h"
#include <assert.h>

namespace TimedText
{

#define BEGIN_STATE(state) case WebVTTTokenizerState::state: state:
#define END_STATE() assert(false); break;
#define ADVANCE_TO(state) \
do { \
  _state = WebVTTTokenizerState::state; \
  c = nextChar(input, position); \
  goto state; \
} while(0)

WebVTTToken::WebVTTToken()
 : _type(Uninitialized)
{
}

WebVTTToken::~WebVTTToken()
{
}

void
WebVTTToken::reset()
{
  _data.clear();
  _annotation.clear();
  _classes.clear();
}

void
WebVTTToken::ensureIsText()
{
  assert(_type == Uninitialized || _type == Text);
  _type = Text;
}

bool
WebVTTToken::appendData(unsigned long ch)
{
  return _data.append(ch);
}

bool
WebVTTToken::appendData(const StringBuilder &buf)
{
  bool ret = _data.append(buf);
  return ret;
}

bool
WebVTTToken::appendClass(const String &styleClass)
{
  return _classes.push(styleClass);
}

void
WebVTTToken::makeEndOfFile()
{
  assert(_type == Uninitialized);
  _type = EndOfFile;
}

void
WebVTTToken::beginEmptyStartTag()
{
  assert(_type == Uninitialized);
  _type = StartTag;
  _data.clear();
}

void
WebVTTToken::beginStartTag(char c)
{
  assert(_type == Uninitialized);
  _type = StartTag;
  _data.append(c);
}

void
WebVTTToken::beginTimestampTag(char c)
{
  assert(_type == Uninitialized);
  _type = TimestampTag;
  _data.append(c);
}

void
WebVTTToken::beginEndTag()
{
  assert(_type == Uninitialized);
  _type = EndTag;
}

static const uint32 endOfFileMark = uint32(-1);

// Tokenizer
WebVTTTokenizer::WebVTTTokenizer()
  : token(0),
    _state(WebVTTTokenizerState::DataState)
{
}

WebVTTTokenizer::~WebVTTTokenizer()
{
}

void
WebVTTTokenizer::reset()
{
	if(token)
    token->reset();
  _state = WebVTTTokenizerState::DataState;
}

uint32
WebVTTTokenizer::nextChar(const String &input, int &position)
{
  if(position < 0 || position >= input.length())
    return endOfFileMark;
  uint32 c = input[position++];
  if(c == '\r') {
    // Convert CR and CRLF to LF
    if(position < input.length() && input[position] == '\n')
      ++position;
    c = '\n';
  }
  return c;
}

bool
WebVTTTokenizer::next(const String &input, int &position, WebVTTToken &result)
{
  // If it's not our first time being called, and we're not being called back
  // with our unfinished token, or the new result token is in an initialized
  // state, then abort.
  if(!(!token || token == &result || 
     result.type() == WebVTTToken::Uninitialized))
    return false;
  token = &result;

  // This is not actually correct, we probably need to treat this as an
  // end-of-file marker
  if(input.isEmpty() || position >= input.length())
    return false;

  uint32 c = nextChar(input, position);

  switch(_state) {
    BEGIN_STATE(DataState)
      if(c == '&') {
        buffer.append('&');
        ADVANCE_TO(EscapeState);
      } else if(c == '<') {
        if(token->type() == WebVTTToken::Uninitialized
           || token->isEmpty())
          ADVANCE_TO(TagState);
        else
          return emitAndResumeIn(WebVTTTokenizerState::TagState);
      } else if(c == endOfFileMark)
        return emitEndOfFile();
      else {
        bufferText(c);
        ADVANCE_TO(DataState);
      }
    END_STATE()

    BEGIN_STATE(EscapeState)
      if(c == ';') {
        uint32 out;
        if(isValidCharEntity(buffer.text(), out))
          bufferText(out);
        else {
          // If it's not a supported character entity, simply append the
          // buffer to the token.
          buffer.append(';');
          bufferText();
        }
        buffer.clear();
        ADVANCE_TO(DataState);
      } else if(c == '&') {
        // Append buffer to result, set buffer to c, and jump to the step
        // labeled next.
        bufferText();
        buffer.append(c);
        ADVANCE_TO(EscapeState);
      } else if(c == '<' || c == endOfFileMark) {
        // Append buffer to result, return a string token whose value is
        // result, and abort these steps.
        bufferText();
        if(c == '<')
          return emitAndResumeIn(WebVTTTokenizerState::TagState);
        return emitEndOfFile();
      } else if(Char::isAsciiAlphanumeric(c)) {
        // Append buffer to result, append c to result, set tokenizer state to
        // the WebVTT data state, and jump to the step labeled next.
        bufferText();
        bufferText(c);
        ADVANCE_TO(DataState);
      }
    END_STATE()

    BEGIN_STATE(TagState)
      if(Char::isHtml5Space(c)) {
        token->beginEmptyStartTag();
        ADVANCE_TO(StartTagAnnotationState);
      } else if(c == '.') {
        token->beginEmptyStartTag();
        ADVANCE_TO(StartTagClassState);
      } else if(c == '/') {
        token->beginEndTag();
        ADVANCE_TO(EndTagState);
      } else if(Char::isAsciiDigit(c)) {
        token->beginTimestampTag(c);
        ADVANCE_TO(TimestampTagState);
      } else if(c == '>' || c == endOfFileMark) {
        token->beginEmptyStartTag();
        return emitAndResumeIn(WebVTTTokenizerState::DataState);
      } else {
        token->beginStartTag(c);
        ADVANCE_TO(StartTagState);
      }
    END_STATE()

    BEGIN_STATE(StartTagState)
      if(Char::isHtml5Space(c))
        // Set tokenizer state to the WebVTT start tag annotation state, and
        // jump to the step labeled next.
        ADVANCE_TO(StartTagAnnotationState);
      else if(c == '.')
        ADVANCE_TO(StartTagClassState);
      else if(c == '>' || c == endOfFileMark)
        return emitAndResumeIn(WebVTTTokenizerState::DataState);
      else {
        appendName(c);
        ADVANCE_TO(StartTagState);
      }
    END_STATE()

    BEGIN_STATE(StartTagClassState)
      if(Char::isHtml5Space(c)) {
        // Append to classes an entry whose value is buffer, set buffer to the
        // empty string, set tokenizer state to the WebVTT start tag annotation
        // state, and jump to the step labeled next.
        appendClass();
        ADVANCE_TO(StartTagAnnotationState);
      } else if(c == '.') {
        // Append to classes an entry whose value is buffer, set buffer to the
        // empty string, and jump to the step labeled next.
        appendClass();
        ADVANCE_TO(StartTagClassState);
      } else if(c == '>' || c == endOfFileMark) {
        // Append to classes an entry whose value is buffer, then return a
        // start tag whose tag name is result, with the classes given in
        // classes but no annotation, and abort these steps.
        appendClass();
        return emitAndResumeIn(WebVTTTokenizerState::DataState);
      } else {
        // Append c to buffer and jump to the step labeled next.
        buffer.append(c);
        ADVANCE_TO(StartTagClassState);      
      }
    END_STATE()

    BEGIN_STATE(StartTagAnnotationState)
      if(c == '>' || c == endOfFileMark) {
        // Remove any leading or trailing space characters from buffer, and
        // replace any sequence of one or more consecutive space characters in
        // buffer with a single U+0020 SPACE character; then, return a start
        //tag whose tag name is result, with the classes given in classes, and
        // with buffer as the annotation, and abort these steps.
        appendAnnotation();
        return emitAndResumeIn(WebVTTTokenizerState::DataState);
      } else {
        // Don't buffer leading space characters, as they will be trimmed off
        // anyways
        if(buffer.length() > 0 || !Char::isHtml5Space(c))
          buffer.append(c);
        ADVANCE_TO(StartTagAnnotationState);
      }
    END_STATE()

    BEGIN_STATE(EndTagState)
      if(c == '>' || c == endOfFileMark)
        return emitAndResumeIn(WebVTTTokenizerState::DataState);
      appendName(c);
      ADVANCE_TO(EndTagState);
    END_STATE()

    BEGIN_STATE(TimestampTagState)
      if(c == '>' || c == endOfFileMark) {
        appendTimestamp();
        return emitAndResumeIn(WebVTTTokenizerState::DataState);
      }
      buffer.append(c);
      ADVANCE_TO(TimestampTagState);
    END_STATE()
  }

  assert(false); // Should never be reached
  return false;
}

bool
WebVTTTokenizer::isValidCharEntity(const char *entity, uint32 &out)
{
  struct CharEnt { const char *entity; uint32 ucs4; };
  const CharEnt ents[] = {
    { "&amp", ';' },
    { "&lt", '<' },
    { "&gt", '>' },
    { "&lrm", 0x200E },
    { "&rlm", 0x200F },
    { "&nbsp", 0x003B },
  };
  const int num = int(sizeof(ents) / sizeof(*ents));
  for(int i=0; i<num; ++i) {
    const CharEnt *ent = ents + i;
    if(!::strcmp(ent->entity, entity)) {
      out = ent->ucs4;
      return true;
    }
  }
  return false;
}

bool
WebVTTTokenizer::bufferText(unsigned long ch)
{
  assert(ch != endOfFileMark);
  token->ensureIsText();
  return token->appendData(ch);
}

bool
WebVTTTokenizer::bufferText()
{
  token->ensureIsText();
  bool ret = token->appendData(buffer);
  buffer.clear();
  return ret;
}

bool
WebVTTTokenizer::haveBufferedTextToken()
{
  return token->type() == WebVTTToken::Text;
}

bool
WebVTTTokenizer::appendName(uint c)
{
  assert(token->type() == WebVTTToken::StartTag
         || token->type() == WebVTTToken::EndTag);
  return token->appendData(c);
}

// Ensure that token is a StartTag, append the buffer to
// its class list, and clear the buffer.
bool
WebVTTTokenizer::appendClass()
{
  assert(token->type() == WebVTTToken::StartTag);
  String result;
  bool ret = buffer.toString(result) && token->appendClass(result);
  buffer.clear();
  return ret;
}

// Ensure that token is a StartTag, trim trailing whitespace
// from the buffer, and clear the buffer
bool
WebVTTTokenizer::appendAnnotation()
{
  assert(token->type() == WebVTTToken::StartTag);
  if(buffer.length() > 0) {
    while(Char::isHtml5Space(buffer[-1]))
      buffer.removeTrailingChar();
  }
  String result;
  bool ret = buffer.toString(result) && token->setAnnotation(result);
  buffer.clear();
  return ret;
}

// Ensure that token is a TimestampTag
bool
WebVTTTokenizer::appendTimestamp()
{
  assert(token->type() == WebVTTToken::TimestampTag);
  bool ret = token->appendData(buffer);
  buffer.clear();
  return ret;
}

bool
WebVTTTokenizer::emitAndResumeIn(State state)
{
  _state = state;
  return true;
}

bool
WebVTTTokenizer::emitEndOfFile()
{
  if(haveBufferedTextToken())
    return true;
  _state = WebVTTTokenizerState::DataState;
  token->reset();
  token->makeEndOfFile();
  return true;
}

} // TimedText