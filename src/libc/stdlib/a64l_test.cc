// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(a64l, examples) {
#define TEST_BASE64(v, str)                         \
  do {                                              \
    ASSERT_EQ(v, a64l(str));                        \
    ASSERT_EQ(v, a64l(str "| Trailing garbage"));   \
                                                    \
    char buf[sizeof(str)];                          \
    ASSERT_EQ(-1, l64a_r(v, buf, sizeof(buf) - 1)); \
    ASSERT_EQ(0, l64a_r(v, buf, sizeof(buf)));      \
    ASSERT_STREQ(str, buf);                         \
  } while (0)
  // Individual characters.
  TEST_BASE64(0, "");
  TEST_BASE64(1, "/");
  TEST_BASE64(2, "0");
  TEST_BASE64(11, "9");
  TEST_BASE64(12, "A");
  TEST_BASE64(37, "Z");
  TEST_BASE64(38, "a");
  TEST_BASE64(63, "z");

  // Random test vectors.
  TEST_BASE64(-798039413, "98iPE1");
  TEST_BASE64(-457520467, "hegiY1");
  TEST_BASE64(-130063910, "O5qDs1");
  TEST_BASE64(1146677630, "yZCK2/");
  TEST_BASE64(1378758273, "/uW9G/");
  TEST_BASE64(1388004411, "vEoiG/");
  TEST_BASE64(1464607185, "F50HL/");
  TEST_BASE64(1717698504, "6zTMa/");
  TEST_BASE64(1783681590, "q6BIe/");
  TEST_BASE64(1812451478, "K0x/g/");
#undef TEST_BASE64
}

TEST(a64l, random) {
  // The low 32 bits should be preserved when converting back and forth.
  for (int i = 0; i < 1000; ++i) {
    long v;
    arc4random_buf(&v, sizeof(v));
    char buf[7];
    ASSERT_EQ(0, l64a_r(v, buf, sizeof(buf)));
    ASSERT_EQ((int32_t)v, a64l(buf));
  }
}
