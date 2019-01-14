// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(posix_memalign, bad) {
  // Invalid alignment.
  for (size_t i = 0; i < sizeof(void *); ++i) {
    ASSERT_EQ(EINVAL, posix_memalign(NULL, i, 1));
  }
}

TEST(posix_memalign, example) {
  for (size_t i = sizeof(void *); i < 4096; i *= 2) {
    void *buf;
    ASSERT_EQ(0, posix_memalign(&buf, i, 1));
    ASSERT_EQ(0, (uintptr_t)buf % i);
    free(buf);
  }
}
