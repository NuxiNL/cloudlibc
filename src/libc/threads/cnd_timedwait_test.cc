// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <threads.h>
#include <time.h>

#include "gtest/gtest.h"

TEST(cnd_timedwait, timedout) {
  mtx_t mtx;
  ASSERT_EQ(thrd_success, mtx_init(&mtx, mtx_plain));
  cnd_t cond;
  ASSERT_EQ(thrd_success, cnd_init(&cond));

  // Attempt to wait on a timestamp that has already passed.
  ASSERT_EQ(thrd_success, mtx_lock(&mtx));
  struct timespec ts = {.tv_sec = 1422982367};
  ASSERT_EQ(thrd_timedout, cnd_timedwait(&cond, &mtx, &ts));
  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));

  cnd_destroy(&cond);
  mtx_destroy(&mtx);
}

struct block {
  mtx_t mutex;
  cnd_t cond;
};

static int do_wakeup(void *arg) {
  auto block = static_cast<struct block *>(arg);
  EXPECT_EQ(thrd_success, mtx_lock(&block->mutex));
  EXPECT_EQ(thrd_success, cnd_signal(&block->cond));
  EXPECT_EQ(thrd_success, mtx_unlock(&block->mutex));
  return 0;
}

TEST(cnd_timedwait, success) {
  struct block block;

  ASSERT_EQ(thrd_success, mtx_init(&block.mutex, mtx_plain));
  ASSERT_EQ(thrd_success, mtx_lock(&block.mutex));
  ASSERT_EQ(thrd_success, cnd_init(&block.cond));

  thrd_t thread;
  ASSERT_EQ(thrd_success, thrd_create(&thread, do_wakeup, &block));

  // Wait for the signal to be sent for up to one second.
  struct timespec ts;
  ASSERT_EQ(TIME_UTC, timespec_get(&ts, TIME_UTC));
  ++ts.tv_sec;
  ASSERT_EQ(thrd_success, cnd_timedwait(&block.cond, &block.mutex, &ts));

  ASSERT_EQ(thrd_success, thrd_join(thread, NULL));
  ASSERT_EQ(thrd_success, mtx_unlock(&block.mutex));
  mtx_destroy(&block.mutex);
  cnd_destroy(&block.cond);
}
