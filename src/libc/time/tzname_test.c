// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <time.h>

TEST(tzname, example) {
  // The global timezone in this environment is always UTC.
  ASSERT_STREQ("UTC", tzname[0]);
  ASSERT_STREQ("UTC", tzname[1]);
}
