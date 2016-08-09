// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(lrand48, bounds) {
  long value = lrand48();
  ASSERT_LE(0, value);
  ASSERT_GE(2147483647, value);
}
