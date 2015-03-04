// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(strlcat, null) {
  ASSERT_EQ(5, strlcat(NULL, "Hello", 0));
}

TEST(strlcat, one) {
  char buf = '\0';
  ASSERT_EQ(6, strlcat(&buf, "Banana", 1));
  ASSERT_EQ('\0', buf);

  buf = 'A';
  ASSERT_EQ(7, strlcat(&buf, "Banana", 1));
  ASSERT_EQ('A', buf);
}

TEST(strlcat, longer) {
  char buf[12] = "AAAAAAAAAAAA";
  ASSERT_EQ(15, strlcat(buf, "Foo", sizeof(buf)));
  ASSERT_ARREQ("AAAAAAAAAAAA", buf, __arraycount(buf));

  buf[4] = '\0';
  ASSERT_EQ(7, strlcat(buf, "Bar", sizeof(buf)));
  ASSERT_ARREQ("AAAABar\0AAAA", buf, __arraycount(buf));

  ASSERT_EQ(16, strlcat(buf, "Very long", sizeof(buf)));
  ASSERT_ARREQ("AAAABarVery\0", buf, __arraycount(buf));
}
