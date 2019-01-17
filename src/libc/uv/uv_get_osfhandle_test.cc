// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(uv_get_osfhandle, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // This function should have no effect in this environment.
  ASSERT_EQ(fd_tmp, uv_get_osfhandle(fd_tmp));
}
