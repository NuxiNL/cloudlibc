// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(atol, examples) {
  ASSERT_EQ(123456, atol("  123456"));
  ASSERT_EQ(-8860, atol("-08860"));
}
