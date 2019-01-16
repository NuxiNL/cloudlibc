// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(backtrace_symbols, zero) {
  // This should return an empty list.
  char **result = backtrace_symbols(NULL, 0);
  ASSERT_NE(NULL, result);
  free(result);
}

TEST(backtrace_symbols, example) {
  void *input[] = {NULL, reinterpret_cast<void *>(malloc),
                   reinterpret_cast<char *>(malloc) + 1};
  char **result = backtrace_symbols(input, std::size(input));
  char buf[128];
  snprintf(buf, sizeof(buf), "%p", NULL);
  ASSERT_STREQ(buf, result[0]);
  snprintf(buf, sizeof(buf), "%p == malloc", malloc);
  ASSERT_STREQ(buf, result[1]);
  snprintf(buf, sizeof(buf), "%p == malloc + 0x1", (char *)malloc + 1);
  ASSERT_STREQ(buf, result[2]);
  free(result);
}
