// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(pthread_cond_signal, zero) {
  pthread_cond_t cond;
  ASSERT_EQ(0, pthread_cond_init(&cond, NULL));
  ASSERT_EQ(0, pthread_cond_signal(&cond));
  ASSERT_EQ(0, pthread_cond_destroy(&cond));
}

struct block {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  bool okay;
};

static void *do_wait(void *arg) {
  auto block = static_cast<struct block *>(arg);
  EXPECT_EQ(0, pthread_mutex_lock(&block->mutex));
  while (!block->okay)
    EXPECT_EQ(0, pthread_cond_wait(&block->cond, &block->mutex));
  EXPECT_EQ(0, pthread_mutex_unlock(&block->mutex));
  return NULL;
}

static void do_sleep(void) {
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 100000000L};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
}

TEST(pthread_cond_signal, one_locked) {
  // Let a single thread wait on the condition variable.
  struct block block;
  ASSERT_EQ(0, pthread_mutex_init(&block.mutex, NULL));
  ASSERT_EQ(0, pthread_cond_init(&block.cond, NULL));
  block.okay = false;

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_wait, &block));
  do_sleep();

  // Call pthread_cond_signal() while holding the lock.
  ASSERT_EQ(0, pthread_mutex_lock(&block.mutex));
  block.okay = true;
  ASSERT_EQ(0, pthread_cond_signal(&block.cond));
  ASSERT_EQ(0, pthread_mutex_unlock(&block.mutex));

  ASSERT_EQ(0, pthread_join(thread, NULL));

  ASSERT_EQ(0, pthread_mutex_destroy(&block.mutex));
  ASSERT_EQ(0, pthread_cond_destroy(&block.cond));
}

TEST(pthread_cond_signal, one_unlocked) {
  // Let a single thread wait on the condition variable.
  struct block block;
  ASSERT_EQ(0, pthread_mutex_init(&block.mutex, NULL));
  ASSERT_EQ(0, pthread_cond_init(&block.cond, NULL));
  block.okay = false;

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_wait, &block));
  do_sleep();

  // Call pthread_cond_signal() without holding the lock.
  ASSERT_EQ(0, pthread_mutex_lock(&block.mutex));
  block.okay = true;
  ASSERT_EQ(0, pthread_mutex_unlock(&block.mutex));
  ASSERT_EQ(0, pthread_cond_signal(&block.cond));

  ASSERT_EQ(0, pthread_join(thread, NULL));

  ASSERT_EQ(0, pthread_mutex_destroy(&block.mutex));
  ASSERT_EQ(0, pthread_cond_destroy(&block.cond));
}

#if 0  // TODO(ed): How to test this without forking?
TEST(pthread_cond_signal, one_shared) {
  // Let another process wait on the condition variable.
  struct block *block = mmap(NULL, sizeof(*block), PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANON, -1, 0);
  ASSERT_NE(MAP_FAILED, block);
  pthread_mutexattr_t mutexattr;
  ASSERT_EQ(0, pthread_mutexattr_init(&mutexattr));
  ASSERT_EQ(0,
            pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED));
  ASSERT_EQ(0, pthread_mutex_init(&block->mutex, &mutexattr));
  ASSERT_EQ(0, pthread_mutexattr_destroy(&mutexattr));
  pthread_condattr_t condattr;
  ASSERT_EQ(0, pthread_condattr_init(&condattr));
  ASSERT_EQ(0, pthread_condattr_setpshared(&condattr, PTHREAD_PROCESS_SHARED));
  ASSERT_EQ(0, pthread_cond_init(&block->cond, &condattr));
  ASSERT_EQ(0, pthread_condattr_destroy(&condattr));
  block->okay = false;

  int fd;
  int ret = pdfork(&fd);
  if (ret == 0) {
    do_wait(block);
    _Exit(0);
  }
  ASSERT_LT(0, ret);
  do_sleep();

  // Call pthread_cond_signal() without holding the lock.
  ASSERT_EQ(0, pthread_mutex_lock(&block->mutex));
  block->okay = true;
  ASSERT_EQ(0, pthread_mutex_unlock(&block->mutex));
  ASSERT_EQ(0, pthread_cond_signal(&block->cond));

  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(0, close(fd));

  ASSERT_EQ(0, pthread_mutex_destroy(&block->mutex));
  ASSERT_EQ(0, pthread_cond_destroy(&block->cond));
  ASSERT_EQ(0, munmap(block, sizeof(*block)));
}
#endif
