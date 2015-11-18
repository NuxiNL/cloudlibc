// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(random, bounds) {
  long value = random();
  ASSERT_LE(0, value);
  ASSERT_GT(2147483648, value);
}
