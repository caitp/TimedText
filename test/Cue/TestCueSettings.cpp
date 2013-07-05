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

void testSetAlign(const char *text, bool expectedSet,
                  Cue::Align expectedAlign = Cue::Middle)
{
  static int run = 0;
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  if(!expectedSet)
    expectedAlign = Cue::defaultAlign;
  ++run;
  EXPECT_EQ(expectedSet, cue.setAlign(text))
    << "in testSetAlign(string) #" << run;
  EXPECT_EQ(expectedAlign, cue.align())
    << "in testSetAlign(string) #" << run;
}

void testSetAlign(Cue::Align align, bool expectedSet,
                  Cue::Align expectedAlign = Cue::Middle)
{
  static int run = 0;
  Cue cue(Cue::WebVTTCue, 0.000, 1.000);
  if(!expectedSet)
    expectedAlign = Cue::defaultAlign;
  ++run;
  EXPECT_EQ(expectedSet, cue.setAlign(align))
    << "in testSetAlign(enum) #" << run;
  EXPECT_EQ(expectedAlign, cue.align())
    << "in testSetAlign(enum) #" << run;
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
  const Cue::Align defaultAlign = Cue::defaultAlign;
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
  testSetAlign(static_cast<Cue::Align>(-1),false,defaultAlign);
  testSetAlign(static_cast<Cue::Align>(68),false,defaultAlign);
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
  testSetLine("",false);
  testSetLine(NULL,false);
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

void testApplySettings(const char *text, Cue::Align expectedAlign,
                       int expectedLine, bool expectedSnapToLines,
                       int expectedPosition, int expectedSize,
                       Cue::Vertical expectedVertical)
{
  static int run = 1;
  // This is a fairly long-winded function...
  String line = String(text);
  Cue cue(Cue::WebVTTCue,0.000, 1.000);
  cue.applySettings(line);
  EXPECT_EQ(expectedAlign, cue.align())
    << "testApplySettings #" << run;
  EXPECT_EQ(expectedLine, cue.line())
    << "testApplySettings #" << run;
  EXPECT_EQ(expectedSnapToLines, cue.snapToLines())
    << "testApplySettings #" << run;
  EXPECT_EQ(expectedPosition, cue.position())
    << "testApplySettings #" << run;
  EXPECT_EQ(expectedSize, cue.size())
    << "testApplySettings #" << run;
  EXPECT_EQ(expectedVertical, cue.vertical())
    << "testApplySettings #" << run;
  ++run;
}

TEST(CueSettings,ApplySettings)
{
  // Avoid using references to const values in EXPECT_EQ statements
  const Cue::Align align = Cue::defaultAlign;
  const int line = Cue::defaultLine;
  const bool snapToLines = Cue::defaultSnapToLines;
  const int position = Cue::defaultPosition;
  const int size = Cue::defaultSize;
  const Cue::Vertical vertical = Cue::defaultVertical;

  // The CueSettings application function Cue::applySettings()
  // This only applies to WebVTT attributes, and specifies an entire
  // line of settings.
  //
  // First, test individual, correct settings
  testApplySettings("align:start", Cue::Start, line, snapToLines, position,
                    size, vertical);
  testApplySettings("align:middle", Cue::Middle, line, snapToLines, position,
                    size, vertical);
  testApplySettings("align:end", Cue::End, line, snapToLines, position, size,
                    vertical);
  testApplySettings("align:left", Cue::Left, line, snapToLines, position, size,
                    vertical);
  testApplySettings("align:right", Cue::Right, line, snapToLines, position,
                    size, vertical);
  testApplySettings("line:-1", align, -1, true, position, size, vertical);
  testApplySettings("line:101", align, 101, true, position, size, vertical);
  testApplySettings("line:0%", align, 0, false, position, size, vertical);
  testApplySettings("line:100%", align, 100, false, position, size, vertical);
  testApplySettings("position:0%", align, line, snapToLines, 0, size,
                    vertical);
  testApplySettings("position:100%", align, line, snapToLines, 100, size,
                    vertical);
  testApplySettings("size:0%", align, line, snapToLines, position, 0,
                    vertical);
  testApplySettings("size:100%", align, line, snapToLines, position, 100,
                    vertical);
  testApplySettings("vertical:lr", align, line, snapToLines, position, size,
                    Cue::VerticalLeftToRight);
  testApplySettings("vertical:rl", align, line, snapToLines, position, size,
                    Cue::VerticalRightToLeft);

  // Test that we handle long words correctly, and are able to recover and
  // parse the rest of the line after
  const char longWord[] = 
                 "Very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,very,very,very,"
                 "very,very,very,very,very,very,very,very,very,veryLONGWORD "
                 "vertical:lr";
  testApplySettings(longWord, align, line, snapToLines, position, size,
                    Cue::VerticalLeftToRight);

  // Test that settings without a separator are ignored
  testApplySettings("alignleft",align,line,snapToLines,position,size,vertical);
  testApplySettings("alignright",align,line,snapToLines,position,size,vertical);
  testApplySettings("alignstart",align,line,snapToLines,position,size,vertical);
  testApplySettings("alignmiddle",align,line,snapToLines,position,size,vertical);
  testApplySettings("alignend",align,line,snapToLines,position,size,vertical);
  testApplySettings("line-1",align,line,snapToLines,position,size,vertical);
  testApplySettings("line101",align,line,snapToLines,position,size,vertical);
  testApplySettings("line0%",align,line,snapToLines,position,size,vertical);
  testApplySettings("line100%",align,line,snapToLines,position,size,vertical);
  testApplySettings("position0%",align,line,snapToLines,position,size,vertical);
  testApplySettings("position100%",align,line,snapToLines,position,size,vertical);
  testApplySettings("size0%",align,line,snapToLines,position,size,vertical);
  testApplySettings("size100%",align,line,snapToLines,position,size,vertical);
  testApplySettings("verticalrl",align,line,snapToLines,position,size,vertical);
  testApplySettings("verticallr",align,line,snapToLines,position,size,vertical);

  // Test that settings that start with a separator are ignored
  testApplySettings(":left",align,line,snapToLines,position,size,vertical);
  testApplySettings(":right",align,line,snapToLines,position,size,vertical);
  testApplySettings(":start",align,line,snapToLines,position,size,vertical);
  testApplySettings(":middle",align,line,snapToLines,position,size,vertical);
  testApplySettings(":end",align,line,snapToLines,position,size,vertical);
  testApplySettings(":-1",align,line,snapToLines,position,size,vertical);
  testApplySettings(":101",align,line,snapToLines,position,size,vertical);
  testApplySettings(":0%",align,line,snapToLines,position,size,vertical);
  testApplySettings(":100%",align,line,snapToLines,position,size,vertical);
  testApplySettings(":0%",align,line,snapToLines,position,size,vertical);
  testApplySettings(":100%",align,line,snapToLines,position,size,vertical);
  testApplySettings(":0%",align,line,snapToLines,position,size,vertical);
  testApplySettings(":100%",align,line,snapToLines,position,size,vertical);
  testApplySettings(":rl",align,line,snapToLines,position,size,vertical);
  testApplySettings(":lr",align,line,snapToLines,position,size,vertical);

  // Test that settings that end with a separator are ignored
  testApplySettings("align:",align,line,snapToLines,position,size,vertical);
  testApplySettings("line:",align,line,snapToLines,position,size,vertical);
  testApplySettings("position:",align,line,snapToLines,position,size,vertical);
  testApplySettings("size:",align,line,snapToLines,position,size,vertical);
  testApplySettings("vertical:",align,line,snapToLines,position,size,vertical);

  // Test that two settings which are not separated by whitespace, are
  // ignored (bad value)
  testApplySettings("align:leftvertical:rl",align,line,snapToLines,position,
                    size,vertical);
}
