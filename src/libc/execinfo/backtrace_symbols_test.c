// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <testing.h>

void _start(void);

TEST(backtrace_symbols, zero) {
  // This should return an empty list.
  char **result = backtrace_symbols(NULL, 0);
  ASSERT_NE(NULL, result);
  free(result);
}

TEST(backtrace_symbols, example) {
  void *input[] = {NULL, _start, (char *)_start + 1};
  char **result = backtrace_symbols(input, __arraycount(input));
  char buf[128];
  snprintf(buf, sizeof(buf), "%p", NULL);
  ASSERT_STREQ(buf, result[0]);
  snprintf(buf, sizeof(buf), "%p == _start", _start);
  ASSERT_STREQ(buf, result[1]);
  snprintf(buf, sizeof(buf), "%p == _start + 0x1", (char *)_start + 1);
  ASSERT_STREQ(buf, result[2]);
  free(result);
}
