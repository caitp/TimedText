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