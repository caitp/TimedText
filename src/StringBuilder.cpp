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
