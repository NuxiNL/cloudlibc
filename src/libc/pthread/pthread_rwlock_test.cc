// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"

// Keeps a rwlock contended for a brief amount of time.
static void *do_block(void *arg) {
  auto rwlock = static_cast<pthread_rwlock_t *>(arg);
  pthread_rwlock_wrlock(rwlock);
  struct timespec ts = {.tv_sec = 1};
  EXPECT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
  pthread_rwlock_unlock(rwlock);
  return NULL;
}

TEST(pthread_rwlock, private) __no_lock_analysis {
  pthread_rwlock_t rwlock;
  ASSERT_EQ(0, pthread_rwlock_init(&rwlock, NULL));

  // Simple uncontended cases.
  ASSERT_EQ(0, pthread_rwlock_wrlock(&rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(&rwlock));
  ASSERT_EQ(0, pthread_rwlock_trywrlock(&rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(&rwlock));

  // Spawn a new thread that will acquire the lock.
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_block, &rwlock));
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 500000000L};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));

  // The rwlock cannot be acquired now.
  ASSERT_EQ(EBUSY, pthread_rwlock_trywrlock(&rwlock));
  ASSERT_EQ(0, pthread_rwlock_wrlock(&rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(&rwlock));

  ASSERT_EQ(0, pthread_rwlock_destroy(&rwlock));
  ASSERT_EQ(0, pthread_join(thread, NULL));
}

#if 0  // TODO(ed): How to test this without forking?
TEST(pthread_rwlock, shared) __no_lock_analysis {
  // Allocate a piece of shared memory to store the rwlock.
  pthread_rwlock_t *rwlock = mmap(NULL, sizeof(*rwlock), PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANON, -1, 0);
  ASSERT_NE(MAP_FAILED, rwlock);
  pthread_rwlockattr_t attr;
  ASSERT_EQ(0, pthread_rwlockattr_init(&attr));
  ASSERT_EQ(0, pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));
  ASSERT_EQ(0, pthread_rwlock_init(rwlock, &attr));
  ASSERT_EQ(0, pthread_rwlockattr_destroy(&attr));

  // Simple uncontended cases.
  ASSERT_EQ(0, pthread_rwlock_wrlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_trywrlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(rwlock));

  // Spawn a new process that will acquire the lock.
  int fd;
  int ret = pdfork(&fd);
  if (ret == 0) {
    do_block(rwlock);
    _Exit(0);
  }
  ASSERT_LT(0, ret);
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 500000000L};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));

  // The rwlock cannot be acquired now.
  ASSERT_EQ(EBUSY, pthread_rwlock_trywrlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_wrlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(rwlock));

  ASSERT_EQ(0, pthread_rwlock_destroy(rwlock));
  ASSERT_EQ(0, munmap(rwlock, sizeof(*rwlock)));
  ASSERT_EQ(0, close(fd));
}
#endif
