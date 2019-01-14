// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(malloc, zero) {
  // For size 0, we should either return null pointers or distinct
  // pointers.
  void *b1 = malloc(0);
  void *b2 = malloc(0);
  ASSERT_TRUE((b1 == NULL && b2 == NULL) ||
              (b1 != NULL && b2 != NULL && b1 != b2));
  free(b1);
  free(b2);
}

TEST(malloc, example) {
  void *buf = malloc(64);
  ASSERT_NE(NULL, buf);
  free(buf);
}
