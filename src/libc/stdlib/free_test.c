// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(free, null) {
  // It is valid to call free() on NULL.
  free(NULL);
}

TEST(free, example) {
  void *buf = malloc(32);
  ASSERT_NE(NULL, buf);
  free(buf);
}
