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

TEST(TextNode,Initialize)
{
  Node node(LeafNode,TextNode);
  EXPECT_EQ(LeafNode,node.type());
  EXPECT_EQ(TextNode,node.element());
}

TEST(TextNode,GetText)
{
  Node node(LeafNode,TextNode);
  EXPECT_STREQ("", node.text());
}

TEST(TextNode,GetVoice)
{
  Node node(LeafNode,TextNode);
  EXPECT_STREQ("", node.voice());
}

TEST(TextNode,GetTimestamp)
{
  Node node(LeafNode,TextNode);
  EXPECT_EQ(MalformedTimestamp,node.timestamp());
}

TEST(TextNode,GetLang)
{
  Node node(InternalNode,TextNode);
  EXPECT_STREQ("", node.lang());
}

TEST(TextNode,GetApplicableClasses)
{
  Node node(TextNode);
  List<String> applicableClasses = node.applicableClasses();
  EXPECT_EQ(0, applicableClasses.count());
}

TEST(TextNode,SetText)
{
  Node node(LeafNode,TextNode);
  EXPECT_TRUE(node.setText(String()));
  EXPECT_TRUE(node.setText(String(0)));
  EXPECT_TRUE(node.setText(String("")));
  EXPECT_TRUE(node.setText(String("Meow")));
  EXPECT_STREQ("Meow", node.text());
}

TEST(TextNode,SetVoice)
{
  Node node(LeafNode,TextNode);
  EXPECT_FALSE(node.setVoice(String()));
  EXPECT_FALSE(node.setVoice(String(0)));
  EXPECT_FALSE(node.setVoice(String("")));
  EXPECT_FALSE(node.setVoice(String("Meow")));
  EXPECT_STREQ("", node.voice());
}

TEST(TextNode,SetTimestamp)
{
  Node node(LeafNode,TextNode);
  EXPECT_FALSE(node.setTimestamp(MalformedTimestamp));
  EXPECT_FALSE(node.setTimestamp(666.0));
  EXPECT_EQ(MalformedTimestamp, node.timestamp());
}

TEST(TextNode,SetLang)
{
  Node node(LeafNode,TextNode);
  EXPECT_FALSE(node.setLang(String()));
  EXPECT_FALSE(node.setLang(String(0)));
  EXPECT_FALSE(node.setLang(String("")));
  EXPECT_FALSE(node.setLang(String("Meow")));
  EXPECT_STREQ("", node.lang());
}

TEST(TextNode,SetApplicableClasses)
{
  List<String> classes;
  EXPECT_TRUE(classes.push(String("typeA")));
  EXPECT_TRUE(classes.push(String("typeB")));
  Node node(LeafNode,TextNode);
  EXPECT_FALSE(node.setApplicableClasses(classes));
  List<String> applicableClasses = node.applicableClasses();
  EXPECT_EQ(0, applicableClasses.count());
}
