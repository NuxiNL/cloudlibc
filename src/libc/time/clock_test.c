// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <time.h>

TEST(clock, example) {
  clock_t a = clock();
  clock_t b = clock();
  ASSERT_LE(a, b);
}
