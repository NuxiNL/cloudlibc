// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_strerror, examples) {
  // Known error code constants.
  ASSERT_STREQ("unknown error", uv_strerror(UV_UNKNOWN));
  ASSERT_STREQ("invalid argument", uv_strerror(UV_EINVAL));
  ASSERT_STREQ("temporary failure", uv_strerror(UV_EAI_AGAIN));
  ASSERT_STREQ("cannot send after transport endpoint shutdown",
               uv_strerror(UV_ESHUTDOWN));

  // Unknown error code constants, causing a dynamic allocation.
  ASSERT_STREQ("Unknown system error -1000000", uv_strerror(-1000000));
  ASSERT_STREQ("Unknown system error 0", uv_strerror(0));
  ASSERT_STREQ("Unknown system error 1000000", uv_strerror(1000000));
}
