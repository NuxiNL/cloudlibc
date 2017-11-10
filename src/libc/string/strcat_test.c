// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <string.h>
#include <testing.h>

TEST(strcat, example) {
  char buf[10] = "\0AAAAAAAAA";
  ASSERT_EQ(buf, strcat(buf, ""));
  ASSERT_ARREQ("\0AAAAAAAAA", buf, sizeof(buf));
  ASSERT_EQ(buf, strcat(buf, "Hello"));
  ASSERT_ARREQ("Hello\0AAAA", buf, sizeof(buf));
  ASSERT_EQ(buf, strcat(buf, ""));
  ASSERT_ARREQ("Hello\0AAAA", buf, sizeof(buf));
  ASSERT_EQ(buf, strcat(buf, "!!!!"));
  ASSERT_ARREQ("Hello!!!!\0", buf, sizeof(buf));
}
