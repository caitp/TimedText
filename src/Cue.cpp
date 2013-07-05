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

#include <TimedText/Cue.h>
#include "StringData.h"
namespace TimedText
{

struct Cue::Data
{
  AtomicInt ref;
  Type type;
  String id;
  String text;
  Timestamp startTime;
  Timestamp endTime;
  bool dirty;
  bool snapToLines;
  int line;
  int size : 8;
  int position : 8;
  Vertical vertical : 8;
  Align align : 8;
};

static String::Data emptyId = { AtomicInt(1), 0, { '\0' } };
static String::Data emptyBody = { AtomicInt(1), 0, { '\0' } };

static Cue::Data emptyCue = {
  AtomicInt(1), Cue::EmptyCue, emptyId, emptyBody, MalformedTimestamp,
  MalformedTimestamp, false, Cue::defaultSnapToLines, Cue::defaultLine,
  Cue::defaultSize, Cue::defaultPosition, Cue::defaultVertical,
  Cue::defaultAlign
};

Cue::Cue() : d(&emptyCue)
{
  d->ref.ref();
}

Cue::Cue(Type type, Timestamp startTime, Timestamp endTime,
         const String &id, const String &text)
{
  d = new Data();
  if(!d) {
    d = &emptyCue;
    d->ref.ref();
    return;
  }
  d->ref = 1;
  d->type = type;
  d->startTime = startTime;
  d->endTime = endTime;
  d->id = id;
  d->text = text;
  d->dirty = !text.isEmpty();
  resetCueSettings();
}

Cue::Cue(const Cue &other)
  : d(other.d)
{
  d->ref.ref();
}

Cue::~Cue()
{
  if(!d->ref.deref())
    delete d;
}

Cue &Cue::operator=(const Cue &other)
{
  if(d != other.d) {
    Data *x = d;
    d = other.d;
    d->ref.ref();
    if(!x->ref.deref())
      delete x;
  }
  return *this;
}

Cue::Type
Cue::type() const
{
  return d->type;
}

String
Cue::id() const
{
  return d->id;
}

String
Cue::text() const
{
  return d->text;
}

Timestamp
Cue::startTime() const
{
  return d->startTime;
}

Timestamp
Cue::endTime() const
{
  return d->endTime;
}

void
Cue::setId(const String &id)
{
  if(type() == EmptyCue)
    return;
  d->id = id;
}

void
Cue::setText(const String &text)
{
  if(type() == EmptyCue)
    return;
  d->text = text;
  d->dirty = true;
}

void
Cue::setStartTime(const Timestamp &ts)
{
  if(type() == EmptyCue)
    return;
  d->startTime = ts;
}

void
Cue::setEndTime(const Timestamp &ts)
{
  if(type() == EmptyCue)
    return;
  d->endTime = ts;
}

// WebVTT CueSettings
bool
Cue::snapToLines() const
{
  return d->snapToLines;
}

int
Cue::line() const
{
  return d->line;
}

int
Cue::size() const
{
  return d->size;
}

int
Cue::position() const
{
  return d->position;
}

Cue::Vertical
Cue::vertical() const
{
  return d->vertical;
}

Cue::Align
Cue::align() const
{
  return d->align;
}

void
Cue::applySettings(const String &settings)
{
  if(type() == EmptyCue)
    return;
  char word[0x200] = "";
  int position = 0;
  while(position < settings.length()) {
    settings.skipWhitespace(position);
    if(!settings.collectWord(position, word)) {
      // If the word was too big, it's probably not valid. Skip it
      // and try the next one.
      settings.skipUntilWhitespace(position);
      continue;
    }
    char *separator = strchr(word, ':');
    if(!separator || separator == word
       || separator + 1 >= &word[sizeof(word)-1]
       || separator[1] == '\0') {
      // If the setting doesn't contain a separator, it's not a valid
      // WebVTT setting. Try the next one. Also, if the separator is
      // the first character, or the final character in the buffer,
      // skip it because it's equally invalid.
      // Also, if the separator is the final character in the word,
      // it's not valid.
      settings.skipUntilWhitespace(position);
      continue;
    }
    const char *value = separator + 1;
    *separator = 0;
    if(!::strcmp("align", word))
      setAlign(value);
    else if(!::strcmp("line", word))
      setLine(value);
    else if(!::strcmp("position", word))
      setPosition(value);
    else if(!::strcmp("size", word))
      setSize(value);
    else if(!::strcmp("vertical", word))
      setVertical(value);
  }
}

bool
Cue::setLine(int line, bool snapToLines)
{
  if(type() == EmptyCue)
    return false;
  if(snapToLines) {
    d->line = line;
    d->snapToLines = snapToLines;
    return true;
  } else if(line >= 0 && line <= 100) {
    d->line = line;
    d->snapToLines = snapToLines;
    return true;
  } else {
    return false;
  }
}

bool
Cue::setSize(int size)
{
  if(type() == EmptyCue)
    return false;
  if(size >= 0 && size <= 100) {
    d->size = size;
    return true;
  }
  return false;
}

bool
Cue::setPosition(int position)
{
  if(type() == EmptyCue)
    return false;
  if(position >= 0 && position <= 100) {
    d->position = position;
    return true;
  }
  return false;
}

bool
Cue::setVertical(Vertical vertical)
{
  if(type() == EmptyCue)
    return false;
  if(vertical >= Horizontal && vertical <= VerticalRightToLeft) {
    d->vertical = vertical;
    return true;
  }
  return false;
}

bool
Cue::setAlign(Align align)
{
  if(type() == EmptyCue)
    return false;
  if(align >= Start && align <= Right) {
    d->align = align;
    return true;
  }
  return false;
}

// Set from text
bool
Cue::setLine(const char *value, int len)
{
  if(type() == EmptyCue)
    return false;
  if(!value)
    return false;
  if(len < 0)
    len = ::strlen(value);
  if(!len)
    return false;
  int digits = 0;
  bool snap = true;
  // Make sure that 'value' contains only '-', '%', or [0-9]
  for(int i=0; i<len; ++i) {
    char c = value[i];
    if(Char::isAsciiDigit(c)) {
      ++digits;
    } else if(c == '-') {
      // This is allowed, but only in position 0
      if(i)
        return false;
      snap = true;
    } else if(c == '%') {
      // This is allowed, but only in the last position
      // If it is the last position, but this is a negative
      // number, then it's trash.
      if(i < len - 1 || value[0] == '-')
        return false;
      snap = false;
    } else {
      // Any other character is forbidden
      return false;
    }
  }
  if(!digits)
    return false; // If there are no digits, it's trash
  int num = String::parseInt(value, len);
  return setLine(num, snap);
}

bool
Cue::setSize(const char *value, int len)
{
  if(type() == EmptyCue)
    return false;
  if(!value)
    return false;
  if(len < 0)
    len = ::strlen(value);
  if(!len)
    return false;
  int digits = 0;
  bool per = false;
  // Make sure that 'value' contains only '%', or [0-9]
  for(int i=0; i<len; ++i) {
    char c = value[i];
    if(Char::isAsciiDigit(c)) {
      ++digits;
    } else if(c == '%') {
      // This is allowed, but only in the last position
      // If it is the last position, but this is a negative
      // number, then it's trash.
      if(!digits || i < len - 1)
        return false;
      per = true;
    } else {
      // Any other character is forbidden
      return false;
    }
  }
  if(!digits || !per)
    return false; // If there are no digits or '%', it's trash
  int num = String::parseInt(value, len);
  return setSize(num);
}

bool
Cue::setPosition(const char *value, int len)
{
  if(type() == EmptyCue)
    return false;
  if(!value)
    return false;
  if(len < 0)
    len = ::strlen(value);
  if(!len)
    return false;
  int digits = 0;
  bool per = false;
  // Make sure that 'value' contains only '%', or [0-9]
  for(int i=0; i<len; ++i) {
    char c = value[i];
    if(Char::isAsciiDigit(c)) {
      ++digits;
    } else if(c == '%') {
      // This is allowed, but only in the last position
      // If it is the last position, but this is a negative
      // number, then it's trash.
      if(!digits || i < len - 1)
        return false;
      per = true;
    } else {
      // Any other character is forbidden
      return false;
    }
  }
  if(!digits || !per)
    return false; // If there are no digits or '%', it's trash
  int num = String::parseInt(value, len);
  return setPosition(num);
}

bool
Cue::setVertical(const char *value, int len)
{
  if(type() == EmptyCue)
    return false;
  if(!value)
    return false;
  if(len < 0)
    len = ::strlen(value);
  if(!len) {
    d->vertical = Horizontal;
    return true;
  } else if(len == 2) {
    if(!::strcmp("lr", value)) {
      d->vertical = VerticalLeftToRight;
      return true;
    } else if(!::strcmp("rl", value)) {
      d->vertical = VerticalRightToLeft;
      return true;
    }
  }
  return false;
}

bool
Cue::setAlign(const char *value, int len)
{
  if(type() == EmptyCue)
    return false;
  if(!value)
    return false;
  if(len < 0)
    len = ::strlen(value);
  // If shorter than shortest, or longer than longest, then kill it.
  if(len < 3 || len > 6)
    return false;
  const char *values[] = {
    "start",
    "middle",
    "end",
    "left",
    "right"
  };
  for(int i=0; i<sizeof(values)/sizeof(*values); ++i) {
    if(!::strcmp(value,values[i])) {
      d->align = static_cast<Align>(i);
      return true;
    }
  }
  return false;
}

void
Cue::resetCueSettings()
{
  if(type() == EmptyCue)
    return;
  // 22. Let cue's text track cue writing direction be horizontal.
  setVertical(defaultVertical);
  // 23. Let cue's text track cue snap-to-lines flag be true.
  // 24. Let cue's text track cue line position be auto.
  setLine(defaultLine, defaultSnapToLines);
  // 25. Let cue's text track cue text position be 50.
  setPosition(defaultPosition);
  // 26. Let cue's text track cue size be 100.
  setSize(defaultSize);
  // 27. Let cue's text track cue alignment be middle alignment.
  setAlign(defaultAlign);
}

} // TimedText
