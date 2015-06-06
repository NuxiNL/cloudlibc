// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(readlinkat, bad) {
  ASSERT_EQ(-1, readlinkat(fd_tmp, "foo", NULL, 0));
  ASSERT_EQ(ENOENT, errno);

  ASSERT_EQ(-1, readlinkat(fd_tmp, ".", NULL, 0));
  ASSERT_EQ(EINVAL, errno);
}

TEST(readlinkat, example) {
  char buf[13];
  ASSERT_EQ(0, symlinkat("Hello, world", fd_tmp, "foo"));

  ASSERT_EQ(11, readlinkat(fd_tmp, "foo", buf, 11));
  ASSERT_ARREQ("Hello, worl", buf, 11);

  ASSERT_EQ(12, readlinkat(fd_tmp, "foo", buf, 12));
  ASSERT_ARREQ("Hello, world", buf, 12);

  ASSERT_EQ(12, readlinkat(fd_tmp, "foo", buf, 13));
  ASSERT_ARREQ("Hello, world", buf, 12);
}
