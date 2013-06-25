#include <TimedText/Format.h>
#include <gtest/gtest.h>
using namespace TimedText;

TEST(Sniffer,WebVTT)
{
  // Good version
  EXPECT_EQ(FormatWebVTT,detectFormat("WEBVTT"));
  static char webvttWithBOM[] = { 0xEF,0xBB,0xBF,'W','E','B','V','T','T' };
  EXPECT_EQ(FormatWebVTT,detectFormat(webvttWithBOM));

  // Test lower-case version (should not sniff FormatWebVTT)
  EXPECT_EQ(FormatUnknown,detectFormat("webvtt"));
  static char mebvttWithBOM[] = { 0xEF,0xBB,0xBF,'w','e','b','v','t','t' };
  EXPECT_EQ(FormatUnknown,detectFormat(mebvttWithBOM));
}