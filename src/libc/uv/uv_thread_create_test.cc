// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <uv.h>

#include "gtest/gtest.h"

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
