// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_buf_init, example) {
  char hello[] = "Hello";
  uv_buf_t buf = uv_buf_init(hello, sizeof(hello));
  ASSERT_EQ(hello, buf.base);
  ASSERT_EQ(sizeof(hello), buf.len);
}
