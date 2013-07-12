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

#ifndef __TimedText_Buffer__
#define __TimedText_Buffer__

#include <TimedText/StringBuilder.h>

namespace TimedText
{

class Buffer
{
public:
  enum Flags
  {
    Synchronous = 0,
    Asynchronous = 1,
    SynchronousMask = (Synchronous|Asynchronous),
    Sleeping = 2,
  };
  Buffer(Flags flags);
  ~Buffer();

  inline bool isSleeping() const
  {
    return (flags & Sleeping) != 0;
  }

  inline bool isSynchronous() const
  {
    return (flags & SynchronousMask) == Synchronous;
  }

  inline bool isAsynchronous() const
  {
    return (flags & SynchronousMask) == Asynchronous;
  }

  // Locking operations (These should be blocking, with high priority)
  virtual void lock() = 0;
  virtual void unlock() = 0;
  // Put buffer thread to sleep (should wake from sleep after refilling)
  virtual void sleep() = 0;

  inline bool eof() const
  {
    return (i >= buffer.size() && final);
  }

  inline int pos() const
  {
    return i;
  }

  inline int size() const
  {
    return buffer.size();
  }

  inline int length() const
  {
    return buffer.size();
  }

  inline int remaining() const
  {
    return size() - pos();
  }

  inline bool isFinal() const
  {
    return final;
  }

  const char *curr() const;

  // Seek is not threadsafe and does not lock.
  // Only use if certain of unique ownership.
  bool seek(int pos, bool abs = false);

  // Pull bytes from front of buffer, to conserve memory
  //
  // performDiscard should not use locking operations, as
  // these are taken care of by the wrapping 'discard'
  // routine
  virtual void performDiscard(int bytes);
  void discard(int bytes);
  void discard();

  // Load new UTF8-encoded data into buffer.
  bool refill(const char utf8[], int count, bool final = false);
  template<size_t N>
  inline bool refill(const char (&utf8)[N], bool final = false)
  {
    if(N > 0 && utf8[N-1] == '\0')
      return refill(utf8, N-1, final);
    return refill(utf8, N, final);
  }
  inline bool finish() { return refill("", 0, true); }

  // Return 'true' if line is read, 0x400 bytes is a pretty generous
  // default maximum line size.
  bool getline(String &result, int maxlen = 0x400);
  bool skipline();

  // Read text as UTF-8, without seeking
  int read(char buffer[], int maximum);

  // Read char and increase position by one byte
  // Does not lock! Be sure to own buffer before
  // calling.
  bool next(char &result);

  bool collectWord(String &result, int *len = 0);
  bool collectDigits(String &result, int *len = 0);
  bool skipWhitespace(int *len = 0);

protected:
  Flags flags;
  bool final;
  int i;
  StringBuilder buffer;
};

} // TimedText

#endif // __TimedText_Buffer__
