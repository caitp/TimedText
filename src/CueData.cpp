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

#include "CueData.h"
#include "StringData.h"

namespace TimedText
{

CueData::CueData(CueType ty)
  : type(ty),
    ref(1),
    startTime(MalformedTimestamp),
    endTime(MalformedTimestamp)
{
}
CueData::CueData(CueType ty, const String &_id, const String &_text)
  : type(ty),
    ref(1),
    id(_id),
    text(_text),
    startTime(MalformedTimestamp),
    endTime(MalformedTimestamp)
{
}

CueData::CueData(CueType ty,
                 const Timestamp &start,
                 const Timestamp &end,
                 const String &_id,
                 const String &_text)
  : type(ty),
    ref(1),
    id(_id),
    text(_text),
    startTime(start),
    endTime(end)
{
}

CueData::~CueData()
{
}

void
CueData::setId(const String &id)
{
}

void
CueData::setText(const String &text)
{
}

void
CueData::setStartTime(const Timestamp &ts)
{
}

void
CueData::setEndTime(const Timestamp &ts)
{
}

bool
CueData::snapToLines() const
{
  return Cue::defaultSnapToLines;
}

int
CueData::line() const
{
  return Cue::defaultLine;
}

int
CueData::size() const
{
  return Cue::defaultSize;
}

int
CueData::position() const
{
  return Cue::defaultPosition;
}

Cue::Vertical
CueData::vertical() const
{
  return Cue::defaultVertical;
}

Cue::Align
CueData::align() const
{
  return Cue::defaultAlign;
}

bool
CueData::setLine(int line, bool snapToLines)
{
  return false;
}

bool
CueData::setSize(int size)
{
  return false;
}

bool
CueData::setPosition(int position)
{
  return false;
}

bool
CueData::setVertical(Cue::Vertical vertical)
{
  return false;
}

bool
CueData::setAlign(Cue::Align align)
{
  return false;
}

bool
CueData::setLine(const char *value, int len)
{
  return false;
}

bool
CueData::setSize(const char *value, int len)
{
  return false;
}

bool
CueData::setPosition(const char *value, int len)
{
  return false;
}

bool
CueData::setVertical(const char *value, int len)
{
  return false;
}

bool
CueData::setAlign(const char *value, int len)
{
  return false;
}

void
CueData::applySettings(const String &settings)
{
}

void
CueData::resetCueSettings()
{
}

bool
CueData::nodes(Node &result) const
{
  return false;
}

bool
CueData::setNodes(const Node &result)
{
  return false;
}

} // TimedText
