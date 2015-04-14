// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#include <testing.h>

TEST(cap_getmode, example) {
  unsigned int mode;
  ASSERT_EQ(0, cap_getmode(&mode));
  ASSERT_TRUE(mode);
}
