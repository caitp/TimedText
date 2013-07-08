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

#include <TimedText/Node.h>
#include <gtest/gtest.h>
using namespace TimedText;

TEST(TimestampNode,Initialize)
{
  Node node(LeafNode,TimestampNode);
  EXPECT_EQ(LeafNode,node.type());
  EXPECT_EQ(TimestampNode,node.element());
}

TEST(TimestampNode,GetText)
{
  Node node(LeafNode,TimestampNode);
  EXPECT_STREQ("", node.text());
}

TEST(TimestampNode,GetVoice)
{
  Node node(LeafNode,TimestampNode);
  EXPECT_STREQ("", node.voice());
}

TEST(TimestampNode,GetTimestamp)
{
  Node node(LeafNode,TimestampNode);
  EXPECT_EQ(MalformedTimestamp,node.timestamp());
}

TEST(TimestampNode,GetLang)
{
  Node node(InternalNode,TimestampNode);
  EXPECT_STREQ("", node.lang());
}

TEST(TimestampNode,GetApplicableClasses)
{
  Node node(TimestampNode);
  List<String> applicableClasses = node.applicableClasses();
  EXPECT_EQ(0, applicableClasses.count());
}

TEST(TimestampNode,SetText)
{
  Node node(LeafNode,TimestampNode);
  EXPECT_FALSE(node.setText(String()));
  EXPECT_FALSE(node.setText(String(0)));
  EXPECT_FALSE(node.setText(String("")));
  EXPECT_FALSE(node.setText(String("Meow")));
  EXPECT_STREQ("", node.text());
}

TEST(TimestampNode,SetVoice)
{
  Node node(LeafNode,TimestampNode);
  EXPECT_FALSE(node.setVoice(String()));
  EXPECT_FALSE(node.setVoice(String(0)));
  EXPECT_FALSE(node.setVoice(String("")));
  EXPECT_FALSE(node.setVoice(String("Meow")));
  EXPECT_STREQ("", node.voice());
}

TEST(TimestampNode,SetTimestamp)
{
  Node node(LeafNode,TimestampNode);
  EXPECT_TRUE(node.setTimestamp(MalformedTimestamp));
  EXPECT_TRUE(node.setTimestamp(666.0));
  EXPECT_EQ(666000, node.timestamp());
}

TEST(TimestampNode,SetLang)
{
  Node node(LeafNode,TimestampNode);
  EXPECT_FALSE(node.setLang(String()));
  EXPECT_FALSE(node.setLang(String(0)));
  EXPECT_FALSE(node.setLang(String("")));
  EXPECT_FALSE(node.setLang(String("Meow")));
  EXPECT_STREQ("", node.lang());
}

TEST(TimestampNode,SetApplicableClasses)
{
  List<String> classes;
  EXPECT_TRUE(classes.push(String("typeA")));
  EXPECT_TRUE(classes.push(String("typeB")));
  Node node(LeafNode,TimestampNode);
  EXPECT_FALSE(node.setApplicableClasses(classes));
  List<String> applicableClasses = node.applicableClasses();
  EXPECT_EQ(0, applicableClasses.count());
}
