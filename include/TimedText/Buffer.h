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
    isSynchronous = 0,
    isAsynchronous = 1,
  };
  Buffer(Flags flags);
  ~Buffer();

  // Locking operations
  virtual void lock() = 0;
  virtual void unlock() = 0;

	virtual bool eof() const = 0;

  virtual bool seek(int pos, bool abs = false) = 0;

  virtual int pos() const = 0;

  // Pull bytes from front of buffer, to conserve memory
  virtual void discard(int bytes) { seek(bytes); };
  void discard() { discard(pos()); }

  // Load new data
  virtual void refill(const char utf8[], int count) = 0;

  // Return 'true' if line is read,
  bool getline(String &result) const;

  // Read text as UTF-8, without seeking
  int read(char buffer[], int maximum);

protected:
  Flags flags;
  StringBuilder buffer;
};

} // TimedText

#endif // __TimedText_Buffer__
