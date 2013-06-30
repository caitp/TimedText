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

#include <TimedText/String.h>
#include <gtest/gtest.h>
using namespace TimedText;

TEST(String,StartsWith)
{
  String str("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_TRUE(str.startsWith("Phnglui"));
  EXPECT_TRUE(str.startsWith("Phnglui", -1));
  EXPECT_TRUE(str.startsWith("Phnglui", 7));
  EXPECT_TRUE(str.startsWith('P'));
}

TEST(String,EndsWith)
{
  String str("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_TRUE(str.endsWith("fhtagn"));
  EXPECT_TRUE(str.endsWith("fhtagn", -1));
  EXPECT_TRUE(str.endsWith("fhtagn", 6));
  EXPECT_TRUE(str.endsWith('n'));
}

TEST(String,Contains)
{
  String str("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_TRUE(str.contains("Cthulhu"));
  EXPECT_TRUE(str.contains("Cthulhu", -1, 0));
  EXPECT_TRUE(str.contains("Cthulhu", 7));
  EXPECT_TRUE(str.contains('C'));
}

TEST(String,Substring)
{
  String str("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  String sub1 = str.substring(18,7);
  String sub2 = str.substring(26);
  String sub3 = str.substring(26,-1);
  String sub4 = str.substring(-1,50);
  String sub5 = str.substring(43,100);
  EXPECT_STREQ("Cthulhu",sub1.text());
  EXPECT_STREQ("R'lyeh wgah nagl fhtagn", sub2.text());
  EXPECT_STREQ("R'lyeh wgah nagl fhtagn", sub3.text());
  EXPECT_STREQ("", sub4.text());
  EXPECT_STREQ("fhtagn", sub5.text());
}

TEST(String,parseInt)
{
  int pos = 0;
  int digits;
  String str("12345678");
  EXPECT_EQ(12345678, str.parseInt(pos, &digits));
  EXPECT_EQ(8, pos);
  EXPECT_EQ(8, digits);
  str = String("~123");
  pos = 0;
  EXPECT_EQ(0, str.parseInt(pos, &digits));
  EXPECT_EQ(0, pos);
  EXPECT_EQ(0, digits);
}

TEST(String,SkipWhitespace)
{
  String str(" \t\f \r\n  Trees cover up a multitude of sins.");
  int pos = 0;
  EXPECT_EQ(8, str.skipWhitespace(pos));
  EXPECT_EQ(8, pos);
  EXPECT_STREQ("Trees cover up a multitude of sins.", str.text() + pos);
  str = String("Trees cover up a multitude of sins.");
  pos = 0;
  EXPECT_EQ(0, str.skipWhitespace(pos));
  EXPECT_EQ(0, pos);
  EXPECT_STREQ("Trees cover up a multitude of sins.", str.text());
}
