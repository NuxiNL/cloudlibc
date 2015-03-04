// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(lrand48, bounds) {
  long value = lrand48();
  ASSERT_LE(0, value);
  ASSERT_GT(2147483648, value);
}
