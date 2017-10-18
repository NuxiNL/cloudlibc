// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

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
