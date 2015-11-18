// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS
#include <string.h>
#include <testing.h>

TEST(strcpy, example) {
  char buf[10] = "AAAAAAAAAA";
  ASSERT_EQ(buf, strcpy(buf, ""));
  ASSERT_ARREQ("\0AAAAAAAAA", buf, sizeof(buf));
  ASSERT_EQ(buf, strcpy(buf, "Hello"));
  ASSERT_ARREQ("Hello\0AAAA", buf, sizeof(buf));
  ASSERT_EQ(buf, strcpy(buf, ""));
  ASSERT_ARREQ("\0ello\0AAAA", buf, sizeof(buf));
  ASSERT_EQ(buf, strcpy(buf, "Example!!"));
  ASSERT_ARREQ("Example!!\0", buf, sizeof(buf));
}
