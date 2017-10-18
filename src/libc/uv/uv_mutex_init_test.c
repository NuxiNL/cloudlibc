// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <uv.h>

static void test_unlocked(void *arg) __no_lock_analysis {
  uv_mutex_t *mutex = arg;

  ASSERT_EQ(0, uv_mutex_trylock(mutex));
  uv_mutex_unlock(mutex);

  uv_mutex_lock(mutex);
  uv_mutex_unlock(mutex);
}

static void test_locked(void *arg) __no_lock_analysis {
  uv_mutex_t *mutex = arg;

  ASSERT_EQ(UV_EBUSY, uv_mutex_trylock(mutex));
}

TEST(uv_mutex_init, example) {
  uv_mutex_t mutex;
  ASSERT_EQ(0, uv_mutex_init(&mutex));

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
