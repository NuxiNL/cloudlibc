// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "gtest/gtest.h"

TEST(reallocarray, overflow) {
  // The multiplication of nelem * elsize would overflow.
  ASSERT_EQ(NULL, reallocarray(NULL, SIZE_MAX / 100 + 2, 100));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(reallocarray, null) {
  // reallocarray() on a null pointer is identical to malloc().
  void *buf = reallocarray(NULL, 20, 30);
  ASSERT_NE(NULL, buf);
  free(buf);
}

TEST(reallocarray, example) {
  // Allocate initial space through malloc().
  char *buf = static_cast<char *>(malloc(64));
  ASSERT_NE(NULL, buf);
  memset(buf, 'A', 64);

  // Grow allocation through reallocarray().
  buf = static_cast<char *>(reallocarray(buf, 16, 8));
  ASSERT_NE(NULL, buf);
  for (size_t i = 0; i < 64; ++i) {
    ASSERT_EQ('A', buf[i]);
  }

  free(buf);
}
