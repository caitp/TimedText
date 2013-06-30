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

#include <TimedText/WebVTTParser.h>
#include <TimedText/SynchronousBuffer.h>
#include <gtest/gtest.h>
using namespace TimedText;

void testParseHeader(const char *text, bool shouldFinal,
                     bool shouldParse,
                     WebVTTParser::Status shouldStatus)
{
  SynchronousBuffer buffer;
  WebVTTParser parser(buffer);
  EXPECT_TRUE(buffer.refill(text, -1, shouldFinal));
  EXPECT_EQ(shouldParse, parser.parseHeader())
    << "when trying to parse \"" << text << "\"";
  EXPECT_EQ(parser.status, shouldStatus);
}

TEST(SynchronousWebVTTParser, ParseHeader)
{
  testParseHeader("WEBVTT", true, true, WebVTTParser::Finished);
  testParseHeader("WEBVTT\n", true, true, WebVTTParser::Finished);
  testParseHeader("WEBVTT\r", true, true, WebVTTParser::Finished);
  testParseHeader("WEBVTT wutnow", true, true, WebVTTParser::Finished);
  testParseHeader("WEBVTT\tohai", true, true, WebVTTParser::Finished);
  testParseHeader("WEBVTTT", true, false, WebVTTParser::Aborted);
  testParseHeader("\xEF\xBB\xBFWEBVTT", true, true, WebVTTParser::Finished);
  testParseHeader("\xEF\xBB\xBFWEBVTT\n", true, true, WebVTTParser::Finished);
  testParseHeader("\xEF\xBB\xBFWEBVTT\r", true, true, WebVTTParser::Finished);
  testParseHeader("\xEF\xBB\xBFWEBVTT ", true, true, WebVTTParser::Finished);
  testParseHeader("\xEF\xBB\xBFWEBVTT\t", true, true, WebVTTParser::Finished);
  testParseHeader("\xEF\xBB\xBFWEBVTTT", true, false, WebVTTParser::Aborted);
  testParseHeader("\xBB\xBFWEBVTT", true, false, WebVTTParser::Aborted);
  testParseHeader("\xBFWEBVTT", true, false, WebVTTParser::Aborted);
  testParseHeader("\xEF\xBBWEBVTT", true, false, WebVTTParser::Aborted);
  testParseHeader("\xEF\xBB\xBFPEBVTT", true, false, WebVTTParser::Aborted);
}
