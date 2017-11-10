// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <testing.h>
#include <uv.h>

TEST(uv_translate_sys_error, example) {
  // Error code that needs to be translated.
  ASSERT_EQ(UV_EINVAL, uv_translate_sys_error(EINVAL));

  // Error code that has already been translated should be passed
  // through without being changed.
  ASSERT_EQ(UV_EINVAL, uv_translate_sys_error(UV_EINVAL));
}
