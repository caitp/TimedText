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

#include <TimedText/List.h>
#include "Atomic.h"
#include "Utility.h"

namespace TimedText
{

struct ListData::Data
{
  AtomicInt ref;
  int alloc, begin, end;
  void *array[1];
};
enum { DataHeaderSize = sizeof(ListData::Data) - sizeof(void *) };

static ListData::Data shared_empty = {
  AtomicInt(1), 0, 0, 0, { 0 }
};

static ListData::Data shared_null = {
  AtomicInt(1), 0, 0, 0, { 0 }
};

bool ListData::ref() { return d->ref.ref(); }
bool ListData::deref() { return d->ref.deref(); }

static int
grow(int size)
{
  volatile int x = allocMore(size * sizeof(void *),
                             DataHeaderSize) / sizeof(void *);
  return x;
}

ListData::Data *ListData::sharedEmpty()
{
  return &shared_empty;
}

ListData::Data *ListData::sharedNull()
{
  return &shared_null;
}

ListData::Data *ListData::detach(int alloc) {
  Data *x = static_cast<Data *>(::malloc(DataHeaderSize + d->alloc * sizeof(void *)));
  if(!x)
    return 0;
  x->ref = 1;
  x->alloc = d->alloc;
  if(!x->alloc)
    x->begin = x->end = 0;
  else {
    x->begin = d->begin;
    x->end = d->end;
  }
  Data *y = d;
  d = x;
  if(!y->ref.deref())
    return y;
  return 0;
}

ListData::Data *
ListData::detach_grow(int *i, int n) {
  Data *x = d;
  int l = x->end - x->begin;
  int nl = l + n;
  int alloc = grow(nl);
  Data *t = static_cast<Data *>(::malloc(DataHeaderSize + alloc * sizeof(void *)));
  if(!t)
    return 0;
  t->ref = 1;
  t->alloc = alloc;
  int bg;
  if(*i < 0) {
    *i = 0;
    bg = (alloc - nl) >> 1;
  } else if(*i > l) {
    *i = l;
    bg = 0;
  } else if(*i < (l >> 1)) {
    bg = (alloc - nl) >> 1;
  } else {
    bg = 0;
  }
  t->begin = bg;
  t->end = bg + nl;
  d = t;
  return x;
}

bool
ListData::realloc(int alloc) {
	if(d->ref != 1)
		return false;
  Data *x = static_cast<Data *>(::realloc(d, DataHeaderSize + alloc * sizeof(void *) ) );
  if(!x)
    return false;
  d = x;
  d->alloc = alloc;
  if(!alloc)
    d->begin = d->end = 0;
  return true;
}

void **
ListData::erase(void **xi) {
  if(d->ref != 1)
    return 0;
  int i = int(xi - (d->array + d->begin));
  if(!remove(i))
    return 0;
  return d->array + d->begin + i;
}

void **
ListData::append(int n) {
  if(d->ref != 1)
    return 0;
  int e = d->end;
  if(e + n > d->alloc) {
    int b = d->begin;
    if(b - n >= 2 * d->alloc / 3) {
      e -= b;
      ::memmove(d->array, d->array + b, e * sizeof(void *));
      d->begin = 0;
    } else {
      if(!realloc(grow(d->alloc + n)))
        return 0;
    }
  }
  d->end = e + n;
  return d->array + e;
}

void **
ListData::append() {
  return append(1);
}

void **
ListData::insert(int i) {
  if(d->ref != 1)
    return 0;
  if(i <= 0)
    return prepend();
  int size = d->end - d->begin;
  if(i >= size)
    return append();
  bool leftward = false;
  if(d->begin == 0) {
    if(d->end == d->alloc) {
      if(!realloc(grow(d->alloc + 1)))
        return 0;
    } else {
      // Space at the end of the array, we're good.
    }
  } else {
    if(d->end == d->alloc) {
      // There's free space at the beginning, so we should
      // move items left towards the beginning.
      leftward = true;
    } else {
      // If there is free space at both ends, move as few items
      // as possible
      leftward = (i < size - i);
    }
  }

  if(leftward) {
    --d->begin;
    ::memmove(d->array + d->begin, d->array + d->begin + 1, i * sizeof(void *));
  } else {
    ::memmove(d->array + d->begin + i + 1, d->array + d->begin + i,
              (size - i) * sizeof(void *));
    ++d->end;
  }
  return d->array + d->begin + i;
}

void **
ListData::prepend()
{
  if(!d->ref == 1)
    return 0;
  if(d->begin == 0) {
    if(d->end >= d->alloc / 3)
      if(!realloc(grow(d->alloc + 1)))
        return 0;
    if(d->end < d->alloc / 3)
      d->begin = d->alloc - 2 * d->end;
    else
      d->begin = d->alloc - d->end;
    ::memmove(d->array + d->begin, d->array, d->end * sizeof(void *));
    d->end += d->begin;
  }
  return d->array + --d->begin;
}

bool
ListData::remove(int i) {
  if(d->ref != 1)
    return false;
  i += d->begin;
  if(i - d->begin < d->end - i) {
    if(int offset = i - d->begin)
      ::memmove(d->array + d->begin + 1, d->array + d->begin, offset * sizeof(void *));
    ++d->begin;
  } else {
    if(int offset = d->end - i - 1)
      ::memmove(d->array + i, d->array + i + 1, offset * sizeof(void *));
    --d->end;
  }
  return true;
}

bool
ListData::remove(int i, int n) {
  if(d->ref != 1)
    return false;
  i += d->begin;
  int middle = i + n/2;
  if(middle - d->begin < d->end - middle) {
    ::memmove(d->array + d->begin + n, d->array + d->begin,
              (i - d->begin) * sizeof(void *));
    d->begin += n;
  } else {
    ::memmove(d->array + i, d->array + i + n,
              (d->end - i - n) * sizeof(void *));
    d->end -= n;
  }
  return true;
}

int
ListData::size() const
{
  return d->end - d->begin;
}

int
ListData::alloc() const
{
  return d->alloc;
}

void **
ListData::begin() const
{
  return d->array + d->begin;
}

void **
ListData::end() const
{
  return d->array + d->end;
}

void **
ListData::at(int i) const
{
  return d->array + d->begin + i;
}

bool
ListData::unique() const
{
  return d->ref == 1;
}

void
ListData::undo_unshift()
{
  ++d->begin;
}

void
ListData::undo_push()
{
  --d->end;
}

void
ListData::kill()
{
  if(d->ref == 0)
    ::free(d);
}

} // TimedText
