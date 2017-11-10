// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <testing.h>
#include <time.h>

TEST(tzname, example) {
  // The global timezone in this environment is always UTC.
  ASSERT_STREQ("UTC", tzname[0]);
  ASSERT_STREQ("UTC", tzname[1]);
}
