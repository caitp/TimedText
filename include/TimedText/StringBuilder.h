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

#ifndef __TimedText_StringBuilder__
#define __TimedText_StringBuilder__

#include <TimedText/String.h>

namespace TimedText
{

// StringBuilder is a mutable, unshared, UTF-8 encoded representation of text.
// It is used to construct String objects.
//
// TimedText has a no-C++-exceptions policy, and so any method which allocates
// memory returns a boolean value representing the success of the allocation.
// The only exception is the constructor.
//
// Prepend and insert routines are added to the interface, but are currently
// not implemented and merely return false. This is primarily for binary
// compatibility.
//
// Bad UTF8 and unicode non-characters are converted to the unicode replacement
// character (U+FFFD).
class StringBuilder
{
public:
  StringBuilder();
  StringBuilder(int capacity, bool &result);
  ~StringBuilder();

  bool toString(String &result) const;
  inline int length() const {
    return d->length;
  }
  inline int size() const {
    return d->length;
  }

  inline bool isEmpty() const {
    return d->length == 0;
  }

  inline int capacity() const {
    return d->alloc;
  }

  inline const char *text() const {
    return d->text;
  }

  void clear();

  char operator[](int i) const
  {
    if(i < 0)
      i += size();
    if(i < 0 || i >= size())
      return '\0';
    return d->text[i];
  }

  int indexOf(const char *text, int len=-1, int from = 0) const;
  int indexOf(const String &str, int from = 0) const {
    return indexOf(str.text(), str.length(), from);
  }

  bool reserve(int capacity);
  bool appendByte(char b);
  bool append(unsigned long ucs4);
  bool append(const char *text, int len);
  inline bool append(const String &str) {
    return append(str.text(), str.length());
  }
  template <size_t N>
  inline bool append(const char (&arr)[N]) {
    if(N > 0 && arr[N-1] == '\0')
      return append(arr, N-1);
    return append(arr, N);
  }
  inline bool append(const StringBuilder &buf) {
    return append(buf.text(), buf.length());
  }

  bool prepend(unsigned long ucs4);
  bool prepend(const char *text, int len);
  inline bool prepend(const String &str) {
    return prepend(str.text(), str.length());
  }
  template <size_t N>
  inline bool prepend(const char (&arr)[N]) {
    return prepend(arr, N);
  }
  inline bool prepend(const StringBuilder &buf) {
    return prepend(buf.text(), buf.length());
  }
  bool insert(int idx, unsigned long ucs4);
  bool insert(int idx, const char *text, int len);
  inline bool insert(int idx, const String &str) {
    return insert(idx, str.text(), str.length());
  }
  template <size_t N>
  inline bool insert(int idx, const char (&arr)[N]) {
    return insert(idx, arr, N);
  }
  inline bool insert(int i, const StringBuilder &buf) {
    return insert(i, buf.text(), buf.length());
  }
  bool replaceAll(const char *search, int len,
                  const char *repl, int rlen);
  bool replaceAll(const char *search, int len,
                  unsigned long replace);
  bool replaceAll(unsigned long search,
                  const char *replace, int rlen);
  bool replaceAll(unsigned long search,
                  unsigned long replace);

  void removeTrailingChar();

private:
  struct Data
  {
    static Data *allocate(Data *old, int size);
    int alloc;
    int length;
    char text[1];
  };
  Data *d;
  bool reallocData(int alloc, bool grow = false);
  void freeData();

  static Data empty;
};

TT_DECLARE_TYPEINFO(StringBuilder, TT_MOVABLE_TYPE);

}

#endif // __TimedText_StringBuilder__
