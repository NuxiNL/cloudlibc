// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"

static void test_unlocked(void *arg) __no_lock_analysis {
  auto rwlock = static_cast<uv_rwlock_t *>(arg);

  ASSERT_EQ(0, uv_rwlock_tryrdlock(rwlock));
  ASSERT_EQ(0, uv_rwlock_tryrdlock(rwlock));
  uv_rwlock_rdunlock(rwlock);
  uv_rwlock_rdunlock(rwlock);

  uv_rwlock_rdlock(rwlock);
  uv_rwlock_rdlock(rwlock);
  uv_rwlock_rdunlock(rwlock);
  uv_rwlock_rdunlock(rwlock);

  ASSERT_EQ(0, uv_rwlock_trywrlock(rwlock));
  uv_rwlock_wrunlock(rwlock);

  uv_rwlock_wrlock(rwlock);
  uv_rwlock_wrunlock(rwlock);
}

static void test_rdlocked(void *arg) __no_lock_analysis {
  auto rwlock = static_cast<uv_rwlock_t *>(arg);

  ASSERT_EQ(0, uv_rwlock_tryrdlock(rwlock));
  ASSERT_EQ(0, uv_rwlock_tryrdlock(rwlock));
  uv_rwlock_rdunlock(rwlock);
  uv_rwlock_rdunlock(rwlock);

  uv_rwlock_rdlock(rwlock);
  uv_rwlock_rdlock(rwlock);
  uv_rwlock_rdunlock(rwlock);
  uv_rwlock_rdunlock(rwlock);

  ASSERT_EQ(UV_EBUSY, uv_rwlock_trywrlock(rwlock));
}

static void test_wrlocked(void *arg) __no_lock_analysis {
  auto rwlock = static_cast<uv_rwlock_t *>(arg);

  ASSERT_EQ(UV_EBUSY, uv_rwlock_tryrdlock(rwlock));
  ASSERT_EQ(UV_EBUSY, uv_rwlock_trywrlock(rwlock));
}

TEST(uv_rwlock_init, example) {
  uv_rwlock_t rwlock;
  ASSERT_EQ(0, uv_rwlock_init(&rwlock));

  // Test operations on an unlocked read-write lock.
  uv_thread_t tid;
  ASSERT_EQ(0, uv_thread_create(&tid, test_unlocked, &rwlock));
  ASSERT_EQ(0, uv_thread_join(&tid));

  // Test operations on a read-locked read-write lock.
  uv_rwlock_rdlock(&rwlock);
  ASSERT_EQ(0, uv_thread_create(&tid, test_rdlocked, &rwlock));
  ASSERT_EQ(0, uv_thread_join(&tid));
  uv_rwlock_rdunlock(&rwlock);

  // Test operations on a write-locked read-write lock.
  uv_rwlock_wrlock(&rwlock);
  ASSERT_EQ(0, uv_thread_create(&tid, test_wrlocked, &rwlock));
  ASSERT_EQ(0, uv_thread_join(&tid));
  uv_rwlock_wrunlock(&rwlock);

  uv_rwlock_destroy(&rwlock);
}
