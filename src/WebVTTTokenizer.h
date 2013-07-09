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

#ifndef __TimedText_WebVTTTokenizer__
#define __TimedText_WebVTTTokenizer__

#include <TimedText/WebVTTParser.h>
#include <TimedText/StringBuilder.h>
#include <TimedText/Timestamp.h>
#include <TimedText/List.h>

namespace TimedText
{

class WebVTTToken
{
public:
  enum Type
  {
    Uninitialized,
    Text,
    StartTag,
    EndTag,
    TimestampTag,
    EndOfFile
  };

  WebVTTToken();
  ~WebVTTToken();

  inline Type type() const {
    return _type;
  }

  inline bool isEmpty() const {
    return _data.isEmpty() && _classes.isEmpty() && _annotation.isEmpty();
  }
  void reset();
  bool data(String &result) const {
    return _data.toString(result);
  }

  Timestamp timestamp() const {
    String string;
    if(type() == TimestampTag && data(string)) {
      int pos = 0;
      Timestamp ts = WebVTTParser::collectTimestamp(string, pos);
      if(pos == string.length())
        return ts;
    }
    return MalformedTimestamp;
  }

  String annotation() const {
    return _annotation;
  }

  List<String> classes() const
  {
    return _classes;
  }

  bool appendByte(char b);
  bool appendData(unsigned long ch);
  bool appendData(const StringBuilder &buf);
  bool appendClass(const String &styleClass);
  inline bool setAnnotation(const String &annotation) {
    _annotation = annotation;
    return true;
  }
  void makeEndOfFile();
  void ensureIsText();

  void beginEmptyStartTag();
  void beginStartTag(char c);
  void beginTimestampTag(char c);
  void beginEndTag();
  
private:
  Type _type;
  // Text, StartTag, EndTag, TimestampTag
  StringBuilder _data;
  // StartTag (Classes)
  List<String> _classes;
  // StartTag (Annotation)
  String _annotation;
};

class WebVTTTokenizerState
{
public:
  enum State
  {
    DataState,
    EscapeState,
    TagState,
    StartTagState,
    StartTagClassState,
    StartTagAnnotationState,
    EndTagState,
    TimestampTagState
  };
};

class WebVTTTokenizer
{
public:
  typedef WebVTTTokenizerState::State State;
  WebVTTTokenizer();
  ~WebVTTTokenizer();
  uint32 nextChar(const String &input, int &position);
  void reset();
  bool next(const String &input, int &position, WebVTTToken &result);

  // Return true if token contains buffered data and is a Text token
  bool haveBufferedTextToken();
  bool bufferByte(char c);
  bool bufferText(unsigned long ch);
  bool bufferText();
  bool appendName(uint c);
  bool appendClass();
  bool appendAnnotation();
  bool appendTimestamp();
  static bool isValidCharEntity(const char *text, uint32 &out);

private:
  bool emitAndResumeIn(State state);
  bool emitEndOfFile();
  WebVTTToken *token;
  State _state;
  StringBuilder buffer;
};

} // TimedText

#endif // __TimedText_WebVTTTokenizer__
