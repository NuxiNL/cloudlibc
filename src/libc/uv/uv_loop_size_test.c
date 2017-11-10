// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <testing.h>
#include <uv.h>

TEST(uv_loop_size, example) {
  ASSERT_EQ(sizeof(uv_loop_t), uv_loop_size());
}
