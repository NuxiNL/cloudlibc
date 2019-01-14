// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "gtest/gtest.h"

TEST(calloc, overflow) {
  // The multiplication of nelem * elsize would overflow.
  ASSERT_EQ(NULL, calloc(SIZE_MAX / 100 + 2, 100));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(calloc, example) {
  // Returned space should be zero.
  char *buf = static_cast<char *>(calloc(10, 10));
  ASSERT_NE(NULL, buf);
  for (size_t i = 0; i < 100; ++i) {
    ASSERT_EQ('\0', buf[i]);
  }
  free(buf);
}
