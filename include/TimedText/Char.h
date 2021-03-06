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

#ifndef __TimedText_Char__
#define __TimedText_Char__

#include <TimedText/Types.h>

namespace TimedText
{

// Character-specific methods and routines should be placed here if they are
// not specific to a single format. If the routine is specific to a single
// TimedText document format, it should be placed in that format's parser
// class.
class Char
{
public:
  // Return true if 'c' is an HTML5 space character (U+0020, U+0009, U+000A,
  // U+000D, or U+000C)
  // HTML5 2.4.1
  static inline bool isHtml5Space(uint32 c)
  {
    return c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\r';
  }

  // Return true if 'c' is an ASCII digit (inclusively between '0' and '9')
  // HTML5 2.4.1
  static inline bool isAsciiDigit(uint32 c)
  {
    return (c >= '0' && c <= '9');
  }

  // HTML5 2.4.1
  static inline bool isAsciiLetter(uint32 c)
  {
    return isAsciiUppercase(c) || isAsciiLowercase(c);
  }

  // HTML5 2.4.1
  static inline bool isAsciiUppercase(uint32 c)
  {
    return c >= 'A' && c <= 'Z';
  }

  // HTML5 2.4.1
  static inline bool isAsciiLowercase(uint32 c)
  {
    return c >= 'a' && c <= 'z';
  }

  // HTML5 2.4.1
  static inline bool isAsciiAlphanumeric(uint32 c)
  {
    return isAsciiLetter(c) || isAsciiDigit(c);
  }

  // Return true if 'c' is an ASCII newline character
  static inline bool isNewlineChar(uint32 c)
  {
    return (c == '\r' || c == '\n');
  }
};


} // TimedText

#endif // __TimedText_Char__
