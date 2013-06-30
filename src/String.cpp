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

#include <TimedText/String.h>
#include <cstring>
#include <climits>
#include <cstdlib>

namespace TimedText
{

String::Data String::sharedEmpty = { 1, 0, { '\0' } };
String::Data String::sharedNull = { 1, 0, { '\0' } };

String::String(const char *utf8, int len)
{
  if(!utf8) {
    d = &sharedNull; 
    ++d->ref;
  } else {
    if(len < 0)
      len = ::strlen(utf8);
    if(len == 0) {
      d = &sharedEmpty;
    } else {
      int alloc = Unicode::utf8Length(utf8,len);
      d = static_cast<Data *>(::malloc(sizeof(Data) + alloc));
      if(!d) {
        d = &sharedNull;
      } else {
        d->ref = 0;
        Unicode::toValidUtf8(d->text, alloc+1, d->length, utf8, len);
      }
      ++d->ref;
    }
  }
}

String::String(const String &str)
  : d(str.d)
{
  d->ref++;
}

String::~String()
{
  if(!--d->ref)
    ::free(d);
}

String &
String::operator=(const String &str)
{
  Data *x = d;
  d = str.d;
  ++d->ref;
  if(!--x->ref)
    ::free(x);
  return *this;
}

String &
String::operator+=(const String &str)
{
  int n = size() + str.size();
  Data *x = static_cast<Data *>(::malloc(sizeof(Data) + n));
  x->ref = 1;
  x->length = n;
  ::memcpy(x->text, d->text, d->length);
  ::memcpy(x->text + d->length, str.text(), str.length());;
  x->text[n] = '\0';
  Data *y = d;
  d = x;
  if(!--y->ref)
    ::free(y);
  return *this;
}

void
String::clear()
{
  Data *old = d;
  d = &sharedEmpty;
  ++d->ref;
  if(!--old->ref)
    ::free(old);
}

int
String::indexOf(const char *find, int len, int from) const
{
  if(!find)
    return -1;
  if(len < 0)
    len = ::strlen(find);
  return findString(text(), length(), from, find, len);
}

bool
String::contains(const char *find, int len, int from) const
{
  if(!find)
    return -1;
  if(len < 0)
    len = ::strlen(find);
  return findString(text(), length(), from, find, len) >= 0;
}

bool
String::contains(unsigned long ucs4, int from) const
{
  char sw[8];
  int sl;
  Unicode::toUtf8(ucs4, sw, sl);
  return contains(sw, sl, from);
}

bool
String::startsWith(const char *find, int len) const
{
  if(!find)
    return false;
  if(len < 0)
    len = ::strlen(find) - 1;
  return findString(text(), length(), 0, find, len) == 0;
}

bool
String::startsWith(unsigned long ucs4) const
{
  char sw[8];
  int sl;
  Unicode::toUtf8(ucs4, sw, sl);
  return startsWith(sw,sl);
}

bool
String::endsWith(const char *find, int len) const
{
  if(!find)
    return -1;
  if(len < 0)
    len = ::strlen(find);
  int from = length() - len;
  return findString(text(), length(), from, find, len) == from;
}

bool
String::endsWith(unsigned long ucs4) const
{
  char sw[8];
  int sl;
  Unicode::toUtf8(ucs4, sw, sl);
  return endsWith(sw,sl);
}

int
String::parseInt(int &position, int *digits) const
{
  // Radix is always 10 here
  const int radix = 10;
  int value = 0;
  int i = 0;
  if(digits)
    *digits = 0;
  if(position < 0 || position >= length())
    return 0;
  // TODO:
  // Can we make this not suck for EBCDIC systems? Do we care?
  for( ; position < length() && Char::isAsciiDigit(d->text[position]); ++i )
    value = (value * radix) + d->text[position++] - '0';
  if(digits)
    *digits = i;
  return value;
}

int
String::parseInt(const char *buffer, int len)
{
  if(!buffer)
    return 0;
  if(len < 0)
    len = ::strlen(buffer);
  if(len == 0)
    return 0;
  // Radix is always 10 here
  const int radix = 10;
  bool neg = false;
  int value = 0;
  int i = 0;
  int d = 0;
  for( int i=0; i<len; ++i) {
    char c = buffer[i];
    if(c == '-' && i == 0) {
      neg = true;
    } else if(Char::isAsciiDigit(c)) {
      ++d;
      value = (value * radix) + c - '0';
    } else {
      break;
    }
  }
  if(!d)
    return 0;
  if(neg)
    value = -value;
  return value;
}

int
String::skipWhitespace(int &position) const
{
  int i = 0;
  if(position < 0 || position >= length())
    return 0;
  for( ; position < length() && Char::isHtml5Space(d->text[position]); ++i)
    ++position;
  return i;
}

String
String::substring(int position) const
{
  if(position < 0 || position >= length())
    return String();
  return String(d->text + position, d->length - position);
}

String
String::substring(int position, int len) const
{
  if(len < 0)
    return substring(position);
  if(len == 0 || position < 0 || position >= length())
    return String();
  if(position + len >= length())
    len = length() - position;
  return String(d->text + position, len);
}

bool
String::collectWord(int &position, char out[], int max) const
{
  int n = 0;
  if(position < 0 || position >= length() || max <= 0)
    return false;
  for( ; position < length() && !Char::isHtml5Space(d->text[position]);
         out[n++] = d->text[position++]);
  if(n < max) {
    out[n] = '\0';
    return true;
  }
  return false;
}

int
String::skipUntilWhitespace(int &position) const
{
  int n = 0;
  if(position < 0 || position >= length())
    return 0;
  for( ; position < length() && !Char::isHtml5Space(d->text[position]);
       ++n, ++position);
  return n;
}

static inline int
findChar(const char *bucket0, int bucketLen, char needle, int from)
{
  const char *c = ::strchr(bucket0 + from, needle);
  return c ? c - bucket0 : -1;
}

#define REHASH(a) \
  if (sl_minus_1 < (int)sizeof(int) * CHAR_BIT) \
    hashBucket -= (a) << sl_minus_1; \
  hashBucket <<= 1

int
String::findString(const char *bucket0, int bucketLen, int from,
                   const char *needle0, int needleLen)
{
  const int l = bucketLen;
  const int sl = needleLen;
  if(from < 0)
    from += l;
  if (unsigned(sl + from) > (unsigned)l)
    return -1;
  if (!sl)
    return from;
  if (!l)
    return -1;

  if (sl == 1)
    return findChar(bucket0, bucketLen, needle0[0], from);

  // We use the Boyer-Moore algorithm in cases where the overhead
  // for the skip table should pay off, otherwise we use a simple
  // hash function.
  if (l > 500 && sl > 5)
    return findStringBoyerMoore(bucket0, bucketLen, from,
                                needle0, needleLen);

  // We use some hashing for efficiency's sake. Instead of
  // comparing strings, we compare the hash value of str with that
  // of a part of this QString. Only if that matches, we call
  // ucstrncmp() or ucstrnicmp().
  const char *needle = (const char *)needle0;
  const char *bucket = (const char *)bucket0 + from;
  const char *end = (const char *)bucket0 + (l-sl);
  const int sl_minus_1 = sl-1;
  int hashNeedle = 0, hashBucket = 0, idx;

  for (idx = 0; idx < sl; ++idx) {
    hashNeedle = ((hashNeedle<<1) + needle[idx]);
    hashBucket = ((hashBucket<<1) + bucket[idx]);
  }
  hashBucket -= bucket[sl_minus_1];

  while (bucket <= end) {
    hashBucket += bucket[sl_minus_1];
    if (hashBucket == hashNeedle && strncmp(needle, bucket, sl) == 0)
      return bucket - bucket0;

    REHASH(*bucket);
    ++bucket;
  }
  return -1;
}

static void
bm_init_skiptable(const char *uc, int len, unsigned char *skiptable)

{
  int l = len < 255 ? len : 255;
  memset(skiptable, l, 256*sizeof(unsigned char));
  uc += len - l;
  while (l--) {
    skiptable[*uc & 0xff] = l;
    uc++;
  }
}

static inline int
bm_find(const char *uc, unsigned int l, int index,
        const char *puc, unsigned int pl,
        const unsigned char *skiptable)
{
  if (pl == 0)
    return index > (int)l ? -1 : index;
  const unsigned int pl_minus_one = pl - 1;
  register const char *current = uc + index + pl_minus_one;
  const char *end = uc + l;
  while (current < end) {
    unsigned int skip = skiptable[*current & 0xff];
    if (!skip) {
      // possible match
      while (skip < pl) {
        if (*(current - skip) != puc[pl_minus_one-skip])
          break;
        skip++;
      }
      if (skip > pl_minus_one) // we have a match
        return (current - uc) - pl_minus_one;
      // in case we don't have a match we are a bit inefficient as we only skip by one
      // when we have the non matching char in the string.
      if (skiptable[*(current - skip) & 0xff] == pl)
        skip = pl - skip;
      else
        skip = 1;
    }
    if (current > end - skip)
      break;
    current += skip;
  }
  return -1; // not found
}

int
String::findStringBoyerMoore(const char *bucket, int bucketLen, int from,
                             const char *needle, int needleLen)
{
  unsigned char skiptable[256];
  bm_init_skiptable(needle, needleLen, skiptable);

  if (from < 0)
    from = 0;

  return bm_find(bucket, bucketLen, from,
                 needle, needleLen, skiptable);
}

}
