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


#ifndef __TimedText_String__
#define __TimedText_String__

#include <TimedText/Char.h>
#include <TimedText/Unicode.h>
#include <cstring>

namespace TimedText
{

class StringBuilder;

// String is an immutable, shared, UTF-8 encoded representation of text.
class String
{
public:
  String() : d(&sharedEmpty) { ++d->ref; }
	explicit String(const char *utf8, int len=-1);
  ~String();
	String(const String &str);
	String &operator=(const String &str);
  String &operator+=(const String &str);

	// Returns true if the string is empty or NULL.
	inline bool isEmpty() const {
    return d->length == 0;
  }

  inline int length() const {
    return d->length;
  }

  inline int size() const {
    return d->length;
  }

	inline const char *text() const {
		return d->text;
	}

  inline char operator[](int i) const {
    if(i < 0 || i >= d->length)
      return '\0';
    return d->text[i];
  }

  void clear();

  int indexOf(const char *text, int len=-1, int from = 0) const;
  inline int indexOf(const String &str, int from = 0) const {
    return indexOf(str.text(), str.length(), from);
  }

  // Return true if string contains 'text'
  bool contains(const char *text, int len=-1, int from = 0) const;
  inline bool contains(const String &other, int from = 0) const {
    return contains(other.text(),other.length(), from);
  }
  template <size_t N>
  inline bool contains(const char (&arr)[N], int from = 0) {
    if(N>0 && arr[N-1]=='\0')
      return contains(arr,N-1,from);
    return contains(arr, N, from);
  }
  bool contains(unsigned long ucs4, int from = 0) const;

  // Return true if string starts with 'text'
  bool startsWith(const char *text, int len=-1) const;
  inline bool startsWith(const String &other) const {
    return startsWith(other.text(),other.length());
  }
  template <size_t N>
  inline bool startsWith(const char (&arr)[N]) {
    if(N>0 && arr[N-1]=='\0')
      return startsWith(arr,N-1);
    return startsWith(arr, N);
  }
  bool startsWith(unsigned long ucs4) const;

  // Return true if string ends with 'text'
  bool endsWith(const char *text, int len=-1) const;
  inline bool endsWith(const String &other) const {
    return endsWith(other.text(),other.length());
  }
  template <size_t N>
  inline bool endsWith(const char (&arr)[N]) {
    if(N>0 && arr[N-1]=='\0')
      return endsWith(arr,N-1);
    return endsWith(arr, N);
  }
  bool endsWith(unsigned long ucs4) const;

  int parseInt(int &position, int *digits) const;
  int skipWhitespace(int &position) const;

  String substring(int position) const;
  String substring(int position, int length) const;

private:
  friend class StringBuilder;
  static int findString(const char *bucket, int bucket_len, int from,
                        const char *needle, int needle_len);
  static int findStringBoyerMoore(const char *bucket, int bucket_len, int from,
                                  const char *needle, int needle_len);
	struct Data
	{
		int ref;
		int length;
		char text[1];
	};
	Data *d;
  static Data sharedEmpty;
  static Data sharedNull;
};

bool isAsciiDigit(char c);

} // TimedText

#endif // __TimedText_String__