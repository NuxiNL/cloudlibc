// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include <errno.h>
#include <stddef.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(cap_rights_get, bad) {
  // Bad file descriptor.
  ASSERT_EQ(-1, cap_rights_get(-1, NULL));
  ASSERT_EQ(EBADF, errno);
}

TEST(cap_rights_get, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // cap_rights_get() should return the union of the base and inheriting
  // rights of a file descriptor.
  cap_rights_t rights;
  ASSERT_EQ(0, cap_rights_get(fd_tmp, &rights));
  cap_rights_t base, inheriting;
  ASSERT_EQ(0, cap_rights_get_explicit(fd_tmp, &base, &inheriting));
  ASSERT_EQ(base.__value | inheriting.__value, rights.__value);
}
