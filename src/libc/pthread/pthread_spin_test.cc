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

// Keeps a spinlock contended for a brief amount of time.
static void *do_block(void *arg) {
  auto spin = static_cast<pthread_spinlock_t *>(arg);
  pthread_spin_lock(spin);
  struct timespec ts = {.tv_sec = 1};
  EXPECT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
  pthread_spin_unlock(spin);
  return NULL;
}

TEST(pthread_spin, private) __no_lock_analysis {
  pthread_spinlock_t spin;
  ASSERT_EQ(0, pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE));

  // Simple uncontended cases.
  ASSERT_EQ(0, pthread_spin_lock(&spin));
  ASSERT_EQ(0, pthread_spin_unlock(&spin));
  ASSERT_EQ(0, pthread_spin_trylock(&spin));
  ASSERT_EQ(0, pthread_spin_unlock(&spin));

  // Spawn a new thread that will acquire the lock.
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_block, &spin));
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 500000000L};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));

  // The spinlock cannot be acquired now.
  ASSERT_EQ(EBUSY, pthread_spin_trylock(&spin));
  ASSERT_EQ(0, pthread_spin_lock(&spin));
  ASSERT_EQ(0, pthread_spin_unlock(&spin));

  ASSERT_EQ(0, pthread_spin_destroy(&spin));
  ASSERT_EQ(0, pthread_join(thread, NULL));
}

#if 0  // TODO(ed): How to test this without forking?
TEST(pthread_spin, shared) __no_lock_analysis {
  // Allocate a piece of shared memory to store the spinlock.
  pthread_spinlock_t *spin = mmap(NULL, sizeof(*spin), PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANON, -1, 0);
  ASSERT_NE(MAP_FAILED, spin);
  ASSERT_EQ(0, pthread_spin_init(spin, PTHREAD_PROCESS_SHARED));

  // Simple uncontended cases.
  ASSERT_EQ(0, pthread_spin_lock(spin));
  ASSERT_EQ(0, pthread_spin_unlock(spin));
  ASSERT_EQ(0, pthread_spin_trylock(spin));
  ASSERT_EQ(0, pthread_spin_unlock(spin));

  // Spawn a new process that will acquire the lock.
  int fd;
  int ret = pdfork(&fd);
  if (ret == 0) {
    do_block(spin);
    _Exit(0);
  }
  ASSERT_LT(0, ret);
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 500000000L};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));

  // The spinlock cannot be acquired now.
  ASSERT_EQ(EBUSY, pthread_spin_trylock(spin));
  ASSERT_EQ(0, pthread_spin_lock(spin));
  ASSERT_EQ(0, pthread_spin_unlock(spin));

  ASSERT_EQ(0, pthread_spin_destroy(spin));
  ASSERT_EQ(0, munmap(spin, sizeof(*spin)));
  ASSERT_EQ(0, close(fd));
}
#endif
