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

#include <TimedText/List.h>
#include <gtest/gtest.h>
using namespace TimedText;

static int testPrimitiveList_run = 1;

template<typename T>
void testPrimitiveList()
{
  const std::string name = "in testPrimitiveList<" + 
                           std::string(TypeInfo<T>::name()) + ">";
  List<T> list;
  int run = testPrimitiveList_run;

  // PUSH
  EXPECT_EQ(0, list.size()) << name << " #" << run;
  for(int i=10; i > 0; --i)
    EXPECT_TRUE(list.push(T(i))) << name << " #" << run;
  EXPECT_EQ(10, list.size()) << name << " #" << run;
  for(int i=0; i < 10; ++i) {
    T result;
    EXPECT_TRUE(list.itemAt(i, result)) << name << " #" << run;
    EXPECT_EQ(T(10-i), result) << name << " #" << run;
  }

  // CLEAR
  list.clear();
  EXPECT_TRUE(list.isEmpty()) << name << " #" << run;

  // UNSHIFT
  EXPECT_EQ(0, list.size()) << name << " #" << run;
  for(int i=10; i > 0; --i)
    EXPECT_TRUE(list.unshift(T(i))) << name << " #" << run;
  EXPECT_EQ(10, list.size()) << name << " #" << run;
  for(int i=0; i < 10; ++i) {
    T result;
    EXPECT_TRUE(list.itemAt(i, result)) << name << " #" << run;
    EXPECT_EQ(T(i+1), result) << name << " #" << run;
  }

  // POP
  for(int i=0; i < 5; ++i) {
    T result;
    EXPECT_TRUE(list.pop(result)) << name << " #" << run;
    EXPECT_EQ(T(10-i), result) << name << " #" << run;
  }
  EXPECT_EQ(5, list.size()) << name << " #" << run;

  // SHIFT
  for(int i=0; i < 5; ++i) {
    T result;
    EXPECT_TRUE(list.shift(result)) << name << " #" << run;
    EXPECT_EQ(T(i+1), result);
  }
  EXPECT_TRUE(list.isEmpty()) << name << " #" << run;

  ++testPrimitiveList_run;
}

TEST(List,Primitive)
{
  testPrimitiveList<int>();
  testPrimitiveList<uint>();
  testPrimitiveList<char>();
  testPrimitiveList<uchar>();
  testPrimitiveList<short>();
  testPrimitiveList<ushort>();
  testPrimitiveList<long>();
  testPrimitiveList<ulong>();
  testPrimitiveList<int64>();
  testPrimitiveList<uint64>();
}
