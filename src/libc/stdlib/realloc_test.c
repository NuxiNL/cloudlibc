// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <string.h>
#include <testing.h>

TEST(realloc, null) {
  // realloc() on a null pointer is identical to malloc().
  void *buf = realloc(NULL, 100);
  ASSERT_NE(NULL, buf);
  free(buf);
}

TEST(realloc, example) {
  // Allocate initial space through malloc().
  char *buf = malloc(64);
  ASSERT_NE(NULL, buf);
  memset(buf, 'A', 64);

  // Grow allocation through realloc().
  buf = realloc(buf, 128);
  ASSERT_NE(NULL, buf);
  for (size_t i = 0; i < 64; ++i) {
    ASSERT_EQ('A', buf[i]);
  }

  free(buf);
}
