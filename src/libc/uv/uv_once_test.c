// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <testing.h>
#include <uv.h>

static uv_once_t once = UV_ONCE_INIT;
static bool called = false;

static void initializer(void) {
  ASSERT_FALSE(called);
  called = true;
}

TEST(uv_once, example) {
  uv_once(&once, initializer);
  ASSERT_TRUE(called);

  uv_once(&once, initializer);
  ASSERT_TRUE(called);
}
