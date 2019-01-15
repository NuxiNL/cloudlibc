// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <string.h>

#include "gtest/gtest.h"

TEST(strndup, null) {
  char *copy = strndup(NULL, 0);
  ASSERT_STREQ("", copy);
  free(copy);
}

TEST(strndup, hello) {
  char *copy = strndup("Hello, world", 5);
  ASSERT_STREQ("Hello", copy);
  free(copy);
}
