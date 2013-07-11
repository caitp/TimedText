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

TEST(StringBuilder,InitializeWithString)
{
  String string("Hello, world!");
  bool result;
  StringBuilder sb(string, result);
  EXPECT_TRUE(result);
  EXPECT_STREQ("Hello, world!", string.text());
  EXPECT_EQ(13, sb.length());
}

TEST(StringBuilder,ResizeData)
{
  StringBuilder sb;
  EXPECT_TRUE(sb.resizeData(100));
  EXPECT_LE(100, sb.capacity());
  EXPECT_EQ(100, sb.length());
  EXPECT_TRUE(sb.resizeData(200));
  EXPECT_LE(200, sb.capacity());
  EXPECT_EQ(200, sb.length());
  EXPECT_TRUE(sb.resizeData(-1));
  EXPECT_GE(16, sb.capacity());
  EXPECT_EQ(0, sb.length());
}

TEST(StringBuilder,Reserve)
{
  StringBuilder sb;
  EXPECT_TRUE(sb.reserve(100));
  EXPECT_EQ(100, sb.capacity());
  EXPECT_EQ(0, sb.length());
}

TEST(StringBuilder,Expand)
{
  char expectedText[] =
  "                         "
  "                         "
  "                         "
  "                         ";
  StringBuilder sb;
  // Expanding by 100 is essentially like appending 100 spaces
  // to the end of the buffer
  EXPECT_TRUE(sb.expand(100));
  EXPECT_EQ(101, sb.length());
  //EXPECT_EQ('\0', sb.text()[100]);
  EXPECT_STREQ(expectedText, sb.text());
}

TEST(StringBuilder,SetText)
{
  String notEmpty("Hello, world!");
  String empty("", 0);
  StringBuilder sb;
  EXPECT_TRUE(sb.setText(notEmpty));
  EXPECT_EQ(13, sb.length());
  EXPECT_STREQ("Hello, world!", sb.text());
  EXPECT_TRUE(sb.setText(empty));
  EXPECT_EQ(0, sb.length());
  EXPECT_STREQ("", sb.text());
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
  EXPECT_TRUE(sb.append(" fhtag", 6));
  EXPECT_EQ(48, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtag", sb.text());
  EXPECT_TRUE(sb.append('n'));
  EXPECT_EQ(49, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", sb.text());
  EXPECT_TRUE(sb.append(NULL, 0));
  EXPECT_EQ(49, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", sb.text());
}

TEST(StringBuilder,Prepend)
{
  bool result;
  StringBuilder sb(128, result);
  EXPECT_TRUE(result);
  EXPECT_TRUE(sb.prepend(" fhtagn"));
  EXPECT_EQ(7, sb.length());
  EXPECT_STREQ(" fhtagn", sb.text());
  EXPECT_TRUE(sb.prepend(" wgah nagl", -1));
  EXPECT_EQ(17, sb.length());
  EXPECT_STREQ(" wgah nagl fhtagn", sb.text());
  EXPECT_TRUE(sb.prepend("hnglui mglw nafh Cthulhu R'lyeh", 31));
  EXPECT_EQ(48, sb.length());
  EXPECT_STREQ("hnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", sb.text());
  EXPECT_TRUE(sb.prepend('P'));
  EXPECT_EQ(49, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", sb.text());
  EXPECT_TRUE(sb.prepend(NULL, 0));
  EXPECT_EQ(49, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", sb.text());
}

TEST(StringBuilder,Insert)
{
  bool result;
  StringBuilder sb(128, result);
  EXPECT_TRUE(sb.insert(0, "Phnglui nafh wgah fhtagn"));
  EXPECT_EQ(24, sb.length());
  EXPECT_STREQ("Phnglui nafh wgah fhtagn", sb.text());
  EXPECT_TRUE(sb.insert(8, "mglw ", -1));
  EXPECT_EQ(29, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh wgah fhtagn", sb.text());
  EXPECT_TRUE(sb.insert(18, "thulhu R'lyeh ", 14));
  EXPECT_EQ(43, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh thulhu R'lyeh wgah fhtagn", sb.text());
  EXPECT_TRUE(sb.insert(18, 'C'));
  EXPECT_EQ(44, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah fhtagn", sb.text());
  EXPECT_TRUE(sb.insert(38, "nagl "));
  EXPECT_EQ(49, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", sb.text());
  EXPECT_TRUE(sb.insert(40,NULL,0));
  EXPECT_EQ(49, sb.length());
  EXPECT_STREQ("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", sb.text());
  EXPECT_TRUE(sb.insert(-1,"Hello!"));
  EXPECT_TRUE(sb.insert(0,"",0));
  EXPECT_TRUE(sb.insert(0, sb.text(), 8));
  EXPECT_STREQ("Phnglui Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn", sb.text());
  StringBuilder s2(3200, result);
  EXPECT_TRUE(result);
  char tmp[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
               "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
               "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
               "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char insert[] = "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                  "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                  "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                  "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba";
  char expected[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
 char expected2[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  EXPECT_TRUE(s2.insert(0, tmp));
  EXPECT_EQ(208,s2.length());
  EXPECT_TRUE(s2.insert(208,tmp));
  EXPECT_EQ(416,s2.length());
  EXPECT_TRUE(s2.insert(208, insert));
  EXPECT_EQ(624,s2.length());
  EXPECT_STREQ(expected, s2.text());
  EXPECT_TRUE(s2.insert(312,s2.text(), s2.length()));
  EXPECT_STREQ(expected2, s2.text());
  EXPECT_EQ(1248, s2.length());
}

TEST(StringBuilder,IndexOf)
{
  bool result;
  StringBuilder sb(String("Hello, World!"), result);
  EXPECT_TRUE(result);
  EXPECT_EQ(-1, sb.indexOf(NULL, 0, 0));
  EXPECT_EQ(-1, sb.indexOf("Hello", 5, 5));
  EXPECT_EQ(-1, sb.indexOf('H', 5));
  EXPECT_EQ(2, sb.indexOf("llo", -1));
  EXPECT_EQ(5, sb.indexOf(','));
  EXPECT_EQ(7, sb.indexOf(String("World!")));
  EXPECT_EQ(-1, sb.indexOf("World!!", 7));
  EXPECT_EQ(-1, sb.indexOf(""));
}

TEST(StringBuilder,ReplaceAll)
{
  // Test that we don't do anything on null ops
  StringBuilder sb; // Empty
  EXPECT_TRUE(sb.replaceAll("Foo", "Bar"));
  const char *sameSearchAndReplace = "Foo";
  EXPECT_TRUE(sb.replaceAll(sameSearchAndReplace,sameSearchAndReplace));

  EXPECT_TRUE(sb.append("Hello Bob!"));
  // Single replacement
  EXPECT_TRUE(sb.replaceAll("Bob", "World"));
  EXPECT_STREQ("Hello World!", sb.text());

  // Replacement and search string equal lengths (best case)
  sb.clear();
  sb.append("Hello, Waldo");
  StringBuilder sb2;
  sb2.append("Hello, Doggy");
  for(int i=0; i<0x2000; ++i) {
    EXPECT_TRUE(sb.insert(0, "Hello, Waldo"));
    EXPECT_TRUE(sb2.insert(0, "Hello, Doggy"));
  }
  EXPECT_TRUE(sb.replaceAll("Waldo","Doggy"));
  EXPECT_EQ(sb2.length(), sb.length());
  EXPECT_STREQ(sb2.text(), sb.text());

  // Replacement smaller than search string (middle case)
  sb.clear();
  sb2.clear();
  sb.append("Hello Tortoise");
  sb2.append("Hello Donkey");
  for(int i=0; i<0x2000; ++i) {
    EXPECT_TRUE(sb.insert(0, "Hello, Tortoise"));
    EXPECT_TRUE(sb2.insert(0, "Hello, Donkey"));
  }
  EXPECT_TRUE(sb.replaceAll("Tortoise", "Donkey"));
  EXPECT_EQ(sb2.length(), sb.length());
  EXPECT_STREQ(sb2.text(), sb.text());

  // Replacement larger than search string (worst case)
  sb.clear();
  sb2.clear();
  sb.append("Hello World");
  sb2.append("Hello Donkey");
  for(int i=0; i<0x2000; ++i) {
    EXPECT_TRUE(sb.insert(0, "Hello World "));
    EXPECT_TRUE(sb2.insert(0, "Hello Donkey "));
  }
  EXPECT_TRUE(sb.replaceAll("World", "Donkey"));
  EXPECT_STREQ(sb2.text(), sb.text());

  // Search for string, replace with UCS4 character
  sb.clear();
  sb2.clear();
  EXPECT_TRUE(sb.append("Hello, World!"));
  EXPECT_TRUE(sb.replaceAll("Hello", 0x55E8));
  EXPECT_TRUE(sb.replaceAll("World", 0x4E16));
  EXPECT_EQ(9, sb.length());
  EXPECT_STREQ("\xe5\x97\xa8, \xe4\xb8\x96!", sb.text());
  // Search for UCS4 character, replace with string
  EXPECT_TRUE(sb.replaceAll(0x55E8, "Hello"));
  EXPECT_TRUE(sb.replaceAll(0x4E16, "World"));
  EXPECT_EQ(13, sb.length());
  EXPECT_STREQ("Hello, World!", sb.text());

  // Search for UCS4 character, replace with UCS4 character
  sb.clear();
  EXPECT_TRUE(sb.append("\xe5\xa5\xbd\xe5\xa4\xa9")); // 好天
  EXPECT_TRUE(sb.replaceAll(0x597D,0x574F)); // 坏天
  EXPECT_EQ(6, sb.length());
  EXPECT_STREQ("\xe5\x9d\x8f\xe5\xa4\xa9", sb.text());
}
