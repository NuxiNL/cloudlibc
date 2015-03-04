// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(memccpy, null) {
  ASSERT_EQ(NULL, memccpy((char *)456, (char *)789, 'A', 0));
}

TEST(memccpy, example) {
  const char buf1[13] = "Test\0string!";
  char buf2[9] = "AAAAAAAAA";
  ASSERT_EQ(&buf2[8], memccpy(buf2, buf1, 'r', 9999));
  ASSERT_ARREQ("Test\0strA", buf2, 9);
}
