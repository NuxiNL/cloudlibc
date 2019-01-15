// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <string.h>

#include "gtest/gtest.h"

TEST(strdup, hello) {
  char *copy = strdup("Hello");
  ASSERT_STREQ("Hello", copy);
  free(copy);
}
