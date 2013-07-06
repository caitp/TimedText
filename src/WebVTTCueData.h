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

#ifndef __TimedText_WebVTTCueData__
#define __TimedText_WebVTTCueData__

#include "CueData.h"

namespace TimedText
{

class WebVTTCueData : public CueData
{
public:
  WebVTTCueData()
    : CueData(WebVTTCue)
  {
    resetCueSettings();
  }
  WebVTTCueData(const String &id, const String &text)
    : CueData(WebVTTCue,id, text)
  {
    resetCueSettings();
  }
  WebVTTCueData(const Timestamp &startTime, const Timestamp &endTime,
                const String &id, const String &text)
    : CueData(WebVTTCue,startTime, endTime, id, text)
  {
    resetCueSettings();
  }

  void setId(const String &_id) { id = _id; }
  void setText(const String &_text) { text = _text; }
  void setStartTime(const Timestamp &ts) { startTime = ts; }
  void setEndTime(const Timestamp &ts) { endTime = ts; }

  bool snapToLines() const { return _snapToLines; }
  int line() const { return _line; }
  int size() const { return _size; }
  int position() const { return _position; }
  Cue::Vertical vertical() const { return _vertical; }
  Cue::Align align() const { return _align; }

  bool setLine(int line, bool snapToLines);
  bool setSize(int size);
  bool setPosition(int position);
  bool setVertical(Cue::Vertical vertical);
  bool setAlign(Cue::Align align);

  bool setLine(const char *value, int len = -1);
  bool setSize(const char *value, int len = -1);
  bool setPosition(const char *value, int len = -1);
  bool setVertical(const char *value, int len = -1);
  bool setAlign(const char *value, int len = -1);

  void applySettings(const String &settings);
  void resetCueSettings();

  bool _snapToLines;
  int _line;
  int _size : 8;
  int _position : 8;
  Cue::Vertical _vertical : 8;
  Cue::Align _align : 8;
};

} // TimedText

#endif // __TimedText_WebVTTCueData__
