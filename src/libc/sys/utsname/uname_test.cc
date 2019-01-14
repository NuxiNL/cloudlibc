// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/utsname.h>

#include "gtest/gtest.h"

TEST(uname, example) {
  struct utsname info;
  ASSERT_EQ(0, uname(&info));
  ASSERT_STREQ("cloudlibc", info.sysname);
#if defined(__x86_64__)
  ASSERT_STREQ("x86_64", info.machine);
#endif
}
