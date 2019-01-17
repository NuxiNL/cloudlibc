// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(close, bad) {
  ASSERT_EQ(-1, close(-123));
  ASSERT_EQ(EBADF, errno);
}

TEST(close, example) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_LE(0, close(fds[0]));
  ASSERT_LE(0, close(fds[1]));
}
