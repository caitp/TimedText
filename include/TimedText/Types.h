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
typedef ::int64_t int64;
typedef ::uint64_t uint64;
#elif defined(_MSC_VER)
typedef __int64 int64;
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

}

#endif // __TimedText_Types__
