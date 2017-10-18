// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <uv.h>

TEST(uv_os_getenv, example) {
  // As environment variables aren't used by CloudABI, this function
  // should always indicate absence.
  char buf[10];
  size_t size = sizeof(buf);
  ASSERT_EQ(UV_ENOENT, uv_os_getenv("SOME_VARIABLE", buf, &size));
}
