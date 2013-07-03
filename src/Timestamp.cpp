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

#include <TimedText/Timestamp.h>

namespace TimedText
{

bool
Timestamp::Components::normalize()
{
  bool normal = true;
  if(msec > 999) {
    unsigned tmp = msec;
    seconds += tmp / millisPerSecond;
    msec = tmp % millisPerSecond;
    normal = false;
  }
  if(seconds > 59) {
    unsigned tmp = seconds;
    minutes += tmp / secondsPerMin;
    seconds = tmp % secondsPerMin;
    normal = false;
  }
  if(minutes > 59) {
    unsigned tmp = minutes;
    hours += tmp / minutesPerHour;
    minutes = tmp % minutesPerHour;
    normal = false;
  }
  return normal;
}

void
Timestamp::setComponents(const Timestamp::Components &c)
{
  Components normal = c;
  normal.normalize();
  ms = (c.hours * millisPerHour)
     + (c.minutes * millisPerMinute)
     + (c.seconds * millisPerSecond)
     + (c.msec);
}

Timestamp
Timestamp::fromComponents(const Timestamp::Components &c)
{
  Components normal = c;
  normal.normalize();
  return Timestamp((c.hours * millisPerHour)
                 + (c.minutes * millisPerMinute)
                 + (c.seconds * millisPerSecond)
                 + (c.msec));
}

bool
Timestamp::toComponents(Timestamp::Components &c) const
{
  if(isMalformed())
    return false;

  c.hours = ms / millisPerHour;
  c.minutes = (ms % millisPerHour) / millisPerMinute;
  c.seconds = (ms % millisPerMinute) / millisPerSecond;
  c.msec = ms % millisPerSecond;
  return true;
}

} // TimedText
