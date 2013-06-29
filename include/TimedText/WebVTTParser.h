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

#ifndef __TimedText_WebVTTParser__
#define __TimedText_WebVTTParser__

#include <TimedText/Buffer.h>

namespace TimedText
{

class WebVTTParser
{
public:
  enum ParseState {
    Initial,
    Header,

    Id,
    TimingsAndSettings,
    CueText,
    BadCue
  };
  enum Status {
    Finished,
    Unfinished,
    Aborted,
  };
  WebVTTParser(Buffer &buffer);
  ~WebVTTParser();

  // Parse the document
  bool parse(Status *status = 0);

  static inline bool isValidSignatureDelimiter(char c)
  {
    // Currently, the 'WEBVTT' signature must be followed by either a
    // SPACE (U+0020), TAB (U+0009), CARRIAGE RETURN (U+000D) or LINE FEED
    // (U+000A), but not a FORM FEED
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
  }

private:
  bool parseHeader();

  Buffer &buffer;
  String line;
  ParseState state;
  Status status;
};

} // TimedText

#endif // __TimedText_WebVTTParser__
