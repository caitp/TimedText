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

#include <TimedText/Timestamp.h>
#include "WebVTTTokenizer.h"
#include <gtest/gtest.h>
using namespace TimedText;

static int testTokenizeStartTagRun = 1;

void testTokenizeStartTag(const char *text,
                          const char *expectedName,
                          const char *expectedAnnotation)
{
  int &run = testTokenizeStartTagRun;
  WebVTTToken result;
  WebVTTTokenizer tokenizer;
  int position = 0;
  String token(text,-1);
  EXPECT_TRUE(tokenizer.next(token, position, result))
    << "testTokenizeStartTag #" << run;
  EXPECT_EQ(WebVTTToken::StartTag, result.type())
    << "testTokenizeStartTag #" << run;
  String name;
  EXPECT_TRUE(result.data(name)) << "testTokenizeStartTag #" << run;
  EXPECT_STREQ(expectedName, name) << "testTokenizeStartTag #" << run;
  EXPECT_STREQ(expectedAnnotation, result.annotation())
    << "testTokenizeStartTag #" << run;
  List<String> classes = result.classes();
  EXPECT_EQ(0, classes.count())
    << "testTokenizeStartTag #" << run;
  ++run;
}

template <int N>
void testTokenizeStartTag(const char *text,
                          const char *expectedName,
                          const char *expectedAnnotation,
                          const char *(&expectedClasses)[N])
{
  int &run = testTokenizeStartTagRun;
  WebVTTToken result;
  WebVTTTokenizer tokenizer;
  int position = 0;
  String token(text);
  EXPECT_TRUE(tokenizer.next(token, position, result))
    << "testTokenizeStartTag #" << run;
  EXPECT_EQ(WebVTTToken::StartTag, result.type())
    << "testTokenizeStartTag #" << run;
  String name;
  EXPECT_TRUE(result.data(name)) << "testTokenizeStartTag #" << run;
  EXPECT_STREQ(expectedName, name) << "testTokenizeStartTag #" << run;
  EXPECT_STREQ(expectedAnnotation, result.annotation())
    << "testTokenizeStartTag #" << run;
  List<String> classes = result.classes();
  EXPECT_EQ(N, classes.count())
    << "testTokenizeStartTag #" << run;
  for(unsigned i=0; i<N; ++i)
    EXPECT_STREQ(expectedClasses[i], classes[i])
      << "testTokenizeStartTag #" << run;
  ++run;
}

// Test a simple start tag (no classes or annotation)
TEST(WebVTTTokenizer,StartTag)
{
  testTokenizeStartTag("<c>", "c", "");
  testTokenizeStartTag("<b>", "b", "");
  testTokenizeStartTag("<i>", "i", "");
  testTokenizeStartTag("<u>", "u", "");
  testTokenizeStartTag("<v>", "v", "");
  testTokenizeStartTag("<lang>", "lang", "");
  testTokenizeStartTag("<ruby>", "ruby", "");
}

// Test a start tag with simple annotation (no spaces)
TEST(WebVTTTokenizer,StartTagWithAnnotation)
{
  testTokenizeStartTag("<c    withAnnotation   >", "c", "withAnnotation");
  testTokenizeStartTag("<b   withAnnotation  >", "b", "withAnnotation");
  testTokenizeStartTag("<i  withAnnotation   >", "i", "withAnnotation");
  testTokenizeStartTag("<u     withAnnotation >", "u", "withAnnotation");
  testTokenizeStartTag("<v  \twithAnnotation\f  >", "v", "withAnnotation");
  testTokenizeStartTag("<lang\n\ten-US\r\f>", "lang", "en-US");
  testTokenizeStartTag("<ruby\n\twithAnnotation\r\f>", "ruby", "withAnnotation");
  testTokenizeStartTag("<rt\n\twithAnnotation\r\f>", "rt", "withAnnotation");
  // Test a start tag with multi-word annotation
  testTokenizeStartTag("<c \n\r  with annotation   >", "c", "with annotation");
  testTokenizeStartTag("<b    with annotation   >", "b", "with annotation");
  testTokenizeStartTag("<i  \f\f  with annotation\t\t\n  >", "i", "with annotation");
  testTokenizeStartTag("<u\t\fwith annotation\r\n>", "u", "with annotation");
  testTokenizeStartTag("<v\n\twith annotation\r\f>", "v", "with annotation");
  testTokenizeStartTag("<lang\n\ten US\r\f>", "lang", "en US");
  testTokenizeStartTag("<ruby\n\twith annotation\r\f>", "ruby", "with annotation");
  testTokenizeStartTag("<rt\n\twith annotation\r\f>", "rt", "with annotation");
}

// Test a start tag with an annotation which begins with a period
// (like a class)
TEST(WebVTTTokenizer,StartTagWithFullStopAnnotation)
{
  testTokenizeStartTag("<c  .meow   >", "c", ".meow");
  testTokenizeStartTag("<b   .meow  >", "b", ".meow");
  testTokenizeStartTag("<i .meow>", "i", ".meow");
  testTokenizeStartTag("<u  .meow >", "u", ".meow");
  testTokenizeStartTag("<v    .meow>", "v", ".meow");
}

// Test a start tag with single class and no annotation
TEST(WebVTTTokenizer,StartTagWithClass)
{
  const char *oneClass[] = { "meow" };
  testTokenizeStartTag("<c.meow>", "c", "", oneClass);
  testTokenizeStartTag("<b.meow>", "b", "", oneClass);
  testTokenizeStartTag("<i.meow>", "i", "", oneClass);
  testTokenizeStartTag("<u.meow>", "u", "", oneClass);
  testTokenizeStartTag("<v.meow>", "v", "", oneClass);
  testTokenizeStartTag("<lang.meow>", "lang", "", oneClass);
  testTokenizeStartTag("<ruby.meow>", "ruby", "", oneClass);
  testTokenizeStartTag("<rt.meow>", "rt", "", oneClass);
}

// Test a start tag with single class and annotation
TEST(WebVTTTokenizer,StartTagWithClassAndAnnotation)
{
  const char *oneClass[] = { "meow" };
  testTokenizeStartTag("<c.meow  gerbilgarb  >", "c", "gerbilgarb", oneClass);
  testTokenizeStartTag("<b.meow gerbilgarb >", "b", "gerbilgarb", oneClass);
  testTokenizeStartTag("<i.meow   gerbilgarb>", "i", "gerbilgarb", oneClass);
  testTokenizeStartTag("<u.meow gerbilgarb >", "u", "gerbilgarb", oneClass);
  testTokenizeStartTag("<v.meow  gerbilgarb  >", "v", "gerbilgarb", oneClass);
  testTokenizeStartTag("<lang.meow en-US>", "lang", "en-US", oneClass);
  testTokenizeStartTag("<ruby.meow derp>", "ruby", "derp", oneClass);
  testTokenizeStartTag("<rt.meow blargh>", "rt", "blargh", oneClass);
}

// Test a start tag with multiple classes and no annotation
TEST(WebVTTTokenizer,StartTagWithClasses)
{
  const char *triClass[] = { "foo", "bar", "baz" };
  testTokenizeStartTag("<c.foo.bar.baz>", "c", "", triClass);
  testTokenizeStartTag("<b.foo.bar.baz>", "b", "", triClass);
  testTokenizeStartTag("<i.foo.bar.baz>", "i", "", triClass);
  testTokenizeStartTag("<u.foo.bar.baz>", "u", "", triClass);
  testTokenizeStartTag("<v.foo.bar.baz>", "v", "", triClass);
  testTokenizeStartTag("<lang.foo.bar.baz>", "lang", "", triClass);
  testTokenizeStartTag("<ruby.foo.bar.baz>", "ruby", "", triClass);
  testTokenizeStartTag("<rt.foo.bar.baz>", "rt", "", triClass);

}

// Test a start tag with multiple classes and annotation
TEST(WebVTTTokenizer,StartTagWithClassesAndAnnotation)
{
  const char *triClass[] = { "foo", "bar", "baz" };
  testTokenizeStartTag("<c.foo.bar.baz derp>", "c", "derp", triClass);
  testTokenizeStartTag("<b.foo.bar.baz  derp  >", "b", "derp", triClass);
  testTokenizeStartTag("<i.foo.bar.baz derp >", "i", "derp", triClass);
  testTokenizeStartTag("<u.foo.bar.baz der p >", "u", "der p", triClass);
  testTokenizeStartTag("<v.foo.bar.baz derka>", "v", "derka", triClass);
  testTokenizeStartTag("<lang.foo.bar.baz en-US>", "lang", "en-US", triClass);
  testTokenizeStartTag("<ruby.foo.bar.baz spoony bool>", "ruby", "spoony bool", triClass);
  testTokenizeStartTag("<rt.foo.bar.baz tatotop>", "rt", "tatotop", triClass);
}

// Test a nameless start tag with single class
TEST(WebVTTTokenizer,NamelessStartTagWithAnnotation)
{
  testTokenizeStartTag("<\r\nwithAnnotation\r\n>", "", "withAnnotation");
  testTokenizeStartTag("<\nwithAnnotation\f\t>", "", "withAnnotation");
  testTokenizeStartTag("<\rwithAnnotation  >", "", "withAnnotation");
  testTokenizeStartTag("< with annotation\r\n>", "", "with annotation");
  testTokenizeStartTag("<\twith annotation\f\t>", "", "with annotation");
  testTokenizeStartTag("<\fwith annotation  >", "", "with annotation");
  testTokenizeStartTag("<\f\f.meow\r\n>", "", ".meow");
  testTokenizeStartTag("<\t.meow\f\t>", "", ".meow");
  testTokenizeStartTag("< \t.meow  >", "", ".meow");
}

// Test a nameless start tag with single class
TEST(WebVTTTokenizer,NamelessStartTagWithClass)
{
  const char *oneClass[] = { "meow" };
  testTokenizeStartTag("<.meow\r\n>", "", "", oneClass);
  testTokenizeStartTag("<.meow\f\t>", "", "", oneClass);
  testTokenizeStartTag("<.meow  >", "", "", oneClass);
}

// Test a nameless start class wtih a single class and annotation
TEST(WebVTTTokenizer,NamelessStartTagWithClassAndAnnotation)
{
  const char *oneClass[] = { "meow" };
  testTokenizeStartTag("<.meow\rgerbilgarb  \n>", "", "gerbilgarb", oneClass);
  testTokenizeStartTag("<.meow\ngerbilgarb \r\n>", "", "gerbilgarb", oneClass);
  testTokenizeStartTag("<.meow\f\tgerbilgarb\r>", "", "gerbilgarb", oneClass);
  testTokenizeStartTag("<.meow\r\ngerbilgarb\t >", "", "gerbilgarb", oneClass);
  testTokenizeStartTag("<.meow  gerbilgarb\f  >", "", "gerbilgarb", oneClass);
}

// Test a nameless start tag with classes
TEST(WebVTTTokenizer,NamelessStartTagWithClasses)
{
  const char *triClass[] = { "foo", "bar", "baz" };
  testTokenizeStartTag("<.foo.bar.baz\r\n>", "", "", triClass);
  testTokenizeStartTag("<.foo.bar.baz\f\t>", "", "", triClass);
  testTokenizeStartTag("<.foo.bar.baz  >", "", "", triClass);
}

// Test a nameless start class wtih classes and annotation
TEST(WebVTTTokenizer,NamelessStartTagWithClassesAndAnnotation)
{
  const char *triClass[] = { "foo", "bar", "baz" };
  testTokenizeStartTag("<.foo.bar.baz\rgerbilgarb  \n>", "", "gerbilgarb", triClass);
  testTokenizeStartTag("<.foo.bar.baz\ngerbilgarb \r\n>", "", "gerbilgarb", triClass);
  testTokenizeStartTag("<.foo.bar.baz\f\tgerbilgarb\r>", "", "gerbilgarb", triClass);
  testTokenizeStartTag("<.foo.bar.baz\r\ngerbilgarb\t >", "", "gerbilgarb", triClass);
  testTokenizeStartTag("<.foo.bar.baz  gerbilgarb\f  >", "", "gerbilgarb", triClass);
}

void testTokenizeTimestampTag(const char *text,
                              Milliseconds expectedTimestamp)
{
  static int run = 1;
  if(expectedTimestamp < 0)
    expectedTimestamp = MalformedTimestamp;
  WebVTTToken result;
  WebVTTTokenizer tokenizer;
  int position = 0;
  String token(text,-1);
  EXPECT_TRUE(tokenizer.next(token, position, result))
    << "testTokenizeTimestampTag #" << run;
  EXPECT_EQ(WebVTTToken::TimestampTag, result.type())
    << "testTokenizeTimestampTag #" << run;
  Timestamp ts = result.timestamp();
  EXPECT_EQ(expectedTimestamp, ts) << "testTokenizeTimestampTag #" << run;
  EXPECT_STREQ("", result.annotation())
    << "testTokenizeTimestampTag #" << run;
  List<String> classes = result.classes();
  EXPECT_EQ(0, classes.count())
    << "testTokenizeTimestampTag #" << run;
  ++run;
}

// TimestampTag Tests
TEST(WebVTTTokenizer,TimestampTag)
{
  testTokenizeTimestampTag("<0:00:00.000>", 0);
  testTokenizeTimestampTag("<00:00:00.000>", 0);
  testTokenizeTimestampTag("<00:00.000>", 0);
  testTokenizeTimestampTag("<0:0:00.000>", -1);
  testTokenizeTimestampTag("<00:0:00.000>", -1);
  testTokenizeTimestampTag("<0:00.000>", -1);
  testTokenizeTimestampTag("<0:00:0.000>", -1);
  testTokenizeTimestampTag("<00:00:0.000>", -1);
  testTokenizeTimestampTag("<00:0.000>", -1);
  testTokenizeTimestampTag("<0:00:00.00>", -1);
  testTokenizeTimestampTag("<00:00:00.00>", -1);
  testTokenizeTimestampTag("<00:00.00>", -1);
  testTokenizeTimestampTag("<0:00:00.000 >", -1);
  testTokenizeTimestampTag("<00:00:00.000 >", -1);
  testTokenizeTimestampTag("<00:00.000\rfoo>", -1);
  testTokenizeTimestampTag("<0:00:00.000\nfulp>", -1);
  testTokenizeTimestampTag("<00:00:00.000\tgaga>", -1);
  testTokenizeTimestampTag("<00:00.000\fgoogoo>", -1);
  testTokenizeTimestampTag("<0:59:59.999>", 3599999);
  testTokenizeTimestampTag("<00:59:59.999>", 3599999);
  testTokenizeTimestampTag("<59:59.999>", 3599999);
}

void testTokenizeEndTag(const char *text,
                        const char *expectedName)
{
  static int run = 1;
  WebVTTToken result;
  WebVTTTokenizer tokenizer;
  int position = 0;
  String token(text,-1);
  EXPECT_TRUE(tokenizer.next(token, position, result))
    << "testTokenizeEndTag #" << run;
  EXPECT_EQ(WebVTTToken::EndTag, result.type())
    << "testTokenizeEndTag #" << run;
  String name;
  EXPECT_TRUE(result.data(name)) << "testTokenizeEndTag #" << run;
  EXPECT_STREQ(expectedName, name) << "testTokenizeEndTag #" << run;
  EXPECT_STREQ("", result.annotation())
    << "testTokenizeEndTag #" << run;
  List<String> classes = result.classes();
  EXPECT_EQ(0, classes.count())
    << "testTokenizeEndTag #" << run;
  ++run;
}

// EndTag Tests
TEST(WebVTTTokenizer,EndTag)
{
  testTokenizeEndTag("</c>","c");
  testTokenizeEndTag("</b>","b");
  testTokenizeEndTag("</i>","i");
  testTokenizeEndTag("</u>","u");
  testTokenizeEndTag("</v>","v");
  testTokenizeEndTag("</lang>","lang");
  testTokenizeEndTag("</ruby>","ruby");
  testTokenizeEndTag("</rt>","rt");
}

void testTokenizeText(const char *text,
                      const char *expectedText)
{
  static int run = 1;
  WebVTTToken result;
  WebVTTTokenizer tokenizer;
  int position = 0;
  String token(text,-1);
  EXPECT_TRUE(tokenizer.next(token, position, result))
    << "testTokenizeText #" << run;
  EXPECT_EQ(WebVTTToken::Text, result.type())
    << "testTokenizeText #" << run;
  String data;
  EXPECT_TRUE(result.data(data));
  EXPECT_STREQ(expectedText, data) << "testTokenizeText #" << run;
  EXPECT_STREQ("", result.annotation())
    << "testTokenizeText #" << run;
  List<String> classes = result.classes();
  EXPECT_EQ(0, classes.count())
    << "testTokenizeText #" << run;
  ++run;
}

// Text Tests
TEST(WebVTTTokenizer,TextTag)
{
  testTokenizeText("\r\n<lang en-US>","\n");
  testTokenizeText("derpington jr.</b>","derpington jr.");
  testTokenizeText("Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn",
                   "Phnglui mglw nafh Cthulhu R'lyeh wgah nagl fhtagn");
  testTokenizeText("Phnglui mglw nafh Cthulhu <ruby>R'lyeh wgah nagl <rt>fhtagn",
                   "Phnglui mglw nafh Cthulhu ");
}
