// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"

static void test_unlocked(void *arg) __no_lock_analysis {
  auto mutex = static_cast<uv_mutex_t *>(arg);

  ASSERT_EQ(0, uv_mutex_trylock(mutex));
  ASSERT_EQ(0, uv_mutex_trylock(mutex));
  uv_mutex_unlock(mutex);
  uv_mutex_unlock(mutex);

  uv_mutex_lock(mutex);
  uv_mutex_lock(mutex);
  uv_mutex_unlock(mutex);
  uv_mutex_unlock(mutex);
}

static void test_locked(void *arg) __no_lock_analysis {
  auto mutex = static_cast<uv_mutex_t *>(arg);

  ASSERT_EQ(UV_EBUSY, uv_mutex_trylock(mutex));
}

TEST(uv_mutex_init_recursive, example) {
  uv_mutex_t mutex;
  ASSERT_EQ(0, uv_mutex_init_recursive(&mutex));

  // Test operations on an unlocked mutex lock.
  uv_thread_t tid;
  ASSERT_EQ(0, uv_thread_create(&tid, test_unlocked, &mutex));
  ASSERT_EQ(0, uv_thread_join(&tid));

  // Test operations on a locked mutex lock.
  uv_mutex_lock(&mutex);
  ASSERT_EQ(0, uv_thread_create(&tid, test_locked, &mutex));
  ASSERT_EQ(0, uv_thread_join(&tid));
  uv_mutex_unlock(&mutex);

  uv_mutex_destroy(&mutex);
}
