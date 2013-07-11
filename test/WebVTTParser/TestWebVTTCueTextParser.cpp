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

#include <TimedText/WebVTTParser.h>
#include <gtest/gtest.h>
using namespace TimedText;

bool
parseCuetext(const char *text, Node &result)
{
  String cuetext(text,-1);
  return WebVTTParser::cuetextToNodes(cuetext, result);
}

TEST(WebVTTCueTextParser,AmpersandEntity)
{
  Node result;
  Node child;
  EXPECT_TRUE(parseCuetext("Potatoes&amp; Balgerps", result));
  EXPECT_EQ(result.element(),InternalTextNode);
  EXPECT_EQ(result.childCount(), 1);
  EXPECT_TRUE(result.itemAt(0, child));
  EXPECT_EQ(TextNode, child.element());
  EXPECT_STREQ("Potatoes; Balgerps", child.text());
}

TEST(WebVTTCueTextParser,LessThanEntity)
{
  Node result;
  Node child;
  EXPECT_TRUE(parseCuetext("&lt;3", result));
  EXPECT_EQ(result.element(),InternalTextNode);
  EXPECT_EQ(result.childCount(), 1);
  EXPECT_TRUE(result.itemAt(0, child));
  EXPECT_EQ(TextNode, child.element());
  EXPECT_STREQ("<3", child.text());
}

TEST(WebVTTCueTextParser,GreaterThanEntity)
{
  Node result;
  Node child;
  EXPECT_TRUE(parseCuetext("--&gt;", result));
  EXPECT_EQ(result.element(),InternalTextNode);
  EXPECT_EQ(result.childCount(), 1);
  EXPECT_TRUE(result.itemAt(0, child));
  EXPECT_EQ(TextNode, child.element());
  EXPECT_STREQ("-->", child.text());
}

TEST(WebVTTCueTextParser,LeftToRightMarkEntity)
{
  Node result;
  Node child;
  EXPECT_TRUE(parseCuetext("\f\t&lrm;", result));
  EXPECT_EQ(result.element(),InternalTextNode);
  EXPECT_EQ(result.childCount(), 1);
  EXPECT_TRUE(result.itemAt(0, child));
  EXPECT_EQ(TextNode, child.element());
  EXPECT_STREQ("\f\t\xe2\x80\x8e", child.text());
}

TEST(WebVTTCueTextParser,RightToLeftMarkEntity)
{
  Node result;
  Node child;
  EXPECT_TRUE(parseCuetext("\f\t&rlm;", result));
  EXPECT_EQ(result.element(),InternalTextNode);
  EXPECT_EQ(result.childCount(), 1);
  EXPECT_TRUE(result.itemAt(0, child));
  EXPECT_EQ(TextNode, child.element());
  EXPECT_STREQ("\f\t\xe2\x80\x8f", child.text());
}

TEST(WebVTTCueTextParser,NoBreakSpaceEntity)
{
  Node result;
  Node child;
  EXPECT_TRUE(parseCuetext("  &nbsp;", result));
  EXPECT_EQ(result.element(),InternalTextNode);
  EXPECT_EQ(result.childCount(), 1);
  EXPECT_TRUE(result.itemAt(0, child));
  EXPECT_EQ(TextNode, child.element());
  EXPECT_STREQ("  \xc2\xa0", child.text());
}

TEST(WebVTTCueTextParser,TimestampTag)
{
  Node result;
  Node child;
  EXPECT_TRUE(parseCuetext("<00:59.000>Valid timestamp tag.", result));
  EXPECT_EQ(result.element(),InternalTextNode);
  EXPECT_EQ(2, result.childCount());
  EXPECT_TRUE(result.itemAt(0, child));
  EXPECT_EQ(TimestampNode, child.element());
  EXPECT_EQ(59000,child.timestamp());
  EXPECT_TRUE(result.itemAt(1, child));
  EXPECT_EQ(TextNode, child.element());
  EXPECT_STREQ("Valid timestamp tag.", child.text());
}

TEST(WebVTTCueTextParser,MalformedTimestampTag)
{
  Node result;
  Node child;
  EXPECT_TRUE(parseCuetext("<0:59.000>Invalid minute component.", result));
  EXPECT_EQ(result.element(),InternalTextNode);
  EXPECT_EQ(1, result.childCount());
  EXPECT_TRUE(result.itemAt(0, child));
  EXPECT_EQ(TextNode, child.element());
  EXPECT_STREQ("Invalid minute component.", child.text());
}

TEST(WebVTTCueTextParser,RubyBaseNestedMarkup)
{
  Node result;
  Node child;
  EXPECT_TRUE(parseCuetext(
    "<ruby>\xec\x95\x88<b>\xeb\x85\x95</b><rt>Hello!</ruby>",
    result));
  EXPECT_EQ(InternalTextNode, result.element());
  EXPECT_EQ(1, result.childCount());
  EXPECT_TRUE(result.itemAt(0, child));
    EXPECT_EQ(RubyNode, child.element());
    EXPECT_EQ(3, child.childCount());
    Node ruby = child;
    EXPECT_TRUE(ruby.itemAt(0, child));
      EXPECT_EQ(TextNode, child.element());
      EXPECT_STREQ("\xec\x95\x88", child.text());
    EXPECT_TRUE(ruby.itemAt(1, child));
      EXPECT_EQ(BoldNode, child.element());
      Node bold = child;
      EXPECT_EQ(1, bold.childCount());
      EXPECT_TRUE(bold.itemAt(0, child));
        EXPECT_EQ(TextNode, child.element());
        EXPECT_STREQ("\xeb\x85\x95", child.text());
    EXPECT_TRUE(ruby.itemAt(2, child));
      EXPECT_EQ(RubyTextNode, child.element());
      EXPECT_EQ(1, child.childCount());
      Node text;
      EXPECT_TRUE(child.itemAt(0, text));
        EXPECT_EQ(TextNode, text.element());
        EXPECT_STREQ("Hello!", text.text());
}
