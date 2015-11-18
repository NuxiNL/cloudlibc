// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <string.h>
#include <testing.h>

TEST(strdup, hello) {
  char *copy = strdup("Hello");
  ASSERT_STREQ("Hello", copy);
  free(copy);
}
