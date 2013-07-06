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
#include <gtest/gtest.h>
using namespace TimedText;

TEST(Cue,Assignment)
{
  Cue cueA(WebVTTCue, 0.000, 1.000);
  Cue cueB;
  cueB = cueA;
  EXPECT_EQ(WebVTTCue, cueB.type());
  EXPECT_EQ(0.000, cueB.startTime().toSeconds());
  EXPECT_EQ(1.000, cueB.endTime().toSeconds());
}

TEST(Cue,SetId)
{
  Cue cue(WebVTTCue, 0.000, 1.000);
  EXPECT_STREQ("",cue.id());
  cue.setId("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", cue.id());
  // TODO: Test with TTML Cues too!
}

TEST(Cue,SetText)
{
  Cue cue(WebVTTCue, 0.000, 1.000);
  EXPECT_STREQ("",cue.text());
  cue.setText("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", cue.text());
  // TODO: Test with TTML Cues too!
}

TEST(Cue,SetStartTime)
{
  Cue cue(WebVTTCue, 0.000, 1.000);
  EXPECT_EQ(0.000,cue.startTime().toSeconds());
  cue.setStartTime(6667.500);
  EXPECT_EQ(6667.500,cue.startTime().toSeconds());
  // TODO: Test with TTML Cues too!
}

TEST(Cue,SetEndTime)
{
  Cue cue(WebVTTCue, 0.000, 1.000);
  EXPECT_EQ(1.000,cue.endTime().toSeconds());
  cue.setEndTime(8362.316);
  EXPECT_EQ(8362.316,cue.endTime().toSeconds());
  // TODO: Test with TTML Cues too!
}

