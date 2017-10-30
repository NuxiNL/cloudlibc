// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/utsname.h>

#include <testing.h>

TEST(uname, example) {
  struct utsname info;
  ASSERT_EQ(0, uname(&info));
  ASSERT_STREQ("cloudlibc", info.sysname);
#if defined(__x86_64__)
  ASSERT_STREQ("x86_64", info.machine);
#endif
}
