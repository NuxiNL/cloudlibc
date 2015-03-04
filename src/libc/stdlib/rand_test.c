// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(rand, bounds) {
  long value = rand();
  ASSERT_LE(0, value);
  ASSERT_GE(RAND_MAX, value);
}
