// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_err_name, examples) {
  // Known error code constants.
  ASSERT_STREQ("UNKNOWN", uv_err_name(UV_UNKNOWN));
  ASSERT_STREQ("EINVAL", uv_err_name(UV_EINVAL));
  ASSERT_STREQ("EAI_AGAIN", uv_err_name(UV_EAI_AGAIN));
  ASSERT_STREQ("ESHUTDOWN", uv_err_name(UV_ESHUTDOWN));

  // Unknown error code constants, causing a dynamic allocation.
  ASSERT_STREQ("Unknown system error -1000000", uv_err_name(-1000000));
  ASSERT_STREQ("Unknown system error 0", uv_err_name(0));
  ASSERT_STREQ("Unknown system error 1000000", uv_err_name(1000000));
}
