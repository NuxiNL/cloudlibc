// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <testing.h>
#include <uv.h>

TEST(uv_get_osfhandle, example) {
  // This function should have no effect in this environment.
  ASSERT_EQ(fd_tmp, uv_get_osfhandle(fd_tmp));
}
