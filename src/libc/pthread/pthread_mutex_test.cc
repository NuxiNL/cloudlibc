// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"

// Keeps a mutex contended for a brief amount of time.
static void *do_block(void *arg) {
  auto mutex = static_cast<pthread_mutex_t *>(arg);
  pthread_mutex_lock(mutex);
  struct timespec ts = {.tv_sec = 1};
  EXPECT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
  pthread_mutex_unlock(mutex);
  return NULL;
}

TEST(pthread_mutex, private) __no_lock_analysis {
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, NULL));

  // Simple uncontended cases.
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));
  ASSERT_EQ(0, pthread_mutex_trylock(&mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));

  // Spawn a new thread that will acquire the lock.
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_block, &mutex));
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 500000000L};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));

  // The mutex cannot be acquired now.
  ASSERT_EQ(EBUSY, pthread_mutex_trylock(&mutex));
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));

  ASSERT_EQ(0, pthread_mutex_destroy(&mutex));
  ASSERT_EQ(0, pthread_join(thread, NULL));
}

#if 0  // TODO(ed): How to test this without forking?
TEST(pthread_mutex, shared) __no_lock_analysis {
  // Create a temporary file to use as shared memory to store the mutex.
  int fd = openat(fd_tmp, "tmp", O_CREAT | O_RDWR);
  ASSERT_EQ(0, ftruncate(fd, sizeof(pthread_mutex_t)));
  pthread_mutex_t *mutex =
      mmap(NULL, sizeof(*mutex), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  ASSERT_EQ(0, close(fd));
  ASSERT_NE(MAP_FAILED, mutex);

  // Create shared mutex.
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));
  ASSERT_EQ(0, pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));
  ASSERT_EQ(0, pthread_mutex_init(mutex, &attr));
  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));

  // Simple uncontended cases.
  ASSERT_EQ(0, pthread_mutex_lock(mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(mutex));
  ASSERT_EQ(0, pthread_mutex_trylock(mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(mutex));

  // Spawn a new process that will acquire the lock.
  int ret = pdfork(&fd);
  if (ret == 0) {
    do_block(mutex);
    _Exit(0);
  }
  ASSERT_LT(0, ret);
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 500000000L};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));

  // The mutex cannot be acquired now.
  ASSERT_EQ(EBUSY, pthread_mutex_trylock(mutex));
  ASSERT_EQ(0, pthread_mutex_lock(mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(mutex));

  ASSERT_EQ(0, pthread_mutex_destroy(mutex));
  ASSERT_EQ(0, munmap(mutex, sizeof(*mutex)));
  ASSERT_EQ(0, close(fd));
}
#endif
