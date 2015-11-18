// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(drand48, bounds) {
  double value = drand48();
  ASSERT_LE(0.0, value);
  ASSERT_GT(1.0, value);
}
