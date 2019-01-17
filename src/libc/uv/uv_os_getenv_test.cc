// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_os_getenv, example) {
  // As environment variables aren't used by CloudABI, this function
  // should always indicate absence.
  char buf[10];
  size_t size = sizeof(buf);
  ASSERT_EQ(UV_ENOENT, uv_os_getenv("SOME_VARIABLE", buf, &size));
}
