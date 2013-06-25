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

#include <TimedText/StringBuilder.h>

namespace TimedText
{
#if 0
StringBuilder::StringBuilder(int capacity, bool &result)
{

}

StringBuilder::~StringBuilder()
{

}

int
StringBuilder::indexOf(const char *text, int len) const
{
  if(!text)
    return -1;
  if(len < 0) {
    len = strlen(text);
    if(!len)
      return -1;
  }
  return String::findString(text(), length(), text, len);
}

bool StringBuilder::toString(String &result) const
{

}

int
StringBuilder::length() const
{

}

int
StringBuilder::size() const
{

}

int
StringBuilder::capacity() const
{

}

void
StringBuilder::clear()
{

}

bool
StringBuilder::reserve(int capacity)
{

}

bool
StringBuilder::append(unsigned long ucs4)
{
  char is[8]; 
  int il;
  Unicode::toUtf8(ucs4, is, il);
  return append(idx, is, il);
}

bool
StringBuilder::append(const char *text, int len)
{

}
  
bool
StringBuilder::prepend(unsigned long ucs4)
{
  char is[8]; 
  int il;
  Unicode::toUtf8(ucs4, is, il);
  return prepend(idx, is, il);
}

bool
StringBuilder::prepend(const char *text, int len)
{

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

}

bool
StringBuilder::replaceAll(const char *search, int len,
                          const char *repl, int rlen)
{
  while(strstr())
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

#endif

}
