// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <regex.h>
#include <stddef.h>
#include <testing.h>

TEST(regerror, example) {
  ASSERT_EQ(8, regerror(0, NULL, NULL, 0));

  char buf[9] = "AAAAAAAAA";
  ASSERT_EQ(8, regerror(0, NULL, buf, 0));
  ASSERT_ARREQ("AAAAAAAAA", buf, 9);

  ASSERT_EQ(8, regerror(0, NULL, buf, 1));
  ASSERT_ARREQ("\0AAAAAAAA", buf, 9);

  ASSERT_EQ(8, regerror(0, NULL, buf, 5));
  ASSERT_ARREQ("Succ\0AAAA", buf, 9);

  ASSERT_EQ(8, regerror(0, NULL, buf, 9));
  ASSERT_ARREQ("Success\0A", buf, 9);
}
