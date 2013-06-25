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
  EXPECT_TRUE(str.startsWith("Phnglui",-1));
  EXPECT_TRUE(str.startsWith("Phnglui",7));
  EXPECT_TRUE(str.startsWith('P'));
}

TEST(String,EndsWith)
{
  String str("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_TRUE(str.endsWith("fhtagn"));
  EXPECT_TRUE(str.endsWith("fhtagn",-1));
  EXPECT_TRUE(str.endsWith("fhtagn",6));
  EXPECT_TRUE(str.endsWith('n'));
}

TEST(String,Contains)
{
  String str("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  EXPECT_TRUE(str.contains("Cthulhu"));
  EXPECT_TRUE(str.contains("Cthulhu",-1));
  EXPECT_TRUE(str.contains("Cthulhu",7));
  EXPECT_TRUE(str.contains('C'));
}
