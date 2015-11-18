// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <threads.h>

static int called = 0;

static void once_func(void) {
  called = 1;
}

TEST(call_once, example) {
  static once_flag flag = ONCE_FLAG_INIT;

  // Function should be called initially.
  call_once(&flag, once_func);
  ASSERT_EQ(1, called);

  // The second invocation should have no effect.
  call_once(&flag, NULL);
}
