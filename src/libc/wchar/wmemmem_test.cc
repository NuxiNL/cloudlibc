// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(wmemmem, null) {
  // wmemmem() should not attempt to access any buffers if the needle
  // has length 0 or is larger than the haystack.
  ASSERT_EQ(NULL, wmemmem(NULL, 0, NULL, 123));
  ASSERT_EQ(NULL, wmemmem(NULL, 123, NULL, 0));
  ASSERT_EQ(NULL, wmemmem(NULL, 123, NULL, 456));
}

TEST(wmemmem, examples) {
  const wchar_t *str = L"Hello world";
  ASSERT_EQ(str + 4, wmemmem(str, 11, L"o worl", 6));
  ASSERT_EQ(NULL, wmemmem(str, 11, L"o worl", 7));
  ASSERT_EQ(str + 6, wmemmem(str, 11, L"world", 5));
  ASSERT_EQ(NULL, wmemmem(str, 11, L"world", 6));
  ASSERT_EQ(str + 6, wmemmem(str, 12, L"world", 6));
  ASSERT_EQ(NULL, wmemmem(str, 11, L"word", 4));
  ASSERT_EQ(NULL, wmemmem(str, 11, L"world!", 6));
}

// Fills a buffer with random letters between A and D.
static void fill_random(wchar_t *buf, size_t len) {
  arc4random_buf(buf, len * sizeof(wchar_t));
  for (size_t i = 0; i < len; ++i)
    buf[i] = (buf[i] & 0x3) + L'A';
}

// Performs a naïve wmemmem() operation.
static wchar_t *naive_memmem(const wchar_t *haystack, size_t haystacklen,
                             const wchar_t *needle, size_t needlelen) {
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
      return (wchar_t *)haystack + i;
  }
  return NULL;
}

// Compares the output of wmemmem() against a naïve implementation.
TEST(wmemmem, random) {
  for (size_t i = 0; i < 1000; ++i) {
    wchar_t haystack[40000];
    wchar_t needle[8];
    size_t needlelen = arc4random_uniform(std::size(needle));
    SCOPED_TRACE(needlelen);
    fill_random(haystack, std::size(haystack));
    fill_random(needle, needlelen);
    ASSERT_EQ(naive_memmem(haystack, std::size(haystack), needle, needlelen),
              wmemmem(haystack, std::size(haystack), needle, needlelen));
  }
}
