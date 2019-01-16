// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <threads.h>
#include <iterator>

#include "gtest/gtest.h"

struct block {
  mtx_t mutex;
  cnd_t cond;
  bool triggered;
};

static int thread_block(void *arg) {
  // Block execution until triggered has been set to true.
  auto block = static_cast<struct block *>(arg);
  EXPECT_EQ(thrd_success, mtx_lock(&block->mutex));
  while (!block->triggered)
    EXPECT_EQ(thrd_success, cnd_wait(&block->cond, &block->mutex));
  EXPECT_EQ(thrd_success, mtx_unlock(&block->mutex));
  return 0;
}

TEST(cnd_wait, signal) {
  struct block block;

  ASSERT_EQ(thrd_success, mtx_init(&block.mutex, mtx_plain));
  ASSERT_EQ(thrd_success, cnd_init(&block.cond));

  // Spawn a number of threads that will block on the condition variable.
  thrd_t threads[10];
  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(thrd_success, thrd_create(&threads[i], thread_block, &block));
  struct timespec ts = {.tv_nsec = 100000000};
  ASSERT_EQ(0, thrd_sleep(&ts));

  // Wake up the threads one by one.
  ASSERT_EQ(thrd_success, mtx_lock(&block.mutex));
  block.triggered = true;
  ASSERT_EQ(thrd_success, mtx_unlock(&block.mutex));
  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(thrd_success, cnd_signal(&block.cond));

  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(thrd_success, thrd_join(threads[i], NULL));
  mtx_destroy(&block.mutex);
  cnd_destroy(&block.cond);
}

TEST(cnd_wait, broadcast) {
  struct block block;

  ASSERT_EQ(thrd_success, mtx_init(&block.mutex, mtx_plain));
  ASSERT_EQ(thrd_success, cnd_init(&block.cond));

  // Spawn a number of threads that will block on the condition variable.
  thrd_t threads[10];
  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(thrd_success, thrd_create(&threads[i], thread_block, &block));
  struct timespec ts = {.tv_nsec = 100000000};
  ASSERT_EQ(0, thrd_sleep(&ts));

  // Wake up the threads in one go.
  ASSERT_EQ(thrd_success, mtx_lock(&block.mutex));
  block.triggered = true;
  ASSERT_EQ(thrd_success, cnd_broadcast(&block.cond));
  ASSERT_EQ(thrd_success, mtx_unlock(&block.mutex));

  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(thrd_success, thrd_join(threads[i], NULL));
  mtx_destroy(&block.mutex);
  cnd_destroy(&block.cond);
}
