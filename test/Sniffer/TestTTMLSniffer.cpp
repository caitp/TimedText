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
