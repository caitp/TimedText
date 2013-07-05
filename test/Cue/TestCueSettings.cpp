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

#include <TimedText/Cue.h>
#include <gtest/gtest.h>
using namespace TimedText;

void testSetAlign(const char *text, bool expectedSet, Cue::Align expectedAlign = Cue::Middle)
{
  static int run = 0;
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  if(!expectedSet)
    expectedAlign = Cue::defaultAlign;
  ++run;
  EXPECT_EQ(expectedSet, cue.setAlign(text)) << "in testSetAlign #" << run;
  EXPECT_EQ(expectedAlign, cue.align()) << "in testSetAlign #" << run;
}

void testSetLine(const char *text, bool expectedSet,
                 int expectedLine = 0, bool expectedSnapToLines = false)
{
  static int run = 0;
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  if(!expectedSet) {
    expectedLine = Cue::defaultLine;
    expectedSnapToLines = Cue::defaultSnapToLines;
  }
  ++run;
  EXPECT_EQ(expectedSet, cue.setLine(text)) << "in testSetLine #" << run;
  EXPECT_EQ(expectedLine, cue.line()) << "in testSetLine #" << run;
  EXPECT_EQ(expectedSnapToLines, cue.snapToLines()) << "in testSetLine #" << run;
}

void testSetPosition(const char *text, bool expectedSet, int expectedPosition = 0)
{
  static int run = 0;
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  if(!expectedSet)
    expectedPosition = Cue::defaultPosition;
  ++run;
  EXPECT_EQ(expectedSet, cue.setPosition(text)) << "in testSetPosition #" << run;
  EXPECT_EQ(expectedPosition, cue.position()) << "in testSetPosition #" << run;
}

void testSetSize(const char *text, bool expectedSet, int expectedSize = 0)
{
  static int run = 0;
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  if(!expectedSet)
    expectedSize = Cue::defaultSize;
  ++run;
  EXPECT_EQ(expectedSet, cue.setSize(text)) << "in testSetSize #" << run;
  EXPECT_EQ(expectedSize, cue.size()) << "in testSetSize #" << run;
}

void testSetVertical(const char *text, bool expectedSet,
                     Cue::Vertical expectedVertical = Cue::Horizontal)
{
  static int run = 0;
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  if(!expectedSet)
    expectedVertical = Cue::defaultVertical;
  ++run;
  EXPECT_EQ(expectedSet, cue.setVertical(text))
    << "in testSetVertical(string) #" << run;
  EXPECT_EQ(expectedVertical, cue.vertical())
    << "in testSetVertical(string) #" << run;
}

void testSetVertical(Cue::Vertical vertical, bool expectedSet,
                     Cue::Vertical expectedVertical = Cue::Horizontal)
{
  static int run = 0;
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  if(!expectedSet)
    expectedVertical = Cue::defaultVertical;
  ++run;
  EXPECT_EQ(expectedSet, cue.setVertical(vertical))
    << "in testSetVertical(enum) #" << run;
  EXPECT_EQ(expectedVertical, cue.vertical())
    << "in testSetVertical(enum) #" << run;
}

TEST(CueSettings,SetId)
{
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  EXPECT_STREQ("",cue.id());
  cue.setId("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", cue.id());
  // TODO: Test with TTML Cues too!
}

TEST(CueSettings,SetText)
{
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  EXPECT_STREQ("",cue.text());
  cue.setText("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", cue.text());
  // TODO: Test with TTML Cues too!
}

TEST(CueSettings,SetStartTime)
{
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  EXPECT_EQ(0.000,cue.startTime().toSeconds());
  cue.setStartTime(6667.334);
  EXPECT_EQ(6667.334,cue.startTime().toSeconds());
  // TODO: Test with TTML Cues too!
}

TEST(CueSettings,SetEndTime)
{
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  EXPECT_EQ(1.000,cue.endTime().toSeconds());
  cue.setEndTime(8362.316);
  EXPECT_EQ(8362.316,cue.endTime().toSeconds());
  // TODO: Test with TTML Cues too!
}

TEST(CueSettings,SetAlign)
{
  testSetAlign("start",true,Cue::Start);
  testSetAlign("middle",true,Cue::Middle);
  testSetAlign("end",true,Cue::End);
  testSetAlign("left",true,Cue::Left);
  testSetAlign("right",true,Cue::Right);
  testSetAlign("12345",false);
  testSetAlign("!@#$",false);
  testSetAlign("bottom",false);
  testSetAlign("",false);
  testSetAlign(NULL,false);
}

TEST(CueSettings,SetLine)
{
  testSetLine("-1",true,-1,true);
  testSetLine("10%",true,10,false);
  testSetLine("0%",true,0,false);
  testSetLine("100%",true,100,false);
  testSetLine("-1%",false);
  testSetLine("101%",false);
  testSetLine("1%1",false);
  testSetLine("1-1",false);
  testSetLine("1z",false);
  testSetLine("%",false);
  testSetLine("-",false);
  testSetLine("alpha",false);
  testSetPosition("",false);
  testSetPosition(NULL,false);
}

TEST(CueSettings,SetPosition)
{
  testSetPosition("0%",true,0);
  testSetPosition("100%",true,100);
  testSetPosition("-1%",false);
  testSetPosition("101%",false);
  testSetPosition("0",false);
  testSetPosition("100",false);
  testSetPosition("%",false);
  testSetPosition("%100",false);
  testSetPosition("alpha",false);
  testSetPosition("",false);
  testSetPosition(NULL,false);
}

TEST(CueSettings,SetSize)
{
  testSetSize("0%",true,0);
  testSetSize("100%",true,100);
  testSetSize("-1%",false);
  testSetSize("101%",false);
  testSetSize("0",false);
  testSetSize("100",false);
  testSetSize("%",false);
  testSetSize("%100",false);
  testSetSize("alpha",false);
  testSetSize("",false);
  testSetSize(NULL,false);
}

TEST(CueSettings,SetVertical)
{
  const Cue::Vertical defaultVertical = Cue::defaultVertical;
  testSetVertical("",true,Cue::Horizontal);
  testSetVertical("lr",true,Cue::VerticalLeftToRight);
  testSetVertical("rl",true,Cue::VerticalRightToLeft);
  testSetVertical(static_cast<Cue::Vertical>(-1),false,defaultVertical);
  testSetVertical(static_cast<Cue::Vertical>(2000),false,defaultVertical);
  testSetVertical(NULL,false);
  testSetVertical("lr ",false);
  testSetVertical("rl ",false);
  testSetVertical("lrr",false);
  testSetVertical("rll",false);
  testSetVertical("0",false);
  testSetVertical("1",false);
  testSetVertical("2",false);
}

TEST(CueSettings,EmptyCue)
{
  // Avoid using references to const values in EXPECT_EQ statements
  const Cue::Align defaultAlign = Cue::defaultAlign;
  const int defaultLine = Cue::defaultLine;
  const int defaultPosition = Cue::defaultPosition;
  const int defaultSize = Cue::defaultSize;
  const bool defaultSnapToLines = Cue::defaultSnapToLines;
  const Cue::Vertical defaultVertical = Cue::defaultVertical;

  // Trying to set empty cue settings should always fail
  Cue cue;

  // Id
  cue.setId("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_STREQ("", cue.id());

  // Text
  cue.setText("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_STREQ("", cue.text());

  // StartTime
  cue.setStartTime(68.067);
  EXPECT_EQ(MalformedTimestamp, cue.startTime());

  // EndTime
  cue.setEndTime(98.678);
  EXPECT_EQ(MalformedTimestamp, cue.endTime());

  // Align
  EXPECT_FALSE(cue.setAlign(Cue::Start));
  EXPECT_FALSE(cue.setAlign(Cue::Middle));
  EXPECT_FALSE(cue.setAlign(Cue::End));
  EXPECT_FALSE(cue.setAlign(Cue::Left));
  EXPECT_FALSE(cue.setAlign(Cue::Right));
  EXPECT_FALSE(cue.setAlign("start"));
  EXPECT_FALSE(cue.setAlign("middle"));
  EXPECT_FALSE(cue.setAlign("end"));
  EXPECT_FALSE(cue.setAlign("left"));
  EXPECT_FALSE(cue.setAlign("right"));
  EXPECT_EQ(defaultAlign,cue.align());

  // Line
  EXPECT_FALSE(cue.setLine(0, false));
  EXPECT_FALSE(cue.setLine(0, false));
  EXPECT_FALSE(cue.setLine(-101, true));
  EXPECT_FALSE(cue.setLine(101, true));
  EXPECT_FALSE(cue.setLine("0%"));
  EXPECT_FALSE(cue.setLine("100%"));
  EXPECT_FALSE(cue.setLine("-101"));
  EXPECT_FALSE(cue.setLine("1001"));
  EXPECT_EQ(defaultLine, cue.line());
  EXPECT_EQ(defaultSnapToLines, cue.snapToLines());

  // Position
  EXPECT_FALSE(cue.setPosition(0));
  EXPECT_FALSE(cue.setPosition(100));
  EXPECT_FALSE(cue.setPosition("0%"));
  EXPECT_FALSE(cue.setPosition("100%"));
  EXPECT_EQ(defaultPosition, cue.position());

  // Size
  EXPECT_FALSE(cue.setSize(0));
  EXPECT_FALSE(cue.setSize(100));
  EXPECT_FALSE(cue.setSize("0%"));
  EXPECT_FALSE(cue.setSize("100%"));
  EXPECT_EQ(defaultSize, cue.size());

  // Vertical
  EXPECT_FALSE(cue.setVertical(Cue::Horizontal));
  EXPECT_FALSE(cue.setVertical(Cue::VerticalLeftToRight));
  EXPECT_FALSE(cue.setVertical(Cue::VerticalRightToLeft));
  EXPECT_FALSE(cue.setVertical(""));
  EXPECT_FALSE(cue.setVertical("lr"));
  EXPECT_FALSE(cue.setVertical("rl"));
  EXPECT_EQ(defaultVertical, cue.vertical());
}
