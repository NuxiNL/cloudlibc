// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <pthread.h>
#include <testing.h>

static void *success(void *arg) __no_lock_analysis {
  // Thread can properly acquire a read lock.
  ASSERT_EQ(0, pthread_rwlock_rdlock(arg));
  ASSERT_EQ(0, pthread_rwlock_rdlock(arg));
  ASSERT_EQ(0, pthread_rwlock_timedrdlock(arg, &(struct timespec){}));
  ASSERT_EQ(0, pthread_rwlock_unlock(arg));
  ASSERT_EQ(0, pthread_rwlock_unlock(arg));
  ASSERT_EQ(0, pthread_rwlock_unlock(arg));

  // But it cannot acquire a write lock.
  ASSERT_EQ(EBUSY, pthread_rwlock_trywrlock(arg));
  ASSERT_EQ(ETIMEDOUT, pthread_rwlock_timedwrlock(arg, &(struct timespec){}));
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
  ASSERT_EQ(EBUSY, pthread_rwlock_tryrdlock(arg));
  ASSERT_EQ(ETIMEDOUT, pthread_rwlock_timedrdlock(arg, &(struct timespec){}));
  ASSERT_EQ(EBUSY, pthread_rwlock_trywrlock(arg));
  ASSERT_EQ(ETIMEDOUT, pthread_rwlock_timedwrlock(arg, &(struct timespec){}));
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
