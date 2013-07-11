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

#include <TimedText/StringBuilder.h>
#include <gtest/gtest.h>
using namespace TimedText;

// Make sure that we aren't allocating anything when constructing
// with the default constructor.
TEST(StringBuilder,InitializeEmpty)
{
  StringBuilder sb;
  EXPECT_EQ(&StringBuilder::empty, sb.d);
}

// Make sure we allocate the correct ammount when constructing
// with the capacity constructor. (The length should be zero)
TEST(StringBuilder,InitializeWithCapacity)
{
  bool result;
  StringBuilder sb(500,result);
  EXPECT_TRUE(result);
  EXPECT_EQ(500,sb.capacity());
  EXPECT_EQ(0,sb.length());
}

// Check that append operation works correctly
TEST(StringBuilder,Append)
{
  bool result;
  StringBuilder sb(128,result);
  EXPECT_TRUE(result);
  EXPECT_TRUE(sb.append("Phnglui"));
  EXPECT_EQ(7, sb.length());
  EXPECT_TRUE(sb.append(' '));
  EXPECT_EQ(8, sb.length());
  EXPECT_TRUE(sb.append("mglw nafh Cthulhu R'lyeh wgah nagl", -1));
  EXPECT_EQ(42, sb.length());
  EXPECT_TRUE(sb.append(" fhtagn", 7));
  EXPECT_EQ(49, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", sb.text());
}
