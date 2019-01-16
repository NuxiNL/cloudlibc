// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <threads.h>

#include "gtest/gtest.h"

static int do_timedout(void *arg) {
  // This should always fail, as the timestamp is in the past.
  auto mtx = static_cast<mtx_t *>(arg);
  struct timespec ts = {.tv_sec = 1422982367};
  EXPECT_EQ(thrd_timedout, mtx_timedlock(mtx, &ts));
  return 0;
}

TEST(mtx_timedlock, timedout) {
  mtx_t mtx;
  ASSERT_EQ(thrd_success, mtx_init(&mtx, mtx_timed));
  ASSERT_EQ(thrd_success, mtx_lock(&mtx));

  thrd_t thrd;
  ASSERT_EQ(thrd_success, thrd_create(&thrd, do_timedout, &mtx));
  ASSERT_EQ(thrd_success, thrd_join(thrd, NULL));

  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));
  mtx_destroy(&mtx);
}

static int do_success(void *arg) __no_lock_analysis {
  // This should block until the lock becomes available.
  struct timespec ts;
  EXPECT_EQ(TIME_UTC, timespec_get(&ts, TIME_UTC));
  ++ts.tv_sec;
  auto mtx = static_cast<mtx_t *>(arg);
  EXPECT_EQ(thrd_success, mtx_timedlock(mtx, &ts));
  EXPECT_EQ(thrd_success, mtx_unlock(mtx));
  return 0;
}

TEST(mtx_timedlock, success) {
  mtx_t mtx;
  ASSERT_EQ(thrd_success, mtx_init(&mtx, mtx_timed));
  ASSERT_EQ(thrd_success, mtx_lock(&mtx));

  thrd_t thrd;
  ASSERT_EQ(thrd_success, thrd_create(&thrd, do_success, &mtx));

  // Unlock the mutex after waiting briefly.
  struct timespec ts = {.tv_nsec = 100000000};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));

  ASSERT_EQ(thrd_success, thrd_join(thrd, NULL));
  mtx_destroy(&mtx);
}
