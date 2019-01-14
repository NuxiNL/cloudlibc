// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <string.h>

#include "gtest/gtest.h"

TEST(realloc, null) {
  // realloc() on a null pointer is identical to malloc().
  void *buf = realloc(NULL, 100);
  ASSERT_NE(NULL, buf);
  free(buf);
}

TEST(realloc, example) {
  // Allocate initial space through malloc().
  char *buf = static_cast<char *>(malloc(64));
  ASSERT_NE(NULL, buf);
  memset(buf, 'A', 64);

  // Grow allocation through realloc().
  buf = static_cast<char *>(realloc(buf, 128));
  ASSERT_NE(NULL, buf);
  for (size_t i = 0; i < 64; ++i) {
    ASSERT_EQ('A', buf[i]);
  }

  free(buf);
}
