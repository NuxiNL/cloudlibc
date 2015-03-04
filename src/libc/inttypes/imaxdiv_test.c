// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <inttypes.h>
#include <testing.h>

TEST(imaxdiv, examples) {
  imaxdiv_t r = imaxdiv(-44, 7);
  ASSERT_EQ(-6, r.quot);
  ASSERT_EQ(-2, r.rem);

  r = imaxdiv(INTMAX_MIN, INTMAX_MIN);
  ASSERT_EQ(1, r.quot);
  ASSERT_EQ(0, r.rem);
}
