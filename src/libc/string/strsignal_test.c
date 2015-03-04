// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <testing.h>

TEST(strsignal, unknown) {
  errno = 1234;
  ASSERT_STREQ("Unknown signal", strsignal(-1));
  ASSERT_EQ(1234, errno);
}

TEST(strsignal, sigabrt) {
  errno = 1234;
  ASSERT_STREQ("Process abort signal", strsignal(SIGABRT));
  ASSERT_EQ(1234, errno);
}
