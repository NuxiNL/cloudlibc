// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <string.h>
#include <testing.h>

TEST(strncat, example) {
  char buf[10] = "\0AAAAAAAAA";
  ASSERT_EQ(buf, strncat(buf, "", 0));
  ASSERT_ARREQ("\0AAAAAAAAA", buf, sizeof(buf));
  ASSERT_EQ(buf, strncat(buf, "Hello", 99999));
  ASSERT_ARREQ("Hello\0AAAA", buf, sizeof(buf));
  ASSERT_EQ(buf, strncat(buf, "", 1));
  ASSERT_ARREQ("Hello\0AAAA", buf, sizeof(buf));
  ASSERT_EQ(buf, strncat(buf, "!!!!!!!!!!!!", 3));
  ASSERT_ARREQ("Hello!!!\0A", buf, sizeof(buf));
}
