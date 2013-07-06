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

#ifndef __TimedText_CueData__
#define __TimedText_CueData__

#include <TimedText/Cue.h>
#include "Atomic.h"
#include "StringData.h"

namespace TimedText
{

class CueData
{
public:
  CueData(CueType type);
  CueData(CueType type, const String &id, const String &text);
  CueData(CueType type, const Timestamp &startTime, const Timestamp &endTime,
          const String &id, const String &text);
  virtual ~CueData();

  virtual void setId(const String &id);
  virtual void setText(const String &text);
  virtual void setStartTime(const Timestamp &ts);
  virtual void setEndTime(const Timestamp &ts);

  virtual bool snapToLines() const;
  virtual int line() const;
  virtual int size() const;
  virtual int position() const;
  virtual Cue::Vertical vertical() const;
  virtual Cue::Align align() const;

  virtual bool setLine(int line, bool snapToLines);
  virtual bool setSize(int size);
  virtual bool setPosition(int position);
  virtual bool setVertical(Cue::Vertical vertical);
  virtual bool setAlign(Cue::Align align);

  virtual bool setLine(const char *value, int len);
  virtual bool setSize(const char *value, int len);
  virtual bool setPosition(const char *value, int len);
  virtual bool setVertical(const char *value, int len);
  virtual bool setAlign(const char *value, int len);

  virtual void applySettings(const String &settings);
  virtual void resetCueSettings();

  virtual bool tree(Node &result) const;

  CueType type;
  AtomicInt ref;
  String id;
  String text;
  Timestamp startTime;
  Timestamp endTime;
};

} // TimedText

#endif // __TimedText_CueData__
