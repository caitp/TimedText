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
  EXPECT_EQ(expectedSet, cue.setVertical(text)) << "in testSetVertical #" << run;
  EXPECT_EQ(expectedVertical, cue.vertical()) << "in testSetVertical #" << run;
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
  testSetVertical("",true,Cue::Horizontal);
  testSetVertical("lr",true,Cue::VerticalLeftToRight);
  testSetVertical("rl",true,Cue::VerticalRightToLeft);
  testSetVertical(NULL,false);
  testSetVertical("lr ",false);
  testSetVertical("rl ",false);
  testSetVertical("lrr",false);
  testSetVertical("rll",false);
  testSetVertical("0",false);
  testSetVertical("1",false);
  testSetVertical("2",false);
}
