// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <uv.h>

TEST(uv_buf_init, example) {
  char hello[] = "Hello";
  uv_buf_t buf = uv_buf_init(hello, sizeof(hello));
  ASSERT_EQ(hello, buf.base);
  ASSERT_EQ(sizeof(hello), buf.len);
}
