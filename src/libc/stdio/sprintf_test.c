// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <stdio.h>
#include <testing.h>

TEST(sprintf, example) {
  char buf[10];
  ASSERT_EQ(5, sprintf(buf, "Hello"));
  ASSERT_EQ(9, sprintf(buf, "%d + %d = %d", 1, 1, 2));
}
