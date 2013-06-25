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

#include <TimedText/Format.h>
#include <gtest/gtest.h>
using namespace TimedText;

TEST(Sniffer,TTML)
{
  EXPECT_EQ(FormatTTML,detectFormat("<tt xml:lang=\"\" "
                                    "xmlns=\"http://www.w3.org/ns/ttml\">"));
  EXPECT_EQ(FormatTTML,detectFormat("<metadata xmlns:ttm=\""
                                    "http://www.w3.org/ns/ttml#metadata\">"));
  EXPECT_EQ(FormatTTML,detectFormat("<styling xmlns:tts=\""
                                    "http://www.w3.org/ns/ttml#styling\">"));
  EXPECT_EQ(FormatTTML,detectFormat("<profile use=\"dfxp-presentation\" "
                            "xmlns=\"http://www.w3.org/ns/ttml#parameter\">"));
  // Now with bad namespace URIs
  EXPECT_EQ(FormatUnknown,detectFormat("<tt xml:lang=\"\" "
                                     "xmlns=\"http://www.w3.org/ns/ttmll\">"));
  EXPECT_EQ(FormatUnknown,detectFormat("<metadata xmlns:ttm=\""
                                    "http://www.w3.org/ns/ttml#mmetadata\">"));
  EXPECT_EQ(FormatUnknown,detectFormat("<styling xmlns:tts=\""
                                     "http://www.w3.org/ns/ttml#sstyling\">"));
  EXPECT_EQ(FormatUnknown,detectFormat("<profile use=\"dfxp-presentation\" "
                            "xmlns=\"http://www.w3.org/ns/ttml$parameter\">"));
}
