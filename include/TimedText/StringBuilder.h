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
  StringBuilder(const String &str, bool &result);
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

  // replace text with contents of string
  bool setText(const String &string);

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
  bool append(uint32 ucs4);
  bool append(const char *text, int len);
  inline bool append(const String &str) {
    return append(str.text(), str.length());
  }
  template <int N>
  inline bool append(const char (&arr)[N]) {
    int n = N;
    if(n > 0 && arr[n-1] == '\0')
      --n;
    return append(arr, n);
  }
  inline bool append(const StringBuilder &buf) {
    return append(buf.text(), buf.length());
  }

  bool prepend(uint32 ucs4);
  inline bool prepend(const char *text, int len) {
    return insert(0, text, len);
  }
  inline bool prepend(const String &str) {
    return insert(0, str.text(), str.length());
  }
  template <int N>
  inline bool prepend(const char (&arr)[N]) {
    int n = N;
    if(n > 0 && arr[n-1] == '\0')
      --n;
    return insert(0, arr, n);
  }
  inline bool prepend(const StringBuilder &buf) {
    return insert(0, buf.text(), buf.length());
  }
  bool insert(int idx, uint32 ucs4);
  bool insert(int idx, const char *text, int len);
  inline bool insert(int idx, const String &str) {
    return insert(idx, str.text(), str.length());
  }
  template <int N>
  inline bool insert(int idx, const char (&arr)[N]) {
    int n = N;
    if(n > 0 && arr[n-1] == '\0')
      --n;
    return insert(idx, arr, n);
  }
  inline bool insert(int i, const StringBuilder &buf) {
    return insert(i, buf.text(), buf.length());
  }
  bool replaceAll(const char *search, int len, const char *repl, int rlen);
  bool replaceAll(const char *search, int len, uint32 replace);
  bool replaceAll(uint32 search, const char *replace, int rlen);
  bool replaceAll(uint32 search, uint32 replace);
  template <int N>
  inline bool replaceAll(const char (&search)[N], uint32 replace) {
    int n = N;
    if(n > 0 && search[n-1] == '\0')
      --n;
    return replaceAll(search, n, replace);
  }
  template <int N>
  inline bool replaceAll(uint32 search, const char (&replace)[N]) {
    int n = N;
    if(n > 0 && replace[n-1] == '\0')
      --n;
    return replaceAll(search, replace, n);
  }
  template <int N, int M>
  inline bool replaceAll(const char (&search)[N], const char (&replace)[M]) {
    int n = N;
    int m = M;
    if(n > 0 && search[n-1] == '\0')
      --n;
    if(m > 0 && replace[m-1] == '\0')
      --m;
    return replaceAll(search, n, replace, m);
  }

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
  bool resizeData(int size);
  void freeData();
  bool replaceHelper(unsigned *, int, int, const char *, int);
  bool expand(int i);

  static Data empty;
};

TT_DECLARE_TYPEINFO(StringBuilder, TT_MOVABLE_TYPE);

}

#endif // __TimedText_StringBuilder__
