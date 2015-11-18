// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <inttypes.h>
#include <testing.h>

TEST(imaxabs, examples) {
  ASSERT_EQ(5, imaxabs(5));
  ASSERT_EQ(5, imaxabs(-5));

  ASSERT_EQ(INTMAX_MAX, imaxabs(INTMAX_MAX));
  ASSERT_EQ(INTMAX_MAX, imaxabs(-INTMAX_MAX));
}
