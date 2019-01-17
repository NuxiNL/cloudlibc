// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_thread_equal, positive) {
  uv_thread_t self1 = uv_thread_self();
  uv_thread_t self2 = uv_thread_self();
  ASSERT_TRUE(uv_thread_equal(&self1, &self2));
}

static void do_nothing(void *arg) {
}

TEST(uv_thread_equal, negative) {
  uv_thread_t self = uv_thread_self();
  uv_thread_t other;
  ASSERT_EQ(0, uv_thread_create(&other, do_nothing, NULL));
  ASSERT_FALSE(uv_thread_equal(&self, &other));
  ASSERT_EQ(0, uv_thread_join(&other));
}
