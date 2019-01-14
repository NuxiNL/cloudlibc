// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(free, null) {
  // It is valid to call free() on NULL.
  free(NULL);
}

TEST(free, example) {
  void *buf = malloc(32);
  ASSERT_NE(NULL, buf);
  free(buf);
}
