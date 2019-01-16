// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdbool.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(pthread_cond_broadcast, zero) {
  pthread_cond_t cond;
  ASSERT_EQ(0, pthread_cond_init(&cond, NULL));
  ASSERT_EQ(0, pthread_cond_broadcast(&cond));
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

TEST(pthread_cond_broadcast, multiple_locked) {
  // Let a number of threads wait on the condition variable.
  struct block block;
  ASSERT_EQ(0, pthread_mutex_init(&block.mutex, NULL));
  ASSERT_EQ(0, pthread_cond_init(&block.cond, NULL));
  block.okay = false;

  pthread_t threads[10];
  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(0, pthread_create(&threads[i], NULL, do_wait, &block));
  do_sleep();

  // Call pthread_cond_broadcast() while holding the lock.
  ASSERT_EQ(0, pthread_mutex_lock(&block.mutex));
  block.okay = true;
  ASSERT_EQ(0, pthread_cond_broadcast(&block.cond));
  ASSERT_EQ(0, pthread_mutex_unlock(&block.mutex));

  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(0, pthread_join(threads[i], NULL));

  ASSERT_EQ(0, pthread_mutex_destroy(&block.mutex));
  ASSERT_EQ(0, pthread_cond_destroy(&block.cond));
}

TEST(pthread_cond_broadcast, multiple_unlocked) {
  // Let a number of threads wait on the condition variable.
  struct block block;
  ASSERT_EQ(0, pthread_mutex_init(&block.mutex, NULL));
  ASSERT_EQ(0, pthread_cond_init(&block.cond, NULL));
  block.okay = false;

  pthread_t threads[10];
  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(0, pthread_create(&threads[i], NULL, do_wait, &block));
  do_sleep();

  // Call pthread_cond_broadcast() without holding the lock.
  ASSERT_EQ(0, pthread_mutex_lock(&block.mutex));
  block.okay = true;
  ASSERT_EQ(0, pthread_mutex_unlock(&block.mutex));
  ASSERT_EQ(0, pthread_cond_broadcast(&block.cond));

  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(0, pthread_join(threads[i], NULL));

  ASSERT_EQ(0, pthread_mutex_destroy(&block.mutex));
  ASSERT_EQ(0, pthread_cond_destroy(&block.cond));
}
