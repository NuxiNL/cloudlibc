// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#include "gtest/gtest.h"

TEST(pthread_cond_timedwait, timedout) {
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, NULL));
  pthread_cond_t cond;
  ASSERT_EQ(0, pthread_cond_init(&cond, NULL));

  // Attempt to wait on a timestamp that has already passed.
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));
  struct timespec ts = {.tv_sec = 1422982367};
  ASSERT_EQ(ETIMEDOUT, pthread_cond_timedwait(&cond, &mutex, &ts));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));

  ASSERT_EQ(0, pthread_cond_destroy(&cond));
  ASSERT_EQ(0, pthread_mutex_destroy(&mutex));
}

struct block {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  bool okay;
};

static void *do_timedwait(void *arg) {
  // We must receive the signal within one second.
  struct timespec ts;
  EXPECT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ++ts.tv_sec;

  auto block = static_cast<struct block *>(arg);
  EXPECT_EQ(0, pthread_mutex_lock(&block->mutex));
  while (!block->okay)
    EXPECT_EQ(0, pthread_cond_timedwait(&block->cond, &block->mutex, &ts));
  EXPECT_EQ(0, pthread_mutex_unlock(&block->mutex));
  return NULL;
}

static void do_sleep(void) {
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 100000000L};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
}

TEST(pthread_cond_timedwait, signalled) {
  // Let a single thread wait on the condition variable.
  struct block block;
  ASSERT_EQ(0, pthread_mutex_init(&block.mutex, NULL));
  ASSERT_EQ(0, pthread_cond_init(&block.cond, NULL));
  block.okay = false;

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_timedwait, &block));
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
