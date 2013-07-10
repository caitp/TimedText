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
// TODO: #include "TTMLCueData.h"
#include "StringData.h"

namespace TimedText
{

class EmptyCueData : public CueData
{
public:
  EmptyCueData() : CueData(EmptyCue) {}
} emptyCue;

Cue::Cue(CueType type) : d(0)
{
  if(type == WebVTTCue)
    d = new WebVTTCueData();
  else if(type == TTMLCue)
    // TODO: Instantiate TTMLCueData
    ;
  if(!d) {
    // If we're making an EmptyCue, or allocation failed,
    // return an empty
    d = &emptyCue;
    d->ref.ref();
  }
}

Cue::Cue(CueType type, Timestamp startTime, Timestamp endTime,
         const String &id, const String &text)
{
  if(type == WebVTTCue)
    d = new WebVTTCueData(startTime, endTime, id, text);
  else if(type == TTMLCue)
    // TODO: Instantiate TTMLCueData
    ;
  if(!d) {
    d = &emptyCue;
    d->ref.ref();
  }
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
    other.d->ref.ref();
    if(!d->ref.deref())
      delete d;
    d = other.d;
  }
  return *this;
}

CueType
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
  d->setId(id);
}

void
Cue::setText(const String &text)
{
  d->setText(text);
}

void
Cue::setStartTime(const Timestamp &ts)
{
  d->setStartTime(ts);
}

void
Cue::setEndTime(const Timestamp &ts)
{
  d->setEndTime(ts);
}

// WebVTT properties
bool
Cue::snapToLines() const
{
  return d->snapToLines();
}

int
Cue::line() const
{
  return d->line();
}

int
Cue::size() const
{
  return d->size();
}

int
Cue::position() const
{
  return d->position();
}

Cue::Vertical
Cue::vertical() const
{
  return d->vertical();
}

Cue::Align
Cue::align() const
{
  return d->align();
}

bool
Cue::setLine(int line, bool snapToLines)
{
  return d->setLine(line, snapToLines);
}

bool
Cue::setSize(int size)
{
  return d->setSize(size);
}

bool
Cue::setPosition(int position)
{
  return d->setPosition(position);
}

bool
Cue::setVertical(Vertical vertical)
{
  return d->setVertical(vertical);
}

bool
Cue::setAlign(Align align)
{
  return d->setAlign(align);
}

// Set from text
bool
Cue::setLine(const char *value, int len)
{
  return d->setLine(value, len);
}

bool
Cue::setSize(const char *value, int len)
{
  return d->setSize(value, len);
}

bool
Cue::setPosition(const char *value, int len)
{
  return d->setPosition(value, len);
}

bool
Cue::setVertical(const char *value, int len)
{
  return d->setVertical(value, len);
}

bool
Cue::setAlign(const char *value, int len)
{
  return d->setAlign(value, len);
}

void
Cue::applySettings(const String &settings)
{
  d->applySettings(settings);
}

void
Cue::resetCueSettings()
{
  d->resetCueSettings();
}

bool
Cue::nodes(Node &result) const
{
  return d->nodes(result);
}

bool
Cue::setNodes(const Node &nodes)
{
  return d->setNodes(nodes);
}

void
Cue::visit(NodeVisitor &visitor)
{
  Node tree;
  if(nodes(tree))
    tree.visit(visitor);
}

} // TimedText
