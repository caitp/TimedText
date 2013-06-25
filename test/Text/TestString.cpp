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
