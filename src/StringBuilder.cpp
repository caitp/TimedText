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

#include <TimedText/StringBuilder.h>
#include "StringData.h"
#include "Utility.h"
#include <cstdlib>

namespace TimedText
{

StringBuilder::Data StringBuilder::empty = { 0, 0, { '\0' } };

StringBuilder::StringBuilder()
  : d(&empty)
{
}

StringBuilder::StringBuilder(int capacity, bool &result)
  : d(&empty)
{
  result = reallocData(capacity, true);
}

StringBuilder::StringBuilder(const String &str, bool &result)
  : d(&empty)
{
  result = setText(str);
}

StringBuilder::~StringBuilder()
{
  freeData();
}

bool
StringBuilder::reallocData(int alloc, bool grow)
{
  if(grow)
    alloc = allocMore(alloc,sizeof(Data));
  Data *x = Data::allocate(d, alloc);
  if(!x) {
    if(!d)
      d = &empty;
    return false;
  }
  d = x;
  return true;
}

bool
StringBuilder::resizeData(int size)
{
  if(size < 0)
    size = 0;

  if(size > d->alloc || (!size < d->length && size < d->alloc >> 1))
      if(!reallocData(size, true))
        return false;
  if (d->alloc >= size) {
    d->length = size;
    d->text[size] = '\0';
  }
  return true;
}

// Private helper for text manipulation, which grows string and sets
// a longer length than is actually asked for, for the benefit of
// callers who know what they're doing. Hence, private.
bool
StringBuilder::expand(int i)
{
  int x = d->length;
  if(!resizeData((maximum(i+1,x))))
    return false;
  if (d->length - 1 > x) {
    char *n = d->text + d->length - 1;
    char *e = d->text + x;
    *n = '\0';
    while (n != e)
      *--n = ' ';
  }
  return true;
}

StringBuilder::Data *
StringBuilder::Data::allocate(Data *old, int alloc)
{
  if(old && old != &empty) {
    // Reallocate existing data
    Data *x = static_cast<Data *>(::realloc(old, sizeof(Data) + alloc));
    if(x)
      x->alloc = alloc;
    return x;
  } else {
    // Allocate new data
    Data *x = static_cast<Data *>(::malloc(sizeof(Data) + alloc));
    if(x) {
      x->alloc = alloc;
      x->length = 0;
      x->text[0] = '\0';
    }
    return x;
  }
}

void
StringBuilder::freeData()
{
  Data *x = d;
  d = &empty;
  if(x && x != &empty)
    ::free(x);
}

int
StringBuilder::indexOf(const char *utf8, int len, int from) const
{
  if(!utf8)
    return -1;
  if(len < 0) {
    len = strlen(utf8);
    if(!len)
      return -1;
  }

  return String::findString(text(), length(), from, utf8, len);
}

int
StringBuilder::indexOf(uint32 ucs4, int from) const
{
  char is[8]; 
  int il;
  Unicode::toUtf8(ucs4, is, il);
  return indexOf(is, il, from);
}

bool StringBuilder::toString(String &result) const
{
  result = String(d->text,d->length);
  return result.d->length==d->length;
}

void
StringBuilder::clear()
{
  d->length = 0;
  d->text[0] = '\0';
}

bool
StringBuilder::setText(const String &string)
{
  int size = string.size();
  const char *text = string.text();
  if(!size) {
    clear();
    return true;
  }

  if(size >= d->alloc)
    if(!reallocData(size))
      return false;

  d->text[size] = '\0';
  ::memcpy(d->text, text, size);

  d->length = size;
  d->text[size] = '\0';

  return true;
}

bool
StringBuilder::reserve(int capacity)
{
  return reallocData(capacity);
}

bool
StringBuilder::append(uint32 ucs4)
{
  char is[8]; 
  int il;
  Unicode::toUtf8(ucs4, is, il);
  return append(is, il);
}

bool
StringBuilder::append(const char *utf8, int len)
{
  if(!utf8)
    return true;
  if(len < 0)
    len = ::strlen(utf8);
  if(len == 0)
    return true;
  int vlen = Unicode::utf8Length(utf8, len);
  if(length() + vlen >= capacity() && !reallocData(length() + vlen, true))
    return false;

  Unicode::toValidUtf8(d->text + d->length, capacity(), vlen, utf8, len);
  d->length += vlen;
  d->text[d->length] = '\0';
  // Should we return false if the UTF8 is not valid?
  return true;
}
  
bool
StringBuilder::prepend(uint32 ucs4)
{
  char is[8]; 
  int il;
  Unicode::toUtf8(ucs4, is, il);
  return insert(0, is, il);
}

bool
StringBuilder::insert(int idx, uint32 ucs4)
{
  char is[8]; 
  int il;
  Unicode::toUtf8(ucs4, is, il);
  return insert(idx, is, il);
}

bool
StringBuilder::insert(int i, const char *text, int len)
{
  if(!text)
    return true;
  if(len < 0)
    len = ::strlen(text);
  if (i < 0 || len <= 0)
    return true;

  if (text >= d->text && text < d->text + d->alloc) {
    // Text is inside the stringbuilder, we will need to
    // store it in a temporary buffer
    char *buffer = static_cast<char *>(::malloc(len));
    if(!buffer)
      return false;
    ::memcpy(buffer, text, len);
    bool result = insert(i, buffer, len);
    ::free(buffer);
    return result;
  }

  // Grow if needed
  if(!expand(maximum(d->length, i) + len - 1))
    return false;

  ::memmove(d->text + i + len, d->text + i, (d->length - i - len));
  ::memcpy(d->text + i, text, len);

  return true;
}

bool
StringBuilder::replaceAll(const char *search, int len,
                          const char *repl, int rlen)
{
  if(search) {
    if(len < 0)
      len = ::strlen(search);
  } else
    return true;
  if(repl) {
    if(rlen < 0)
      rlen = ::strlen(repl);
  }
  // Don't perform null ops
  if(d->length == 0) {
    if(len)
      return true;
  } else {
    if(search == repl && len == rlen)
      return true;
  }

  StringMatcher matcher(search, len);

  int i = 0;

  for(;;) {
    unsigned indices[1024];
    unsigned pos = 0;
    while(pos < 1023) {
      i = matcher.findIn(d->text, d->length, i);
      if(i == -1)
        break;
      indices[pos++] = i;
      i += len;
      if(!i)
        ++i;
    }
    if(!pos)
      break;

    // Die if we have an allocation failure
    if(!replaceHelper(indices, pos, len, repl, rlen))
      return false;

    if(i == -1)
      break;
    i += pos*(rlen - len);
  }

  return true;
}

bool
StringBuilder::replaceHelper(unsigned *indices, int n, int len,
                             const char *repl, int rlen)
{
  if(len == rlen) {
    // Fastest case -- no allocation changes needed
    for (int i = 0; i < n; ++i)
      memcpy(d->text + indices[i], repl, rlen);
  } else if(rlen < len) {
    // Second fastest case, no need to resize buffer
    unsigned to = indices[0];
    if(rlen)
      ::memcpy(d->text + to, repl, rlen);
    to += rlen;
    unsigned from = indices[0] + len;
    for(int i=1; i<n; ++i) {
      int size = int(indices[i] - from);
      if(size > 0) {
        // Close the distance between the replacement text and the
        // rest of the buffer
        ::memmove(d->text + to, d->text + from, size);
        to += size;
      }
      if(rlen) {
        // Copy in the replacement text
        ::memcpy(d->text + to, repl, rlen);
        to += rlen;
      }
      from = indices[i] + len;
    }
    int size = d->length - from;
    if(size > 0)
      ::memmove(d->text + to, d->text + from, size);
    if(!resizeData(d->length-(n*(len-rlen))))
      return false;
  } else {
    // Worst case, replace from back
    int adjust = n*(rlen-len);
    int nlen = d->length + adjust;
    int end = d->length;
    if(!resizeData(nlen))
      return false;
    while (n) {
      --n;
      int from = int(indices[n] + len);
      int start = indices[n] + n*(rlen-len);
      int to = start + rlen;
      ::memmove(d->text + to, d->text + from, (end - from));
      ::memcpy(d->text + start, repl, rlen);
      end = from-len;
    }
  }
  return true;
}

bool
StringBuilder::replaceAll(const char *search, int len,
                          uint32 replace)
{
  char ur[8];
  int rl;
  Unicode::toUtf8(replace, ur, rl);
  return replaceAll(search, len, ur, rl);  
}

bool
StringBuilder::replaceAll(uint32 search,
                          const char *replace, int rlen)
{
  char us[8];
  int sl;
  Unicode::toUtf8(search, us, sl);
  return replaceAll(us, sl, replace, rlen);
}

bool
StringBuilder::replaceAll(uint32 search,
                          uint32 replace)
{
  char us[8], ur[8];
  int sl, rl;
  Unicode::toUtf8(search, us, sl);
  Unicode::toUtf8(replace, ur, rl);
  return replaceAll(us, sl, ur, rl);
}

void
StringBuilder::removeTrailingChar()
{
  int pos = d->length - 1;
  while(pos > 0 && Unicode::utf8IsTrail(d->text[pos])) --pos;
  // pos should point to a lead or single byte now...
  if(pos > 0) {
    d->length = pos;
    d->text[d->length] = '\0';
  }
}

}
