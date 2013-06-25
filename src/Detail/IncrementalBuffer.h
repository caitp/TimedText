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

#ifndef __TimedText_IncrementalBuffer_Detail__
#define __TimedText_IncrementalBuffer_Detail__

#include <TimedText/IncrementalBuffer.h>

namespace TimedText
{

// This object is shared between the thread controlling the
// parser, and the parser's thread itself (they should not be
// the same thread).
//
// Details regarding locking/unlocking of buffers is left up to
// the implementation.
class IncrementalBufferDetail
{
public:
  virtual ~Detail() = 0;

  // Return true if the end of the document has been reached.
  // Otherwise return false
  virtual bool eof() const = 0;

  // Discard bytes from the front of the buffer
  virtual void discard(int bytes) = 0;

  // Read up to 'maximum' bytes from the front of the buffer
  // do not discard them.
  virtual int read(char buffer[], int maximum) = 0;
};

} // TimedText

#endif // __TimedText_IncrementalBuffer_Detail__
