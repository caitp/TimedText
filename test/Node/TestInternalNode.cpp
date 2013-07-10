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
#include <stdlib.h>
using namespace TimedText;

TEST(InternalNode,Initialize)
{
  // Trying to create an internal node with invalid element types should
  // always fail, and return an EmptyNode instead.
  Node node(InternalNode,static_cast<NodeElementType>(FirstNodeElementType));
  EXPECT_EQ(EmptyNode,node.type());
  EXPECT_EQ(NullNode,node.element());
  Node node2(InternalNode,static_cast<NodeElementType>(LastNodeElementType));
  EXPECT_EQ(EmptyNode,node2.type());
  EXPECT_EQ(NullNode,node2.element());
}

// Tests for internal nodes are shared, because the same method is used
// for each internal node class.
TEST(InternalNode,GetChildren)
{
  Node node(InternalNode,VoiceNode);
  const List<Node> &children = node.children();
  EXPECT_TRUE(children.isEmpty());
}

TEST(InternalNode,PushPop)
{
  Node node(InternalNode,VoiceNode);
  Node leaf(LeafNode,TextNode);
  Node result;
  EXPECT_TRUE(node.push(leaf));
  EXPECT_EQ(1, node.childCount());
  EXPECT_TRUE(node.pop(result));
  EXPECT_EQ(LeafNode, result.type());
  EXPECT_EQ(TextNode, result.element());
}

TEST(InternalNode,UnshiftShift)
{
  Node node(InternalNode,BoldNode);
  Node leaf(LeafNode,TextNode);
  Node result;
  EXPECT_TRUE(node.unshift(leaf));
  EXPECT_EQ(1, node.childCount());
  EXPECT_TRUE(node.shift(result));
  EXPECT_EQ(LeafNode, result.type());
  EXPECT_EQ(TextNode, result.element());
}

TEST(InternalNode,InsertTake)
{
  Node node(InternalNode,ItalicNode);
  Node leaf(LeafNode,TextNode);
  Node result;
  EXPECT_TRUE(node.insert(0, leaf));
  EXPECT_EQ(1, node.childCount());
  EXPECT_TRUE(node.take(0, result));
  EXPECT_EQ(LeafNode,result.type());
  EXPECT_EQ(TextNode,result.element());
}

TEST(InternalNode,Begin)
{
  Node node(InternalNode,UnderlineNode);
  Node leaf(LeafNode,TextNode);
  // Need data for this test to be meaningful :(
  EXPECT_TRUE(node.push(leaf));
  Node::iterator it = node.begin();
  ASSERT_NE(uintptr(0), uintptr(it.i));
  EXPECT_EQ(leaf, *it);
}

TEST(InternalNode,End)
{
  Node node(InternalNode,DivNode);
  Node leaf(LeafNode,TextNode);
  
  // Need data for this test to be meaningful :(
  EXPECT_TRUE(node.push(leaf));
  Node::iterator it = node.end();
  ASSERT_NE(uintptr(0), uintptr((--it).i));
  EXPECT_EQ(leaf, *it);
}

TEST(InternalNode,ConstBegin)
{
  Node node(InternalNode,SpanNode);
  const Node &ref = node;
  Node leaf(LeafNode,TextNode);
  // Need data for this test to be meaningful :(
  EXPECT_TRUE(node.push(leaf));
  Node::const_iterator it = ref.begin();
  ASSERT_NE(uintptr(0), uintptr(it.i));
  EXPECT_EQ(leaf, *it);
}

TEST(InternalNode,ConstEnd)
{
  Node node(InternalNode,PNode);
  const Node &ref = node;
  Node leaf(LeafNode,TextNode);
  EXPECT_TRUE(node.push(leaf));
  Node::const_iterator it = ref.end();
  ASSERT_NE(uintptr(0), uintptr((--it).i));
  EXPECT_EQ(leaf, *it);
}
