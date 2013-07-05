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

#ifndef __TimedText_Atomic__
#define __TimedText_Atomic__

#ifdef USE_NSPR_ATOMIC
#  undef USE_NSPR_ATOMIC
#endif
#ifdef USE_WTF_ATOMIC
#  undef USE_WTF_ATOMIC
#endif
#ifdef USE_LIBATOMIC_OPS_ATOMIC
#  undef USE_LIBATOMIC_OPS_ATOMIC
#endif

#if defined(USE_NSPR)
#  include <pratom.h>
#  if defined(_PR_HAVE_ATOMIC_OPS)
#    define USE_NSPR_ATOMIC
#  endif
#endif
#if defined(USE_WEBKIT)
#  include <wtf/Atomics.h>
#  define USE_WTF_ATOMIC
#endif
#if defined(USE_LIBATOMIC_OPS)
#  include <atomic_ops.h>
#  define USE_LIBATOMIC_OPS_ATOMIC
#endif

namespace TimedText
{

#ifdef HAVE_ATOMIC
#  undef HAVE_ATOMIC
#endif
#if defined(USE_NSPR_ATOMIC)
// Using NSPR's atomic ops to implement atomic ints
// This code is based on the NSPR 4.10 API, but may
// work in older versions (or newer ones) as well
class AtomicInt
{
public:
  PRInt32 value;
  // Non-atomic ops:
  AtomicInt() : value(0) {}
  AtomicInt(int val) : value(val) {}
  operator int() const { return int(value); }

  // Atomic ops:
  bool ref() {
    // Atomically increment self
    return PR_ATOMIC_INCREMENT(&value) != 0;
  }

  bool deref() {
    return PR_ATOMIC_DECREMENT(&value) != 0;
  }
};
#define HAVE_ATOMIC
#endif

#if !defined(HAVE_ATOMIC) && defined(USE_WTF_ATOMIC)
// Using WebKit's WTF library for atomic ops
class AtomicInt
{
public:
  int value;
  // Non-atomic ops:
  AtomicInt() : value(0) {}
  AtomicInt(int val) : value(val) {}
  operator int() const { return int(value); }

  // Atomic ops:
  bool ref() {
    // Atomically increment self
    return WTF::atomicIncrement(&value) != 0;
  }

  bool deref() {
    return WTF::atomicDecrement(&value) != 0;
  }
};
#define HAVE_ATOMIC
#endif

#if !defined(HAVE_ATOMIC) && defined(USE_LIBATOMIC_OPS_ATOMIC)
// Using libatomic_ops library for atomic ops implementation
class AtomicInt
{
public:
  volatile ::AO_t value;
  // Non-atomic ops:
  AtomicInt() : value(0) {}
  AtomicInt(int val) : value(val) {}
  operator int() const { return int(value); }

  // Atomic ops:
  bool ref() {
    // Atomically increment self
    return ::AO_fetch_and_add1_release_write(&value) != 0;
  }

  bool deref() {
    return ::AO_fetch_and_sub1_release_write(&value) != 0;
  }
};
#define HAVE_ATOMIC
#endif

#if !defined(HAVE_ATOMIC)
// implement non-atomic version here, for convenience
class AtomicInt
{
public:
  int value;
  AtomicInt() : value(0) {}
  AtomicInt(int val) : value(val) {}
  operator int() const { return int(value); }

  bool ref() {
    // Atomically increment self
    return ++value != 0;
  }

  bool deref() {
    return --value != 0;
  }
};
#endif

} // TimedText

#endif // __TimedText_Atomic__
