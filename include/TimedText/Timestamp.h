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

#ifndef __TimedText_Timestamp__
#define __TimedText_Timestamp__

#include <TimedText/Types.h>

namespace TimedText
{

typedef int64 Milliseconds;

static const Milliseconds MalformedTimestamp = -1;

class Timestamp
{
public:
  static const Milliseconds millisPerHour = 3600000;
  static const Milliseconds millisPerMinute = 60000;
  static const Milliseconds millisPerSecond =  1000;
  static const Milliseconds secondsPerMin =  60;
  static const Milliseconds minutesPerHour = 60;
  Timestamp() : ms(MalformedTimestamp) {}
  Timestamp(Milliseconds msec) : ms(msec < 0 ? MalformedTimestamp : msec) {}
  Timestamp(double seconds) : ms(seconds * millisPerSecond) {}

  inline bool operator==(Milliseconds msec) const {
    return ms == msec;
  }

  inline bool operator==(double seconds) const {
    return toSeconds() == seconds;
  }

  inline bool isMalformed() const {
    return ms == MalformedTimestamp;
  }

  struct Components
  {
    unsigned hours, minutes, seconds, msec;
    bool normalize();
  };

  static Timestamp fromComponents(const Components &c);

  void set(unsigned hours, unsigned minutes, unsigned seconds, unsigned msec)
  {
    Components c = { hours, minutes, seconds, msec };
    setComponents(c);
  }

  void set(unsigned minutes, unsigned seconds, unsigned msec)
  {
    Components c = { 0, minutes, seconds, msec };
    setComponents(c);
  }

  void setComponents(const Components &c);
  bool toComponents(Components &c) const;

  inline double toSeconds() const {
    return double(ms) / 1000;
  }

  inline operator Milliseconds() const {
    return ms;
  }
private:
  Milliseconds ms;
};

} // TimedText

#endif // __TimedText_Timestamp__
