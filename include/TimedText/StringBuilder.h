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
  StringBuilder(int capacity, bool &result);
  ~StringBuilder();

  bool toString(String &result) const;
  int length() const;
  int size() const;
  int capacity() const;
  void clear();

  int indexOf(const char *text, int len=-1) const;
  int indexOf(const String &str) const {
    return indexOf(str.text(), str.length());
  }

  bool reserve(int capacity);
  bool append(unsigned long ucs4);
  bool append(const char *text, int len);
  inline bool append(const String &str) {
    return append(str.text(), str.length());
  }
  template <size_t N>
  inline bool append(const char (&arr)[N]) {
    return append(arr, N);
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
  bool insert(int idx, unsigned long ucs4);
  bool insert(int idx, const char *text, int len);
  inline bool insert(int idx, const String &str) {
    return insert(idx, str.text(), str.length());
  }
  template <size_t N>
  inline bool insert(int idx, const char (&arr)[N]) {
    return insert(idx, arr, N);
  }
  bool replaceAll(const char *search, int len,
                  const char *repl, int rlen);
  bool replaceAll(const char *search, int len,
                  unsigned long replace);
  bool replaceAll(unsigned long search,
                  const char *replace, int rlen);
  bool replaceAll(unsigned long search,
                  unsigned long replace);

private:
  struct Data
  {
    int alloc;
    int length;
    char text[1];
  };
  Data *d;
};

}

#endif // __TimedText_StringBuilder__
