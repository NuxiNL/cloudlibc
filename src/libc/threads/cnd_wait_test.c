// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdbool.h>
#include <testing.h>
#include <threads.h>

struct block {
  mtx_t mutex;
  cnd_t cond;
  bool triggered;
};

static int thread_block(void *arg) {
  // Block execution until triggered has been set to true.
  struct block *block = arg;
  ASSERT_EQ(thrd_success, mtx_lock(&block->mutex));
  while (!block->triggered)
    ASSERT_EQ(thrd_success, cnd_wait(&block->cond, &block->mutex));
  ASSERT_EQ(thrd_success, mtx_unlock(&block->mutex));
  return 0;
}

TEST(cnd_wait, signal) {
  struct block block;

  ASSERT_EQ(thrd_success, mtx_init(&block.mutex, mtx_plain));
  ASSERT_EQ(thrd_success, cnd_init(&block.cond));

  // Spawn a number of threads that will block on the condition variable.
  thrd_t threads[10];
  for (size_t i = 0; i < __arraycount(threads); ++i)
    ASSERT_EQ(thrd_success, thrd_create(&threads[i], thread_block, &block));
  ASSERT_EQ(0, thrd_sleep(&(struct timespec){.tv_nsec = 100000000}));

  // Wake up the threads one by one.
  ASSERT_EQ(thrd_success, mtx_lock(&block.mutex));
  block.triggered = true;
  ASSERT_EQ(thrd_success, mtx_unlock(&block.mutex));
  for (size_t i = 0; i < __arraycount(threads); ++i)
    ASSERT_EQ(thrd_success, cnd_signal(&block.cond));

  for (size_t i = 0; i < __arraycount(threads); ++i)
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
  for (size_t i = 0; i < __arraycount(threads); ++i)
    ASSERT_EQ(thrd_success, thrd_create(&threads[i], thread_block, &block));
  ASSERT_EQ(0, thrd_sleep(&(struct timespec){.tv_nsec = 100000000}));

  // Wake up the threads in one go.
  ASSERT_EQ(thrd_success, mtx_lock(&block.mutex));
  block.triggered = true;
  ASSERT_EQ(thrd_success, cnd_broadcast(&block.cond));
  ASSERT_EQ(thrd_success, mtx_unlock(&block.mutex));

  for (size_t i = 0; i < __arraycount(threads); ++i)
    ASSERT_EQ(thrd_success, thrd_join(threads[i], NULL));
  mtx_destroy(&block.mutex);
  cnd_destroy(&block.cond);
}
