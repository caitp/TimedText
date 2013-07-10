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

#ifndef __TimedText_Types__
#define __TimedText_Types__

// You're going to have stdint.h, and if you don't, it's
// not my problem.
#if !defined(_MSC_VER) || _MSC_VER > 1500
#  include <stdint.h>
#endif

namespace TimedText
{

// I need this... It's very important ;-;
typedef unsigned int uint;

// Other similar types are okay too...
// ... but rather unlikely to be used.
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

#if !defined(_MSC_VER) || _MSC_VER > 1500
typedef ::int8_t int8;
typedef ::int16_t int16;
typedef ::int32_t int32;
typedef ::int64_t int64;
typedef ::uint8_t uint8;
typedef ::uint16_t uint16;
typedef ::uint32_t uint32;
typedef ::uint64_t uint64;
#elif defined(_MSC_VER)
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;
typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
#else
#  error "Don't know int64 type for this platform. File a bug!"
#endif

// Typeinfo, needed for container class magic
template <typename T>
class TypeInfo
{
public:
  enum {
    isPointer = false,
    isComplex = true,
    isStatic = true,
    isLarge = (sizeof(T)>sizeof(void*)),
    isDummy = false
  };
};

template <typename T>
class TypeInfo<T*>
{
public:
  enum {
    isPointer = true,
    isComplex = false,
    isStatic = false,
    isLarge = false,
    isDummy = false
  };
};

enum {
  TT_COMPLEX_TYPE = 0,
  TT_PRIMITIVE_TYPE = 1,
  TT_STATIC_TYPE = 0,
  TT_MOVABLE_TYPE = 2,
  TT_DUMMY_TYPE = 4,
};

#define TT_DECLARE_TYPEINFO_INTERNAL(TYPE,FLAGS) \
class TypeInfo<TYPE> \
{ \
public: \
  enum { \
    isComplex = (((FLAGS) & TT_PRIMITIVE_TYPE) == 0), \
    isStatic = (((FLAGS) & (TT_MOVABLE_TYPE | TT_PRIMITIVE_TYPE)) == 0), \
    isLarge = (sizeof(TYPE)>sizeof(void*)), \
    isPointer = false, \
    isDummy = (((FLAGS) & TT_DUMMY_TYPE) != 0) \
  }; \
  static inline const char *name() { return #TYPE; } \
}

#define TT_DECLARE_TYPEINFO(TYPE,FLAGS) \
template<> \
TT_DECLARE_TYPEINFO_INTERNAL(TYPE,FLAGS)

TT_DECLARE_TYPEINFO(bool,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(char,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(uchar,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(short,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(ushort,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(int,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(uint,TT_PRIMITIVE_TYPE);
#if !defined(__WORDSIZE) || __WORDSIZE != 64
TT_DECLARE_TYPEINFO(long,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(ulong,TT_PRIMITIVE_TYPE);
#endif
TT_DECLARE_TYPEINFO(int64,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(uint64,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(float,TT_PRIMITIVE_TYPE);
TT_DECLARE_TYPEINFO(double,TT_PRIMITIVE_TYPE);

template<unsigned bytes> struct IntForSize { };
template<> struct IntForSize<1>{ typedef int8 type; };
template<> struct IntForSize<2>{ typedef int16 type; };
template<> struct IntForSize<4>{ typedef int32 type; };
template<> struct IntForSize<8>{ typedef int64 type; };

template<unsigned bytes> struct UintForSize { };
template<> struct UintForSize<1>{ typedef uint8 type; };
template<> struct UintForSize<2>{ typedef uint16 type; };
template<> struct UintForSize<4>{ typedef uint32 type; };
template<> struct UintForSize<8>{ typedef uint64 type; };

typedef IntForSize<sizeof(void *)>::type ptrdiff;
typedef UintForSize<sizeof(void *)>::type uintptr;

}

#endif // __TimedText_Types__
