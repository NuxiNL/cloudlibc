// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "gtest/gtest.h"

TEST(memmem, null) {
  // memmem() should not attempt to access any buffers if the needle has
  // length 0 or is larger than the haystack.
  ASSERT_EQ(NULL, memmem(NULL, 0, NULL, 123));
  ASSERT_EQ(NULL, memmem(NULL, 123, NULL, 0));
  ASSERT_EQ(NULL, memmem(NULL, 123, NULL, 456));
}

TEST(memmem, examples) {
  const char *str = "Hello world";
  ASSERT_EQ(str + 4, memmem(str, 11, "o worl", 6));
  ASSERT_EQ(NULL, memmem(str, 11, "o worl", 7));
  ASSERT_EQ(str + 6, memmem(str, 11, "world", 5));
  ASSERT_EQ(NULL, memmem(str, 11, "world", 6));
  ASSERT_EQ(str + 6, memmem(str, 12, "world", 6));
  ASSERT_EQ(NULL, memmem(str, 11, "word", 4));
  ASSERT_EQ(NULL, memmem(str, 11, "world!", 6));
}

// Fills a buffer with random letters between A and D.
static void fill_random(char *buf, size_t len) {
  arc4random_buf(buf, len);
  for (size_t i = 0; i < len; ++i)
    buf[i] = (buf[i] & 0x3) + 'A';
}

// Performs a naïve memmem() operation.
static char *naive_memmem(const char *haystack, size_t haystacklen,
                          const char *needle, size_t needlelen) {
  if (needlelen > haystacklen)
    return NULL;
  for (size_t i = 0; i + needlelen <= haystacklen; ++i) {
    bool match = true;
    for (size_t j = 0; j < needlelen; ++j) {
      if (haystack[i + j] != needle[j]) {
        match = false;
        break;
      }
    }
    if (match)
      return (char *)haystack + i;
  }
  return NULL;
}

// Compares the output of memmem() against a naïve implementation.
TEST(memmem, random) {
  for (size_t i = 0; i < 1000; ++i) {
    char haystack[40000];
    char needle[8];
    size_t needlelen = arc4random_uniform(sizeof(needle));
    SCOPED_TRACE(needlelen);
    fill_random(haystack, sizeof(haystack));
    fill_random(needle, needlelen);
    ASSERT_EQ(naive_memmem(haystack, sizeof(haystack), needle, needlelen),
              memmem(haystack, sizeof(haystack), needle, needlelen));
  }
}
