// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(arc4random_uniform, bounds) {
  // This shouldn't crash, but behave identically to arc4random().
  arc4random_uniform(0);

  // At least check whether the result is within bounds.
  ASSERT_GT(1, arc4random_uniform(1));
  ASSERT_GT(2, arc4random_uniform(2));
  ASSERT_GT(10, arc4random_uniform(10));
}
