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

TEST(EmptyNode,Initialize)
{
  Node node;
  EXPECT_EQ(EmptyNode,node.type());
  EXPECT_EQ(NullNode,node.element());
}

TEST(EmptyNode,GetText)
{
  Node node;
  EXPECT_STREQ("", node.text());
}

TEST(EmptyNode,GetVoice)
{
  Node node;
  EXPECT_STREQ("", node.voice());
}

TEST(EmptyNode,GetTimestamp)
{
  Node node;
  EXPECT_EQ(MalformedTimestamp,node.timestamp());
}

TEST(EmptyNode,GetLang)
{
  Node node;
  EXPECT_STREQ("", node.lang());
}

TEST(EmptyNode,GetApplicableClasses)
{
  Node node;
  List<String> applicableClasses = node.applicableClasses();
  EXPECT_EQ(0, applicableClasses.count());
}

TEST(EmptyNode,GetChildren)
{
  Node node;
  const List<Node> &children = node.children();
  EXPECT_TRUE(children.isEmpty());
}

TEST(EmptyNode,SetText)
{
  Node node;
  EXPECT_FALSE(node.setText(String()));
  EXPECT_FALSE(node.setText(String(0)));
  EXPECT_FALSE(node.setText(String("")));
  EXPECT_FALSE(node.setText(String("Meow")));
  EXPECT_STREQ("", node.text());
}

TEST(EmptyNode,SetVoice)
{
  Node node;
  EXPECT_FALSE(node.setVoice(String()));
  EXPECT_FALSE(node.setVoice(String(0)));
  EXPECT_FALSE(node.setVoice(String("")));
  EXPECT_FALSE(node.setVoice(String("Meow")));
  EXPECT_STREQ("", node.voice());
}

TEST(EmptyNode,SetTimestamp)
{
  Node node;
  EXPECT_FALSE(node.setTimestamp(MalformedTimestamp));
  EXPECT_FALSE(node.setTimestamp(666.0));
  EXPECT_EQ(MalformedTimestamp, node.timestamp());
}

TEST(EmptyNode,SetLang)
{
  Node node;
  EXPECT_FALSE(node.setLang(String()));
  EXPECT_FALSE(node.setLang(String(0)));
  EXPECT_FALSE(node.setLang(String("")));
  EXPECT_FALSE(node.setLang(String("Meow")));
  EXPECT_STREQ("", node.lang());
}

TEST(EmptyNode,SetApplicableClasses)
{
  List<String> classes;
  EXPECT_TRUE(classes.push(String("typeA")));
  EXPECT_TRUE(classes.push(String("typeB")));
  Node node;
  EXPECT_FALSE(node.setApplicableClasses(classes));
  List<String> applicableClasses = node.applicableClasses();
  EXPECT_EQ(0, applicableClasses.count());
}

TEST(EmptyNode,PushPop)
{
  Node node;
  Node leaf(LeafNode,TextNode);
  Node result;
  EXPECT_FALSE(node.push(leaf));
  EXPECT_EQ(0, node.childCount());
  EXPECT_FALSE(node.pop(result));
  EXPECT_EQ(EmptyNode, result.type());
  EXPECT_EQ(NullNode,result.element());
}

TEST(EmptyNode,UnshiftShift)
{
  Node node;
  Node leaf(LeafNode,TextNode);
  Node result;
  EXPECT_FALSE(node.unshift(leaf));
  EXPECT_EQ(0, node.childCount());
  EXPECT_FALSE(node.shift(result));
  EXPECT_EQ(EmptyNode, result.type());
  EXPECT_EQ(NullNode,result.element());
}

TEST(EmptyNode,InsertTake)
{
  Node node;
  Node leaf(LeafNode,TextNode);
  Node result;
  EXPECT_FALSE(node.insert(0, leaf));
  EXPECT_EQ(0, node.childCount());
  EXPECT_FALSE(node.take(0, result));
  EXPECT_EQ(EmptyNode,result.type());
  EXPECT_EQ(NullNode,result.element());
}

TEST(EmptyNode,Begin)
{
  Node node;
  Node::iterator it = node.begin();
  EXPECT_EQ(0, it.i);
}

TEST(EmptyNode,End)
{
  Node node;
  Node::iterator it = node.end();
  EXPECT_EQ(0, it.i);
}

TEST(EmptyNode,ConstBegin)
{
  const Node node;
  Node::const_iterator it = node.begin();
  EXPECT_EQ(0, it.i);
}

TEST(EmptyNode,ConstEnd)
{
  const Node node;
  Node::const_iterator it = node.end();
  EXPECT_EQ(0, it.i);
}
