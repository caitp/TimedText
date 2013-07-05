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

#include <TimedText/SynchronousBuffer.h>
#include <gtest/gtest.h>
using namespace TimedText;

TEST(SynchronousBuffer,RefillValidUtf8)
{
  SynchronousBuffer buffer;
  buffer.finish();
  EXPECT_TRUE(buffer.eof());
  EXPECT_TRUE(buffer.refill("Phnglui mglw nafh Cthulhu R'lyeh "
  	                        "wgah nagl fhtagn", 49));
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh "
  	           "wgah nagl fhtagn", buffer.curr());
}

TEST(SynchronousBuffer,SeekAbsolute)
{
	SynchronousBuffer buffer;
  EXPECT_TRUE(buffer.refill("Phnglui mglw nafh Cthulhu R'lyeh "
                            "wgah nagl fhtagn"));
  buffer.seek(18, true);
  EXPECT_EQ(18, buffer.i);
}

TEST(SynchronousBuffer,SeekRelative)
{
  SynchronousBuffer buffer;
  EXPECT_TRUE(buffer.refill("Phnglui mglw nafh Cthulhu R'lyeh "
                            "wgah nagl fhtagn"));
  // Absolute seek first, so that we don't start from 0
  buffer.seek(18, true);
  buffer.seek(-12);
  EXPECT_EQ(6, buffer.i);
}

TEST(SynchronousBuffer,ReadValidUtf8)
{
  // Make sure this is filled with 0s for STREQ, because the read routine
  // does not zero terminate
  char text[0x40] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
  SynchronousBuffer buffer;
  EXPECT_TRUE(buffer.refill("Phnglui mglw nafh Cthulhu R'lyeh "
                            "wgah nagl fhtagn"));
  buffer.seek(18,true);
  EXPECT_EQ(7,buffer.read(text, 7));
  EXPECT_STREQ("Cthulhu", text);
  buffer.seek(8);
  EXPECT_EQ(23, buffer.read(text, sizeof(text)));
  EXPECT_STREQ("R'lyeh wgah nagl fhtagn", text);
}

TEST(SynchronousBuffer,CollectWord)
{
  SynchronousBuffer buffer;
  String word;
  char unused;
  int n;
  EXPECT_TRUE(buffer.refill("Phnglui mglw nafh Cthulhu R'lyeh "
                            "wgah nagl fhtagn"));
  EXPECT_TRUE(buffer.collectWord(word, &n));
  EXPECT_EQ(7, n);
  EXPECT_STREQ("Phnglui", word.text());
  word.clear();
  EXPECT_TRUE(buffer.next(unused));
  EXPECT_TRUE(buffer.collectWord(word, &n));
  EXPECT_EQ(4, n);
  EXPECT_STREQ("mglw", word.text());
  word.clear();
  EXPECT_TRUE(buffer.next(unused));
  EXPECT_TRUE(buffer.collectWord(word, &n));
  EXPECT_EQ(4, n);
  EXPECT_STREQ("nafh", word.text());
  word.clear();
  EXPECT_TRUE(buffer.next(unused));
  EXPECT_TRUE(buffer.collectWord(word, &n));
  EXPECT_EQ(7, n);
  EXPECT_STREQ("Cthulhu", word.text());
  word.clear();
  EXPECT_TRUE(buffer.next(unused));
}


TEST(SynchronousBuffer,CollectDigits)
{

  SynchronousBuffer buffer;
  String digits;
  int n;
  char unused;
  EXPECT_TRUE(buffer.refill("1234x56789x0",-1,true));
  EXPECT_TRUE(buffer.collectDigits(digits,&n));
  EXPECT_STREQ("1234",digits.text());
  EXPECT_EQ(4, n);
  EXPECT_TRUE(buffer.next(unused)); // skip 'x'
  EXPECT_TRUE(buffer.collectDigits(digits, &n));
  EXPECT_STREQ("123456789", digits.text());
  EXPECT_EQ(5, n);
  EXPECT_TRUE(buffer.next(unused)); // skip 'x'

  EXPECT_TRUE(buffer.collectDigits(digits, &n));
  EXPECT_STREQ("1234567890", digits.text());
  EXPECT_EQ(1, n);
}

TEST(SynchronousBuffer,SkipWhitespace)
{
  SynchronousBuffer buffer;
  int n;
  EXPECT_TRUE(buffer.refill("\t\t\t\t\r\n\r\n   \f\f\fHello World!"));
  EXPECT_TRUE(buffer.skipWhitespace(&n));
  EXPECT_EQ(14, n);
  EXPECT_STREQ("Hello World!", buffer.curr());
}