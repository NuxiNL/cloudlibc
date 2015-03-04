// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <string.h>
#include <testing.h>

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
