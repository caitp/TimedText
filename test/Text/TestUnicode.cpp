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

#include <TimedText/Unicode.h>
#include <gtest/gtest.h>
using namespace TimedText;

static inline void
testUtf8LeadByte(char ch, bool isLead, bool isSingle,
                               bool isTrail)
{
  EXPECT_EQ(isLead,Unicode::utf8IsLead(ch));
  EXPECT_EQ(isTrail,Unicode::utf8IsTrail(ch));
  EXPECT_EQ(isSingle,Unicode::utf8IsSingle(ch));
}

// Ensure that we treat things as lead/trail/single UTF8
// sequences, depending on the contents of the first byte.
TEST(Unicode,Utf8LeadByte)
{
  testUtf8LeadByte(0xEC,true,false,false);
  testUtf8LeadByte(0x95,false,false,true);
  testUtf8LeadByte(0x88,false,false,true);
  testUtf8LeadByte('a',false,true,false);
  testUtf8LeadByte('A',false,true,false);
  testUtf8LeadByte('z',false,true,false);
  testUtf8LeadByte('Z',false,true,false);
}

// Ensure that get expected multibyte character lengths
// based on the lead byte
TEST(Unicode,Utf8CharLength)
{
  // U+0000 .. U+007F
  EXPECT_EQ(0,Unicode::utf8NumTrailBytes(0x00));
  EXPECT_EQ(0,Unicode::utf8NumTrailBytes(0x7F));
  EXPECT_EQ(1,Unicode::utf8Length(0x00));
  EXPECT_EQ(1,Unicode::utf8Length(0x7F));

  // U+0080 .. U+07FF
  EXPECT_EQ(1,Unicode::utf8NumTrailBytes(0xC2));
  EXPECT_EQ(1,Unicode::utf8NumTrailBytes(0xC3));
  EXPECT_EQ(2,Unicode::utf8Length(0x80));
  EXPECT_EQ(2,Unicode::utf8Length(0xFF));

  // U+0800 .. U+FFFF
  EXPECT_EQ(2,Unicode::utf8NumTrailBytes(0xE0));
  EXPECT_EQ(2,Unicode::utf8NumTrailBytes(0xEF));
  EXPECT_EQ(3,Unicode::utf8Length(0x800));
  EXPECT_EQ(3,Unicode::utf8Length(0xFFFD));

  // U+10000 .. U+1FFFFF
  EXPECT_EQ(3,Unicode::utf8NumTrailBytes(0xF0));
  EXPECT_EQ(3,Unicode::utf8NumTrailBytes(0xF4));
  EXPECT_EQ(4,Unicode::utf8Length(0x10000));
  // This is in the private use plane, but it shouldn't be a replacement
  // character.
  EXPECT_EQ(4,Unicode::utf8Length(0x10FFFD));

  // Anything beyond U+10FFFF is considered a non-character. utf8Length
  // returns 0 for non characters.
  EXPECT_EQ(0,Unicode::utf8Length(0x110000));
}

template<size_t expectedLen>
static inline void testToUtf8(unsigned long ucs4, bool expectedIsUnicode,
                              const char (&expectedValue)[expectedLen] ) {
  char utf8[4] = "";
  int len;
  EXPECT_EQ(expectedIsUnicode,Unicode::toUtf8(ucs4,utf8,len));
  // Buffers are bigger than they need to be, because we need a NULL
  // terminator for EXPECT_STREQ to work correctly.
  //
  // Could do this differently by constructing std::strings, but this isn't
  // so bad, for now.
  EXPECT_EQ(static_cast<int>(expectedLen-1),len);
  EXPECT_STREQ(expectedValue,utf8);
}

TEST(Unicode,ToUtf8)
{
  const char replacement[] = "\xef\xbf\xbd";
  // Test that we get the unicode replacement character for codepoints beyond
  // the unicode codepoint limit
  testToUtf8(Unicode::CodepointLimit + 1,false,replacement);

  // Test that we get the unicode replacement character
  // for the non-character block in the Arabic Presentation Forms script
  for(unsigned long i = Unicode::NonCharBegin; i <= Unicode::NonCharEnd; ++i)
    testToUtf8(i,false,replacement);
  // Test that we get the unicode replacement character
  // for each plane's non-character bytes
  for(int i=0; i < 17; ++i) {
    unsigned long codePoint = i << 16;
    testToUtf8(codePoint | 0xFFFE,false,replacement);
    testToUtf8(codePoint | 0xFFFF,false,replacement);
  }

  // For each 7bit ASCII character, make sure that we get the expected single
  // byte result
  for(unsigned long codePoint = 0; codePoint < 0x80; ++codePoint) {
    const char expected[] = { static_cast<char>(codePoint & 0xFF), '\x00' };
    testToUtf8(codePoint,true,expected);
  }

  // Also make sure that Latin1/ISO-8859-1 characters are encoded correctly:
  for(unsigned long codePoint = 0x80; codePoint < 0x100; ++codePoint) {
    const char expected[] = { static_cast<char>(0xC0 | (codePoint >> 6)), 
                              static_cast<char>(0x80 | (codePoint & 0x3F)),
                              '\x00' };
    testToUtf8(codePoint,true,expected);
  }
}