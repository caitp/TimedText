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
{
  d = &empty;
}

StringBuilder::StringBuilder(int capacity, bool &result)
  : d(&empty)
{
  result = reallocData(capacity, true);
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
  d = Data::allocate(!d || d==&empty ? 0 : d, alloc);
  if(!d) {
    d = &empty;
    return false;
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
StringBuilder::reserve(int capacity)
{
  return reallocData(capacity);
}

bool
StringBuilder::appendByte(char b)
{
  if(!b)
    return append(0);
  if(d->length + 1 >= d->alloc && !reallocData(d->length + 1, true))
    return false;
  d->text[d->length++] = b;
  d->text[d->length] = '\0';
  return true;
}

bool
StringBuilder::append(unsigned long ucs4)
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
StringBuilder::prepend(unsigned long ucs4)
{
  char is[8]; 
  int il;
  Unicode::toUtf8(ucs4, is, il);
  return prepend(is, il);
}

bool
StringBuilder::prepend(const char *text, int len)
{
  return false;
}
  
bool
StringBuilder::insert(int idx, unsigned long ucs4)
{
  char is[8]; 
  int il;
  Unicode::toUtf8(ucs4, is, il);
  return insert(idx, is, il);
}

bool
StringBuilder::insert(int idx, const char *text, int len)
{
  return false;
}

bool
StringBuilder::replaceAll(const char *search, int len,
                          const char *repl, int rlen)
{
  return false;
}

bool
StringBuilder::replaceAll(const char *search, int len,
                          unsigned long replace)
{
  char ur[8];
  int rl;
  Unicode::toUtf8(replace, ur, rl);
  return replaceAll(search, len, ur, rl);  
}

bool
StringBuilder::replaceAll(unsigned long search,
                          const char *replace, int rlen)
{
  char us[8];
  int sl;
  Unicode::toUtf8(search, us, sl);
  return replaceAll(us, sl, replace, rlen);
}

bool
StringBuilder::replaceAll(unsigned long search,
                          unsigned long replace)
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
