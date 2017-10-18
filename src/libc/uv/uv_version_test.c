// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <uv.h>

TEST(uv_version, example) {
  // This implementation should be compatible with libuv 1.15.0.
  ASSERT_EQ(1 << 16 | 15 << 8 | 0, uv_version());
}
