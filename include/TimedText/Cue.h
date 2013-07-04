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

#ifndef __TimedText_Cue__
#define __TimedText_Cue__

#include <TimedText/String.h>
#include <TimedText/Timestamp.h>
#include <climits>

namespace TimedText
{

class Cue
{
public:
  enum Type
  {
    EmptyCue = 0,
    WebVTTCue,
    TTMLCue,
  };

  // WebVTT constants
  enum Vertical
  {
    Horizontal = 0,
    VerticalLeftToRight,
    VerticalRightToLeft
  };
  enum Align
  {
    Start = 0,
    Middle,
    End,
    Left,
    Right
  };

  enum
  {
    Auto = INT_MAX
  };

  static const Vertical defaultVertical = Horizontal;
  static const int defaultLine = Auto;
  static const bool defaultSnapToLines = true;
  static const int defaultPosition = 50;
  static const int defaultSize = 100;
  static const Align defaultAlign = Middle;

  Cue();
  Cue(Type type, Timestamp startTime, Timestamp endTime,
      const String &id = String(), const String &text = String());
  Cue(const Cue &other);
  ~Cue();

  Cue &operator=(const Cue &other);

  Type type() const;
  String id() const;
  String text() const;
  Timestamp startTime() const;
  Timestamp endTime() const;

  void setId(const String &id);
  void setText(const String &text);
  void setStartTime(const Timestamp &ts);
  void setEndTime(const Timestamp &ts);
  void applySettings(const String &settings);

  // WebVTT-specific API
  bool snapToLines() const;
  int line() const;
  int size() const;
  int position() const;
  Vertical vertical() const;
  Align align() const;

  bool setLine(int line, bool snapToLines);
  bool setSize(int size);
  bool setPosition(int position);
  bool setVertical(Vertical vertical);
  bool setAlign(Align align);

  bool setLine(const char *value, int len = -1);
  bool setSize(const char *value, int len = -1);
  bool setPosition(const char *value, int len = -1);
  bool setVertical(const char *value, int len = -1);
  bool setAlign(const char *value, int len = -1);

  void resetCueSettings();

  struct Data;
protected:
  Data *d;
};

TT_DECLARE_TYPEINFO(Cue, TT_MOVABLE_TYPE);

}

#endif // __TimedText_Cue__
