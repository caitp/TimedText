#include <TimedText/Buffer.h>

namespace TimedText
{

Buffer::Buffer(Flags _flags)
  : flags(_flags), final(false), i(0)
{
}

Buffer::~Buffer()
{
}

const char *
Buffer::curr() const
{
  if(i < 0 || i >= buffer.size())
    return "";
  return buffer.text() + i;
}

bool
Buffer::seek(int n, bool abs)
{
  if(!abs)
    n += i;
  if(n < 0 || n >= buffer.size())
    return false;
  i = n;
  return true;
}

void
Buffer::performDiscard(int bytes)
{
  seek(bytes);
}

void
Buffer::discard(int bytes)
{
  if(!bytes)
    return;
  lock();
  performDiscard(bytes);
  unlock();
}

void
Buffer::discard()
{
  int bytes = pos();
  if(!bytes)
    return;
  lock();
  performDiscard(bytes);
  unlock();
}

bool
Buffer::refill(const char utf8[], int count, bool final)
{
  bool result;
  lock();
  this->final = final;
	result = buffer.append(utf8,count);
  unlock();
  return result;
}

int
Buffer::read(char out[], int max)
{
  int ret = max;
  if(ret <= 0)
    return 0;
  lock();
  if(buffer.size() - i < max)
    ret = buffer.size() - i;
  ::memcpy(out, buffer.text() + i, ret);
  unlock();
  return ret;
}

} // TimedText
