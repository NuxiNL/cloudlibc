// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdbool.h>
#include <testing.h>
#include <uv.h>

static void entry(void *arg) {
  ASSERT_FALSE(*(bool *)arg);
  *(bool *)arg = true;
}

TEST(uv_thread_create, example) {
  uv_thread_t tid;
  bool invoked = false;
  ASSERT_EQ(0, uv_thread_create(&tid, entry, &invoked));
  ASSERT_EQ(0, uv_thread_join(&tid));
  ASSERT_TRUE(invoked);
}
