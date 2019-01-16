// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>

#include "gtest/gtest.h"

static void *success(void *arg) __no_lock_analysis {
  // Thread can properly acquire a read lock.
  auto rwlock = static_cast<pthread_rwlock_t *>(arg);
  EXPECT_EQ(0, pthread_rwlock_rdlock(rwlock));
  EXPECT_EQ(0, pthread_rwlock_rdlock(rwlock));
  struct timespec ts = {};
  EXPECT_EQ(0, pthread_rwlock_timedrdlock(rwlock, &ts));
  EXPECT_EQ(0, pthread_rwlock_unlock(rwlock));
  EXPECT_EQ(0, pthread_rwlock_unlock(rwlock));
  EXPECT_EQ(0, pthread_rwlock_unlock(rwlock));

  // But it cannot acquire a write lock.
  EXPECT_EQ(EBUSY, pthread_rwlock_trywrlock(rwlock));
  EXPECT_EQ(ETIMEDOUT, pthread_rwlock_timedwrlock(rwlock, &ts));
  return NULL;
}

TEST(pthread_rwlock_rdlock, success) {
  // Attempt to acquire the read lock while holding a read lock.
  pthread_rwlock_t rwlock;
  ASSERT_EQ(0, pthread_rwlock_init(&rwlock, NULL));
  ASSERT_EQ(0, pthread_rwlock_rdlock(&rwlock));

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, success, &rwlock));
  ASSERT_EQ(0, pthread_join(thread, NULL));

  ASSERT_EQ(0, pthread_rwlock_unlock(&rwlock));
  ASSERT_EQ(0, pthread_rwlock_destroy(&rwlock));
}

static void *busy(void *arg) {
  // Thread cannot acquire the lock.
  auto rwlock = static_cast<pthread_rwlock_t *>(arg);
  EXPECT_EQ(EBUSY, pthread_rwlock_tryrdlock(rwlock));
  struct timespec ts = {};
  EXPECT_EQ(ETIMEDOUT, pthread_rwlock_timedrdlock(rwlock, &ts));
  EXPECT_EQ(EBUSY, pthread_rwlock_trywrlock(rwlock));
  EXPECT_EQ(ETIMEDOUT, pthread_rwlock_timedwrlock(rwlock, &ts));
  return NULL;
}

TEST(pthread_rwlock_rdlock, busy) {
  // Attempt to acquire the read lock while holding a write lock.
  pthread_rwlock_t rwlock;
  ASSERT_EQ(0, pthread_rwlock_init(&rwlock, NULL));
  ASSERT_EQ(0, pthread_rwlock_wrlock(&rwlock));

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, busy, &rwlock));
  ASSERT_EQ(0, pthread_join(thread, NULL));

  ASSERT_EQ(0, pthread_rwlock_unlock(&rwlock));
  ASSERT_EQ(0, pthread_rwlock_destroy(&rwlock));
}
