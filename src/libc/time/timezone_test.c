// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <time.h>

TEST(timezone, example) {
  // The global timezone in this environment is always UTC.
  ASSERT_EQ(0, timezone);
}
