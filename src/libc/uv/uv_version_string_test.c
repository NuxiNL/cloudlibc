// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <uv.h>

TEST(uv_version_string, example) {
#ifdef __CloudABI__
  // This implementation should be compatible with libuv 1.15.0, but
  // should be branded as such.
  ASSERT_STREQ("1.15.0-cloudlibc", uv_version_string());
#else
  ASSERT_STREQ("1.15.0", uv_version_string());
#endif
}
