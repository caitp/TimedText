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

static void testVisitor(const char *cuetext, NodeVisitor &visitor)
{
  Cue cue(WebVTTCue);
  cue.setText(String(cuetext));
  WebVTTParser::parseCuetext(cue);
  cue.visit(visitor);
}

static const char webvttCue1Level[] =
"<b></b><i></i><u></u><c></c><v Cthulhu></v><lang en-US></lang><ruby></ruby>";

TEST(WebVTTCueVisitor,SingleLevel)
{
  class Visitor : public NodeVisitor
  {
  public:
    Visitor() : visited(0) {}
    void fail() {
      ASSERT_TRUE(false);
    }
    bool enter(const Node &node) {
      fail();
      return false;
    }
    void leave(const Node &node) {
    }
    void visit(Node &node) {
      EXPECT_EQ(0, node.childCount()) << "All nodes should be empty";
      visited++;
    }
    int visited;
  };
  Visitor visitor;
  testVisitor(webvttCue1Level, visitor);
  EXPECT_EQ(7, visitor.visited);
}

static const char webvttCue3Level[] =
"<lang en-US><rt>Hello universe </rt><b>It's alright</b></lang>";

TEST(WebVTTCueVisitor,ThreeLevel)
{
  class Visitor : public NodeVisitor
  {
  public:
    Visitor() : visited(0), depth(0), deepest(0) {}
    void fail() {
      ASSERT_TRUE(false);
    }
    bool enter(const Node &node) {
      EXPECT_TRUE(node.element() == LangNode || node.element() == BoldNode);
      stack.push(node);
      ++depth;
      if(depth > deepest)
        deepest = depth;
      return true;
    }
    void leave(const Node &node) {
      Node parent;
      EXPECT_TRUE(stack.pop(parent));
      EXPECT_TRUE(parent==node);
      --depth;
    }
    void visit(Node &node) {
      Node parent;
      stack.lastItem(parent);
      if(parent.element() == LangNode) {
        // There should be no <rt> outside of <ruby>
        EXPECT_NE(RubyTextNode, node.element());
        if(node.element() == TextNode)
          EXPECT_STREQ("Hello universe ", node.text());
      } else if(parent.element() == BoldNode) {
        EXPECT_EQ(TextNode, node.element());
        EXPECT_STREQ("It's alright", node.text());
      }
      visited++;
    }
    List<Node> stack;
    int visited;
    int depth;
    int deepest;
  };
  Visitor visitor;
  testVisitor(webvttCue3Level, visitor);
  EXPECT_EQ(0, visitor.depth);
  EXPECT_EQ(2, visitor.deepest);
  EXPECT_EQ(4, visitor.visited);
}

static const char webvttCueSkipBold[] =
"I don't want to visit <b>Cthulhu's </b>evil <i>henchmen<b> of doom</b></i>";
TEST(WebVTTCueVisitor,SkipBoldBranches)
{
  class Visitor : public NodeVisitor
  {
  public:
    Visitor() : visited(0), depth(0), deepest(0), textTokens(0) {}
    void fail() {
      ASSERT_TRUE(false);
    }
    bool enter(const Node &node) {
      if(node.element() == BoldNode)
        return false;
      ++depth;
      if(depth > deepest)
        deepest = depth;
      return true;
    }
    void leave(const Node &node) {
      --depth;
    }
    void visit(Node &node) {
      if(node.element() == TextNode) {
        text += node.text();
        ++textTokens;
      }
      visited++;
    }
    String text;
    int visited;
    int depth;
    int deepest;
    int textTokens;
  };
  Visitor visitor;
  testVisitor(webvttCueSkipBold, visitor);
  EXPECT_STREQ("I don't want to visit evil henchmen", visitor.text);
  EXPECT_EQ(0, visitor.depth);
  EXPECT_EQ(1, visitor.deepest);

  // Of the 8 total tokens, only on two are not visited (the inner text
  // of the bold nodes)
  EXPECT_EQ(6, visitor.visited);
  EXPECT_EQ(3, visitor.textTokens);
}