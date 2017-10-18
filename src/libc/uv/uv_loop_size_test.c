// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <uv.h>

TEST(uv_loop_size, example) {
  ASSERT_EQ(sizeof(uv_loop_t), uv_loop_size());
}
