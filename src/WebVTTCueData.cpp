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

#include "WebVTTCueData.h"

namespace TimedText
{

bool
WebVTTCueData::setLine(int line, bool snapToLines)
{
  if(snapToLines) {
    _line = line;
    _snapToLines = snapToLines;
    return true;
  } else if(line >= 0 && line <= 100) {
    _line = line;
    _snapToLines = snapToLines;
    return true;
  } else {
    return false;
  }
}

bool
WebVTTCueData::setSize(int size)
{
  if(size >= 0 && size <= 100) {
    _size = size;
    return true;
  }
  return false;
}

bool
WebVTTCueData::setPosition(int position)
{
  if(position >= 0 && position <= 100) {
    _position = position;
    return true;
  }
  return false;
}

bool
WebVTTCueData::setVertical(Cue::Vertical vertical)
{
  if(vertical >= Cue::Horizontal && vertical <= Cue::VerticalRightToLeft) {
    _vertical = vertical;
    return true;
  }
  return false;
}

bool
WebVTTCueData::setAlign(Cue::Align align)
{
  if(align >= Cue::Start && align <= Cue::Right) {
    _align = align;
    return true;
  }
  return false;
}

// Set from text
bool
WebVTTCueData::setLine(const char *value, int len)
{
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
WebVTTCueData::setSize(const char *value, int len)
{
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
WebVTTCueData::setPosition(const char *value, int len)
{
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
WebVTTCueData::setVertical(const char *value, int len)
{
  if(!value)
    return false;
  if(len < 0)
    len = ::strlen(value);
  if(!len) {
    _vertical = Cue::Horizontal;
    return true;
  } else if(len == 2) {
    if(!::strcmp("lr", value)) {
      _vertical = Cue::VerticalLeftToRight;
      return true;
    } else if(!::strcmp("rl", value)) {
      _vertical = Cue::VerticalRightToLeft;
      return true;
    }
  }
  return false;
}

bool
WebVTTCueData::setAlign(const char *value, int len)
{
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
      _align = static_cast<Cue::Align>(i);
      return true;
    }
  }
  return false;
}

void
WebVTTCueData::applySettings(const String &settings)
{
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

void
WebVTTCueData::resetCueSettings()
{
  // 22. Let cue's text track cue writing direction be horizontal.
  setVertical(Cue::defaultVertical);
  // 23. Let cue's text track cue snap-to-lines flag be true.
  // 24. Let cue's text track cue line position be auto.
  setLine(Cue::defaultLine, Cue::defaultSnapToLines);
  // 25. Let cue's text track cue text position be 50.
  setPosition(Cue::defaultPosition);
  // 26. Let cue's text track cue size be 100.
  setSize(Cue::defaultSize);
  // 27. Let cue's text track cue alignment be middle alignment.
  setAlign(Cue::defaultAlign);
}

} // TimedText
