// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <time.h>

TEST_SEPARATE_PROCESS(tzset, example) {
  // The tzset() function should reset the contents of tzname to proper
  // values.
  tzname[0] = (char *)"bad";
  tzname[1] = (char *)"incorrect";
  tzset();
  ASSERT_STREQ("UTC", tzname[0]);
  ASSERT_STREQ("UTC", tzname[1]);
}
