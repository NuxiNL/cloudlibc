// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(mrand48, bounds) {
  long value = mrand48();
  ASSERT_LE(-2147483648, value);
  ASSERT_GT(2147483648, value);
}
