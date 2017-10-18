// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <uv.h>

TEST(uv_get_osfhandle, example) {
  // This function should have no effect in this environment.
  ASSERT_EQ(fd_tmp, uv_get_osfhandle(fd_tmp));
}
