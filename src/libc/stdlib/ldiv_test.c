// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(ldiv, examples) {
  ldiv_t r = ldiv(5, 2);
  ASSERT_EQ(2, r.quot);
  ASSERT_EQ(1, r.rem);

  r = ldiv(-11, 2);
  ASSERT_EQ(-5, r.quot);
  ASSERT_EQ(-1, r.rem);

  r = ldiv(-43, -3);
  ASSERT_EQ(14, r.quot);
  ASSERT_EQ(-1, r.rem);

  r = ldiv(18, -7);
  ASSERT_EQ(-2, r.quot);
  ASSERT_EQ(4, r.rem);
}
